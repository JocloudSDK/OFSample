/** 
@file
@brief 引用计数器工具类
@version 2012-08-28 wesleywu
*/

#include "refcount.h"

namespace Perf
{
	CRefCount::CRefCount() : ref_count(new int(1))
	{ 
	}

	CRefCount::CRefCount(const CRefCount& r)
		: ref_count(r.ref_count)
	{
		(*ref_count)++;
	}

	CRefCount::~CRefCount()
	{
		(*ref_count)--;
		if ((*ref_count) == 0)
		{
			delete ref_count;
		}
	}

	bool CRefCount::attach(const CRefCount& r)
	{
		(*ref_count)--;
		(*r.ref_count)++;

		if ((*ref_count) == 0)
		{
			delete ref_count;
			ref_count = r.ref_count;
			return true;
		}

		ref_count = r.ref_count;
		return false;
	}
}