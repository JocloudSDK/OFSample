/*! @file wmisampling.h .h 
 *  @brief 调用WMI进行Disk和Network相关信息采集
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include "stlalloc.h"
#include "logfile.h"

namespace Perf
{
	class CWmiServerProxy;
	class CWmiHiPerfServerProxy;
	class CWbemClassObjectHandle;

	class CWmiSampling
	{
	public:
		/**
		 * Get the singleton Instance of CWmiSampling class
		 * @Method:		GetInstance
		 * @FullName:	Perf::CWmiSampling::GetInstance
		 * @Access:		Public static
		 * @Return:		CWmiSampling*
		 */
		static CWmiSampling *GetInstance();

		/**
		 * Save the sampled datas to file
		 * @Method:		Save
		 * @FullName:	Perf::CWmiSampling::Save
		 * @Access:		Public
		 * @Param:		[in] HANDLE hFile
		 * @Return:		DWORD
		 */
		DWORD Save(HANDLE hFile);

		void DoHiPerfSampling();

	protected:	

		/**
		 * Sampling upload/download traffic of the active network interface.
		 * @Method:		NetworkTrafficSampling
		 * @FullName:	Perf::CWmiSampling::NetworkTrafficSampling
		 * @Access:		Protected virtual
		 * @Return:		void
		 */
		virtual void NetworkSampling();

		/**
		 * Sampling I/O and DiskQueueLength of the Disk
		 * @Method:		DiskStatusSampling
		 * @FullName:	Perf::CWmiSampling::DiskStatusSampling
		 * @Access:		Protected virtual
		 * @Return:		void
		 */
		virtual void DiskSampling();

		CWmiSampling(void);
		~CWmiSampling();
		
	private:
		HRESULT	initializeSampling();
		DWORD	saveNetworkData(HANDLE hFile);
		DWORD	saveDiskData(HANDLE hFile);
		void	sendNetworkToTools(const NETWORK* p);		// Param
		void	sendDiskToTools(const DISK* p);				// Param

	private:
		/* Members */
		CWmiHiPerfServerProxy	*m_pWmiHiPerfServerProxy;
		CWbemClassObjectHandle	*m_pNetworkSamplingObject;
		CWbemClassObjectHandle	*m_pDiskSamplingObject;

		struct DISK_DATA
		{
			DWORD		dwCount;
			DWORD		dwSize;
			DISK*		arItem;
		};
		std::vector<DISK_DATA*, CMyStlAlloc<DISK_DATA*> > m_vecDiskBlock;

		/*CWmiServerProxy*	m_pDiskServerProxy;*/

		struct NETWORK_DATA
		{
			DWORD		dwCount;
			DWORD		dwSize;
			NETWORK*	arItem;
		};
		std::vector<NETWORK_DATA*, CMyStlAlloc<NETWORK_DATA*> > m_vecNetworkBlock;

		/*CWmiServerProxy*	m_pNetWorkServerProxy;*/

		CRITICAL_SECTION	m_cri;

		BOOL m_bInitialized;
	};
}

