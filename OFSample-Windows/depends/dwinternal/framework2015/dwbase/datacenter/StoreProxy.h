#pragma once

#include <assert.h>
#include <vector>
#include "container/ContainerDef.h"
#include "ITablePub.h"
#include "MemPool/MemPool.h"

namespace Data
{
	template<class Type>
	class CStoreProxy
	{
	public:
		CStoreProxy(E_KEY_TYPE eKeyType, E_INDEX_TYPE eIndexType);
		~CStoreProxy(void);

		//return说明：
		//	first	如果指定row已经存在，返回pVal值的地址，否则返回新插入pVal的值的地址，如果操作失败，返回NULL。
		//	second	是否真的插入了
		std::pair<Type **, BOOL> Insert(const KEY& oKey, Type * pVal);

		Type**				Find(const KEY& oKey) const;	//如果没有找到，返回NULL
		void				GetAll(std::vector<Type *> &vec) const;
		void				GetAllAddr(std::vector<Type **> &vec) const;

		void				Clear();
		Type *				Erase(const KEY& oKey);			//如果没有找到，返回NULL

		UINT				Size() const;
		BOOL				Empty() const;

		E_KEY_TYPE			GetKeyType() const;
		E_INDEX_TYPE		GetIndexType() const;

	private:
		struct wstr_less
		{
			bool operator()(const LPCWSTR& _Left, const LPCWSTR& _Right) const
			{
				return wcscmp(_Left, _Right) < 0;
			}
		};

		typedef DWMap<DWORD, Type *> MapDword;
		typedef DWMap<LPCWSTR, Type *, wstr_less> MapString;
		typedef DWHashMap<DWORD, Type *> HashDword;
		typedef DWHashMap<LPCWSTR, Type *> HashString; //备注：hash_compare支持const wchar *做字符串内容hash
		typedef DWTinyMap<DWORD, Type *, 0xFFF> TinyMap;

		union UnPointer
		{
			MapDword*	pMapDw;
			MapString*	pMapStr;
			HashDword*	pHashDw;
			HashString*	pHashStr;
			TinyMap*	pTinyMap;	
		};

		UnPointer	m_Store;

		enum E_TYPE
		{
			TYPE_E_MAP_DWORD,
			TYPE_E_MAP_STRING,
			TYPE_E_HASH_DWORD,
			TYPE_E_HASH_STRING,
			TYPE_E_TINY_MAP,//只支持DWORD
		};
		E_TYPE		m_eType;
	};




	//CStoreProxy实现*****************************************************************************************

	template<class Type>
	CStoreProxy<Type>::CStoreProxy(E_KEY_TYPE eKeyType, E_INDEX_TYPE eIndexType) 
	{
		assert(eKeyType == KEY_E_DWORD || eKeyType == KEY_E_STR);

		if (eIndexType == INDEX_E_MAP)
		{
			m_eType = (eKeyType == KEY_E_DWORD ? TYPE_E_MAP_DWORD : TYPE_E_MAP_STRING);
		}
		else if (eIndexType == INDEX_E_HASH)
		{
			m_eType = (eKeyType == KEY_E_DWORD ? TYPE_E_HASH_DWORD : TYPE_E_HASH_STRING);
		}
		else if (eIndexType == INDEX_E_TINY_MAP)
		{
			assert(eKeyType == KEY_E_DWORD);
			m_eType = TYPE_E_TINY_MAP;
		}
		else
		{
			assert(! L"不支持的索引类型");
		}


		switch(m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				m_Store.pMapDw = new MapDword;
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				m_Store.pMapStr = new MapString;
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				m_Store.pHashDw = new HashDword;
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				m_Store.pHashStr = new HashString;
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				m_Store.pTinyMap = new TinyMap;
			}
			break;
		default:
			assert(0);
		}
	}

	template<class Type>
	CStoreProxy<Type>::~CStoreProxy(void)
	{
		Clear();

		switch(m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				delete m_Store.pMapDw;
				m_Store.pMapDw = NULL;
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				delete m_Store.pMapStr;	
				m_Store.pMapStr = NULL;
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				delete m_Store.pHashDw;	
				m_Store.pHashDw = NULL;
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				delete m_Store.pHashStr;	
				m_Store.pHashStr = NULL;
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				delete m_Store.pTinyMap;
				m_Store.pTinyMap = NULL;
			}
			break;
		default:
			assert(0);
		}
	}

	template<class Type>
	void CStoreProxy<Type>::GetAll(std::vector<Type *> &vec) const
	{
		vec.clear();

		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				vec.reserve(m_Store.pMapDw->size());
				for (MapDword::iterator it = m_Store.pMapDw->begin(); it != m_Store.pMapDw->end(); ++it)
					vec.push_back(it->second);
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				vec.reserve(m_Store.pMapStr->size());
				for (MapString::iterator it = m_Store.pMapStr->begin(); it != m_Store.pMapStr->end(); ++it)
					vec.push_back(it->second);
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				vec.reserve(m_Store.pHashDw->size());
				for (HashDword::iterator it = m_Store.pHashDw->begin(); it != m_Store.pHashDw->end(); ++it)
					vec.push_back(it->second);
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				vec.reserve(m_Store.pHashStr->size());
				for (HashString::iterator it = m_Store.pHashStr->begin(); it != m_Store.pHashStr->end(); ++it)
					vec.push_back(it->second);
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				m_Store.pTinyMap->all_values(vec);
			}
			break;
		default:
			assert(0);
		}
	}

	template<class Type>
	void CStoreProxy<Type>::GetAllAddr(std::vector<Type **> &vec) const
	{
		vec.clear();

		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				vec.reserve(m_Store.pMapDw->size());
				for (MapDword::iterator it = m_Store.pMapDw->begin(); it != m_Store.pMapDw->end(); ++it)
					vec.push_back(&it->second);
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				vec.reserve(m_Store.pMapStr->size());
				for (MapString::iterator it = m_Store.pMapStr->begin(); it != m_Store.pMapStr->end(); ++it)
					vec.push_back(&it->second);
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				vec.reserve(m_Store.pHashDw->size());
				for (HashDword::iterator it = m_Store.pHashDw->begin(); it != m_Store.pHashDw->end(); ++it)
					vec.push_back(&it->second);
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				vec.reserve(m_Store.pHashStr->size());
				for (HashString::iterator it = m_Store.pHashStr->begin(); it != m_Store.pHashStr->end(); ++it)
					vec.push_back(&it->second);
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				m_Store.pTinyMap->all_values_addr(vec);
			}
			break;
		default:
			assert(0);
		}
	}

	template<class Type>
	std::pair<Type **, BOOL> CStoreProxy<Type>::Insert(const KEY& oKey, Type * pVal)
	{
		std::pair<Type **, BOOL> ret(NULL, FALSE);

		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				assert(oKey.eType == KEY_E_DWORD);

				std::pair<MapDword::iterator, bool> res = m_Store.pMapDw->insert(std::make_pair(oKey.unKey.dwKey, pVal));
				ret.second = res.second;
				ret.first = &res.first->second;
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				assert(oKey.eType == KEY_E_STR);
				assert(NULL != oKey.unKey.lpszKey);

				UINT cbSize = (wcslen(oKey.unKey.lpszKey) + 1) * sizeof(WCHAR);
				WCHAR *p = (WCHAR *)mp_alloc(cbSize);
				memcpy(p, oKey.unKey.lpszKey, cbSize);
				
				std::pair<MapString::iterator, bool> res = m_Store.pMapStr->insert(std::make_pair(p, pVal));
				if (!res.second)
					mp_free(p);

				ret.second = res.second;
				ret.first = &res.first->second;
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				assert(oKey.eType == KEY_E_DWORD);

				std::pair<HashDword::iterator, bool> res = m_Store.pHashDw->insert(std::make_pair(oKey.unKey.dwKey, pVal));
				ret.second = res.second;
				ret.first = &res.first->second;
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				assert(oKey.eType == KEY_E_STR);
				assert(NULL != oKey.unKey.lpszKey);

				UINT cbSize = (wcslen(oKey.unKey.lpszKey) + 1) * sizeof(WCHAR);
				WCHAR *p = (WCHAR *)mp_alloc(cbSize);
				memcpy(p, oKey.unKey.lpszKey, cbSize);

				std::pair<HashString::iterator, bool> res = m_Store.pHashStr->insert(std::make_pair(p, pVal));
				if (!res.second)
					mp_free(p);

				ret.second = res.second;
				ret.first = &res.first->second;
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				assert(oKey.eType == KEY_E_DWORD);

				ret = m_Store.pTinyMap->insert(oKey.unKey.dwKey, pVal);
			}
			break;
		default:
			assert(0);
			break;
		}

		return ret;
	}

	template<class Type>
	Type** CStoreProxy<Type>::Find(const KEY& oKey) const
	{
		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				MapDword::iterator it = m_Store.pMapDw->find(oKey.unKey.dwKey);
				if (it == m_Store.pMapDw->end())
					return NULL;

				return &it->second;
			}
			
		case TYPE_E_MAP_STRING:
			{
				assert(NULL != oKey.unKey.lpszKey);
				MapString::iterator it = m_Store.pMapStr->find(oKey.unKey.lpszKey);
				if (it == m_Store.pMapStr->end())
					return NULL;

				return &it->second;
			}

		case TYPE_E_HASH_DWORD:
			{
				HashDword::iterator it = m_Store.pHashDw->find(oKey.unKey.dwKey);
				if (it == m_Store.pHashDw->end())
					return NULL;

				return &it->second;
			}

		case TYPE_E_HASH_STRING:
			{
				assert(NULL != oKey.unKey.lpszKey);
				HashString::iterator it = m_Store.pHashStr->find(oKey.unKey.lpszKey);
				if (it == m_Store.pHashStr->end())
					return NULL;

				return &it->second;
			}
		case TYPE_E_TINY_MAP:
			{
				assert(oKey.eType == KEY_E_DWORD);
				return m_Store.pTinyMap->find(oKey.unKey.dwKey);
			}
		default:
			{
				assert(0);
				return NULL;
			}
		}
	}

	template<class Type>
	Type * CStoreProxy<Type>::Erase(const KEY& oKey)
	{
		Type * pRet = NULL;

		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			{
				MapDword::iterator it = m_Store.pMapDw->find(oKey.unKey.dwKey);
				if (it != m_Store.pMapDw->end())
				{
					pRet = it->second;
					m_Store.pMapDw->erase(it);
				}
			}
			break;
		case TYPE_E_MAP_STRING:
			{
				assert(NULL != oKey.unKey.lpszKey);
				MapString::iterator it = m_Store.pMapStr->find(oKey.unKey.lpszKey);
				if (it != m_Store.pMapStr->end())
				{
					pRet = it->second;
					mp_free(const_cast<wchar_t *>(it->first));
					m_Store.pMapStr->erase(it);
				}
			}
			break;
		case TYPE_E_HASH_DWORD:
			{
				HashDword::iterator it = m_Store.pHashDw->find(oKey.unKey.dwKey);
				if (it != m_Store.pHashDw->end())
				{
					pRet = it->second;
					m_Store.pHashDw->erase(it);
				}
			}
			break;
		case TYPE_E_HASH_STRING:
			{
				assert(NULL != oKey.unKey.lpszKey);
				HashString::iterator it = m_Store.pHashStr->find(oKey.unKey.lpszKey);
				if (it != m_Store.pHashStr->end())
				{
					pRet = it->second;
					mp_free(const_cast<wchar_t *>(it->first));
					m_Store.pHashStr->erase(it);
				}
			}
			break;
		case TYPE_E_TINY_MAP:
			{
				assert(oKey.eType == KEY_E_DWORD);
				std::pair<Type *, BOOL> result = m_Store.pTinyMap->erase(oKey.unKey.dwKey);
				if (result.second)
					pRet = result.first;
			}
			break;
		default:
			assert(0);
		}

		return pRet;
	}

	template<class Type>
	void CStoreProxy<Type>::Clear()
	{
		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			m_Store.pMapDw->clear();
			break;

		case TYPE_E_MAP_STRING:
			for (MapString::iterator it = m_Store.pMapStr->begin(); it !=  m_Store.pMapStr->end(); ++it)
				mp_free(const_cast<wchar_t *>(it->first));
			m_Store.pMapStr->clear();
			break;

		case TYPE_E_HASH_DWORD:
			m_Store.pHashDw->clear();
			break;

		case TYPE_E_HASH_STRING:
			for (HashString::iterator it = m_Store.pHashStr->begin(); it !=  m_Store.pHashStr->end(); ++it)
				mp_free(const_cast<wchar_t *>(it->first));
			m_Store.pHashStr->clear();
			break;

		case TYPE_E_TINY_MAP:
			m_Store.pTinyMap->clear();
			break;

		default:
			assert(0);
		}
	}

	template<class Type>
	UINT CStoreProxy<Type>::Size() const
	{
		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			return m_Store.pMapDw->size();

		case TYPE_E_MAP_STRING:
			return m_Store.pMapStr->size();

		case TYPE_E_HASH_DWORD:
			return m_Store.pHashDw->size();

		case TYPE_E_HASH_STRING:
			return m_Store.pHashStr->size();

		case TYPE_E_TINY_MAP:
			return m_Store.pTinyMap->size();
		default:
			assert(0);
			return 0;
		}
	}

	template<class Type>
	BOOL CStoreProxy<Type>::Empty() const
	{
		switch (m_eType)
		{
		case TYPE_E_MAP_DWORD:
			return m_Store.pMapDw->empty();
		
		case TYPE_E_MAP_STRING:
			return m_Store.pMapStr->empty();
		
		case TYPE_E_HASH_DWORD:
			return m_Store.pHashDw->empty();
		
		case TYPE_E_HASH_STRING:
			return m_Store.pHashStr->empty();
		
		case TYPE_E_TINY_MAP:
			return m_Store.pTinyMap->size() == 0;

		default:
			assert(0);
			return TRUE;
		}
	}

	template<class Type>
	E_KEY_TYPE CStoreProxy<Type>::GetKeyType() const
	{
		E_KEY_TYPE eKeyType = KEY_E_NULL;
		switch (m_eType)
		{
		case TYPE_E_HASH_DWORD:
		case TYPE_E_MAP_DWORD:
		case TYPE_E_TINY_MAP:
			eKeyType = KEY_E_DWORD;
			break;

		case TYPE_E_MAP_STRING:
		case TYPE_E_HASH_STRING:
			eKeyType = KEY_E_STR;
			break;

		default:
			assert(0);
			break;
		}

		return eKeyType;
	}

	template<class Type>
	E_INDEX_TYPE CStoreProxy<Type>::GetIndexType() const
	{
		E_INDEX_TYPE eIndexType = INDEX_E_NULL;
		switch (m_eType)
		{
		case TYPE_E_MAP_STRING:
		case TYPE_E_MAP_DWORD:
			eIndexType = INDEX_E_MAP;
			break;

		case TYPE_E_HASH_DWORD:
		case TYPE_E_HASH_STRING:
			eIndexType = INDEX_E_HASH;
			break;
		
		case TYPE_E_TINY_MAP:
			eIndexType = INDEX_E_TINY_MAP;
			break;

		default:
			assert(0);
			break;
		}

		return eIndexType;
	}
}


