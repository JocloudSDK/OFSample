
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <string>
#include <vector>

#include "dwglobal.h"

namespace DwUtility
{
	namespace system
	{
		struct UIPIMsgFilters
		{
			DWORD message;
			bool  isAdd;
		};

		bool DW_DWUTILITY_EXPORT isPlayingGame();
		bool DW_DWUTILITY_EXPORT isPlayingGameEx1();
		bool DW_DWUTILITY_EXPORT isPlayingGameEx2();
		bool DW_DWUTILITY_EXPORT changeUIPIMessageFilter(const UIPIMsgFilters* msgFilters, DWORD cnt);
		bool DW_DWUTILITY_EXPORT easyRunDll(const std::wstring &path, const std::string &method);
		bool DW_DWUTILITY_EXPORT deleteFolder(const std::wstring& folder);

		bool DW_DWUTILITY_EXPORT openUrl(const std::wstring& url);

		bool DW_DWUTILITY_EXPORT createProcessLowPriority(const std::wstring &szProcessName, const std::wstring &szCmdLine, DWORD createFlags);

		void DW_DWUTILITY_EXPORT forceWindowToTop(HWND hwnd);

		bool DW_DWUTILITY_EXPORT IsVistaOrWin7();
		bool DW_DWUTILITY_EXPORT IsVersionXP();

		int DW_DWUTILITY_EXPORT EnableDebugPriv(LPCTSTR lpName);

		bool DW_DWUTILITY_EXPORT TrySetDllDirectory(const std::wstring& strPath);

		DW_DWUTILITY_EXPORT const OSVERSIONINFOEX& GetWindowsVersion();

		class DW_DWUTILITY_EXPORT Mutex {
			public:
				Mutex(const wchar_t *name = NULL);
				~Mutex();
				bool Lock();
				bool LockWithTimeout(DWORD millisecond);
				void Unlock();
			private:
				HANDLE m_handle;
				bool m_locked;
		};

		class DW_DWUTILITY_EXPORT MutexLocker {
			public:
				explicit MutexLocker(Mutex* mutex);
				~MutexLocker();
			private:
				Mutex* m_mutex;
		};

		const int KBeforeLoadLibrary = 1;
		const int KMD5CheckStage = 2;
		const int KLoadLibraryStage = 4;
		HMODULE DW_DWUTILITY_EXPORT loadLibraryEx(const std::wstring &path, unsigned int flag);
		typedef bool (CALLBACK *LoadLibraryExFailCallBack)(const std::wstring &path, int stage);
		void DW_DWUTILITY_EXPORT InstallLoadLibraryExFailCallBack(LoadLibraryExFailCallBack callback);
		bool DW_DWUTILITY_EXPORT md5Check(const std::wstring &path);
		std::string DW_DWUTILITY_EXPORT getCPUName();
		DWORD DW_DWUTILITY_EXPORT getCoreNumber();
		bool DW_DWUTILITY_EXPORT getGPUInfo(std::vector<std::wstring> &cards) ;

		bool DW_DWUTILITY_EXPORT killProcess(DWORD dwProgressId, HANDLE hProgress);
	}
}
