#pragma once

#include "../mempool/MemPool.h"

namespace Data
{
	class CFieldBlob
	{
	public:
		inline static CFieldBlob * createInstance(const BYTE * pbData, DWORD cbSize)
		{
			CFieldBlob *p = (CFieldBlob *)mp_alloc(sizeof(CFieldBlob) + cbSize);
			
			p->m_dwRef = 1;
			p->m_cbSize = cbSize;
			memcpy(p + 1, pbData, cbSize);

			return p;
		}

		inline const BYTE * getPtr() const { return (const BYTE *)(this + 1); }
		inline DWORD getSize() const { return m_cbSize; }
		inline BOOL isEqual(const BYTE * pbData, DWORD cbSize) const { return cbSize == m_cbSize && memcmp(getPtr(), pbData, cbSize) == 0; }

		inline void addRef()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			::InterlockedIncrement((LONG *)&m_dwRef);
		}

		inline void release()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			{
				m_cbSize = 0;
				mp_free(this);
			}
		}

	private:
		CFieldBlob(void); //不用实现，调不着
		~CFieldBlob(void);//不用实现，调不着

		DWORD			m_dwRef;
		DWORD			m_cbSize;
	};
};

