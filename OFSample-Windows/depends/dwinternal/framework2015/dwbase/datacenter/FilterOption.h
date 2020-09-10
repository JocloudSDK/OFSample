#pragma once

#include "StringTable.h"

namespace Data 
{
	class CFilterOptionPrivate;
	class DWBASE_EXPORTS CFilterOption
	{
	public:
		CFilterOption();
		~CFilterOption();

		//可以指定多个字段，做复合排序，
		CFilterOption & orderBy(const NAME_ID &name, BOOL bAscending = TRUE);
		
		//设置最大数目，例如可以提取排序后的头100个元素，默认不设限制
		CFilterOption & setLimit(DWORD dwLimitCount);
		
		operator const CFilterOption *() const;

		const CFilterOptionPrivate * _private() const;
	
	private:
		CFilterOptionPrivate * m_pri;
	};
};