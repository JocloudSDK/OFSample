#pragma once

#include "../mempool/MemPool.h"


namespace Data
{

#pragma pack(push, 2)

	template<typename T>
	class CFieldString
	{
	public:
		inline static CFieldString * createInstance(const T* str, DWORD dwStrLen)
		{
			assert(sizeof(CFieldString) == sizeof(DWORD) + sizeof(WORD));

			UINT dwSize = sizeof(T) * (dwStrLen + 1);
			CFieldString *p = (CFieldString *)mp_alloc(sizeof(CFieldString) + dwSize);
			
			p->m_dwRef = 1;
			p->m_wLenLow = (WORD)dwStrLen;
			memcpy(p + 1, str, dwSize);

			return p;
		}

		inline const T* getStr() const
		{
			return (const T*)(this + 1);
		}

		inline BOOL isEqual(const T *str, DWORD dwStrLen) const
		{
			return m_wLenLow == (WORD)dwStrLen && memcmp(getStr(), str, sizeof(T) * dwStrLen) == 0;
		}

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
				m_wLenLow = 0;
				mp_free(this);
			}
		}

	private:
		CFieldString(void); //不用实现，调不着
		~CFieldString(void);//不用实现，调不着

		DWORD			m_dwRef;
		WORD			m_wLenLow;
	};

#pragma pack(pop)

	typedef CFieldString<wchar_t> CFieldStringW;
	typedef CFieldString<char> CFieldStringA;
};

