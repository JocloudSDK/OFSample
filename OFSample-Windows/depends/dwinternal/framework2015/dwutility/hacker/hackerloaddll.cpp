#include "hackerloaddll.h"
#include "detours.h"
#include <tchar.h>
#pragma warning( push )
#pragma warning( disable : 4091 )
#include <shlwapi.h>
#include <ShlObj.h>
#include <string>
#include "dwbase/log.h"
#include "file/ufile.h"

#pragma comment (lib, "Shlwapi.lib")

static HMODULE hKernelbase = GetModuleHandleW(L"kernelbase.dll");	
static HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
static void **s_pLoadLibraryExW = (void**)::GetProcAddress(hKernelbase, "LoadLibraryExW");
static void **s_pLoadLibraryExW2 = (void**)::GetProcAddress(hKernel32, "LoadLibraryExW");
static void * &fnLoadLibraryExW = s_pLoadLibraryExW != NULL ? *s_pLoadLibraryExW : *s_pLoadLibraryExW2;

DETOUR_TRAMPOLINE(HMODULE WINAPI Detour_LoadLibraryExW(LPCWSTR , HANDLE , DWORD ), fnLoadLibraryExW);

namespace DwUtility
{
	namespace hacker
	{
		std::wstring YYHackerLoadDll::m_targetFileExtName;
		std::set<std::wstring> YYHackerLoadDll::m_findNameContainList;
		std::wstring YYHackerLoadDll::m_hackerFilePath;

		YYHackerLoadDll::YYHackerLoadDll()
		{	
		}

		YYHackerLoadDll::~YYHackerLoadDll()
		{
		}

		void YYHackerLoadDll::Hook()
		{
			DetourFunctionWithTrampoline((PBYTE)Detour_LoadLibraryExW, (PBYTE)NEW_LoadLibraryExW);
		}

		void YYHackerLoadDll::SetHackerInfo(const std::wstring& targetFileExtName,const std::set<std::wstring>& findNameContainList,
			const std::wstring& hackerFilePath)
		{
				m_targetFileExtName = targetFileExtName;
				m_findNameContainList = findNameContainList;
				m_hackerFilePath = hackerFilePath;
		}

		YYHackerLoadDll *YYHackerLoadDll::m_instance = NULL;
		YYHackerLoadDll * YYHackerLoadDll::instance()
		{
			if( m_instance == NULL )
			{
				m_instance = new YYHackerLoadDll();
			}
			return m_instance;
		}

		HMODULE WINAPI YYHackerLoadDll::NEW_LoadLibraryExW( LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags )
		{
			std::wstring fileName = lpLibFileName;
			if (m_targetFileExtName.empty() || m_findNameContainList.empty() || m_hackerFilePath.empty())
			{
				return Detour_LoadLibraryExW(fileName.c_str(), hFile, dwFlags);
			}

			if (std::wstring::npos != fileName.rfind(m_targetFileExtName.c_str()))
			{
				bool bHist = false;
				for (std::set<std::wstring>::iterator it = m_findNameContainList.begin(); it != m_findNameContainList.end(); ++it)
				{
					if (std::wstring::npos != fileName.find(it->c_str()))
					{
						bHist = true;
						LogFinal(LOG::KAxhost)<<"Hist target file="<<fileName.c_str();
						break;
					}
				}

				if (bHist && DwUtility::ufile::FileExist(m_hackerFilePath))
				{
					//如果命中，并且指定加载的文件存在.
					fileName = m_hackerFilePath;
					LogFinal(LOG::KAxhost)<<"Use yy flash.ocx="<<m_hackerFilePath.c_str();
				}
			}

			return Detour_LoadLibraryExW(fileName.c_str(), hFile, dwFlags);
		}
	}
}

#pragma warning( pop )


