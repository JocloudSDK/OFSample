#include "stable.h"
#include "EventCollect.h"
#include "RowArrayImpl.h"
#include "RowImpl.h"
#include "EventDispatch.h"

namespace Data
{
	CEventCollect::CEventCollect(void)
	{
	}

	CEventCollect::~CEventCollect(void)
	{
		assert(m_lstEvent.empty() && _T("暂时m_lstEvent不应该有内容的"));
		for (DWList<DATA_EVENT>::iterator it = m_lstEvent.begin(); it != m_lstEvent.end(); ++it)
		{
			for (UINT  i = 0; i < it->vecRow.size(); i++)
			{
				it->vecRow[i].pRowCur->release();
				if (it->vecRow[i].pRowOrig)
					it->vecRow[i].pRowOrig->release();
			}
		}
		m_lstEvent.clear();

	}

	void CEventCollect::AddRowEvent(IRow *pRowCur, IRow *pRowOrig, E_ROW_EVENT eRowEvent)
	{
		assert(pRowCur && pRowCur != pRowOrig);
		assert((eRowEvent == ROW_E_CHANGED && pRowOrig) || (eRowEvent != ROW_E_CHANGED && !pRowOrig));

		BOOL bIsConnect = FALSE;
		switch (eRowEvent)
		{
		case ROW_E_ADD:
			if (m_pSigs[INDEX_E_ADD].isInit || m_pSigs[INDEX_E_Q_ADD].isInit) 
				bIsConnect = TRUE;
			break;
		case ROW_E_CHANGED:
			if (m_pSigs[INDEX_E_CHANGED].isInit || m_pSigs[INDEX_E_Q_CHANGED].isInit || m_pSigChangedListEx.isInit || m_pQueuedSigChangedListEx.isInit) 
				bIsConnect = TRUE;
			break;
		case ROW_E_DEL:
			if (m_pSigs[INDEX_E_DEL].isInit || m_pSigs[INDEX_E_Q_DEL].isInit) 
				bIsConnect = TRUE;
			break;
		default:
			assert(! L"事件类型不支持");
			break;
		}

		if (!bIsConnect)
			return;

		if (!m_lstEvent.empty() && m_lstEvent.back().eRowEvent == eRowEvent)
		{
			ROW_PAIR rowPair;

			pRowCur->addRef();
			rowPair.pRowCur = pRowCur;

			if (pRowOrig && (m_pSigChangedListEx.isInit || m_pQueuedSigChangedListEx.isInit))
			{				
				pRowOrig->addRef();
				rowPair.pRowOrig = pRowOrig;
			}
			else
			{
				rowPair.pRowOrig = NULL;
			}

			m_lstEvent.back().vecRow.push_back(rowPair);
		}
		else
		{			
			m_lstEvent.push_back(DATA_EVENT());
			DATA_EVENT &oEvent = m_lstEvent.back();
			oEvent.eRowEvent = eRowEvent;

			ROW_PAIR rowPair;
			pRowCur->addRef();
			rowPair.pRowCur = pRowCur;
			if (pRowOrig && (m_pSigChangedListEx.isInit || m_pQueuedSigChangedListEx.isInit))
			{				
				pRowOrig->addRef();
				rowPair.pRowOrig = pRowOrig;
			}
			else
			{
				rowPair.pRowOrig = NULL;
			}

			oEvent.vecRow.push_back(rowPair);
		}
	}

	void CEventCollect::AddResetEvent(const std::vector<CRowImpl *> & vecRow)
	{
		//仅仅连接了事件才处理，提升性能
		if (m_pSigs[INDEX_E_RESET].isInit || m_pSigs[INDEX_E_Q_RESET].isInit)
		{
			m_lstEvent.push_back(DATA_EVENT());
			DATA_EVENT &oEvent = m_lstEvent.back();
			oEvent.eRowEvent = ROW_E_RESET;

			for (UINT i = 0; i < vecRow.size(); i++)
			{
				ROW_PAIR rowPair;
				vecRow[i]->addRef();
				rowPair.pRowCur = vecRow[i];
				rowPair.pRowOrig = NULL;

				oEvent.vecRow.push_back(rowPair);
			}
		}
	}

	void CEventCollect::OnTrigger(CTableDataImpl *pTableDataImpl)
	{
		if (pTableDataImpl == NULL)
		{
			assert(0);
			return;
		}

		CRITICAL_SECTION * pCriEdit = pTableDataImpl->getEditCri();

		::EnterCriticalSection(pCriEdit);
		DWList<CEventCollect::DATA_EVENT> lstTemp;
		m_lstEvent.swap(lstTemp);
		::LeaveCriticalSection(pCriEdit);

		//触发事件
		for (DWList<CEventCollect::DATA_EVENT>::iterator it = lstTemp.begin(); it != lstTemp.end(); ++it)
		{
			CRowArrayImpl * pArrRowCur = CRowArrayImpl::CreateInstance(pTableDataImpl);
			pArrRowCur->reserve(it->vecRow.size());
			for (UINT i = 0; i < it->vecRow.size(); i++)
				pArrRowCur->push_back_take_ownership(it->vecRow[i].pRowCur);

			CRowArrayImpl * pArrRowOrig = NULL;
			for (UINT i = 0; i < it->vecRow.size(); i++)
			{
				if (it->vecRow[i].pRowOrig)
				{
					if (!pArrRowOrig)
					{
						pArrRowOrig = CRowArrayImpl::CreateInstance(pTableDataImpl);
						pArrRowOrig->reserve(it->vecRow.size());
					}
					pArrRowOrig->push_back_take_ownership(it->vecRow[i].pRowOrig);
				}
			}

			if (pArrRowOrig && pArrRowOrig->size() != pArrRowCur->size())
			{
				Error::OutputWarning(L"在挂接ChangedListEx事件前表中已经有数据了，可能出现数据事件丢失");
				pArrRowOrig->release();
				pArrRowOrig = NULL;
			}

			it->vecRow.clear();//Row引用计数给pArrRowCur和pArrRowOrig，这不需要释放了

			switch (it->eRowEvent)
			{
			case ROW_E_ADD:
				{
					if (m_pSigs[INDEX_E_ADD].isInit) 
						CEventDispatch::GetInstance()->Emit(&(m_pSigs[INDEX_E_ADD].sig), pArrRowCur);
					if (m_pSigs[INDEX_E_Q_ADD].isInit)
						CEventDispatch::GetInstance()->QueuedEmit(&(m_pSigs[INDEX_E_Q_ADD].sig), pArrRowCur);
				}
				break;
			case ROW_E_DEL:
				{
					if (m_pSigs[INDEX_E_DEL].isInit) 
						CEventDispatch::GetInstance()->Emit(&(m_pSigs[INDEX_E_DEL].sig), pArrRowCur);
					if (m_pSigs[INDEX_E_Q_DEL].isInit)
						CEventDispatch::GetInstance()->QueuedEmit(&(m_pSigs[INDEX_E_Q_DEL].sig), pArrRowCur);
				}
				break;
			case ROW_E_CHANGED:
				{
					if (m_pSigs[INDEX_E_CHANGED].isInit) 
						CEventDispatch::GetInstance()->Emit(&(m_pSigs[INDEX_E_CHANGED].sig), pArrRowCur);
					if (m_pSigs[INDEX_E_Q_CHANGED].isInit)
						CEventDispatch::GetInstance()->QueuedEmit(&(m_pSigs[INDEX_E_Q_CHANGED].sig), pArrRowCur);
		
					if (m_pSigChangedListEx.isInit && pArrRowOrig) 
						CEventDispatch::GetInstance()->Emit(&(m_pSigChangedListEx.sig), pArrRowCur, pArrRowOrig);
					if (m_pQueuedSigChangedListEx.isInit && pArrRowOrig)
						CEventDispatch::GetInstance()->QueuedEmit(&(m_pQueuedSigChangedListEx.sig), pArrRowCur, pArrRowOrig);
				}
				break;
			case ROW_E_RESET:
				{
					if (m_pSigs[INDEX_E_RESET].isInit) 
						CEventDispatch::GetInstance()->Emit(&(m_pSigs[INDEX_E_RESET].sig), pArrRowCur);
					if (m_pSigs[INDEX_E_Q_RESET].isInit)
						CEventDispatch::GetInstance()->QueuedEmit(&(m_pSigs[INDEX_E_Q_RESET].sig), pArrRowCur);
				}
				break;
			default:
				assert(0);
				break;
			}

			pArrRowCur->release();
			if (pArrRowOrig)
				pArrRowOrig->release();
		}
	}

	xsignals::signal<void (IRowArrayPtr)> * CEventCollect::getSigXXX(enum E_SIG_INDEX eSig, CRITICAL_SECTION * pCriEdit)
	{
		::EnterCriticalSection(pCriEdit);
		m_pSigs[eSig].isInit = true;
		::LeaveCriticalSection(pCriEdit);
	
		return &(m_pSigs[eSig].sig);
	}

	xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> * CEventCollect::getSigChangedListEx(CRITICAL_SECTION * pCriEdit)
	{
		::EnterCriticalSection(pCriEdit);
		m_pSigChangedListEx.isInit = true;
		::LeaveCriticalSection(pCriEdit);
	
		return &(m_pSigChangedListEx.sig);
	}

	xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> * CEventCollect::getQueuedSigChangedListEx(CRITICAL_SECTION * pCriEdit)
	{
		::EnterCriticalSection(pCriEdit);
		m_pQueuedSigChangedListEx.isInit = true;
		::LeaveCriticalSection(pCriEdit);

		return &(m_pQueuedSigChangedListEx.sig);
	}
};
