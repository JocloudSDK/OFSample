/** 
@file
@brief CrashReport.dll和Bugreport.exe通信用接口定义
@version 2011-7-10 bobdeng
*/

#pragma once

//内部用，使用者无需关注
enum { MAX_CUSTOMDATA_NUMS = 4, };
struct REPORT_PARAMS
{
	DWORD							dwReportId;

	char							szProductId[MAX_PATH];
	char							szProductVer[MAX_PATH];
	char							szProductVerDetail[MAX_PATH];

	char							szProductName[MAX_PATH];

	char							szUserId[MAX_PATH];

	DWORD							dwPID;
	DWORD							dwTID;

	char *							lpszCustomProperty;
	int								cchCustomProperty;

	char *							lpszCustomFilePaths;
	int								nFileCount;	

	char *							lpszCallStack;
	int								cchCallStack;

	char							szExcepModPath[MAX_PATH];

	BOOL							bShowReportUI; //仅Crash有效
	struct _EXCEPTION_POINTERS *	pExceptionInfo;//仅Crash有效
	HANDLE							evDumpComplete;//仅Crash有效

	char							szRestartExePath[MAX_PATH];

	char							szDynaReleaseVer[MAX_PATH]; //动态发布版本

	struct CUSTOM_DATA
	{
		void* lpCustomData;
		int cchCustomData;
	} curstomData[MAX_CUSTOMDATA_NUMS];
	int								curCustomDataIdx;

    char							lpszExcepModuleVer[MAX_PATH];
    char							lpszHiidoModuleId[MAX_PATH];
    char							lpszModuleDescName[MAX_PATH];

	unsigned __int64				ullTotalVirtualMem; //总共虚拟内存.
	unsigned __int64				ullAvailVirtualMem; //剩余虚拟内存.
	DWORD							dwGdiHandles;
	DWORD							dwUserOjbect;
};

