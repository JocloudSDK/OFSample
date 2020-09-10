#include "stable.h"
#include "FilterOptionPrivate.h"

namespace Data
{
	CFilterOptionPrivate::CFilterOptionPrivate()
	{
		m_dwLimitCount = 0xFFFFFFFF;
	}

	CFilterOptionPrivate::~CFilterOptionPrivate()
	{
	}

	void CFilterOptionPrivate::setLimit(DWORD dwLimitCount)
	{
		assert(dwLimitCount > 0);
		m_dwLimitCount = dwLimitCount;
	}

	void CFilterOptionPrivate::orderBy(const NAME_ID &name, BOOL bAscending)
	{
		for (UINT i = 0; i < m_vecOrderCase.size(); i++)
		{
			if (m_vecOrderCase[i].name == name)
			{
				assert(! L"select条件设置出现冲突");
				return;
			}
		}

		RAW_ORDER_CASE stOrder;
		stOrder.name = name;
		stOrder.value = bAscending;
		m_vecOrderCase.push_back(stOrder);
	}

	BOOL CFilterOptionPrivate::getAllOrdercase(const CTableDataImpl::REAL_COLS *pCols, std::vector<ORDER_CASE> &vecCase) const
	{
		vecCase.clear();
		vecCase.reserve(m_vecOrderCase.size());

		BOOL bCheckError = FALSE;

		for (UINT i = 0; i < m_vecOrderCase.size() && !bCheckError; i++)
		{			
			DWORD dwIndex = -1;
			DWORD dwOffsetBit;
			E_FIELD eType;

			for (UINT j = 0; j < pCols->dwColCount; j++)
			{
				if (pCols->arCol[j].dwId == m_vecOrderCase[i].name.m_nId)
				{
					dwIndex = j;
					dwOffsetBit = pCols->arCol[j].wOffsetBit;
					eType = (E_FIELD)pCols->arCol[j].wType;
					break;
				}
			}

			if (dwIndex != -1)
			{
				switch (eType)
				{
				case FIELD_E_STR:
				case FIELD_E_DWORD64:
				case FIELD_E_DWORD:
				case FIELD_E_WORD:
				case FIELD_E_BYTE:
				case FIELD_E_BOOL:
					{
						ORDER_CASE stCase;
						stCase.bAscending = m_vecOrderCase[i].value.toBool();
						stCase.dwColIndex = dwIndex;
						stCase.dwOffsetBit = dwOffsetBit;
						stCase.eType = eType;

						vecCase.push_back(stCase);
					}
					break;
				default:
					{
						bCheckError = TRUE;
						LPCWSTR lpszName = m_vecOrderCase[i].name.GetName();
						Error::OutputError(_T("列%s的字段类型不支持排序！！"), lpszName ? lpszName : _T("空"));
					}
					break;
				}
			}
			else
			{
				bCheckError = TRUE;

				LPCWSTR lpszName = m_vecOrderCase[i].name.GetName();
				Error::OutputError(_T("指定了不存在的排序列%s！！"), lpszName ? lpszName : _T("空"));
			}
		}

		return !bCheckError;
	}

};
