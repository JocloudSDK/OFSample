#include "stable.h"
#include <assert.h>
#include <tchar.h>
#include "struct.h"


namespace Perf
{
	typedef NTSTATUS (__stdcall *NTQUERYSYSTEMINFORMATION) (
		__in       SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__inout    PVOID SystemInformation,
		__in       ULONG SystemInformationLength,
		__out_opt  PULONG ReturnLength);	

	typedef NTSTATUS (__stdcall *NTQUERYINFOMATIONPROCESS) (
			IN HANDLE ProcessHandle,
			IN int ProcessInformationClass,
			OUT PVOID ProcessInformation,
			IN ULONG ProcessInformationLength,
			OUT PULONG ReturnLength OPTIONAL);	

	static BOOL GetMemoryUsageEx(DWORD dwPid, HANDLE hProcess, DWORD * lpMemUse, DWORD * lpVMSize)
	{
		DW_UNUSED(dwPid);
		*lpMemUse = *lpVMSize = 0;

		static HMODULE hNtdll = GetModuleHandle(_T("ntdll.dll"));
		static NTQUERYINFOMATIONPROCESS pZwQueryInformationProcess = (NTQUERYINFOMATIONPROCESS)GetProcAddress(hNtdll, "ZwQueryInformationProcess");
		
		if (!pZwQueryInformationProcess)
			return FALSE;

		BOOL bRet = FALSE;
		DWORD rtLen = 0;
		VM_COUNTERS vmc = {0};
		NTSTATUS st = pZwQueryInformationProcess(hProcess, ProcessVmCounters, &vmc, sizeof(vmc)-4, &rtLen);
		if(st >= 0)
		{
			*lpMemUse = vmc.WorkingSetSize;
			*lpVMSize = vmc.PagefileUsage;
			bRet = TRUE;
		}

		st = pZwQueryInformationProcess(hProcess,ProcessVmCounters, &vmc, sizeof(vmc), &rtLen);
		if(st >= 0 && rtLen == sizeof(vmc))
		{
			*lpMemUse = vmc.WorkingSetSize;
			*lpVMSize = vmc.PrivateUsage;
			bRet = TRUE;
		}	

		return bRet;
	}

	void GetMemoryUsage(DWORD * lpMemUse, DWORD * lpVMSize)
	{
		GetMemoryUsageEx(GetCurrentProcessId(), GetCurrentProcess(), lpMemUse, lpVMSize);
	}
}
