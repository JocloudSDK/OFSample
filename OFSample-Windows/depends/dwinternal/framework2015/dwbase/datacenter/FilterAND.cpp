#include "stable.h"
#include "FilterAND.h"
#include "ConditionExpr.h"

namespace Data
{
	CFilterAND::CFilterAND()
	{
		m_pri = new CConditionExpr(COMBINE_E_AND);
	}

	CFilterAND::~CFilterAND()
	{
		delete m_pri;
		m_pri = NULL;
	}

	CFilterAND & CFilterAND::equal(const NAME_ID & name, const DWVariant & value)
	{
		m_pri->equal(name, value);
		return *this;
	}

	CFilterAND & CFilterAND::notEqual(const NAME_ID & name, const DWVariant & value)
	{
		m_pri->notEqual(name, value);
		return *this;
	}

	CFilterAND & CFilterAND::less(const NAME_ID & name, const DWVariant & value)
	{
		m_pri->less(name, value);
		return *this;
	}

	CFilterAND & CFilterAND::greater(const NAME_ID & name, const DWVariant & value)
	{
		m_pri->greater(name, value);
		return *this;
	}

	CFilterAND & CFilterAND::null(const NAME_ID & name)
	{
		m_pri->null(name);
		return *this;
	}

	CFilterAND & CFilterAND::notNull(const NAME_ID & name)
	{
		m_pri->notNull(name);
		return *this;
	}

	CFilterAND & CFilterAND::modified(const NAME_ID & name)
	{
		m_pri->modified(name);
		return *this;
	}

	CFilterAND & CFilterAND::AND(const CFilter *pFilter)
	{
		m_pri->combine(pFilter->_private());
		return *this;
	}

	const CConditionExpr * CFilterAND::_private() const
	{
		return m_pri;
	}

	CFilterAND::operator const CFilter *() const
	{
		return this;
	}
};
