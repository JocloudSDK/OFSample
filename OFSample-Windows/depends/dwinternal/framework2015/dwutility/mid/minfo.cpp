#include "minfo.h"
#include "machineinfo.h"
#include "mid/winwmi.h"
#include "mid/hdbios.h"
#include "mid/cpuid.h"
#include "thread/winthread.h"
#include "dwbase/log.h"

#include "text/ustring.h"
#include "encryption/md5encrypt.h"

#include <Nb30.h>
#include <iphlpapi.h>
#include <tlhelp32.h>

#define IF_MAC_NULL		"minfo" // 有mac取不到的情况！！！！

std::string g_Mid;             // 机器id
std::string g_Mid3;            // 机器id 3
std::string g_GlobalMid;       // 全局id
std::string g_wmiStr;          // wmi
int g_isNetBar = -1;		   // -1未初始化,0不是网吧环境，1是网吧环境
volatile LONG g_wmiFlag = 0;     // 0初始状态，2完成 

const static wchar_t* g_wmiList[] = {
	// name                   param
	L"Win32_LogicalDisk"    , L"VolumeSerialNumber",
	L"Win32_OperatingSystem", L"SerialNumber"      ,
	// L"Win32_Account"     , L"SID"               ,
	L"Win32_DiskDrive"      , L"PNPDeviceID"       ,
	L"Win32_SoundDevice"    , L"PNPDeviceID"       ,
	L"Win32_VideoController", L"PNPDeviceID"       ,
	NULL
};

UCHAR dwNetbios( PNCB pncb )
{
    UCHAR ret = NRC_PENDING;
    HMODULE h = LoadLibrary(L"NETAPI32.DLL");
    if(h)
    {
        void* pfNetbios = (void*)GetProcAddress(h, "Netbios");
        if(pfNetbios)
        {
            ret = ((UCHAR (WINAPI *)(PNCB))pfNetbios)(pncb);
        }

        FreeLibrary(h);
    }

    return ret;
}

ULONG dwGetAdaptersInfo(
                        IN PIP_ADAPTER_INFO AdapterInfo, 
                        IN OUT PULONG SizePointer
                        )
{
    ULONG ret = ERROR_NOT_SUPPORTED;
    HMODULE h = LoadLibrary(L"IPHLPAPI.DLL");
    if(h)
    {
        void* pfGetAdaptersInfo = (void*)GetProcAddress(h, "GetAdaptersInfo");
        if(pfGetAdaptersInfo)
        {
            ret = ((ULONG (WINAPI *)(PIP_ADAPTER_INFO, PULONG))pfGetAdaptersInfo)(AdapterInfo, SizePointer);
        }

        FreeLibrary(h);
    }

    return ret;
}

namespace
{
	void GetEXAKEYCpu(int iea, std::string& info)
	{
		BOOL bException = FALSE;
		UINT ua,ub,uc,ud;

		__try 
		{		
			_asm 
			{
				pushfd
				pushad

				mov eax, iea
				cpuid
				mov ua, eax
				mov ub, ebx
				mov uc, ecx
				mov ud, edx	

				popad
				popfd
			}
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			bException = TRUE;
		}

		if (!bException)
		{
			char buf[64];
			int  len = sprintf(buf, "%d:%.8x %.8x %.8x %.8x  ",iea,ua,ub,uc,ud);
			info.append(buf, len);
		}
	}

//=========== mac =========
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER	NameBuff[30];

	} ASTAT, * PASTAT;

	bool getMac(int lana_num, std::string& info)
	{
		// 定义一个存放返回网卡信息的变量
		ASTAT Adapter;
		NCB ncb;
		UCHAR uRetCode;
		memset( &ncb, 0, sizeof(ncb) );
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_num ;// 指定网卡号
		// 首先对选定的网卡发送一个NCBRESET命令,以便进行初始化
		uRetCode = dwNetbios( &ncb );
		memset( &ncb, 0, sizeof(ncb) );

		ncb.ncb_command = NCBASTAT;

		ncb.ncb_lana_num = lana_num; // 指定网卡号

		strcpy((char *)ncb.ncb_callname,"*" );

		ncb.ncb_buffer = (unsigned char *) &Adapter; // 指定返回的信息存放的变量

		ncb.ncb_length = sizeof(Adapter);

		// 接着,可以发送NCBASTAT命令以获取网卡的信息
		uRetCode = dwNetbios(&ncb);
		if (uRetCode != 0)
		{
			return false;
		}

		// 把网卡MAC地址格式化成常用的16进制形式,如0010A4E45802
		char buf[64];
		int len = sprintf(buf, "%02d%02X%02X%02X%02X%02X%02X",	
			lana_num % 100,
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]);
		info.append(buf, len);
		return true;
	}

	unsigned __stdcall getWmiThread(void* pParam)
	{
		PERFLOG;
		(void)pParam;

		if (::InterlockedExchangeAdd(&g_wmiFlag, 0) != 1)
			return 0;

		HRESULT hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
		if (SUCCEEDED(hr))
		{
			CWinWmi wmi;
			if (wmi.Init())
			{
				std::wstring tmp;

				for (int i = 0; g_wmiList[i] != NULL; i += 2)
				{
					tmp += wmi.GetInfo(g_wmiList[i], g_wmiList[i+1]);
				}

				g_wmiStr  = DwUtility::text::toNarrowString(tmp.c_str(), tmp.length());
				::InterlockedExchange(&g_wmiFlag, 2);
				g_wmiFlag = 2; // 完成
			}

			wmi.Uninit();

			::CoUninitialize();
		}
        return 0;
    }
}

namespace DwUtility
{
	std::string  minfo::getCpuInfo()
	{
		PERFLOG;
		WORD  index    = 0;
		DWORD features = 0;

		try 
		{
			index = wincpuidext();
		} catch (...)
		{
		}

		try 
		{
			features = wincpufeatures();
		} catch (...)
		{
		}

		// 12位的cpuid信息	
		char buf[32] = {0};
		sprintf(buf, "%.4x%.8x", index, features);
		return buf;
	}

	std::string  minfo::getCpuALLInfo()
	{
		std::string info;
		for (int i=0;i<5;i++)
		{
			GetEXAKEYCpu(i, info);
		}
		return info;
	}

	BOOL __stdcall getMacWithAdapter(IP_ADAPTER_INFO* AdapterInfo , DWORD& dwSize)
	{
		return (ERROR_SUCCESS == dwGetAdaptersInfo( // Call GetAdapterInfo
			AdapterInfo, // [out] buffer to receive data
			&dwSize))?TRUE:FALSE; // [in] size of receive data buffer
	}

// Fetches the MAC address and prints it
	BOOL __stdcall getMacStringWithAdapter(std::string& address , BOOL bALL)
	{
		IP_ADAPTER_INFO AdapterInfo[32] = {0}; // Allocate information
		DWORD dwBufLen = sizeof(AdapterInfo); // Save memory size of buffer

		if( !getMacWithAdapter(AdapterInfo , dwBufLen) )
		{
			return FALSE;
		}

		// valid, no buffer overflow
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to		
		do 
		{
			char byte[10] = {0};
			sprintf(byte ,"%02d",pAdapterInfo->Index % 100);
			address += byte;
			for (UINT i = 0; i < pAdapterInfo->AddressLength; i++) 
			{	
				byte[2] = byte[3] = '\0';
				sprintf( byte ,"%02X", pAdapterInfo->Address[i] );
				address += byte;
			}
			if( !bALL )
			{// once get one
				break;
			}
			pAdapterInfo = pAdapterInfo->Next; // Progress through
		}while(pAdapterInfo); // Terminate if last adapter

		return TRUE;
	}

	std::string  minfo::getMacInfo()
	{
		std::string info; // 返回值

		NCB ncb;
		UCHAR uRetCode;
		LANA_ENUM lana_enum;
		memset( &ncb, 0, sizeof(ncb) );
		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *) &lana_enum;
		ncb.ncb_length = sizeof(lana_enum);

		// 向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡、每张网卡的编号等
		uRetCode = dwNetbios( &ncb );

		if (uRetCode == 0)
		{
			// 对每一张网卡,以其网卡编号为输入编号,获取其MAC地址
			for (int i=0; i<lana_enum.length; ++i)
			{
				if (getMac(lana_enum.lana[i], info))
				{
					break;
				}
			}
		}
		else
		{
			getMacStringWithAdapter(info,FALSE);
		}

		return info;
	}

	std::string minfo::getMacInfo( const std::string &ipAddr )
	{
		std::string info;
		IP_ADAPTER_INFO AdapterInfo[32] = {0}; // Allocate information
		DWORD dwBufLen = sizeof(AdapterInfo); // Save memory size of buffer

		if( !getMacWithAdapter(AdapterInfo , dwBufLen) )
		{
			return info;
		}

		// valid, no buffer overflow
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
		while(pAdapterInfo)
		{
			if ( 0 == ipAddr.compare(pAdapterInfo->IpAddressList.IpAddress.String) )
			{
				char byte[16] = {0};
				int len = 0;
 				len = sprintf(byte, "%02d", pAdapterInfo->Index % 100);
 				if ( len < 0 )
 				{
					byte[0] = byte[1] = '0';
 				}
				info.append(byte, 2);
				for (UINT i = 0; i < pAdapterInfo->AddressLength; i++) 
				{	
					byte[2] = byte[3] = '\0';
					len = sprintf(byte, "%02X", pAdapterInfo->Address[i]);
					info.append(byte, len);
				}
				break;
			}
			pAdapterInfo = pAdapterInfo->Next; // Progress through
		}

		return info;
	}

	std::string  minfo::getMacAllInfo()
	{
		PERFLOG;
		std::string info; // 返回值

		NCB ncb;
		UCHAR uRetCode;
		LANA_ENUM lana_enum;
		memset( &ncb, 0, sizeof(ncb) );
		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *) &lana_enum;
		ncb.ncb_length = sizeof(lana_enum);

		// 向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡、每张网卡的编号等
		uRetCode = dwNetbios( &ncb );

		if (uRetCode == 0)
		{
			// 对每一张网卡,以其网卡编号为输入编号,获取其MAC地址
			for (int i=0; i<lana_enum.length; ++i)
			{
				getMac(lana_enum.lana[i], info);
			}
		}
		else
		{
		   getMacStringWithAdapter(info,TRUE);
		}

		return info;
	}

	std::string  minfo::getWMIInfo()
	{
		if (::InterlockedCompareExchange(&g_wmiFlag, 1, 0) == 0)
		{
			PERFLOG;

			LogDev("getWMIInfo") << "getWMIInfo begin: open thread to getWMIInfo";

			HANDLE h = (HANDLE)_beginthreadex(0, 0, getWmiThread, 0, NULL, NULL);
			::CloseHandle(h);

			LogDev("getWMIInfo") << "getWMIInfo end";
		}

		std::string str;
		if (::InterlockedExchangeAdd(&g_wmiFlag, 0) == 2)
		{
			str = g_wmiStr;
		}
		return str;
	}

	std::string  minfo::getHDinfo()
	{
		PERFLOG;
		std::string info; // 返回值

		BYTE szSystemInfo[4096];
		UINT uSystemInfoLen = 0;

		{
			OSVERSIONINFO ovi = { 0 };
			ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx( &ovi );

			if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT )
			{
				// Only Windows 2000, Windows XP, Windows Server 2003...
				if (!WinNTHDSerialNumAsPhysicalRead(szSystemInfo, &uSystemInfoLen, 1024))
				{
					WinNTHDSerialNumAsScsiRead(szSystemInfo, &uSystemInfoLen, 1024);
				}
			}
		}

		if (uSystemInfoLen > 0)
		{
			char* temp = new char[uSystemInfoLen+1];
			int count = 0;
			for (int i=uSystemInfoLen-1;i>0;i--)
			{
				if (szSystemInfo[i]!=' ')
				{
					temp[count] = szSystemInfo[i];
					++count;
				}
			}
			temp[count] = 0;
			info = temp;
			delete[] temp;
		}
		return info;
	}

	std::string  minfo::getBiosInfo()
	{
		PERFLOG;
		std::string info; // 返回值

		SIZE_T ssize; 

		LARGE_INTEGER so; 
		so.LowPart=0x000f0000;
		so.HighPart=0x00000000; 
		ssize=0xffff; 
		wchar_t strPH[30]=L"\\device\\physicalmemory"; 

		DWORD ba=0;

		UNICODE_STRING struniph; 
		struniph.Buffer=(wchar_t *)strPH; 
		struniph.Length=0x2c; 
		struniph.MaximumLength =0x2e; 

		OBJECT_ATTRIBUTES obj_ar; 
		obj_ar.Attributes =64;
		obj_ar.Length =24;
		obj_ar.ObjectName=&struniph;
		obj_ar.RootDirectory=0; 
		obj_ar.SecurityDescriptor=0; 
		obj_ar.SecurityQualityOfService =0; 

		HMODULE hinstLib = LoadLibraryA("ntdll.dll"); 
		ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection"); 
		ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection"); 
		ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection"); 
		//调用函数，对物理内存进行映射 
		HANDLE hSection; 
		if( 0 == ZWopenS(&hSection,4,&obj_ar))
		{
			if ( 
				0 == ZWmapV( 
				( HANDLE )hSection,   //打开Section时得到的句柄 
				( HANDLE )(DWORD_PTR)0xFFFFFFFF, //将要映射进程的句柄， 
				&ba,                  //映射的基址 
				0,
				0xFFFF,               //分配的大小 
				&so,                  //物理内存的地址 
				&ssize,               //指向读取内存块大小的指针 
				1,                    //子进程的可继承性设定 
				0,                    //分配类型 
				2                     //保护类型 
				) )
				//执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里 
				//映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射 
			{		
				BYTE* pBiosSerial = ( BYTE* )(DWORD_PTR)ba;
				UINT uBiosSerialLen = FindAwardBios( &pBiosSerial );
				if( uBiosSerialLen == 0U )
				{
					uBiosSerialLen = FindAmiBios( &pBiosSerial );
					if( uBiosSerialLen == 0U )
					{
						uBiosSerialLen = FindPhoenixBios( &pBiosSerial );
					}
				}
				if( uBiosSerialLen != 0U )
				{
					info.append((const char*)pBiosSerial, uBiosSerialLen);
				}
				ZWunmapV((HANDLE)(DWORD_PTR)0xFFFFFFFF, (void*)(DWORD_PTR)ba);
			}
		}
		return info;
	}

	std::string minfo::getMid()
	{
		PERFLOG;
		if (g_Mid.empty())
		{
			std::string info;
			std::string temp;

			// cpu all info 包含了 cpu info
			temp = getCpuInfo();
			info += temp;

			// mac all info 包含了 mac info
			temp = getMacAllInfo();
			info += temp;

			// wmi
			temp = getWMIInfo();
			info += temp;

			// hd
			try
			{
				temp = getHDinfo();
			}
			catch (...)
			{
			}
			info += temp;

			// bios
			try
			{
				temp = getBiosInfo();
			}
			catch (...)
			{
			}
			info += temp;

			// 算md5
			CMachineInfo::md5(info.c_str(), info.length(), g_Mid);
		}
		return g_Mid;
	}

	void minfo::getMid2(std::string& mid, std::string& midstr)
	{
		PERFLOG;
		std::string info;
		std::string temp;
		int num = 0;

		// cpu all info 包含了 cpu info
		temp = getCpuInfo();
		info += temp;
		if (!temp.empty())
		{
			++num;
		}

		// mac all info 包含了 mac info
		temp = getMacAllInfo();
		info += temp;
		if (!temp.empty())
		{
			++num;
		}

		// wmi
		temp = getWMIInfo();
		info += temp;
		if (!temp.empty())
		{
			++num;
		}

		// hd
		try
		{
			temp = getHDinfo();
		}
		catch (...)
		{
		}
		info += temp;
		if (!temp.empty())
		{
			num = 10; // 有这个就行了
		}

		// bios
		try
		{
			temp = getBiosInfo();
		}
		catch (...)
		{
		}
		info += temp;
		if (!temp.empty())
		{
			++num;
		}

		// 算md5，这是真实的mid
		CMachineInfo::md5(info.c_str(), info.length(), mid);
		g_Mid = mid; // 用重新算的这个，理论上是一样的

		// 如果没有3项以上可以取到的话生成一个guid
		if (num < 3)
		{
			GUID guid;
			if (SUCCEEDED(CoCreateGuid(&guid)))
			{
				CMachineInfo::md5((const char*)&guid, sizeof(guid), midstr);
			}
		}
		else
		{
			midstr = mid; // 认为这个是可信的
		}
	}

	std::string minfo::getGlobalMid()
	{
		// 是否已经得到了
		if (!g_GlobalMid.empty())
		{
			return g_GlobalMid;
		}

		// 文件名
		std::string mac = getMacInfo();
		if (mac.empty())
		{
			mac = IF_MAC_NULL;
		}

		// 读文件
		std::string mid;
		std::string midstr;
		getMid2(mid, midstr);

		if (!CMachineInfo::ReadGlobalMid(mac.c_str(), mid, g_GlobalMid))
		{
			// 读文件错误了就用mid
			g_GlobalMid = midstr;
		}

		// 文件成功的时候是guid
		return g_GlobalMid;
	}

	bool minfo::createGlobalMid()
	{
		PERFLOG;
		// 文件名
		std::string mac = getMacInfo();
		if (mac.empty())
		{
			mac = IF_MAC_NULL;
		}

		std::string guidstr; // guid

		// 计算mid
		std::string mid = getMid();

		// 读文件
		if (!CMachineInfo::HasGlobalMidFile(mac.c_str()))
		{
			// 处理兼容问题
			CMachineInfo::ReadGuid(guidstr);
		}
		else
		{
			// 有新的文件
			// 那么兼容问题肯定已经处理过了
			std::string gmid;
			if (CMachineInfo::ReadGlobalMid(mac.c_str(), mid, gmid))
			{
				// 新的文件读也成功了
				g_GlobalMid = gmid;
				return true;
			}
		}

		// 需要重新生成
		if (guidstr.empty())
		{
			// 生成一个guid
			GUID guid;
			if (FAILED(CoCreateGuid(&guid)))
			{
				return false;
			}
			CMachineInfo::md5((const char*)&guid, sizeof(guid), guidstr);
		}

		// 根据mid和guid生成文件
		if (CMachineInfo::WriteGlobalMid(mac.c_str(), mid, guidstr))
		{
			// 生成文件成功了
			g_GlobalMid = guidstr;
			return true;
		}
		return false;
	}

	std::string  getMacAllInfo2()
	{
		PERFLOG;

		std::string info;

		NCB ncb = {0};
		LANA_ENUM lana_enum = {0};
		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *) &lana_enum;
		ncb.ncb_length = sizeof(lana_enum);

		// 向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡、每张网卡的编号等
		UCHAR uRetCode = dwNetbios( &ncb );

		if (uRetCode == 0)
		{
			// 对每一张网卡,以其网卡编号为输入编号,获取其MAC地址
			std::vector<std::string> macs;
			for (int i = 0; i < lana_enum.length; ++i)
			{
				std::string mac;
				getMac(lana_enum.lana[i], mac);
				macs.push_back(mac);
			}

			std::sort(macs.begin(), macs.end(), std::less<std::string>());

			for (size_t i = 0; i < macs.size(); i++)
			{
				info += macs[i];
			}
		}
		else
		{
			getMacStringWithAdapter(info, TRUE);
		}

		return info;
	}

	std::string minfo::getMid3()
	{
		PERFLOG;
		if (g_Mid3.empty())
		{
			std::string info;

			info += getCpuInfo();
			info += getMacAllInfo2();
			info += getWMIInfo();

			try
			{
				info += getHDinfo();
			}
			catch (...)
			{
			}

			// bios
			try
			{
				info += getBiosInfo();
			}
			catch (...)
			{
			}

			DwUtility::encryption::MD5CryptA(info.c_str(), info.length(), g_Mid3);
		}
		return g_Mid3;
	}

	bool minfo::getRunEnv(const std::wstring& netbarlists)
	{
		if(g_isNetBar==-1)
		{
			/*收集当前运行EXE*/
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (INVALID_HANDLE_VALUE == hSnapshot)
			{
				g_isNetBar = 1;
				return true;
			}


			struct wstr_less
			{
				bool operator()(const std::wstring& _Left, const std::wstring& _Right) const
				{
					return _wcsicmp(_Left.c_str(), _Right.c_str()) < 0;//忽略大小写
				}
			};

			struct MODULE_INFO
			{
				std::wstring strExe;
				std::wstring strModule;
			};

			std::set<std::wstring, wstr_less> setExeName;

			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);
			BOOL bRun = Process32First(hSnapshot, &pe32);
			while(bRun)
			{
				if (0 != pe32.th32ProcessID)
				{
					setExeName.insert(pe32.szExeFile);
				}

				bRun = Process32Next(hSnapshot, &pe32);
			}

			CloseHandle(hSnapshot);

			std::vector<MODULE_INFO> vecInfo;
			vecInfo.reserve(120);

			int nPos = 0;
			std::wstring strSub;
			int nFind = netbarlists.find(_T("|"));
			while(nFind != std::wstring::npos )
			{
				MODULE_INFO infoModule;
				strSub = netbarlists.substr(nPos, nFind - nPos);
				nPos = nFind + 1; //_T("|")

				nFind = strSub.find(_T("&"));
				if (nFind == std::wstring::npos)
				{
					infoModule.strExe = strSub;
				}
				else
				{
					infoModule.strExe = strSub.substr(0, nFind);
					infoModule.strModule = strSub.substr(nFind + 1);
				}

				vecInfo.push_back(infoModule);
				nFind = netbarlists.find(_T("|"), nPos);
			}

			/*检验是否存在网吧维护程序*/
			for(unsigned int i = 0; i < vecInfo.size(); i++)
			{
				std::wstring exeName = vecInfo[i].strExe;
				std::transform(exeName.begin(), exeName.end(), exeName.begin(), ::tolower);
				if (setExeName.end() != setExeName.find(exeName.c_str()))
				{
					if (!vecInfo[i].strModule.empty())
					{
						std::wstring moduleName = vecInfo[i].strModule;
						std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), tolower);
						if (setExeName.end() != setExeName.find(moduleName.c_str()))
						{
							g_isNetBar = 1;
							return true;
						}
						g_isNetBar = 0;
						return false;
					}

					g_isNetBar = 1;
					return true;
				}
			}
			g_isNetBar = 0;
			return false;
		}
		else
		{
			return g_isNetBar==1;
		}
	}
}