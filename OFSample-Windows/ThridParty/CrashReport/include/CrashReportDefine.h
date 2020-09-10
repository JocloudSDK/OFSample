/** 
@file
@brief CrashReport.dll��Bugreport.exeͨ���ýӿڶ���
@version 2011-7-10 bobdeng
*/

#pragma once

//�ڲ��ã�ʹ���������ע
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

	BOOL							bShowReportUI; //��Crash��Ч
	struct _EXCEPTION_POINTERS *	pExceptionInfo;//��Crash��Ч
	HANDLE							evDumpComplete;//��Crash��Ч

	char							szRestartExePath[MAX_PATH];

	char							szDynaReleaseVer[MAX_PATH]; //��̬�����汾

	struct CUSTOM_DATA
	{
		void* lpCustomData;
		int cchCustomData;
	} curstomData[MAX_CUSTOMDATA_NUMS];
	int								curCustomDataIdx;

    char							lpszExcepModuleVer[MAX_PATH];
    char							lpszHiidoModuleId[MAX_PATH];
    char							lpszModuleDescName[MAX_PATH];

	unsigned __int64				ullTotalVirtualMem; //�ܹ������ڴ�.
	unsigned __int64				ullAvailVirtualMem; //ʣ�������ڴ�.
	DWORD							dwGdiHandles;
	DWORD							dwUserOjbect;
};

