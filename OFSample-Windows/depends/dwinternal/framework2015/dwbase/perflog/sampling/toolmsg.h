/** 
@file
@brief �ɼ�ģ��͹��߽�����Ϣ����
@version 2011-9-7 bobdeng
		 2012-8-30 wesleywu   add Network & Disk
*/

#pragma once

#define WM_APP_PERF (WM_APP + 0xf)
const wchar_t c_szPerfWndNameFmt[] = L"PerfSamplingWndName_%d";

namespace Perf
{
	enum eCMD_ID
	{
		eCmdAddReceiver,
		eCmdDelReceiver,
		eCmdFilePathName,
		eCmdEndPerfLog,
	};

	enum eMSG_ID
	{
		eMsgCPU,
		eMsgMem,
		eMsgKa,
		eMsgDisk,
		eMsgNetwork
	};

	struct MSG_CPU
	{
		DWORD				dwNumProcessor;
		DWORD				dwTID;		//�߳�ID, 0��ʾ���̣���1��ʾϵͳ
		LONGLONG			llPerfCount;
		DWORD				dwUserTime;
		DWORD				dwKernelTime;
	};

	struct MSG_MEM
	{
		LONGLONG			llPerfCount;
		DWORD				dwMemUse;
		DWORD				dwVMSize; 
	};

	struct MSG_DISK
	{
		LONGLONG			llPerfCount;
		DWORD				dwDiskWriteBytesPerSec;
		DWORD				dwDiskReadBytesPerSec;
		DWORD				dwDiskQueueLength;
	};

	struct MSG_NETWORK
	{
		LONGLONG			llPerfCount;
		DWORD				dwBytesSentPerSec;
		DWORD				dwBytesReceivedPerSec;
	};

	struct MSG_KA			//�߳̿�
	{
		DWORD		dwTID;
		char		szThreadName[512];
		LONGLONG	llPerfCountBegin;
		LONGLONG	llPerfCountEnd;
	};
}
