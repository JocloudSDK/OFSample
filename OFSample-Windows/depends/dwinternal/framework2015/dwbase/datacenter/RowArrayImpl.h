#pragma once

#include "ITable.h"
#include "TableDataImpl.h"
#include "container/ContainerDef.h"

namespace Data
{
	class CRowArrayImpl : public IRowArray
	{
	public:
		static CRowArrayImpl * CreateInstance(CTableDataImpl* pTableDataImplRef) { return new CRowArrayImpl(pTableDataImplRef); }

		//IUnk
		virtual void		addRef()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			::InterlockedIncrement((LONG *)&m_dwRef);
		}

		virtual void		release()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
				delete this;
		}

		virtual	IWeakRef *	weakPtr()
		{
			assert(! L"不支持弱指针");
			return NULL;
		}

		virtual bool queryInterface(REFDWUUID iid, void **ppv)
		{
			(void)iid;
			*ppv = NULL;
			assert(! L"不支持queryInterface");
			return false;
		}

		//IRowArray
		virtual UINT		size() { return m_vecRow.size(); }
		virtual IRowPtrCR	at(UINT i) { return m_vecRow[i]; }
		virtual void		push_back(IRow *pRow)
		{			
			pRow->addRef();
			m_vecRow.push_back(pRow);
		}

		virtual IRowPtrCR	front()
		{
			assert (!m_vecRow.empty());
			return m_vecRow.front();
		}

		virtual IRowPtrCR	back()
		{
			assert (!m_vecRow.empty());
			return m_vecRow.back();
		}

		virtual void		reserve(UINT count) { m_vecRow.reserve(count); }
		virtual void		swap(IRowArray *pArrRow)
		{
			CRowArrayImpl *pImpl = static_cast<CRowArrayImpl *> (pArrRow);//need not dynamic_cast.
			m_vecRow.swap(pImpl->m_vecRow);
		}

		virtual void		clear()
		{
			for (UINT i = 0; i < m_vecRow.size(); i++)
				m_vecRow[i]->release();
			m_vecRow.clear();
		}

		virtual BOOL		empty() { return m_vecRow.empty(); }

		virtual comptr<IRowArray> clone(BOOL bCloneRows)
		{
			CRowArrayImpl* pClonedRowArray = CRowArrayImpl::CreateInstance(m_pTableDataImplRef);
			if (bCloneRows)
			{
				for (UINT i = 0; i < m_vecRow.size(); ++i)
				{
					pClonedRowArray->push_back(m_vecRow[i]);
				}
			}
			comptr<IRowArray> spClonedRowArray;
			spClonedRowArray.attach(pClonedRowArray);
			return spClonedRowArray;
		}

		virtual NAME_ID			getTableName() 
		{
			if (m_pTableDataImplRef)
			{
				return m_pTableDataImplRef->getTableName();
			}
			else
			{
				return NAME_ID();
			}
			
		}

		//区别于pash_back，内部不会调用addRef，当CRowArrayImpl销毁时，自动调用release释放
		inline void push_back_take_ownership(IRow *pRow)
		{
			assert(NULL != pRow);
			m_vecRow.push_back(pRow);
		}

	private:
		CRowArrayImpl(CTableDataImpl* pTableDataImplRef)
			: m_pTableDataImplRef(pTableDataImplRef)
		{
			m_dwRef = 1;
			if (m_pTableDataImplRef)
			{
				m_pTableDataImplRef->addRef();
			}
		}
		~CRowArrayImpl(void)
		{ 
			assert(m_dwRef == 0); 
			clear();
			if (m_pTableDataImplRef)
			{
				m_pTableDataImplRef->release();
			}
		}

		DWORD				m_dwRef;
		DWVector<IRow*>		m_vecRow;
		CTableDataImpl* m_pTableDataImplRef;//强制引用
	};
};
