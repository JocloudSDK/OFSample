#include "usystem.h"
#include <intrin.h>
#include <process.h>
#pragma warning( push )
#pragma warning( disable : 4091 )
#include <Shlwapi.h>
#include <ShlObj.h>
#include <set>
#include "dwutility/dwjsondocument.h"
#include "dwutility/ufile.h"
#include "dwutility/dwencrypt.h"
#include "dwutility/environmentpath.h"
#include "dwutility/ustring.h"
#include "dwutility/md5.h"
#include "dwutility/appdatapath.h"
#include "app/urlhandler.h"
#include "dwbase/log.h"


namespace
{
	bool _jsonLoadFromEncryptFile(const std::wstring& filename, DWJsonDocument& jsonDoc, const std::string &key, int round)
	{
		std::string featureData;
		if (!DwUtility::ufile::ReadFileData(filename, featureData))
		{
			//DW_ASSERT(false);
			return false;
		}

		DwUtility::encryption::DWEncrypt decrypt(DwUtility::encryption::DWEncrypt::TEADecrypt, key, round);
		decrypt.addData(featureData);
		std::string result = decrypt.result();

		return jsonDoc.loadFromData(result.c_str());
	}

	void toBackSlash(std::wstring &s)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			if (s[i] == '/')
			{
				s[i] = '\\';
			}
		}
	}

	class MD5Cache
	{
		enum CacheSate
		{
			Invalid,
			NeedCheck,
			NoCheck
		};

		enum DllCheckState
		{
			UnMatch,
			UnCheck,
			Match
		};
	public:
		MD5Cache():m_state(Invalid) {}

		bool isOk(const std::wstring &path)
		{
			if (m_state == Invalid)
			{
				loadCacheInfo();
			}

			if (m_state == NoCheck)
			{
				return true;
			}

			std::wstring key;
			if (!normalize(path, key))	//不是YY Bin下的dll
			{
				return true;
			}
		
			for (size_t i = 0; i < m_data.size(); ++i)
			{
				if (_wcsicmp(m_data[i].dllName.c_str(), key.c_str()) != 0)
				{
					continue;
				}

				if (m_data[i].state == Match)
				{
					return true;
				}
				
				if (m_data[i].state == UnCheck)
				{
					return checkOneDllMd5(i);
				}

				if (m_data[i].state == UnMatch)
				{
					return false;
				}
			}

			return false;
		};

	private:
		
		bool normalize(const std::wstring &path, std::wstring &key)
		{
			std::wstring p = path;
			toBackSlash(p);
			std::wstring prefix = DwUtility::app::appRootPath();
			toBackSlash(prefix);
			size_t pos = p.find(prefix.c_str());
			if (pos != 0)	//并不是yy bin下的dll
			{
				return false; 
			}
			key = p.substr(prefix.size());

			return true;
		}
		bool loadCacheInfo()
		{
			m_state = NeedCheck;
			std::wstring path = DwUtility::app::appRootPath();
			path += L"config/local.config";
			DWJsonDocument jsonDoc;
			_jsonLoadFromEncryptFile(path, jsonDoc, "md5", 64);
			DWJsonObject jsonObj = jsonDoc.rootObject();
			if (jsonObj.isValid())
			{
				for (int i = 0; i < jsonObj.getChildCount(); ++i)
				{
					DWJsonObject obj = jsonObj.getChildObject(i);
					if (obj.isValid())
					{
						std::string n = obj.name();
						Data d;
						DwUtility::text::toWideString(n.c_str(), n.size(), d.dllName);
						d.md5 = obj.value();
						d.state = UnCheck;
						m_data.push_back(d);
					}
				}
			}

			return true;
		}

		bool checkOneDllMd5(size_t i)
		{
			if (m_data[i].state != UnCheck)
			{
				return true;
			}

			std::wstring path = DwUtility::app::appRootPath() + m_data[i].dllName;
			std::string md5;
			bool b = DwUtility::encryption::GetFileMD5A(path.c_str(), md5);
			if (b)
			{
				if (_stricmp(md5.c_str(), m_data[i].md5.c_str()) == 0)
				{
					m_data[i].state = Match;
				}
				else
				{
					m_data[i].state = UnMatch;
				}
			}

			return m_data[i].state == Match;
		}
	private:
		struct Data
		{
			std::wstring dllName;	//其实是相对于bin目录的路径
			std::string md5;
			DllCheckState state;	
		};
		std::vector<Data> m_data;
		CacheSate m_state;	
	};

	DwUtility::system::LoadLibraryExFailCallBack g_callBack = NULL;
	MD5Cache *g_cache = NULL;
	bool checkDllMd5(const std::wstring &path)
	{
		if (g_cache == NULL)
		{
			g_cache = new MD5Cache;
		}

		return g_cache->isOk(path);
	}
}

bool openUrlWithIE(const wchar_t* url)
{
	if (!url || !(*url))
		return false;

	wchar_t buf[MAX_PATH + 1] = { 0 };
	::SHGetSpecialFolderPathW(NULL, buf, CSIDL_PROGRAM_FILES, FALSE);
	buf[MAX_PATH] = '\0';
	std::wstring ie(buf);
	ie += L"\\Internet Explorer\\iexplore.exe";
	if (!DwUtility::AppDataPath::IsFileExist(ie.c_str())
		|| (DWORD)::ShellExecuteW(NULL, L"open", ie.c_str(), url, NULL, SW_SHOW) <= 32)
	{
		return (DWORD)::ShellExecute(NULL, L"open", L"iexplore.exe", url, NULL, SW_SHOW) > 32;
	}
	return true;
}

static unsigned __stdcall openUrlThread(void* param)
{
	assert(param != NULL);
	wchar_t* url = (wchar_t*)param;

	// handler link
	std::set<DwUtility::app::IUrlHandler*> urlHandlers = ((DwUtility::app::DwUrlProcessor*)DwUtility::app::getUrlProcessor())->urlHanders();
	for (std::set<DwUtility::app::IUrlHandler*>::const_iterator iter = urlHandlers.begin(); iter != urlHandlers.end(); ++iter)
	{
		DwUtility::app::IUrlHandler* handler = (*iter);
		if (NULL == handler)
		{
			continue;
		}

		bool isHandle = handler->process(url);
		if (isHandle)
		{
			return 0;
		}
	}

    wchar_t pathBrowser[MAX_PATH] = {0};
    DWORD nLength = sizeof(wchar_t)*MAX_PATH;
    DWORD dwType = 0;
    SHGetValue(HKEY_CLASSES_ROOT, L"http\\shell\\open\\command", L"", &dwType, pathBrowser, &nLength);
    std::wstring str(pathBrowser);
    if (str.find(L"%1") != std::wstring::npos)
    {
        //用shellExecuteEx
        SHELLEXECUTEINFO shelli;
        ZeroMemory(&shelli, sizeof(SHELLEXECUTEINFO));
        shelli.cbSize = sizeof(SHELLEXECUTEINFO);
        shelli.fMask = SEE_MASK_FLAG_NO_UI;
        shelli.lpVerb = L"open";
        shelli.lpFile = url;
        shelli.nShow = SW_SHOWDEFAULT;

        //先尝试使用默认浏览器打开，如果打开失败，则显式使用IE浏览器
        if (!ShellExecuteEx(&shelli))
        {
			openUrlWithIE(url);
        }
    }
    else
    {
        str += L" ";
        str += url;

        // 后面带有 -nohome的，用createProcess (360修改IE的。）
        TCHAR buf[4096] = {0};;
        wcscpy_s(buf, _countof(buf), str.c_str());

        STARTUPINFO si = {0};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pInfo = {0};
        if (!::CreateProcess(
            NULL, 
            buf, 
            NULL, NULL, NULL, 
            CREATE_NO_WINDOW, 
            NULL, NULL, 
            &si, 
            &pInfo)
		)
		{
			// 有些BT的电脑上http\\shell\\open不存在，所以这里强制使用IE打开
			openUrlWithIE(url);
		}

        ::CloseHandle(pInfo.hProcess);
        ::CloseHandle(pInfo.hThread);
    }

    delete[] url;
	return 0;
}

extern bool CreateProcessWithExplorerIL(LPWSTR szProcessName, LPWSTR szCmdLine, DWORD createFlags);

namespace DwUtility
{
	namespace system
	{
		void removeFolderRecursive(const std::wstring &folder)
		{
			WIN32_FIND_DATA data = {0};
			std::wstring str = folder + L"/*";
			HANDLE h = FindFirstFile(str.c_str(), &data);
			if (h == INVALID_HANDLE_VALUE)
			{
				return ;
			}

			do
			{
				if (std::wstring(L".") != data.cFileName &&
					std::wstring(L"..") != data.cFileName )
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						removeFolderRecursive(folder + L"/" + data.cFileName);
					}
					else
					{
						std::wstring file = folder + L"/" + data.cFileName;
						if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
						{
							DWORD dwAttr = ~FILE_ATTRIBUTE_READONLY;
							SetFileAttributes(file.c_str(), data.dwFileAttributes & dwAttr);
						}
						DeleteFile(file.c_str());
					}
				}
				
				BOOL b = FindNextFile(h, &data);
				if (!b/* && GetLastError() == ERROR_NO_MORE_FILES*/)
				{
					break;
				}
			} while (true);

			FindClose(h);
			RemoveDirectory(folder.c_str());
		}

		// 8.36增加isPlayingGameEx2方法判断是否全屏游戏状态，增加日志观察判断效果
		bool isPlayingGame()
		{
			bool bEx1 = isPlayingGameEx1();
			bool bEx2 = isPlayingGameEx2();
			LogFinal("uSystem")<<"isPlayingGame: bEx1="<<bEx1<<", bEx2="<<bEx2;
			return bEx1;
		}

		bool isPlayingGameEx1()
		{
			RECT rcWorkArea;
			rcWorkArea.top = rcWorkArea.left = 0;
			rcWorkArea.right = ::GetSystemMetrics(SM_CXSCREEN);
			rcWorkArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);


			POINT point[5] = {
				{ rcWorkArea.left+1,	rcWorkArea.bottom-1  },
				{ rcWorkArea.right-1,	rcWorkArea.bottom-1 },
				{ rcWorkArea.right/2,	rcWorkArea.bottom/2  },
				{ rcWorkArea.left+1,	rcWorkArea.top+1	 },
				{ rcWorkArea.right-1,	rcWorkArea.top+1     },
			};
            DWORD first = 0;
            DWORD next = 0;
            ::GetWindowThreadProcessId(::WindowFromPoint(point[0]), &first);

			for (int i=1; i<5; i++)
			{
                ::GetWindowThreadProcessId(::WindowFromPoint(point[i]), &next);
				if (first != next)
				{
					return false;
				}
			}

			//判断是否跟桌面同一进程
			DWORD dwTask = 0;
			DWORD dwPoint = 0;
			::GetWindowThreadProcessId(::FindWindow(L"Shell_TrayWnd", NULL), &dwTask);
			::GetWindowThreadProcessId(::WindowFromPoint(point[0]), &dwPoint);
			
			if (dwTask == dwPoint)
			{
				return false;
			}

			//属于当前进程
			DWORD dwYY = ::GetCurrentProcessId();
			if (dwPoint == dwYY)
			{
				return false;
			}

			return true;
		}

		bool isPlayingGameEx2()//这种判断会把屏保算在内
		{
			bool bFullScreen = false;
			HWND hwnd = ::GetForegroundWindow();
			RECT rcApp, rcDesk;
			::GetWindowRect(GetDesktopWindow(), &rcDesk);
			if (hwnd != GetDesktopWindow() && hwnd != GetShellWindow())
			{
				GetWindowRect(hwnd, &rcApp);
				if (rcApp.left <= rcDesk.left
					&& rcApp.top <= rcDesk.top
					&& rcApp.right >= rcDesk.right
					&& rcApp.bottom >= rcDesk.bottom)
				{

					bFullScreen = true;
				}
			}

			return bFullScreen;
		}

#ifndef MSGFLT_ADD
#define MSGFLT_ADD = 1;  // Add message to UIPI Filter
#define MSGFLT_REMOVE 2  // Remove message to UIPI Filter
#endif

		bool changeUIPIMessageFilter(const DwUtility::system::UIPIMsgFilters* msgFilters, DWORD cnt)
		{
			typedef bool (__stdcall *ChangeWindowMessageFilterType)(DWORD, unsigned long);

			ChangeWindowMessageFilterType change_window_message_filter_type = NULL;
			HMODULE hModule = LoadLibrary(L"user32.dll");
			if (!hModule)
				return false;

			change_window_message_filter_type = (ChangeWindowMessageFilterType)::GetProcAddress(hModule, "ChangeWindowMessageFilter");
			if (!change_window_message_filter_type)
			{
				FreeLibrary(hModule);
				return false;
			}

			for (DWORD i = 0; i < cnt; ++i)
			{
				if (!change_window_message_filter_type(msgFilters[i].message, msgFilters[i].isAdd ? MSGFLT_ADD : MSGFLT_REMOVE))
				{
					FreeLibrary(hModule);
					return false;
				}
			}

			FreeLibrary(hModule);
			return true;
		}

		bool easyRunDll(const std::wstring &path, const std::string &method)
		{
			LRESULT (CALLBACK *lpDllEntryPoint)();
			HINSTANCE hLib = ::LoadLibrary(path.c_str());
			if (hLib < (HINSTANCE)HINSTANCE_ERROR)
			{
				return false;
			}
			(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, method.c_str());
			if(lpDllEntryPoint)
			{
				(*lpDllEntryPoint)();
				return true;
			}
			else
			{
				return false;
			}
		}

		bool deleteFolder(const std::wstring& folder)
		{
			SHFILEOPSTRUCT sc;
			::ZeroMemory(&sc, sizeof(SHFILEOPSTRUCT));
			sc.wFunc = FO_DELETE;
			TCHAR* pBuffer = new TCHAR[folder.size()+20];
			::ZeroMemory(pBuffer, sizeof(TCHAR)*(folder.size()+20));
			lstrcpyn(pBuffer, folder.data(), folder.size() + 1);	//including a terminating null character,so plus 1
			if (pBuffer[folder.size()-1] == L'\\')
			{
				pBuffer[folder.size()-1] = 0;
			}
			sc.pFrom = pBuffer;
			sc.fFlags = FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMATION;
			if (0 != SHFileOperation(&sc))	
			{	////An unknown error occurred. This is typically due to an invalid path in the source or destination. This error does not occur on Windows Vista and later.
				removeFolderRecursive(folder);
			}
			delete [] pBuffer;

			return true;
		}

		bool IsVistaOrWin7()
		{
			OSVERSIONINFOEX systeminfo = {0};
			systeminfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((LPOSVERSIONINFO)&systeminfo);
			if (systeminfo.dwMajorVersion >= 6)
			{
				return true;
			}
			return false;
		}
		
		bool IsVersionXP(void)
		{
			OSVERSIONINFOEX os;
			os.dwOSVersionInfoSize=sizeof(os);
			GetVersionEx((OSVERSIONINFO *)&os);
			if(os.dwMajorVersion == 5 && os.dwMinorVersion == 1)
			{
				return true;
			}
			return false;
		}

		int EnableDebugPriv(LPCTSTR lpName)
		{
			HANDLE hToken;
			TOKEN_PRIVILEGES tp;
			LUID luid;
			//打开进程令牌环
			OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
			//获得进程本地唯一ID
			LookupPrivilegeValue(NULL, lpName, &luid);

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			tp.Privileges[0].Luid = luid;
			//调整权限
			AdjustTokenPrivileges(hToken,0,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL); 
			return 0;
		}

		bool TrySetDllDirectory(const std::wstring& strPath)
		{
			bool result = false;
			__try
			{
				result = !!(::SetDllDirectory(strPath.c_str()));
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				// NOTICE: 部分用户环境下ntdll(6.1.7601.16385) crash
				result = false;
			}
			return result;
		}

		bool openUrl(const std::wstring& url)
		{
			if (url.empty())
			{
				return false;
			}

			wchar_t* param = new wchar_t[url.length()+1];
			wcscpy(param, url.c_str());
			HANDLE h = (HANDLE)_beginthreadex(NULL, 0, openUrlThread, param, CREATE_SUSPENDED, NULL);
			if (h == NULL)
			{
				delete[] param;
				return false;
			}

			::ResumeThread(h);
			::CloseHandle(h);
			return true;
		}

		const OSVERSIONINFOEX& GetWindowsVersion()
		{
			struct OSVERINFO
			{
				OSVERSIONINFOEX osvi;
				SYSTEM_INFO si;

				BOOL bCompatMode;
				DWORD dwMajorVersion;
				DWORD dwMinorVersion;
				DWORD dwBuildNumber;
				DWORD dwProductType;
			};

			static OSVERINFO g_osver_info = { 0 };

			typedef void (WINAPI *GetNtVersionNumbersFunctor)(LPDWORD, LPDWORD, LPDWORD);
			typedef NTSTATUS (WINAPI *RtlGetVersionFunctor)(PRTL_OSVERSIONINFOEXW);

			OSVERSIONINFOEX& osvi = g_osver_info.osvi;
			if (osvi.dwOSVersionInfoSize == 0)
			{
				BOOL bGetVersionEx = FALSE;
				OSVERSIONINFO osi = { 0 };
				osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				if (bGetVersionEx = ::GetVersionEx(&osi))
				{
					osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
					bGetVersionEx = ::GetVersionEx((OSVERSIONINFO*)(&osvi));
				}

				RTL_OSVERSIONINFOEXW rtl_osvi = {0};
				rtl_osvi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
				RtlGetVersionFunctor RtlGetVersion = (RtlGetVersionFunctor)(::GetProcAddress(::GetModuleHandle(L"ntdll.dll"), "RtlGetVersion"));
				BOOL bRtlGetVersion = (RtlGetVersion != NULL ? RtlGetVersion(&rtl_osvi)==0 : FALSE);

				GetNtVersionNumbersFunctor GetNtVersionNumbers = (GetNtVersionNumbersFunctor)(::GetProcAddress(::GetModuleHandle(L"ntdll.dll"), "RtlGetNtVersionNumbers"));
				BOOL bGetNtVersionNumbers = (GetNtVersionNumbers != NULL);
				if (bGetNtVersionNumbers)
				{
					GetNtVersionNumbers(&g_osver_info.dwMajorVersion, &g_osver_info.dwMinorVersion, &g_osver_info.dwBuildNumber);

					if (osvi.dwMajorVersion > 5 && bRtlGetVersion)
					{
						osvi.dwPlatformId       = rtl_osvi.dwPlatformId;
						osvi.dwMajorVersion     = rtl_osvi.dwMajorVersion;
						osvi.dwMinorVersion     = rtl_osvi.dwMinorVersion;
						wcscpy_s(osvi.szCSDVersion, _countof(osvi.szCSDVersion), rtl_osvi.szCSDVersion);
						osvi.wServicePackMajor  = rtl_osvi.wServicePackMajor;
						osvi.wServicePackMinor  = rtl_osvi.wServicePackMinor;
						osvi.wSuiteMask         = rtl_osvi.wSuiteMask;
						osvi.wProductType       = rtl_osvi.wProductType;

					}

					g_osver_info.bCompatMode = (osvi.dwMajorVersion != g_osver_info.dwMajorVersion || osvi.dwMinorVersion != g_osver_info.dwMinorVersion || osvi.dwBuildNumber != (g_osver_info.dwBuildNumber & 0xffff));
					g_osver_info.dwBuildNumber &= 0xffff;
				}
				else 
				{
					g_osver_info.dwMajorVersion = osvi.dwMajorVersion;
					g_osver_info.dwMinorVersion = osvi.dwMinorVersion;
					g_osver_info.dwBuildNumber  = osvi.dwBuildNumber;
				}

			}

			return osvi;
		}

		Mutex::Mutex(const wchar_t *name)
			:m_handle(::CreateMutex(NULL, FALSE, name)), m_locked(false)
		{
			DW_ASSERT(m_handle != NULL);
		}

		Mutex::~Mutex()
		{
			::CloseHandle(m_handle);
		}

		bool Mutex::Lock()
		{
			DWORD res = ::WaitForSingleObject(m_handle, INFINITE);
			DW_ASSERT(!m_locked);
			m_locked = res == WAIT_ABANDONED || res == WAIT_OBJECT_0;
			return m_locked;
		}

		bool Mutex::LockWithTimeout(DWORD millisecond)
		{
			DWORD res = ::WaitForSingleObject(m_handle, millisecond);
			DW_ASSERT(!m_locked);
			m_locked = res == WAIT_ABANDONED || res == WAIT_OBJECT_0;
			return m_locked;
		}

		void Mutex::Unlock()
		{
			if(m_locked)
			{
				::ReleaseMutex(m_handle);
				m_locked = false;
			}
		}

		//explicit
		MutexLocker::MutexLocker(Mutex* mutex)
			:m_mutex(mutex)
		{
			m_mutex->Lock();
		}

		MutexLocker::~MutexLocker()
		{
			m_mutex->Unlock();
		}

		bool createProcessLowPriority(const std::wstring & szProcessName, const std::wstring &szCmdLine, DWORD createFlags)
		{
			TCHAR bufProcessName[1024] = {0};
			TCHAR bufCmdLine[1024] = {0};
			wcscpy_s(bufProcessName, 1023, szProcessName.c_str());
			wcscpy_s(bufCmdLine, 1023, szCmdLine.c_str());
			return CreateProcessWithExplorerIL(szProcessName.empty()?NULL:bufProcessName, bufCmdLine, createFlags);
		}

		void forceWindowToTop( HWND hwnd )
		{
			if (!::SetForegroundWindow(hwnd))
			{
				//setForegroundWindow 偶尔会失败，原因不详
				::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
			::SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		HMODULE loadLibraryEx(const std::wstring &path, unsigned int flag)
		{
			md5Check(path);
			HMODULE h = ::LoadLibraryEx(path.c_str(), NULL, flag);
			if (h == NULL)
			{
				if (g_callBack)
				{
					g_callBack(path, KLoadLibraryStage);
				}
			}

			return h;
		}

		bool md5Check(const std::wstring &path)
		{
			bool checkMd5 = false;
			if (g_callBack)
			{
				std::wstring p(path);
				toBackSlash(p);
				checkMd5 = g_callBack(p, KBeforeLoadLibrary);
			}

			if (!checkMd5)
			{
				return true;
			}

			if (checkDllMd5(path))
			{
				return true;
			}

			if (g_callBack)
			{
				g_callBack(path, KMD5CheckStage);
			}

			return false;
		};

		DWORD countSetBits(ULONG_PTR bitMask)
		{
			DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
			DWORD bitSetCount = 0;
			ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    
			DWORD i;

			for (i = 0; i <= LSHIFT; ++i)
			{
				bitSetCount += ((bitMask & bitTest)?1:0);
				bitTest/=2;
			}

			return bitSetCount;
		}


		std::string DW_DWUTILITY_EXPORT getCPUName()
		{
			std::string cpuName;
			int cpuInfo[4] = {-1};
			__cpuid(cpuInfo, 0x80000000);
			int nExIds_ = cpuInfo[0];
			char szInfo[17] = {0};
			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuid(cpuInfo, i);
				if (i == 0x80000002 || i == 0x80000003 || i == 0x80000004)
				{
					memcpy_s((void*)szInfo,16,(void*)cpuInfo,16);
					cpuName.append(szInfo);
				}
			}
			
			return cpuName;
		}

		DWORD DW_DWUTILITY_EXPORT getCoreNumber()
		{
			typedef BOOL (WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

			LPFN_GLPI glpi = NULL;
			BOOL done = FALSE;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
			DWORD returnLength = 0;
			DWORD logicalProcessorCount = 0;
			DWORD numaNodeCount = 0;
			DWORD processorCoreCount = 0;
			DWORD byteOffset = 0;

			glpi = reinterpret_cast<LPFN_GLPI>( GetProcAddress( GetModuleHandle(L"kernel32"), "GetLogicalProcessorInformation") );
			if (NULL == glpi) 
			{
				return (-1);
			}

			while (!done)
			{
				DWORD rc = glpi(buffer, &returnLength);

				if (FALSE == rc) 
				{
					if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
					{
						if (buffer) 
							free(buffer);

						buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
							returnLength);

						if (NULL == buffer) 
						{
							return (-2);
						}
					} 
					else 
					{
						return (-3);
					}
				} 
				else
				{
					done = TRUE;
				}
			}

			ptr = buffer;

			while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) 
			{
				switch (ptr->Relationship) 
				{
				case RelationNumaNode:
					numaNodeCount++;
					break;

				case RelationProcessorCore:
					processorCoreCount++;
					logicalProcessorCount += countSetBits(ptr->ProcessorMask);
					break;
				default:
					break;
				}
				byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
				ptr++;
			}
			free(buffer);

			return processorCoreCount;
		}

		bool getGPUInfo(std::vector<std::wstring> &cards) 
		{
			typedef BOOL (WINAPI * PEnumDisplayDevices)(PVOID,DWORD,PVOID,DWORD);

			int nDeviceIndex = 0;
			PEnumDisplayDevices pEnumDisplayDevices = NULL;
			HINSTANCE  hInstUser32 = NULL;
			DISPLAY_DEVICE DispDev = {0}; 
			bool bRet = false;
			HRESULT hr = S_OK;

			hInstUser32 = LoadLibrary(L"User32.DLL");
			if (!hInstUser32) 
			{
				return FALSE;  
			}

			// Get the address of the EnumDisplayDevices function 
			pEnumDisplayDevices = reinterpret_cast<PEnumDisplayDevices>( GetProcAddress(hInstUser32, "EnumDisplayDevicesW") );
			if (!pEnumDisplayDevices)
			{
				FreeLibrary(hInstUser32);
				return FALSE;
			}

			while(true)
			{
				ZeroMemory(&DispDev, sizeof(DispDev));
				DispDev.cb = sizeof(DispDev); 

				// After the first call to EnumDisplayDevices,  
				// DispDev.DeviceString is the adapter name 
				if (hr = pEnumDisplayDevices(NULL, nDeviceIndex++, &DispDev, 0))
				{  
					//_tcscpy(szSaveDeviceName, DispDev.DeviceName);
					if (FAILED(hr))
					{
						// TODO: write error handler 
						break;
					}
					if (!(DispDev.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
					{
						cards.push_back(DispDev.DeviceString);
						bRet = true;
					}
				} 
				else    
				{
					break;
				}
			}

			FreeLibrary(hInstUser32);

			return bRet;
		}

		bool killProcess(DWORD dwProgressId, HANDLE hProgress)
		{
			if (0 == dwProgressId || NULL == hProgress)
				return false;

			if (dwProgressId == ::GetCurrentProcessId())
				return false;   // 不kill自己

			DWORD pid = ::GetProcessId(hProgress);
			if (pid != dwProgressId)
				return false;

			BOOL bRet = ::TerminateProcess((HANDLE)(hProgress), PROCESS_TERMINATE);
			return !!bRet;
		}

		void InstallLoadLibraryExFailCallBack(DwUtility::system::LoadLibraryExFailCallBack callback)
		{
			g_callBack = callback;	
		}
	}
}
#pragma warning( pop )