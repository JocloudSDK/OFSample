#include "stable.h"
#include "FilterOR.h"
#include "ConditionExpr.h"

namespace Data
{
	CFilterOR::CFilterOR()
	{
		m_pri = new CConditionExpr(COMBINE_E_OR);
	}

	CFilterOR::~CFilterOR()
	{
		delete m_pri;
		m_pri = NULL;
	}

	CFilterOR & CFilterOR::push(const CFilter *pFilter)
	{
		m_pri->combine(pFilter->_private());
		return *this;
	}

	CFilterOR::operator const CFilter *() const
	{
		return this;
	}

	const CConditionExpr * CFilterOR::_private() const
	{
		return m_pri;
	}
};
