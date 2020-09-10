#include "stable.h"

#include "ExtraTimer.h"

namespace DwUtility
{
	namespace timer
	{
		VOID CALLBACK timerProc(HWND hwnd,
			UINT uMsg,
			UINT_PTR idEvent,
			DWORD dwTime
			);

		ITimerSenseObj2::~ITimerSenseObj2()
		{
			CTimer::Instance()->UnRegisterListener(this);
		}

		class CTimerImpl
		{
		public:
			CTimerImpl()
			{
				m_dwThreadId = GetCurrentThreadId();
			}

			~CTimerImpl()
			{
				DW_ASSERT(m_timerMap.size() == m_listnerMap.size());

				for(TimerMap::iterator it = m_timerMap.begin(); it != m_timerMap.end(); ++it)
				{
					KillTimer(NULL, it->first);
				}
			}

			void addTimer(ITimerSenseObj2 *listner, UINT32 interval)
			{
				DW_ASSERT(m_dwThreadId == GetCurrentThreadId());
				DW_ASSERT(m_timerMap.size() < 256);

				removeTimer(listner);

				DWORD timerId = SetTimer(NULL, 0, interval, timerProc);
				m_listnerMap[listner] = timerId;
				m_timerMap[timerId] = listner;
			}

			void removeTimer(ITimerSenseObj2 *listner)
			{
				DW_ASSERT(m_dwThreadId == GetCurrentThreadId());

				ListnerMap::iterator it = m_listnerMap.find(listner);
				if(it != m_listnerMap.end())
				{
					DWORD timerId = it->second;
					KillTimer(NULL, timerId);
					m_timerMap.erase(timerId);
					m_listnerMap.erase(it);
				}
			}

			void timeout(DWORD timerId)
			{
				TimerMap::iterator it = m_timerMap.find(timerId);
				if(it != m_timerMap.end())
				{
					it->second->TimeArrive();
				}
			}

		private:
			typedef std::map<DWORD, ITimerSenseObj2 *> TimerMap;
			typedef std::map<ITimerSenseObj2 *, DWORD> ListnerMap;
			TimerMap m_timerMap;
			ListnerMap m_listnerMap;
			DWORD m_dwThreadId;
		};

		CTimerImpl *_timerImpl = NULL;

		VOID CALLBACK timerProc(HWND hwnd,
			UINT uMsg,
			UINT_PTR idEvent,
			DWORD dwTime
			)
		{
			_timerImpl->timeout(idEvent);
		}

		CTimer::CTimer()
		{
			_timerImpl = new CTimerImpl();
		}

		CTimer::~CTimer()
		{
			delete _timerImpl;
		}

		CTimer * CTimer::Instance()
		{
			static CTimer __timer;
			return &__timer;
		}

		void CTimer::RegisterListener(DwUtility::timer::ITimerSenseObj2 *listener, UINT timeSpan)
		{
			_timerImpl->addTimer(listener, timeSpan);
		}

		void CTimer::UnRegisterListener(DwUtility::timer::ITimerSenseObj2 *listener)
		{
			_timerImpl->removeTimer(listener);
		}

		BOOL CTimer::SetTick( UINT millisecon )
		{
			DW_ASSERT_X(false, __FUNCTION__, "Deprecated method!");
			DW_UNUSED(millisecon);
			return TRUE;
		}

		void CTimer::Release()
		{
			DW_ASSERT_X(false, __FUNCTION__, "Deprecated method!");
		}
	}
}

