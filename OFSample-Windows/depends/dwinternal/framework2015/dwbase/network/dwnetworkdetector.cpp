#include "stable.h"

#include <process.h>

#include <iphlpapi.h>
#include <winsock2.h>
#include <IPExport.h>
#include <icmpapi.h>

#include "dwbase/log.h"
#include "dwbase/dwqueuedfunc.h"
#include "dwnetworkdetector.h"

struct NETWORKDETECT_SIGNAL
{
	NETWORKDETECT_SIGNAL(DwNetworkDetector *p, bool b)
		: pThis(p), isNetAvailable(b)
	{

	}
	DwNetworkDetector *pThis;
	bool isNetAvailable;
};
void CALLBACK emitSignal(void *param)
{
	NETWORKDETECT_SIGNAL *p = (NETWORKDETECT_SIGNAL*)param;
	p->pThis->sigNetStatus(p->isNetAvailable);
	delete p;
}

DwNetworkDetector::DwNetworkDetector()
	: m_quitEvent(NULL)
	, m_hWorkThread(NULL)
{
}

DwNetworkDetector::~DwNetworkDetector()
{
	stop();
}

bool DwNetworkDetector::start()
{
	if (m_hWorkThread != NULL)
	{
		return true;
	}

	m_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, _threadStub, this, 0, NULL);
	if (m_hWorkThread != NULL)
	{
		m_quitEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("network_detector"));
	}

	return (m_hWorkThread != NULL);
}

void DwNetworkDetector::stop()
{
	if (m_hWorkThread == NULL)
	{
		return;
	}

	SetEvent(m_quitEvent);
	WaitForSingleObject(m_hWorkThread, INFINITE);

	CloseHandle(m_hWorkThread);
	m_hWorkThread = NULL;
	CloseHandle(m_quitEvent);
	m_quitEvent = NULL;
}

UINT DwNetworkDetector::_threadStub(PVOID param)
{
	DwNetworkDetector *pThis = (DwNetworkDetector *)param;
	return pThis->_threadProc();
}

UINT DwNetworkDetector::_threadProc()
{
	while (_triggerLayer())
	{
		bool isNetAvailable = _logicLayer();
		DwQueuedFunc::instance()->post(emitSignal, new NETWORKDETECT_SIGNAL(this, isNetAvailable));
		_reportLayer();
	}

	return 0;
}

bool DwNetworkDetector::_triggerLayer()
{
	HANDLE triggers[2];
	triggers[0] = m_quitEvent;
	triggers[1] = WSACreateEvent();
	
	OVERLAPPED ol;
	ol.hEvent = triggers[1];

	HANDLE handle;
	NotifyAddrChange(&handle, &ol);

	DWORD waitObj = WaitForMultipleObjects(2, triggers, FALSE, INFINITE);
	WSACloseEvent(ol.hEvent);

	switch (waitObj)
	{
	case WAIT_OBJECT_0:
		{
			return false;
		}
		break;

	case WAIT_OBJECT_0 + 1:
		{
			return true;
		}
		break;

	default:
		{
			return true;
		}
		break;
	}
}

bool DwNetworkDetector::_logicLayer()
{
	// ping www.yy.com => 115.238.171.133
	IPAddr remoteAddr = inet_addr("115.238.171.133");
	HANDLE hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	ICMP_ECHO_REPLY replyBuf;
	DWORD ret = IcmpSendEcho(hIcmpFile, remoteAddr, NULL, 0,
		NULL, &replyBuf, sizeof(replyBuf), 1000);
	IcmpCloseHandle(hIcmpFile);

	if (ret == 0)
	{
		return false;
	}
	else
	{
		return (replyBuf.Status == IP_SUCCESS);
	}
}

bool DwNetworkDetector::_reportLayer()
{
	std::auto_ptr<BYTE> buf;
	ULONG buf_size = 0;
	ULONG ret = GetAdaptersInfo(NULL, &buf_size);
	if (ret == ERROR_BUFFER_OVERFLOW)
	{
		buf.reset(new BYTE[buf_size]);
	}
	else
	{
		return false;
	}

	ret = GetAdaptersInfo((PIP_ADAPTER_INFO)buf.get(), &buf_size);
	if (ret != ERROR_SUCCESS)
	{
		return false;
	}

	PIP_ADAPTER_INFO pInfo = (PIP_ADAPTER_INFO)buf.get();
	do
	{
		LogFinal(LOG::KDataReport) << "Network Adapter: '" << pInfo->Description
			<< "' Type: " << pInfo->Type << " DhcpEnable: " << pInfo->DhcpEnabled
			<< " HaveWins: " << pInfo->HaveWins;

		PIP_ADDR_STRING pip_addr_str = &pInfo->IpAddressList;
		do
		{
			LogFinal(LOG::KDataReport) << "Network IP List: " << pip_addr_str->IpAddress.String
				<< " Mask: " << pip_addr_str->IpMask.String;

			pip_addr_str = pip_addr_str->Next;
		} while (pip_addr_str);

		pip_addr_str = &pInfo->GatewayList;
		do
		{
			LogFinal(LOG::KDataReport) << "Gateway IP List: " << pip_addr_str->IpAddress.String
				<< " Mask: " << pip_addr_str->IpMask.String;

			pip_addr_str = pip_addr_str->Next;
		} while (pip_addr_str);

		pip_addr_str = &pInfo->DhcpServer;
		do
		{
			LogFinal(LOG::KDataReport) << "DhcpSvr IP List: " << pip_addr_str->IpAddress.String
				<< " Mask: " << pip_addr_str->IpMask.String;

			pip_addr_str = pip_addr_str->Next;
		} while (pip_addr_str);
		pInfo = pInfo->Next;
	} while (pInfo);

	return true;
}
