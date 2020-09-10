#pragma once

#include "stlalloc.h"
#include "logfile.h"
#include <map>
#include <vector>

namespace Perf
{
	struct CallStackTls;
	class CCallStackSampling
	{
		struct CallStackTls
		{
			DWORD		dwLen;
			DWORD		dwSize;		//��λΪBYTE
			DWORD		dwTID;		//�߳�ID
			HANDLE		hThread;	//�߳̾��
			void *		pBuf;
			CALLSTACK *	pLast;		//ǰһ��CallStack��û����ΪNULL
		};

	public:
		static CCallStackSampling *GetInstance();
		DWORD Save(HANDLE hFile);
		void AddThread(DWORD dwTID);
		void CallStackSampling(LONGLONG llPerfCount = 0);

	private:
		CCallStackSampling();

		static CallStackTls * AllocCallStackBlock(DWORD dwTID, HANDLE hThread);
		static int GetStack(HANDLE hThread, PCONTEXT pCt, BYTE *buf, DWORD dwMaxCount);
		
		DWORD SaveModuleList(HANDLE hFile);
		DWORD SaveCallStackData(HANDLE hFile);
		void CallStack(DWORD dwTID, struct CallStackTls** ppBlock, LONGLONG llPerfCount = 0);

		typedef std::map<DWORD, struct CallStackTls*, std::less<DWORD>, CMyStlAlloc<std::pair<DWORD, struct CallStackTls *> > > CMapThread;
		CMapThread			m_mapThread;
		
		std::vector<struct CallStackTls*, CMyStlAlloc<struct CallStackTls*> > m_vecBlock;

		CRITICAL_SECTION	m_cri;
	};
}
