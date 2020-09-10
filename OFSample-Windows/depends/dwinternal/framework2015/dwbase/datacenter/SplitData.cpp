#include "stable.h"
#include "SplitData.h"
#include "SplitDataPrivate.h"

namespace Data
{
	CSplitData::CSplitData()
	{
		m_pri = new CSplitDataPrivate();
	}

	CSplitData::~CSplitData()
	{
		delete m_pri;
		m_pri = NULL;
	}

	void CSplitData::addIf(const CFilter * pFilter, int nGroupId)
	{
		m_pri->addIf(pFilter, nGroupId);
	}

	void CSplitData::addElseIf(const CFilter * pFilter, int nGroupId)
	{
		m_pri->addElseIf(pFilter, nGroupId);
	}

	void CSplitData::addElse(int nGroupId)
	{
		m_pri->addElse(nGroupId);
	}

	void CSplitData::setSplitBy(const NAME_ID &colName)
	{
		m_pri->setSplitBy(colName);
	}

	BOOL CSplitData::split(IRowArray * pRowArr, std::vector<GROUP> &vecResult)
	{
		return m_pri->split(pRowArr, vecResult);
	}

	const CSplitDataPrivate * CSplitData::_private() const
	{
		return m_pri;
	}
};
