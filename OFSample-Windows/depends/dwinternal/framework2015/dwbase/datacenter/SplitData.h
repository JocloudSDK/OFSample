#pragma once

#include "StringTable.h"
#include "ITable.h"
#include "Filter.h"

namespace Data 
{
	class CSplitDataPrivate;
	class DWBASE_EXPORTS CSplitData
	{
	public:
		struct GROUP
		{
			int	nGroupId;
			IRowArrayPtr pRowArr;
		};

		void addIf(const CFilter * pFilter, int nGroupId);
		void addElseIf(const CFilter * pFilter, int nGroupId);
		void addElse(int nGroupId);
		void setSplitBy(const NAME_ID &colName);
		
		BOOL split(IRowArray * pRowArr, std::vector<GROUP> &vecResult);

		const CSplitDataPrivate * _private() const;
		
		CSplitData();
		~CSplitData();


	private:
		CSplitDataPrivate * m_pri;
	};
};