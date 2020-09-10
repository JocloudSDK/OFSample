#pragma once

#include "Filter.h"

namespace Data 
{
	class CConditionExpr;
	class DWBASE_EXPORTS CFilterOR : public CFilter
	{
	public:
		CFilterOR();
		~CFilterOR();

		CFilterOR & push(const CFilter *pFilter);		

		operator const CFilter *() const;

		virtual const CConditionExpr * _private() const;
	
	private:
		CConditionExpr * m_pri;
	};
};