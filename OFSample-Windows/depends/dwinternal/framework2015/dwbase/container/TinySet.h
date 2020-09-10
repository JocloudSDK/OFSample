#pragma once

#include "TinyBase.h"

template<class Key>
struct set_traits{
	set_traits() : key(){}
	Key key;
};

template<class Key, UINT BUCKET_SIZE = 0xff>
class DWTinySet : public DWTinyBase<set_traits<Key>, BUCKET_SIZE>
{
	typedef DWTinySet<Key, BUCKET_SIZE> _Myt;
	typedef DWTinyBase<set_traits<Key>, BUCKET_SIZE> _Mybase;

public:
	DWTinySet()
	{ 
		check_type_at_compile(Key());
	}
	DWTinySet(const _Myt & src) : _Mybase(src) 
	{ 
		check_type_at_compile(Key());
	}
	~DWTinySet() {}

	BOOL insert(Key key)
	{
		set_traits<Key> node;
		node.key = key;
		return _Mybase::insert(node).second;
	}

	BOOL erase(Key key)
	{
		return _Mybase::erase(key).second;
	}

	BOOL has(Key key) const
	{
		return _Mybase::find(key).second;
	}

	//如果外面需要排序后集合，可以用先取出vector，然后std::sort排序
	void all(std::vector<Key> &vec) const
	{
		vec.clear();
		vec.reserve(m_dwSize);

		for (UINT i = 0; i < m_vecData.size(); i++)
		{
			_Mybase::NOTE_SET *pSet = m_vecData[i];
			if (pSet)
			{
				for (UINT j = 0; j < pSet->dwCount; j++)
					vec.push_back(pSet->pNodes[j].key);
			}
		}

		assert(vec.size() == m_dwSize);
	}

	void clear()
	{
		return _Mybase::clear();
	}

	UINT size() const 
	{
		return _Mybase::size();
	}		

	void swap(_Myt & right)
	{
		_Mybase::swap(right);
	}

	_Myt & operator = (const _Myt & src)
	{
		clear();
		copy_from(src); 
		return *this;	
	}
};
