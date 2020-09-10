#pragma once

#include "ITableEdit.h"
#include "container/ContainerDef.h"

namespace Data
{
	class CRowEditArrayImpl : public IRowEditArray
	{
	public:
		static CRowEditArrayImpl * CreateInstance() { return new CRowEditArrayImpl(); }

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

		//IRowEditArray
		virtual UINT			size() { return m_vecRowEdit.size(); }
		virtual IRowEditPtrCR	at(UINT i) { return m_vecRowEdit[i]; }
		virtual void			push_back(IRowEdit *pRowEdit)
		{			
			pRowEdit->addRef();
			m_vecRowEdit.push_back(pRowEdit);
		}

		virtual IRowEditPtrCR	front()
		{
			assert (!m_vecRowEdit.empty());
			return m_vecRowEdit.front();
		}

		virtual IRowEditPtrCR	back()
		{
			assert (!m_vecRowEdit.empty());
			return m_vecRowEdit.back();
		}

		virtual void			reserve(UINT count) { m_vecRowEdit.reserve(count); }
		virtual void			swap(IRowEditArray *pArrRowEdit)
		{
			CRowEditArrayImpl *pImpl = static_cast<CRowEditArrayImpl *> (pArrRowEdit);//need not dynamic_cast.
			m_vecRowEdit.swap(pImpl->m_vecRowEdit);
		}

		virtual void			clear()
		{
			for (UINT i = 0; i < m_vecRowEdit.size(); i++)
				m_vecRowEdit[i]->release();
			m_vecRowEdit.clear();
		}

		virtual BOOL			empty() { return m_vecRowEdit.empty(); }

		//区别于pash_back，内部不会调用addRef，当CRowEditArrayImpl销毁时，自动调用release释放
		inline void push_back_take_ownership(IRowEdit *pRowEdit)
		{
			assert(NULL != pRowEdit);
			m_vecRowEdit.push_back(pRowEdit);
		}

	private:
		CRowEditArrayImpl(void) { m_dwRef = 1; }
		~CRowEditArrayImpl(void) { assert(m_dwRef == 0); clear(); }

		DWORD				m_dwRef;
		DWVector<IRowEdit*>	m_vecRowEdit;
	};
};
