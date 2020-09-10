#pragma once 

#ifndef _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
//static_assert(false, "<hash_map> is deprecated and will be REMOVED. "
//	"Please use <unordered_map>. You can define "
//	"_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS "
//	"to acknowledge that you have received this warning.");
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#endif /* _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS */

#include <vector>
#include <list>
#include <set>
#include <map>
#include <hash_map>
#include "MemAlloctor.h"
#include "TinySet.h"
#include "TinyMap.h"

template<class _Ty,
class _Ax = CMemAlloc<_Ty> >
class DWVector : public std::vector<_Ty, _Ax>
{
};

template<class _Ty,
class _Ax = CMemAlloc<_Ty> >
class DWList : public std::list<_Ty, _Ax >
{
};

template<class _Kty,
class _Pr = std::less<_Kty>,
class _Alloc = CMemAlloc<_Kty> >
class DWSet	: public std::set<_Kty, _Pr, _Alloc >
{
};

template<class _Kty,
class _Ty,
class _Pr = std::less<_Kty>,
class _Alloc = CMemAlloc<std::pair<const _Kty, _Ty> > >
class DWMap	: public std::map<_Kty, _Ty, _Pr, _Alloc >
{
};

template<class _Kty,
class _Ty,
class _Tr = stdext::hash_compare<_Kty, std::less<_Kty> >,
class _Alloc = CMemAlloc< std:: pair<const _Kty, _Ty> > >
class DWHashMap : public stdext::hash_map<_Kty, _Ty, _Tr, _Alloc >
{
};

