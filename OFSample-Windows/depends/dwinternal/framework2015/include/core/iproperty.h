
#ifndef __POPO3_PROPERTY_HPP_INCLUDE__
#define __POPO3_PROPERTY_HPP_INCLUDE__

#include <map>
#include <set>
#include <list>
#include <string>

#include "core/int_types.h"
#include "core/packet.h"

namespace sox
{

/** simple check only */
inline unsigned xatol(const char * s, size_t len, const char ** end = NULL)
{
	long n = 0;
	size_t i = 0;
	for (; i<len; ++i)
	{
		int t = s[i] - '0';
		if (t>9 || t<0) break;
		n = n * 10 + t;
	}
	if (end) *end = (i<len) ? (s + i) : NULL;
	return n;
}

/**  uint2string */
inline std::string ultoa10(size_t n)
{
	char abuf[16]; // XXX
	char *p = abuf + 15;
	*p-- = 0;
	do
	{
		*p-- = char('0' + (n % 10));
	} while (n /= 10);
	return std::string(p + 1, abuf + 14 - p);
}

typedef std::map<uint16_t, std::string> properties_type;

struct Properties : public sox::Marshallable
{
	properties_type props;
	typedef properties_type::iterator  iterator;
	typedef properties_type::const_iterator const_iterator;

	Properties()
	{
		props.clear();
	}

	virtual void marshal(sox::Pack & p) const
	{
		sox::marshal_container(p, props);
	}

	// 不清除已有的数据，按 replace 方式合并进去
	// 如： props[first] = second
	// 实现在后面
	virtual void unmarshal(const sox::Unpack &p);
	virtual std::ostream & trace(std::ostream & os) const;
};

typedef std::map<std::string, Properties> map2props_t;

typedef std::map<uint32_t, Properties> sid2props_map_t;

struct MapProperties : public sox::Marshallable
{
	map2props_t mapp;

	virtual void marshal(sox::Pack & p) const
	{
		sox::marshal_container(p, mapp);
	}

	// 不清除已有的数据，按 replace 方式合并进去
	// 如： props[first] = second
	// 实现在后面
	virtual void unmarshal(const sox::Unpack &p);
	virtual std::ostream & trace(std::ostream & os) const
	{
		return sox::trace_container(os, mapp);
	}
};

struct UidProperties : public Properties
{
	std::string uid;

	virtual void marshal(sox::Pack &p) const 
	{
		p << uid;
		Properties::marshal(p);
	}
	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uid;
		Properties::unmarshal(p);
	}
	virtual std::ostream & trace(std::ostream & os) const
	{
		os << uid << "\n";
		return Properties::trace(os);
	}

	void swap(UidProperties & b)
	{
		uid.swap(b.uid);
		props.swap(b.props);
	}
};

struct EditProperties : public Properties
{
	uint16_t op;
	uint16_t tag;

	EditProperties()
	{
		op = 0;
		tag = 0;
	}

	virtual void marshal(sox::Pack & p) const
	{
		p << op << tag;
		Properties::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> op >> tag;
		Properties::unmarshal(p);
	}

	virtual std::ostream & trace(std::ostream & os) const
	{
		os << "op=" << op << " tag=" << tag << "\n";
		return Properties::trace(os);
	}

	void swap(EditProperties & b)
	{
		std::swap(op, b.op);
		std::swap(tag, b.tag);
		props.swap(b.props);
	}
};

struct UidEditProperties : public EditProperties
{
	std::string uid;

	virtual void marshal(sox::Pack & p) const
	{
		p << uid;
		EditProperties::marshal(p);
	}
	
	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uid;
		EditProperties::unmarshal(p);
	}
	
	virtual std::ostream & trace(std::ostream & os) const
	{
		os << uid << "\n";
		return EditProperties::trace(os);
	}

	void swap(UidEditProperties & b)
	{
		uid.swap(b.uid);
		EditProperties::swap(b);
	}
};

namespace properties
{

/////////////////////////////////////////////////////////////////////
inline void swap(Properties & a, Properties & b) {  a.props.swap(b.props); }

// 没有找到返回空串
inline std::string get(const Properties &a, uint32_t tag, const std::string & _def)
{
	properties_type::const_iterator i = a.props.find(tag);
	return (i != a.props.end()) ? i->second : _def;
}

inline std::string get(const Properties & a, uint32_t tag)
{
	return get(a, tag, std::string());
}

inline const std::string * find(const Properties & a, uint32_t tag)
{
	properties_type::const_iterator i = a.props.find(tag);
	return (i != a.props.end()) ? &(i->second) : NULL;
}

/////////////////////////////////////////////////////////////////////
// translate
inline int getint(const Properties & a, uint32_t tag, int _default = 0)
{
	if (const std::string * val = find(a, tag))
		return sox::xatol(val->data(), val->size());
	return _default;
}

inline void setint(Properties & a, uint32_t tag, unsigned val)
{
	a.props[tag] = sox::ultoa10(val);
}

inline void copyif(Properties & a, const Properties & b, uint32_t tag)
{
	if (const std::string * val = find(b, tag))
		a.props[tag] = *val;
}

//////////////////////////////////////////////////////////////////
// 
// 修改属性 (不存在就添加)
// 修改检测: 返回 true 表示有改变
// 参数 apply 表示是否真的修改，或者只是检测
inline bool replace(properties_type & a,
		const properties_type::value_type & value, bool apply = true)
{
	std::pair<properties_type::iterator, bool> r = a.insert(value);

	if (!r.second)
	{
		// exist
		if (value.second == r.first->second)
			return false; // same
		else if (apply)
			r.first->second = value.second; // exist and update
	}
	return true;
}

inline bool replace(properties_type & a,
		uint32_t tag, const std::string & value, bool apply = true)
{
	return replace(a, std::make_pair(tag, value), apply);
}

inline bool replace(Properties & a,
		uint32_t tag, const std::string & value, bool apply = true)
{
	return replace(a.props, std::make_pair(tag, value), apply);
}

inline bool replace(Properties & a,
		const properties_type::value_type & value, bool apply = true)
{
	return replace(a.props, value, apply);
}

inline bool remove(Properties & a, uint32_t tag, bool apply = true)
{
	properties_type::iterator i = a.props.find(tag);
	if (i != a.props.end())
	{
		// find
		if (apply) a.props.erase(i);
		return true; 
	}
	else
		return false;
}

// 把 b 的所有属性合并到 a 中
inline bool replace(Properties & a, const Properties & b, bool apply = true)
{
	bool changed = false;
	for (properties_type::const_iterator i = b.props.begin(); i != b.props.end(); ++i)
		changed = replace(a, *i, apply) || changed; // 必须放在后面，教训
	return changed;
}

// 从 a 中删除所有 b 中的属性
inline bool remove(Properties & a, const Properties & b, bool apply = true)
{
	bool changed = false;
	for (properties_type::const_iterator i = b.props.begin(); i != b.props.end(); ++i)
		changed = remove(a, i->first, apply) || changed; // 必须放在后面，教训
	return changed;
}

inline bool replace_test(const Properties & a, uint32_t tag, const std::string & value)
{
	if (const std::string * p = find(a, tag))
		return *p != value;
	return true;
}

// 把 b 中不产生效果的属性删除，不改变 a
inline void replace_diff(const Properties & a, Properties & b)
{
	Properties tmp;
	for (properties_type::const_iterator i = b.props.begin();
		i != b.props.end(); ++i)
	{
		if (replace_test(a, i->first, i->second))
			tmp.props.insert(*i);
	}
	swap(b, tmp);
}

//////////////////////////////////////////////////////////////////////
// MapProperties
inline void swap(MapProperties & a, MapProperties & b) { a.mapp.swap(b.mapp); }

inline void replace(map2props_t & a, const map2props_t & b)
{
	for (map2props_t::const_iterator i = b.begin(); i != b.end(); ++i)
		replace(a[i->first], i->second);
}

inline void replace(MapProperties & a, const MapProperties & b)
{
	replace(a.mapp, b.mapp);
}

inline void replace(map2props_t & a, const MapProperties & b)
{
	replace(a, b.mapp);
}

// 把 b 的所有属性合并到 a 中，不检查是否改变
inline void replace_nocheck(std::string & a, const std::string & b)
{
	a = b;
}

// 把 b 的所有属性合并到 a 中，不检查是否改变
inline void replace_nocheck(Properties & a, const Properties & b)
{
	for (properties_type::const_iterator i = b.props.begin(); i != b.props.end(); ++i)
		a.props[i->first] = i->second;
}

template < class _Map >
struct map_replace_iterator
#ifdef _MSC_VER
#if _MSC_VER > 1200
	: public std::iterator<std::output_iterator_tag, void, void, void, void>
#else 
	: public std::iterator<std::output_iterator_tag, void, void>
#endif 
#else
	: public std::iterator<std::output_iterator_tag, void, void, void, void>
#endif
{
	typedef map_replace_iterator<_Map> _this_t;
	typedef _Map container_type;
	typedef typename container_type::reference reference;

	explicit map_replace_iterator(container_type& _Cont) : _map(&_Cont) { }
	_this_t& operator=(typename container_type::const_reference _value)
	{
		std::pair<typename container_type::iterator, bool> r =
			_map->insert(_value);
		if (!r.second)
			replace_nocheck(r.first->second, _value.second);
		return (*this);
	}

	_this_t& operator*()     { return (*this); }
	_this_t& operator++()    { return (*this); }
	_this_t& operator++(int) { return (*this); }

	container_type * _map;	// pointer to container
};

template < class _Map >
map_replace_iterator<_Map> replace_iterator(_Map & _map)
{
	return map_replace_iterator<_Map>(_map);
}
} // namespace properties

inline void Properties::unmarshal(const sox::Unpack &p)
{
	sox::unmarshal_container(p, properties::replace_iterator(props));
}

inline void MapProperties::unmarshal(const sox::Unpack &p)
{
	sox::unmarshal_container(p, properties::replace_iterator(mapp));
}

inline std::ostream & Properties::trace(std::ostream & os) const
{
	for (properties_type::const_iterator i = props.begin(); i != props.end(); ++i)
	{
		os << "(" << std::hex << i->first;
		os << std::dec << "," << i->second;
		os << ")\n";
	}
	return os;
}

} // namespace popo3

#endif  // __POPO3_PROPERTY_HPP_INCLUDE__
