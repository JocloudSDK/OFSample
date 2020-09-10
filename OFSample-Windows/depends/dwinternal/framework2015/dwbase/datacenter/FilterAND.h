#pragma once

#include "Filter.h"
#include "StringTable.h"
#include "dwbase/variant.h"

//class DWVariant;

namespace Data 
{
	class CConditionExpr;
	class DWBASE_EXPORTS CFilterAND : public CFilter
	{
	public:
		CFilterAND();
		~CFilterAND();

		//����ͬʱָ������ֶΣ�����������������
		CFilterAND & equal(const NAME_ID & name, const DWVariant & value);
		CFilterAND & notEqual(const NAME_ID & name, const DWVariant & value);
		
		CFilterAND & less(const NAME_ID & name, const DWVariant & value);
		CFilterAND & greater(const NAME_ID & name, const DWVariant & value);
		
		CFilterAND & null(const NAME_ID & name);
		CFilterAND & notNull(const NAME_ID & name);
		
		CFilterAND & AND(const CFilter *pFilter);	
		CFilterAND & modified(const NAME_ID & name);//������CSplitData

		operator const CFilter *() const;
		virtual const CConditionExpr * _private() const;

	private:
		CConditionExpr * m_pri;
	};
};
