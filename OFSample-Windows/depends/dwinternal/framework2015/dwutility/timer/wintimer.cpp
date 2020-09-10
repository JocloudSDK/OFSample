#include "stable.h"
#include "wintimer.h"
#include <vector>

/*
 *	���¶�ʱ��ʵ��ԭ������std::map��ʽʵ�ֵģ�����Ӫ�����з���map��ĳ��δ֪�����ʲ��ߣ�Ӧ��˵�ܵͣ������»ᱻд����
 *	�������Ǳ�ĵط����еĳ������Խ���ˣ����º������map���ַǷ�ָ�����
 *	�������ԭ���������������ʱ��ʹ���Ѿ��ܳ�ʱ���ˣ�Ҳû���ֹ��κα��������������뵽�Ľ���취�ǲ��ñ����ķ�ʽ��
 *	�޸�һ�������ʱ����ʵ�֣���ʹ�õ��ڴ漯������������map������ɢ���ڴ�ʹ�÷�ʽ������std::vector�����ڴ���֯��ʽ��
 *	���ٳ��ֱ�д���ĳ����ʣ�����ԭ��δ����������
 */

class CLock
{
public:
	CLock()
	{
		InitializeCriticalSection(&m_cs);
	}
	~CLock()
	{
		DeleteCriticalSection(&m_cs);
	}
	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}
	bool TryLock()
	{
		TryEnterCriticalSection(&m_cs);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}
protected:
	CRITICAL_SECTION m_cs;
};
CLock g_lock;

//----------------------------------------------------------------------------------------------------
struct tagTimerItem 
{
	UINT_PTR	m_uId;
	DwUtility::timer::ITimerFunc* m_pfn;

	tagTimerItem() : m_uId(0), m_pfn(NULL) {}
	tagTimerItem( UINT_PTR uId, DwUtility::timer::ITimerFunc* pfn ) : m_uId(uId) , m_pfn(pfn){}
};

typedef std::vector<tagTimerItem>	TIMER_VECTOR;
TIMER_VECTOR g_vecTimer;

//----------------------------------------------------------------------------------------------------
void InsertTimerItem( UINT_PTR uId, DwUtility::timer::ITimerFunc* pfn )
{
	g_lock.Lock();

	bool bIsAdd	=	false;
	TIMER_VECTOR::iterator iter = g_vecTimer.begin();
	while ( iter != g_vecTimer.end() )
	{
		if (iter->m_uId == uId)
		{	
			bIsAdd = true;
			iter->m_pfn = pfn;
			break;
		}
		++iter;
	}

	if (!bIsAdd)
	{
		g_vecTimer.push_back( tagTimerItem(uId, pfn) );
	}	

	g_lock.UnLock();
}

void RemoveTimerItem( UINT_PTR uId )
{
	g_lock.Lock();

	TIMER_VECTOR::iterator iter = g_vecTimer.begin();
	while ( iter != g_vecTimer.end() )
	{
		if (iter->m_uId == uId)
		{
			g_vecTimer.erase( iter );
			break;
		}
		++iter;
	}

	g_lock.UnLock();
}

namespace DwUtility
{
	namespace timer
	{
		void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

		ITimerFunc* FindTimerProc( UINT_PTR uId )
		{
			ITimerFunc* pfn	=	NULL;

			g_lock.Lock();

			TIMER_VECTOR::iterator iter = g_vecTimer.begin();
			while ( iter != g_vecTimer.end() )
			{
				if (iter->m_uId == uId)
				{
					pfn = iter->m_pfn;
					break;
				}
				++iter;
			}

			g_lock.UnLock();

			return pfn;
		}

		UINT_PTR CWinTimer::SetTimer(UINT uElapse, ITimerFunc* pfn)
		{
			DW_ASSERT(uElapse > 0);
			DW_ASSERT(pfn != NULL);
			UINT_PTR id = ::SetTimer(NULL, 0, uElapse, TimerProc);
			if(id != 0)
			{
				InsertTimerItem( id, pfn );
			}
			return id;
		}

		void CWinTimer::KillTimer(UINT_PTR nIDEvent)
		{
			if (nIDEvent != 0)
			{
				::KillTimer(NULL, nIDEvent);
				RemoveTimerItem(nIDEvent);
			}
		}

		//----------------------------------------------------------------------------------------------------
		CSingleTimer::CSingleTimer()
		{
			m_pReceiver = NULL;
		}

		void CSingleTimer::watch(ISingleTimerFunc* pfn)
		{
			m_pReceiver = pfn;
		}

		void CSingleTimer::revoke(){
			m_pReceiver = NULL;
		}

		// ������ʱ��������id��ʧ�ܷ���0
		UINT_PTR CSingleTimer::SetTimer(UINT uElapse, TIMERPROC lpTimerFunc)
		{
			UINT_PTR id = ::SetTimer(NULL, 0, uElapse, lpTimerFunc);
			return id;
		}

		// ����ָ��id�Ķ�ʱ��
		void CSingleTimer::KillTimer(UINT_PTR nIDEvent)
		{
			::KillTimer(NULL, nIDEvent);
		}

		void CSingleTimer::OnTimer(UINT_PTR idEvent)
		{
			if (m_pReceiver)
			{
				m_pReceiver->OnTimer(idEvent);
			}
		}

		//----------------------------------------------------------------------------------------------------
		// ��Ϣ������
		void CALLBACK TimerProc(HWND hwnd, UINT, UINT_PTR idEvent, DWORD)
		{
			DW_ASSERT(hwnd == NULL);
			ITimerFunc * pfunc = FindTimerProc( idEvent );

			if (pfunc)
			{
				pfunc->OnTimer(idEvent);
			}
			else
			{
				DW_ASSERT(0);
			}
		}
	}
}
