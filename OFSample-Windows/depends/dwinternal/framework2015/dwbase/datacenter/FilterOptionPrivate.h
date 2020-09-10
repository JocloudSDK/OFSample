#pragma once

#include "StringTable.h"
#include "../variant/Variant.h"
#include "DataCenterPub.h"
#include "TableDataImpl.h"

namespace Data 
{
	class CFilterOptionPrivate
	{
	public:
		struct RAW_ORDER_CASE
		{
			NAME_ID		name;
			DWVariant	value;
		};

		CFilterOptionPrivate();
		~CFilterOptionPrivate();	
		
		void	orderBy(const NAME_ID &name, BOOL bAscending);
		void	setLimit(DWORD dwLimitCount);
		DWORD	getLimit() const { return m_dwLimitCount; }
		
		BOOL	getAllOrdercase(const CTableDataImpl::REAL_COLS *pCols, std::vector<ORDER_CASE> &vecCase) const;
	
	
	private:
		std::vector<RAW_ORDER_CASE>		m_vecOrderCase;
		DWORD							m_dwLimitCount;
	};
};