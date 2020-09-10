#pragma once

#include "container/ContainerDef.h"
#include "ITable.h"
#include "DataCenterPub.h"

namespace Data
{
	class CRowImpl;
	class CTableDataImpl;
	class CEventCollect
	{
	public:

		CEventCollect(void);
		~CEventCollect(void);

		void	AddRowEvent(IRow *pRowCur, IRow *pRowOrig, E_ROW_EVENT eRowEvent); //外面保证多线程安全
		void	AddResetEvent(const std::vector<CRowImpl *> & vecRow); //外面保证多线程安全
		void	OnTrigger(CTableDataImpl *pTableDataImpl);
	
		//事件订阅
		xsignals::signal<void (IRowArrayPtr)> * getSigAddList (CRITICAL_SECTION * pCriEdit)				{ return getSigXXX(INDEX_E_ADD, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigDelList (CRITICAL_SECTION * pCriEdit)				{ return getSigXXX(INDEX_E_DEL, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigChangedList (CRITICAL_SECTION * pCriEdit)			{ return getSigXXX(INDEX_E_CHANGED, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigReset (CRITICAL_SECTION * pCriEdit)				{ return getSigXXX(INDEX_E_RESET, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigAddList (CRITICAL_SECTION * pCriEdit)		{ return getSigXXX(INDEX_E_Q_ADD, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigDelList (CRITICAL_SECTION * pCriEdit)		{ return getSigXXX(INDEX_E_Q_DEL, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigChangedList (CRITICAL_SECTION * pCriEdit)	{ return getSigXXX(INDEX_E_Q_CHANGED, pCriEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigReset (CRITICAL_SECTION * pCriEdit)			{ return getSigXXX(INDEX_E_Q_RESET, pCriEdit); }
		
		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getSigChangedListEx(CRITICAL_SECTION * pCriEdit);
		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getQueuedSigChangedListEx(CRITICAL_SECTION * pCriEdit);

	private:
		enum E_SIG_INDEX;
		xsignals::signal<void (IRowArrayPtr)> * getSigXXX(enum E_SIG_INDEX eSig, CRITICAL_SECTION * pCriEdit);

		struct ROW_PAIR
		{
			IRow * pRowCur;
			IRow * pRowOrig;
		};
		struct DATA_EVENT 
		{
			DWVector<ROW_PAIR> vecRow; //保持了引用计数
			E_ROW_EVENT eRowEvent;
		};
		DWList<DATA_EVENT>			m_lstEvent;
	
		//事件挂接
		enum E_SIG_INDEX{
			INDEX_E_ADD = 0,
			INDEX_E_DEL,
			INDEX_E_CHANGED,
			INDEX_E_RESET,
			INDEX_E_Q_ADD,
			INDEX_E_Q_DEL,
			INDEX_E_Q_CHANGED,
			INDEX_E_Q_RESET,
			INDEX_E_MAX_COUNT,
		};
		
		struct signal_state1
		{
			signal_state1()
			{
				isInit = false;
			}
			bool isInit;
			xsignals::signal<void (IRowArrayPtr)> sig;
		};
		signal_state1 m_pSigs[INDEX_E_MAX_COUNT];

		struct signal_state2
		{
			signal_state2()
			{
				isInit = false;
			}
			bool isInit;
			xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> sig;
		};
		signal_state2 m_pSigChangedListEx;
		signal_state2 m_pQueuedSigChangedListEx;
	};
};
