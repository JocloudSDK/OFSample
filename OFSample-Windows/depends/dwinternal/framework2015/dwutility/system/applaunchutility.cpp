#include <windows.h>
#include <tchar.h>


/*
设置某个token的权限
*/
bool SetPrivilege(
		  HANDLE hToken,          
		  LPCTSTR lpszPrivilege, 
		  DWORD dwAttributes=SE_PRIVILEGE_ENABLED   
		  ) 
{
	bool bRet = false;
	LUID luid;

	if ( LookupPrivilegeValue( 
			NULL,            // lookup privilege on local system
			lpszPrivilege,   // privilege to lookup 
			&luid ) )        // receives LUID of privilege
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = dwAttributes;

		// Enable the privilege or disable all privileges.

		if (AdjustTokenPrivileges(
				hToken, 
				FALSE, 
				&tp, 
				sizeof(TOKEN_PRIVILEGES), 
				(PTOKEN_PRIVILEGES) NULL, 
				(PDWORD) NULL))
		{
			bRet = true;
		}
	}

	return bRet;
}

/*!
Function removes the priveleges which are not associated by default with explorer.exe at Medium Integration Level in Vista
@returns HRESULT of the operation on SE_CREATE_GLOBAL_NAME (="SeCreateGlobalPrivilege")
*/
inline bool ReducePrivilegesForMediumIL(HANDLE hToken) 
{
	bool bRet = false;
	bRet = SetPrivilege(hToken, SE_CREATE_GLOBAL_NAME, SE_PRIVILEGE_REMOVED);

	SetPrivilege(hToken, SE_BACKUP_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_CREATE_PAGEFILE_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, TEXT("SeCreateSymbolicLinkPrivilege"), SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_DEBUG_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_IMPERSONATE_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_INC_BASE_PRIORITY_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_INCREASE_QUOTA_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_LOAD_DRIVER_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_MANAGE_VOLUME_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_PROF_SINGLE_PROCESS_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_REMOTE_SHUTDOWN_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_RESTORE_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_SECURITY_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_SYSTEM_ENVIRONMENT_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_SYSTEM_PROFILE_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_SYSTEMTIME_NAME, SE_PRIVILEGE_REMOVED);
	SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, SE_PRIVILEGE_REMOVED);

	return bRet;
}


bool isVista()
{
	bool bVista = false;
	// When the function is called from IS12, GetVersionEx returns dwMajorVersion=5 on Vista!
	HMODULE hmodKernel32=LoadLibrary(L"Kernel32");
	if(hmodKernel32 && GetProcAddress(hmodKernel32, "GetProductInfo"))
	{
		bVista=true;
	}

	if(hmodKernel32) 
	{
		FreeLibrary(hmodKernel32);
	}

	return bVista;
}

/*!
@brief Gets Integration level of the given process in Vista. 
In the older OS assumes the integration level is equal to SECURITY_MANDATORY_HIGH_RID

The function opens the process for all access and opens its token for all access. 
Then it extracts token information and closes the handles.
@param[in] dwProcessId ID of the process to operate
@param[out] pdwProcessIL pointer to write the value
@return HRESULT
@retval <return value> { description }
@remarks Function check for OS version by querying the presence of Kernel32.GetProductInfo function. 
This way is used due to the function is called from InstallShield12 script, so GetVersionEx returns incorrect value.
@todo restrict access rights when quering for tokens
*/
bool GetProcessIL(DWORD dwProcessId, LPDWORD pdwProcessIL)
{
	bool bRet = false;
	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;
	PTOKEN_MANDATORY_LABEL pTIL=NULL;
	do
	{	
		if(!pdwProcessIL)
		{
			break;
		}

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, FALSE, dwProcessId);
		if (hProcess == NULL)
		{
			break;
		}

		if(!OpenProcessToken(hProcess, TOKEN_QUERY_SOURCE|TOKEN_QUERY/*TOKEN_ALL_ACCESS*/, &hToken))
		{
			break;
		}
		DWORD dwSize=0;
		if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwSize) 
			&& ERROR_INSUFFICIENT_BUFFER==GetLastError() && dwSize)
		{
			pTIL=(PTOKEN_MANDATORY_LABEL)HeapAlloc(GetProcessHeap(), 0, dwSize);
		}

		if(pTIL && GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwSize, &dwSize))
		{
			LPBYTE lpb=GetSidSubAuthorityCount(pTIL->Label.Sid);
			if(lpb == NULL)
			{
				break;
			}
			*pdwProcessIL = *GetSidSubAuthority(pTIL->Label.Sid, *lpb-1);
		}
		bRet = true;
	}while (false);

	if(pTIL != NULL)
	{
		HeapFree(GetProcessHeap(), 0, pTIL);
	}

	if (hToken != NULL)
	{
		CloseHandle(hToken);
	}

	if (hProcess != NULL)
	{
		CloseHandle(hProcess);
	}
	return bRet;
}

/*!
@brief Function launches process with the integration level of Explorer on Vista. On the previous OS, simply creates the process.

Function gets the integration level of the current process and Explorer, then launches the new process.
If the integration levels are equal, CreateProcess is called. 
If Explorer has Medium IL, and the current process has High IL, new token is created, its rights are adjusted 
and CreateProcessWithTokenW is called. 
If Explorer has Medium IL, and the current process has High IL, error is returned.
@param[in] szProcessName - the name of exe file (see CreatePorcess()) 
@param[in] szCmdLine - the name of exe file (see CreatePorcess())
@return HRESULT code
@note The function cannot be used in services, due to if uses USER32.FindWindow() to get the proper instance of Explorer. 
The parent of new process in taskmgr.exe, but not the current process.
@sa ReducePrivilegesForMediumIL()
*/
bool CreateProcessWithExplorerIL(LPWSTR szProcessName, LPWSTR szCmdLine, DWORD createFlags)
{
	HRESULT hr=S_OK;
	PROCESS_INFORMATION ProcInfo = {0};
	STARTUPINFO StartupInfo = {0};
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	HANDLE hNewToken = NULL;
	HINSTANCE hmodAdvApi32 = NULL;

	do
	{
		if (!isVista())
		{
			break;
		}

		DWORD dwCurIL=SECURITY_MANDATORY_HIGH_RID; 
		DWORD dwExplorerID=0, dwExplorerIL=SECURITY_MANDATORY_HIGH_RID;

		HWND hwndShell=::FindWindow( _T("Progman"), NULL);
		if(hwndShell == NULL)
		{
			break;
		}
		GetWindowThreadProcessId(hwndShell, &dwExplorerID);
		
		if (!GetProcessIL(dwExplorerID, &dwExplorerIL)
			|| !GetProcessIL(GetCurrentProcessId(), &dwCurIL))
		{
			break;
		}

		if (dwCurIL!=SECURITY_MANDATORY_HIGH_RID 
			|| dwExplorerIL!=SECURITY_MANDATORY_MEDIUM_RID)
		{
			break;
		}

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, FALSE, dwExplorerID);
		if (hProcess == NULL)
		{
			break;
		}

		if (!OpenProcessToken(hProcess, TOKEN_QUERY_SOURCE|TOKEN_QUERY|TOKEN_DUPLICATE/*TOKEN_ALL_ACCESS*/, &hToken))
		{
			break;
		}

		if(!DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL,
			SecurityImpersonation, TokenPrimary, &hNewToken))
		{
			break;
		}

		if(dwCurIL==SECURITY_MANDATORY_MEDIUM_RID && dwExplorerIL==SECURITY_MANDATORY_MEDIUM_RID)
		{
			hr = ReducePrivilegesForMediumIL(hNewToken);
		}//if(dwCurIL==...)

		typedef BOOL (WINAPI *LPFN_CreateProcessWithTokenW)(
			HANDLE hToken,
			DWORD dwLogonFlags,
			LPCWSTR lpApplicationName,
			LPWSTR lpCommandLine,
			DWORD dwCreationFlags,
			LPVOID lpEnvironment,
			LPCWSTR lpCurrentDirectory,
			LPSTARTUPINFOW lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInfo
			);
		LPFN_CreateProcessWithTokenW fnCreateProcessWithTokenW=NULL;
		
		hmodAdvApi32=LoadLibraryA("AdvApi32");
		if(hmodAdvApi32 == NULL)
		{
			break;
		}
		
		fnCreateProcessWithTokenW=(LPFN_CreateProcessWithTokenW)GetProcAddress(hmodAdvApi32, "CreateProcessWithTokenW");
		if(fnCreateProcessWithTokenW == NULL)
		{
			break;
		}
		
		LPTCH env = ::GetEnvironmentStrings();

		DWORD dwSize = ::GetCurrentDirectory(0, NULL);
		TCHAR *curDir = new TCHAR[dwSize+1];
		::ZeroMemory(curDir, sizeof(TCHAR) * (dwSize + 1));
		::GetCurrentDirectory(dwSize, curDir);

		fnCreateProcessWithTokenW(hNewToken, 0, 
			szProcessName, szCmdLine, 
			createFlags, env, curDir, &StartupInfo, &ProcInfo);
		CloseHandle(ProcInfo.hProcess);
		CloseHandle(ProcInfo.hThread);
		//CreateProcessAsUser(hNewToken, szProcessName, szCmdLine, NULL, NULL, FALSE, createFlags, NULL, NULL, &StartupInfo, &ProcInfo);

		::FreeEnvironmentStrings(env);
		delete []curDir;

	}while(false);

	if(hmodAdvApi32 != NULL)	
	{
		FreeLibrary(hmodAdvApi32);
	}

	if (hNewToken != NULL)
	{
		CloseHandle(hNewToken);
	}

	if (hToken != NULL)
	{
		CloseHandle(hToken);
	}

	if (hProcess != NULL)
	{
		CloseHandle(hProcess);
	}

	if(ProcInfo.dwProcessId == 0)
	{// 2K | XP | Vista & !UAC
		CreateProcess(szProcessName, szCmdLine, 
			NULL, NULL, FALSE, createFlags, NULL, NULL, &StartupInfo, &ProcInfo);
	}// 2K | XP | Vista & !UAC

	if (ProcInfo.hProcess != NULL)
	{
		CloseHandle(ProcInfo.hProcess);
	}

	if (ProcInfo.hThread != NULL)
	{
		CloseHandle(ProcInfo.hThread);
	}

	return ProcInfo.dwProcessId != 0;
}

