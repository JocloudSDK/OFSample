/*! @file refcount.h 
 *  @brief 引用计数工具类
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */
#pragma once

namespace Perf
{
	class CRefCount
	{
	public:
		CRefCount();

		CRefCount(const CRefCount& r);

		~CRefCount();

		inline bool only()
		{
			return ((*ref_count) == 1);
		}


		bool attach(const CRefCount& r);

	private:
		CRefCount &operator=(const CRefCount& r);

		int *ref_count;
	};
}