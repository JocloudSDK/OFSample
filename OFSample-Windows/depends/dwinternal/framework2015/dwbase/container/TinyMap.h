#pragma once

#include "TinyBase.h"

template<class Key, class Type>
struct map_traits{
	
	typedef map_traits<Key, Type> _Myt;

	map_traits() : key(), value() {}
	map_traits(Key k, const Type& v) : key(k), value(v) {}
	map_traits(const _Myt &src) : key(src.key), value(src.value) {}
	bool operator < (const _Myt &src) const { return key < src.key; }
	
	Key key;
	Type value;
};

template<class Key, class Type, UINT BUCKET_SIZE = 0xFF>
class DWTinyMap : public DWTinyBase<map_traits<Key, Type>, BUCKET_SIZE>
{
public:
	typedef DWTinyMap<Key, Type, BUCKET_SIZE> _Myt;
	typedef DWTinyBase<map_traits<Key, Type>, BUCKET_SIZE>	_Mybase;
	typedef map_traits<Key, Type>							node_type;

	DWTinyMap()
	{ 
		check_type_at_compile(Key()); 
		check_type_at_compile(Type());
	}
	DWTinyMap(const _Myt & src) : _Mybase(src)
	{ 
		check_type_at_compile(Key());
		check_type_at_compile(Type());
	}
	~DWTinyMap() {}

	//return说明：
	//	first	如果指定row已经存在，返回值的地址，否则返回新插入的值的地址
	//	second	是否真的插入了
	std::pair<Type *, BOOL> insert(Key key, const Type &value)
	{
		std::pair<node_type *, BOOL> result = _Mybase::insert(node_type(key, value));
		return std::pair<Type *, BOOL>(&result.first->value, result.second);
	}

	Type & operator [] (Key key)
	{
		std::pair<node_type *, BOOL> result = _Mybase::insert(node_type(key, Type()));
		return result.first->value;		
	}

	_Myt & operator = (const _Myt & src)
	{
		clear();
		copy_from(src); 
		return *this;	
	}

	std::pair<Type, BOOL> erase(Key key)
	{
		std::pair<node_type, BOOL> result = _Mybase::erase(key);
		return std::pair<Type, BOOL>(result.first.value, result.second);
	}

	const Type * find(Key key) const
	{
		std::pair<node_type *, BOOL> result = _Mybase::find(key);
		if (result.second)
			return &result.first->value;
		return NULL;
	}

	Type * find(Key key)
	{
		std::pair<node_type *, BOOL> result = _Mybase::find(key);
		if (result.second)
			return &result.first->value;
		return NULL;
	}

	//如果外面需要排序后集合，可以用先取出vector，然后std::sort排序
	void all(std::vector<node_type> & vec) const
	{
		vec.clear();
		vec.reserve(m_dwSize);

		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			_Mybase::NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				for (UINT j = 0; j < pSet->dwCount; j++)
					vec.push_back(pSet->pNodes[j]);
			}
		}

		assert(vec.size() == m_dwSize);
	}

	void all_values(std::vector<Type> & vec) const
	{
		vec.clear();
		vec.reserve(m_dwSize);

		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			_Mybase::NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				for (UINT j = 0; j < pSet->dwCount; j++)
					vec.push_back(pSet->pNodes[j].value);
			}
		}

		assert(vec.size() == m_dwSize);
	}

	void all_values_addr(std::vector<Type *> & vec) const
	{
		vec.clear();
		vec.reserve(m_dwSize);

		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			_Mybase::NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				for (UINT j = 0; j < pSet->dwCount; j++)
					vec.push_back(&pSet->pNodes[j].value);
			}
		}

		assert(vec.size() == m_dwSize);
	}

	/*比all取出来排序慢，先不公开
	void to_map(std::map<Key, Type> & m) const
	{
		m.clear();
		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			_Mybase::NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				for (UINT j = 0; j < pSet->dwCount; j++)
					m[pSet->pNodes[j].key] = pSet->pNodes[j].value;
			}
		}

		assert(m.size() == m_dwSize);
	}
	*/

	void swap(_Myt & right)
	{
		_Mybase::swap(right);
	}

	void clear()
	{
		return _Mybase::clear();
	}

	UINT size() const 
	{
		return _Mybase::size();
	}
};
