#pragma once

#include "StringTable.h"
#include "../variant/Variant.h"
#include "DataCenterPub.h"
#include "TableDataImpl.h"
#include "ConditionExpr.h"
#include "SplitData.h"
#include "RowImpl.h"

namespace Data 
{
	class CSplitDataPrivate
	{
	public:
		struct RAW_GROUP_INFO
		{
			int			   nGroupId;
			CConditionExpr stExpr;
		};

		CSplitDataPrivate();
		~CSplitDataPrivate();	
		
		void addIf(const CFilter * pFilter, int nGroupId);
		void addElseIf(const CFilter * pFilter, int nGroupId);
		void addElse(int nGroupId);

		void setSplitBy(const NAME_ID &colName);
		
		BOOL split(IRowArray * pRowArr, std::vector<CSplitData::GROUP> &vecResult);
	
	private:
		static void pushRow(CRowImpl *pRowObj, int nGroupId, const NAME_ID & nameBatchBy, E_FIELD eType, DWVariant &varBatchBy, std::vector<CSplitData::GROUP> &vecResult);

		struct GROUP_INFO
		{
			int			   nGroupId;
			MATCH_NODE *   pMatchNode;
		};

		struct PUSH_ROW_CONTEXT
		{
			DWORD dwColIndex;

		};

		std::vector<RAW_GROUP_INFO> m_vecGroupInfo;
		NAME_ID						m_nameBatchBy;
	};
};