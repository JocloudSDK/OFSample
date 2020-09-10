
/****************************************************************************
Author: houjinxin
Email : houjinxin@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#include "stable.h"
#include <wininet.h>
#include "log/log.h"
#include "dwutility/aesencrypt.h"
#include "dwutility/ufile.h"
#include "dwutility/appdatapath.h"
#include "datacenter/CriticalSection.h"
#include "exceptionreport.h"

EXTERN_C PVOID _ReturnAddress();

const DWORD c_interval       = 20 * 60 * 1000;//设定上报的时间间隔 暂设20minutes
const TCHAR c_upload_url[]   = _T("http://exceptionreport.yy.duowan.com/exceptionreport/admin/upload.php");
const char  c_boundary[]     =	"-----------------------------19810202abcd1234";
const TCHAR c_http_headers[] = _T("Content-Type: multipart/form-data; boundary=---------------------------19810202abcd1234\r\n");
const char AES_EXCEPTION_KEY[] = "b578c164b8c0aa8c";
struct DwAssertInfo
{
    std::string moduleName;
    std::string expression;
    std::string productVersion;
    std::string file;
    std::string func;
    std::string what;
    int user_id;
    int line;
    bool bAssert;	//是否需要在返回后assert
};
typedef std::set<UINT64> CSetHashSet;
typedef std::map<UINT64, DwAssertInfo> CMapAssertInfo;
class DwAssert
{    
public:
    DwAssert();
    static DwAssert* getInstance();
    #ifdef _DEBUG
        bool receiveAssertReport(bool result, const char *module, const char *file, const char *func, int line, const char *expression, const char *what);
    #else
        bool initAssertMacro(int usrId,LPCSTR yyVersion);
        bool receiveAssertReport(bool result,const char *module, const char *file, const char *func, int line, const char *expression, const char *what);
        void flushAssertReportToFile();
    #endif  
        HMODULE GetModuleFromAddr(PVOID p);  
private:     
    static UINT64 makeAssertHash(const char *file, const int line);
    #ifdef _DEBUG
        bool doAssert(bool exp, const char *module, const char *file, const char *func, int line, const char *expression, const char *what);        
        CSetHashSet m_assertOnceSet;
    #else
        static void checkAndUpload();
        static DWORD assertUploadProc(std::string *lpPara);
        static std::string getAssertDataToUpload();
        static std::string trimPath(char *rawFileName);
        static std::string U16toUTF8(const wchar_t *s);
        static std::string IntToStr(const DWORD dwValue);         
        static bool AtoW(LPCSTR lpszSrc, int cch/*=-1*/, std::wstring &wstrDst);
        static bool Upload(LPCTSTR lpszUrl, LPCTSTR lpszHeaders, const void* pPostData, int nLen, std::string &strResult);        
       
        int m_idYY;//用户yy
        std::string m_productVersion;//用户当前当前使用的产品版本 
        std::string m_reportToUpload;//从文件中读取的上次yy退出之前未上传的assert report数据        
        CMapAssertInfo m_relAssertInfo;//收集进程运行过程中assert failed数据 
    #endif
        CCriticalLock m_criLock;//debug和release都会用到
};
DwAssert::DwAssert()
{
    #ifndef _DEBUG
        m_idYY = 0;

    #endif
}
DwAssert* DwAssert::getInstance()
{
    static DwAssert *pAssert = NULL;
    if (!pAssert)
    {
#ifdef _DEBUG
        int nOldFlag = _CrtSetDbgFlag(0);
#endif

        pAssert = new DwAssert();

#ifdef _DEBUG
        _CrtSetDbgFlag(nOldFlag);
#endif
    }
    return pAssert;
}

UINT64 DwAssert::makeAssertHash(const char *file, const int line)
{
    UINT64 h = ((UINT64)file) << 32;
    UINT64 l = line;
    return h | l;
}

inline HMODULE DwAssert::GetModuleFromAddr(PVOID p)
{
    MEMORY_BASIC_INFORMATION m = {0};
    VirtualQuery(p, &m, sizeof(MEMORY_BASIC_INFORMATION));
    return (HMODULE) m.AllocationBase;
}
#ifdef _DEBUG
    bool DwAssert::receiveAssertReport(bool result, const char *module, const char *file, const char *func, int line, const char *expression, const char *what)
    {
        return doAssert(result, module, file, func, line, expression, what);
    }

    bool DwAssert::doAssert(bool exp, const char *module, const char *file, const char *func, int line, const char *expression, const char *what)
    {
        UINT64 assertHash = makeAssertHash(file, line);
        CAutoLock hashLock(m_criLock);
        if (m_assertOnceSet.insert(assertHash).second)
        {
            char szPopMsgWndInfo[4096] = {0};
            _snprintf_s(szPopMsgWndInfo, _countof(szPopMsgWndInfo), _TRUNCATE, "         Assert Failed!\r\n\r\n    Module:  %s\r\n           File:  %s\r\n         Func:  %s\r\n          Line:  %d\r\nExpression:  %s\r\nDescription:  %s\r\n",
                module, file, func, line, expression,what);
            MessageBoxA(NULL, szPopMsgWndInfo, "YY_ASSERT ERROR!", MB_SERVICE_NOTIFICATION);

            return true;//开发机返回true 调试的时候死机 
        }

        return false;
    }
#else
    bool DwAssert::initAssertMacro(int usrId,LPCSTR yyVersion)
    {
        std::wstring yyInstallDir = DwUtility::AppDataPath::GetLogPath();
        TCHAR tmpAssertDataFile[MAX_PATH] = {0};
        _snwprintf_s(tmpAssertDataFile, _countof(tmpAssertDataFile), _TRUNCATE, _T("%sexception.tmp"), yyInstallDir.c_str());
        DwUtility::ufile::ReadFileData(tmpAssertDataFile,m_reportToUpload);       
        DwUtility::ufile::DeleteFileOrDirectory(tmpAssertDataFile);

        if (m_reportToUpload.size() > 0)
        {
            m_reportToUpload = DwUtility::encryption::AESDecrypt(m_reportToUpload,AES_EXCEPTION_KEY);
            checkAndUpload();
        }

        m_idYY           = usrId;
        m_productVersion = yyVersion;
        SetTimer(NULL, 0, c_interval, (TIMERPROC)checkAndUpload); // time identifier = 0, delay = 10s

        return true;
    }

    bool DwAssert::receiveAssertReport(bool result,const char *module, const char *file, const char *func, int line, const char *expression, const char *what)
    {
        UINT64 assertHash = makeAssertHash(file, line);
        CAutoLock writeLock(m_criLock);
        if (m_relAssertInfo.find(assertHash) != m_relAssertInfo.end())
        {
            return false;
        }

        char szCppName[MAX_PATH]    = {0};
        char szModuleName[MAX_PATH] = {0};  
        strcpy_s(szModuleName, _countof(szModuleName), module);
        strcpy_s(szCppName, _countof(szCppName), file);

        DwAssertInfo info;
        info.moduleName             = trimPath(szModuleName);
        info.expression             = expression;
        info.productVersion         = m_productVersion;
        info.file                   = trimPath(szCppName);
        info.func                   = func;
        info.what                   = what;
        info.user_id                = m_idYY;
        info.line                   = line;
        info.bAssert                = false;    
        m_relAssertInfo[assertHash] = info;

        return true;
    }

    void DwAssert::flushAssertReportToFile()
    {
        std::string strDelayException = getAssertDataToUpload();
        if (strDelayException.size() > 0)
        {
			std::wstring yyInstallDir = DwUtility::AppDataPath::GetLogPath();
            TCHAR assertDelayUploadFile[MAX_PATH] = {0};
            _snwprintf_s(assertDelayUploadFile, _countof(assertDelayUploadFile), _TRUNCATE, _T("%sexception.tmp"), yyInstallDir.c_str());
            strDelayException = DwUtility::encryption::AESEncrypt(strDelayException,AES_EXCEPTION_KEY);            
            DwUtility::ufile::WriteFileData(assertDelayUploadFile,strDelayException);
        }
    }

    std::string DwAssert::IntToStr(const DWORD dwValue)
    {
        char temp[128] = {0};
        _snprintf_s(temp, _countof(temp), _TRUNCATE, "%u", dwValue);
        return temp;
    }

    DWORD DwAssert::assertUploadProc(std::string *lpPara)
    {
        std::string strRecv;
        if (!Upload(c_upload_url, c_http_headers, lpPara->c_str(), lpPara->size(), strRecv))
        {
            LogFinal(LOG::KFatalException)<<L"AssertReport上传失败";
        }
        delete lpPara;
        lpPara = NULL;

        return 0;
    }

    void DwAssert::checkAndUpload()
    {
        std::string *pFileUpload = new std::string;
        DwAssert* objAssert      = DwAssert::getInstance();
        if (objAssert->m_reportToUpload.size() > 0)//初始化第一次执行 检测YY上次退出时是否有数据未上传
        {
            pFileUpload->swap(objAssert->m_reportToUpload);//初始化的时候执行 不需要加锁操作
        }
        else
        {
            *pFileUpload = getAssertDataToUpload();//s_relAssertInfo中是否有最新assert report需要上报                
        }
        if (pFileUpload->size() > 0)
        {
            HANDLE handle = NULL;
            handle        = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)assertUploadProc, pFileUpload, 0, 0);
            if (handle != NULL)
                ::CloseHandle(handle);
        }
    }

    std::string DwAssert::getAssertDataToUpload()
    {
        DwAssert* objAssert = DwAssert::getInstance();
        CAutoLock readLock(objAssert->m_criLock);
        if (objAssert->m_relAssertInfo.size() > 0)
        {
            std::string  strValue;   
            int maxCountToUpload = 0;
            std::map<UINT64,DwAssertInfo>::const_iterator iter_end = objAssert->m_relAssertInfo.end();
            for(std::map<UINT64,DwAssertInfo>::const_iterator iter = objAssert->m_relAssertInfo.begin(); iter != iter_end && maxCountToUpload < 1000; ++iter, maxCountToUpload++)
            {
                strValue += iter->second.moduleName;
                strValue += "&&";
                strValue += iter->second.file;
                strValue += "&&";
                strValue += IntToStr(iter->second.line);
                strValue += "&&";
                strValue += iter->second.expression;
                strValue += "&&";
                strValue += IntToStr(iter->second.user_id);
                strValue += "&&";
                strValue += iter->second.productVersion;
                strValue += "&&";
                strValue += iter->second.what;
                strValue += "&&";
                strValue += "chengxuyuan";//保留字段 模块负责人
                strValue += "&&";
                strValue += iter->second.func;
                strValue += "@@";              
            }
            objAssert->m_relAssertInfo.clear();

            std::wstring wstrValue; 
            AtoW(strValue.c_str(), -1, wstrValue);    
            std::string  strPostData = std::string(c_boundary)
                + "\r\nContent-Disposition: form-data; name=\"reportData"
                + "\"\r\n\r\n"
                + U16toUTF8(wstrValue.c_str())
                + "\r\n";
            return strPostData;
        }
        return "";   
    }

    std::string DwAssert::U16toUTF8(const wchar_t *s)
    {
        std::string strRet;

        if (s == NULL)
            return strRet;

        int nLen = wcslen(s);
        strRet.resize(nLen * 4 + 1);

        int nDst = 0;
        for (int i = 0; i < nLen; i++)
        {
            wchar_t c = s[i];

            if (c < 0x80)
            {
                strRet[nDst++] = (char)c;
            }
            else if (c < 0x800)
            {
                strRet[nDst++] = 0xc0 | ( c >> 6 );
                strRet[nDst++] = 0x80 | ( c & 0x3f );
            }
            else if (c < 0x10000)
            {
                strRet[nDst++] = 0xe0 | ( c >> 12 );
                strRet[nDst++] = 0x80 | ( (c >> 6) & 0x3f );
                strRet[nDst++] = 0x80 | ( c & 0x3f );
            }
            else if (c < 0x200000) 
            {
                strRet[nDst++] = 0xf0 | ( (int)c >> 18 );
                strRet[nDst++] = 0x80 | ( (c >> 12) & 0x3f );
                strRet[nDst++] = 0x80 | ( (c >> 6) & 0x3f );
                strRet[nDst++] = 0x80 | ( c & 0x3f );
            }
        }

        strRet[nDst] = 0;
        strRet.resize(nDst);
        return strRet;
    }

    bool DwAssert::AtoW(LPCSTR lpszSrc, int cch/*=-1*/, std::wstring &wstrDst)
    {
        if (cch == -1)
            cch = (int)strlen(lpszSrc);

        wstrDst.resize(cch, NULL);
        int n = MultiByteToWideChar(CP_ACP, 0, lpszSrc, cch, &wstrDst[0], cch);
        wstrDst.resize(n);
        return n > 0;
    }
    
	std::string DwAssert::trimPath(char *rawFileName)
    {
        std::string strTmp;
        std::string strTrimResult;

        char *pBegin  = NULL;
        pBegin        = strrchr(rawFileName,'\\');
        if (pBegin == NULL )
        {
            return "";
        } 
        else
        {
            strTmp        = rawFileName;
            strTrimResult = strTmp.substr(pBegin - rawFileName + 1, strTmp.size()-(pBegin - rawFileName));
            return strTrimResult;
        }  
    }

    bool DwAssert::Upload(LPCTSTR lpszUrl, LPCTSTR lpszHeaders, const void* pPostData, int nLen, std::string &strResult)
    {
        bool bRet = FALSE;
        strResult = "";

        HINTERNET hOpen = NULL;
        HINTERNET hConnect = NULL;
        HINTERNET hRequest = NULL;

        DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;

        TCHAR szHostname[256] = {0};
        URL_COMPONENTS urlcomp = {0};
        urlcomp.dwStructSize = sizeof(URL_COMPONENTS);
        urlcomp.lpszHostName = szHostname;
        urlcomp.dwHostNameLength = sizeof(szHostname)/sizeof(TCHAR) - 1;
        urlcomp.dwUrlPathLength = 1;
        LPCTSTR acceptTypes[2] = {_T("*/*"), 0};

        do
        {
            if (!InternetCrackUrl(lpszUrl, lstrlen(lpszUrl), 0, &urlcomp))
                break;

            hOpen = InternetOpen(_T(""), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
            if (!hOpen)
                break;

            hConnect = InternetConnect(hOpen, szHostname, urlcomp.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (!hConnect)
                break;

            if (pPostData)
                hRequest = HttpOpenRequest(hConnect, _T("POST"), urlcomp.lpszUrlPath, _T("HTTP/1.1"), NULL, acceptTypes, dwFlags, 0);
            else
                hRequest = HttpOpenRequest(hConnect, _T("GET"), urlcomp.lpszUrlPath, _T("HTTP/1.0"), NULL, acceptTypes, dwFlags, 0);

            if (!hRequest)
                break;

            if (pPostData)
            {
                INTERNET_BUFFERS inbuffer = {0};
                inbuffer.dwStructSize		= sizeof(INTERNET_BUFFERS);
                inbuffer.Next				= NULL;
                inbuffer.lpcszHeader		= lpszHeaders;
                inbuffer.dwHeadersLength	= lpszHeaders? lstrlen(lpszHeaders) : 0;
                inbuffer.dwHeadersTotal		= lpszHeaders? lstrlen(lpszHeaders) : 0;
                inbuffer.lpvBuffer			= (void *)pPostData;
                inbuffer.dwBufferLength		= nLen;
                inbuffer.dwBufferTotal		= nLen;
                if (!HttpSendRequestEx(hRequest, &inbuffer, NULL, 0, 0))
	                break;
                if (!HttpEndRequest(hRequest, NULL, 0, 0))
	                break;
            } 
            else 
            {
                if (!HttpSendRequest(hRequest, lpszHeaders, lpszHeaders ? -1 : 0, NULL, 0))
	                break;
            }

            char buf[8192] = {0}; 
            DWORD dwReadBytes = 0;

            DWORD dwStatusCode = -1;
            while (InternetReadFile(hRequest, buf, sizeof(buf), &dwReadBytes))
            {
                if (dwStatusCode == -1)
                {
	                DWORD dwData = 0;
	                DWORD dwDataLen = sizeof(dwData);
	                if (HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwData, &dwDataLen, NULL))
		                dwStatusCode = dwData;
	                else
		                assert(0);
                }

                if (dwReadBytes == 0) 
                {
	                bRet = TRUE;
	                break;
                }

                strResult.append(buf, dwReadBytes);
            }
        } while (0);


        if (hRequest)
            InternetCloseHandle(hRequest);
        if (hConnect)
            InternetCloseHandle(hConnect);
        if (hOpen) 
            InternetCloseHandle(hOpen);

        return bRet;
    }
#endif

#ifdef _DEBUG // debug下直接弹窗提示开发人员
    bool dwAssertReportOnce(bool exp, const char *file, const char *func, int line, const char *expression, const char *what)
    {
        if (!exp)
        {
            /************************获取异常所属DLL模块********************************/
            DwAssert* ptrYYAssert       = DwAssert::getInstance();
            HMODULE hCallerModule       = ptrYYAssert->GetModuleFromAddr(_ReturnAddress());
            char szModuleName[MAX_PATH] = {0};
            ::GetModuleFileNameA(hCallerModule, szModuleName, _countof(szModuleName));
            /************************************************************************/

            if (ptrYYAssert->receiveAssertReport(exp, szModuleName, file, func, line, expression, what))
            {
                LogFinal(LOG::KFatalException)<<L"YY_ASSERT断言失败";
                return true;
            } 
            else
            {
                return false;
            }
        }
        return false;//
    }
#else
    /************************************************************************/
    /*release下将异常上报到服务器*/
    /************************************************************************/   
    //首先检测在进程退出之前是否有未上报的数据，如果有则上报
    bool dwInitAssertReport(const UINT usrId,LPCSTR yyVersion)
    {            
        if (DwAssert::getInstance()->initAssertMacro(usrId,yyVersion))
        {                 
            LogFinal(LOG::KFatalException)<<L"YY_ASSERT初始化成功";
            return true;
        } 
        else
        {
            return false;
        }
    }

    //进程退出之前将未上报的异常报告写到文件中 下次启动时上报
    void dwUninitAssertReport()
    {    
        DwAssert::getInstance()->flushAssertReportToFile();            
    }

	//接收程序运行过程中触发的异常
    bool dwAssertReport(bool result, const char *file, const char *func, int line, const char *expression, const char *what)
    {
		//服务器低载，因此下掉此功能
		return false;
        if (!result)
        {      
            LogFinal(LOG::KFatalException)<<L"YY_ASSERT断言失败"; 
            /************************获取异常所属DLL模块********************************/
            DwAssert* obj_yyassert     = DwAssert::getInstance();
            HMODULE hCallerModule       = obj_yyassert->GetModuleFromAddr(_ReturnAddress());
            char szModuleName[MAX_PATH] = {0};
            ::GetModuleFileNameA(hCallerModule, szModuleName, _countof(szModuleName));
            /************************************************************************/
            if (!obj_yyassert->receiveAssertReport(result, szModuleName, file, func, line, expression, what))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return result;
    }
#endif