#include "stable.h"
#include "FilterOption.h"
#include "FilterOptionPrivate.h"

namespace Data
{
	CFilterOption::CFilterOption()
	{
		m_pri = new CFilterOptionPrivate();
	}

	CFilterOption::~CFilterOption()
	{
		delete m_pri;
		m_pri = NULL;
	}

	CFilterOption & CFilterOption::setLimit(DWORD dwLimitCount)
	{
		m_pri->setLimit(dwLimitCount);
		return *this;
	}

	CFilterOption & CFilterOption::orderBy(const NAME_ID &name, BOOL bAscending)
	{
		m_pri->orderBy(name, bAscending);
		return *this;
	}

	const CFilterOptionPrivate * CFilterOption::_private() const
	{
		return m_pri;
	}

	CFilterOption::operator const CFilterOption *() const
	{
		return this;
	}
};
