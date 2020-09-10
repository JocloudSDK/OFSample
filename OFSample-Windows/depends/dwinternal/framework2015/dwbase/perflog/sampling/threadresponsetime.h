
#pragma once 

#include <set>

namespace Perf
{
	class CThreadResponseTime
	{
	public:
		static CThreadResponseTime *GetInstance();
		void AddThread(DWORD dwTID);

	private:
		CThreadResponseTime();
		static LRESULT WINAPI Tramp_DispatchMessage(const MSG *lpmsg);

		DWORD m_dwTlsDirty;
		WORD  m_wIdResponse;

		std::set<DWORD>		m_setThread;
		CRITICAL_SECTION	m_cri;
	};
}
