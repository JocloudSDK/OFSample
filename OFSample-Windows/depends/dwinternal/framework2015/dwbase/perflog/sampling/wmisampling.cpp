/** 
@file wmisampling.cpp
@brief 调用WMI进行Disk和Network相关信息采集
@version 2012-08-29 wesleywu
*/

#include "stable.h"
#include "wmisampling.h"
#include "wmi/variant.h"
#include "wmi/wbemclassobjecthandle.h"
#include "wmi/wbemlocator.h"
#include "wmi/wmiserverproxy.h"
#include "wmi/wmihiperfserverproxy.h"
#include "wmi/comenviroment.h"
#include "control.h"
#include "toolmsg.h"
#include "toolexchangecenter.h"

const wchar_t* c_szDiskServerClass = L"Win32_PerfFormattedData_PerfDisk_PhysicalDisk";
const wchar_t* c_szDiskServerCategory = c_szDiskServerClass;
const wchar_t* c_szNetworkServerClass = L"Win32_PerfFormattedData_Tcpip_NetworkInterface";
const wchar_t* c_szNetworkServerCategory = c_szNetworkServerClass;

const wchar_t* c_szDiskObjectName = L"Disk";
const wchar_t* c_szNetworkObjectName = L"Network";

/*const int c_nMaxDiskQueueLength = 32;*/

namespace Perf
{
	CWmiSampling::CWmiSampling(void)
		: m_bInitialized(FALSE)
		, m_pDiskSamplingObject(NULL)
		, m_pNetworkSamplingObject(NULL)
		, m_pWmiHiPerfServerProxy(NULL)
	{
		::InitializeCriticalSection(&m_cri);	
	}

	CWmiSampling::~CWmiSampling()
	{
	}


	CWmiSampling * CWmiSampling::GetInstance()
	{
		static CWmiSampling *s_pImpl = NULL;
		if (!s_pImpl) {
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CWmiSampling), TRUE)) CWmiSampling();
		}

		s_pImpl->initializeSampling();

		return s_pImpl;
	}

	DWORD CWmiSampling::Save( HANDLE hFile )
	{
		DWORD dwRet = saveDiskData(hFile);
		dwRet += saveNetworkData(hFile);
		return dwRet;
	}


	void CWmiSampling::DoHiPerfSampling()
	{
		/* Refresh! */
		if (!m_pWmiHiPerfServerProxy || S_OK != m_pWmiHiPerfServerProxy->Refresh())
		{
			return;
		}

	 	/* Sampling Disk */
		DiskSampling();

		/* Sampling Network */
		NetworkSampling();
	}

	void CWmiSampling::NetworkSampling()
	{
		if (!CControl::GetInstance()->IsSamplingWMI() || !m_pNetworkSamplingObject)
		{
			/* m_pNetworkSamplingObject != 0 ENSURED that m_pWmiHiPerfServerProxy != 0 */
			return;
		}

		::EnterCriticalSection(&m_cri);

		/* Allocate Memories if required */
		if (m_vecNetworkBlock.empty() || m_vecNetworkBlock[m_vecNetworkBlock.size()-1]->dwCount == m_vecNetworkBlock[m_vecNetworkBlock.size()-1]->dwSize)
		{
			NETWORK_DATA *pBlock = (NETWORK_DATA *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(NETWORK_DATA), FALSE);
			pBlock->dwCount = 0;
			pBlock->dwSize = c_dwSamplingBlockSize / sizeof(NETWORK);
			pBlock->arItem = (NETWORK *)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSamplingBlockSize, TRUE);

			m_vecNetworkBlock.push_back(pBlock);
		}
		

		/* Get Properties */
		CVariant varBytesSentPerSec, varBytesReceivedPerSec;
		m_pNetworkSamplingObject->GetProperty(L"BytesReceivedPerSec", varBytesReceivedPerSec);
		m_pNetworkSamplingObject->GetProperty(L"BytesSentPerSec", varBytesSentPerSec);

		/* Assignment value to pBlock */
		NETWORK_DATA *pBlock = m_vecNetworkBlock[m_vecNetworkBlock.size()-1];
		pBlock->arItem[pBlock->dwCount].llPerfCount = m_pWmiHiPerfServerProxy->LastRefreshPerfCount();
		pBlock->arItem[pBlock->dwCount].dwBytesSentPerSec = varBytesSentPerSec.isNotNull() ?
			varBytesSentPerSec.safeCast<DWORD>() : 0;
		pBlock->arItem[pBlock->dwCount].dwBytesReceivedPerSec = varBytesReceivedPerSec.isNotNull() ? 
			varBytesReceivedPerSec.safeCast<DWORD>() : 0;	

		/* Send Message to PerfTool */
		sendNetworkToTools(&pBlock->arItem[pBlock->dwCount]);

		/* Post Processes */
		::InterlockedIncrement((LONG*)&pBlock->dwCount); 
		::LeaveCriticalSection(&m_cri);		

		return;
	}

	void CWmiSampling::DiskSampling()
	{
		if (!CControl::GetInstance()->IsSamplingWMI() || !m_pDiskSamplingObject)
		{
			/* m_pDiskSamplingObject != 0 ENSURED that m_pWmiHiPerfServerProxy != 0 */
			return;
		}

		::EnterCriticalSection(&m_cri);

		/* Allocate Memories if required */
		if (m_vecDiskBlock.empty() || m_vecDiskBlock[m_vecDiskBlock.size()-1]->dwCount == m_vecDiskBlock[m_vecDiskBlock.size()-1]->dwSize)
		{
			DISK_DATA *pBlock = (DISK_DATA *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(DISK_DATA), FALSE);
			pBlock->dwCount = 0;
			pBlock->dwSize = c_dwSamplingBlockSize / sizeof(DISK);
			pBlock->arItem = (DISK *)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSamplingBlockSize, TRUE);

			m_vecDiskBlock.push_back(pBlock);
		}

		/* Get Properties */
		CVariant varDiskQueueLength, varDiskReadBytesPerSec, varDiskWriteBytesPerSec;
		m_pDiskSamplingObject->GetProperty(L"AvgDiskQueueLength", varDiskQueueLength);
		m_pDiskSamplingObject->GetProperty(L"DiskReadBytesPerSec", varDiskReadBytesPerSec);
		m_pDiskSamplingObject->GetProperty(L"DiskWriteBytesPerSec", varDiskWriteBytesPerSec);

		/* Assignment value to pBlock */
		DISK_DATA *pBlock = m_vecDiskBlock[m_vecDiskBlock.size()-1];
		pBlock->arItem[pBlock->dwCount].llPerfCount = m_pWmiHiPerfServerProxy->LastRefreshPerfCount();
		pBlock->arItem[pBlock->dwCount].dwDiskQueueLength = varDiskQueueLength.isNotNull() ? 
			varDiskQueueLength.safeCast<DWORD>() : 0;
// 		if (pBlock->arItem[pBlock->dwCount].dwDiskQueueLength >= c_nMaxDiskQueueLength)
// 		{
// 			pBlock->arItem[pBlock->dwCount].dwDiskQueueLength = c_nMaxDiskQueueLength;
// 		}

		pBlock->arItem[pBlock->dwCount].dwDiskReadBytesPerSec = varDiskReadBytesPerSec.isNotNull() ? 
			varDiskReadBytesPerSec.safeCast<DWORD>() : 0;
		pBlock->arItem[pBlock->dwCount].dwDiskWriteBytesPerSec = varDiskWriteBytesPerSec.isNotNull() ?
			varDiskWriteBytesPerSec.safeCast<DWORD>() : 0;

		/* Send Message to PerfTool */
		sendDiskToTools(&pBlock->arItem[pBlock->dwCount]);

		/* Post Processes */
		::InterlockedIncrement((LONG*)&pBlock->dwCount); 
		::LeaveCriticalSection(&m_cri);
		
		return;
	}

	DWORD CWmiSampling::saveNetworkData( HANDLE hFile )
	{
		::EnterCriticalSection(&m_cri);
		DWORD dwWritten = 0;

		NETWORK_BLOCK_HEAD stHead;
		stHead.header.dwSize = sizeof(stHead);
		stHead.header.dwFlag = FLAG_E_NETWORK;
		stHead.dwCount = 0;

		/* Calculate Block Size */
		for (UINT i = 0; i < m_vecNetworkBlock.size(); i++)
		{
			stHead.header.dwSize += m_vecNetworkBlock[i]->dwCount * sizeof(NETWORK);
			stHead.dwCount += m_vecNetworkBlock[i]->dwCount;
		}

		/* Write Block Head */
		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		/* Write All Blocks */
		for (UINT i = 0; i < m_vecNetworkBlock.size(); i++)
		{
			::WriteFile(hFile, m_vecNetworkBlock[i]->arItem, m_vecNetworkBlock[i]->dwCount * sizeof(NETWORK), &dwWritten, 0);
		}

		::LeaveCriticalSection(&m_cri);
		return stHead.header.dwSize;
	}

	DWORD CWmiSampling::saveDiskData( HANDLE hFile )
	{
		::EnterCriticalSection(&m_cri);
		DWORD dwWritten = 0;

		DISK_BLOCK_HEAD stHead;
		stHead.header.dwSize = sizeof(stHead);
		stHead.header.dwFlag = FLAG_E_DISK;
		stHead.dwCount = 0;

		/* Calculate Block Size */
		for (UINT i = 0; i < m_vecDiskBlock.size(); i++)
		{
			stHead.header.dwSize += m_vecDiskBlock[i]->dwCount * sizeof(DISK);
			stHead.dwCount += m_vecDiskBlock[i]->dwCount;
		}

		/* Write Block Head */
		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecDiskBlock.size(); i++)
		{
			::WriteFile(hFile, m_vecDiskBlock[i]->arItem, m_vecDiskBlock[i]->dwCount * sizeof(DISK), &dwWritten, 0);
		}

		/* Write All Blocks */
		::LeaveCriticalSection(&m_cri);
		return stHead.header.dwSize;
	}

	void CWmiSampling::sendNetworkToTools(const NETWORK* p)
	{
		Perf::MSG_NETWORK stMsg;
		stMsg.llPerfCount			= p->llPerfCount;
		stMsg.dwBytesSentPerSec		= p->dwBytesSentPerSec;
		stMsg.dwBytesReceivedPerSec = p->dwBytesReceivedPerSec;

		CToolExchangeCenter::GetInstance()->SendToTools(eMsgNetwork, &stMsg, sizeof(stMsg));
	}

	void CWmiSampling::sendDiskToTools(const DISK* p)
	{
		Perf::MSG_DISK stMsg;
		stMsg.llPerfCount				= p->llPerfCount;
		stMsg.dwDiskWriteBytesPerSec	= p->dwDiskWriteBytesPerSec;
		stMsg.dwDiskReadBytesPerSec		= p->dwDiskReadBytesPerSec;
		stMsg.dwDiskQueueLength			= p->dwDiskQueueLength;

		CToolExchangeCenter::GetInstance()->SendToTools(eMsgDisk, &stMsg, sizeof(stMsg));
	}

	HRESULT CWmiSampling::initializeSampling()
	{
		if (m_bInitialized)
		{
			return S_OK;
		}

		HRESULT hr = CWmiHiPerfServerProxy::CreateWmiHiPerfServerProxy(local_host(), &m_pWmiHiPerfServerProxy);

		if (FAILED(hr) || NULL == m_pWmiHiPerfServerProxy)
		{
			return hr;
		}		

		// Add Win32_PerfFormattedData_PerfDisk_PhysicalDisk Object
		m_pDiskSamplingObject = m_pWmiHiPerfServerProxy->AddNamedObject(c_szDiskObjectName, c_szDiskServerClass, L"_Total");

		// Find a "NetEnabled" NetworkInterface
		CWmiServerProxy networkAdapterServer(local_host(), L"Win32_NetworkAdapter", L"Win32_NetworkAdapter");
		networkAdapterServer.SetEqualFilter(L"NetEnabled", L"true");
		networkAdapterServer.Start();
		if (CWbemClassObjectHandle obj = networkAdapterServer.Next())
		{
			// Add Win32_PerfFormattedData_Tcpip_NetworkInterface Object
			CVariant v;
			obj.GetProperty(L"Name", v);
			m_pNetworkSamplingObject = m_pWmiHiPerfServerProxy->AddNamedObject(c_szNetworkObjectName, c_szNetworkServerClass, v.toString());
		}		

		if (NULL == m_pDiskSamplingObject || NULL == m_pNetworkSamplingObject)
		{
			return S_FALSE;
		} 
		else
		{
			m_bInitialized = TRUE;
			return S_OK;
		}
	}
}
