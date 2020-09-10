#include "stable.h"
#include "SplitDataPrivate.h"

namespace Data
{
	CSplitDataPrivate::CSplitDataPrivate()
	{
	}

	CSplitDataPrivate::~CSplitDataPrivate()
	{
	}

	void CSplitDataPrivate::addIf(const CFilter * pFilter, int nGroupId)
	{
		if (!m_vecGroupInfo.empty())
		{
			assert(! L"IfThen �ظ���");
			return;
		}
		
		RAW_GROUP_INFO stInfo;
		stInfo.nGroupId = nGroupId;
		stInfo.stExpr = *pFilter->_private();
		m_vecGroupInfo.push_back(stInfo);
	}

	void CSplitDataPrivate::addElseIf(const CFilter * pFilter, int nGroupId)
	{
		if (m_vecGroupInfo.empty())
		{
			assert(! L"ȱ��addIf");
			return;
		}

		RAW_GROUP_INFO stInfo;
		stInfo.nGroupId = nGroupId;
		stInfo.stExpr = *pFilter->_private();
		m_vecGroupInfo.push_back(stInfo);
	}

	void CSplitDataPrivate::addElse(int nGroupId)
	{
		if (m_vecGroupInfo.empty())
		{
			assert(! L"ȱ��addIf");
			return;
		}

		RAW_GROUP_INFO stInfo;
		stInfo.nGroupId = nGroupId;
		m_vecGroupInfo.push_back(stInfo);
	}

	void CSplitDataPrivate::setSplitBy(const NAME_ID &colName)
	{
		m_nameBatchBy = colName;
	}

	BOOL CSplitDataPrivate::split(IRowArray * pRowArr, std::vector<CSplitData::GROUP> &vecResult)
	{
		vecResult.clear();

		//���m_vecGroupInfo�Ƿ���Ч
		if (!m_vecGroupInfo.empty() && (m_vecGroupInfo.size() < 2 || !m_vecGroupInfo[m_vecGroupInfo.size() - 1].stExpr.isEmpty()))
		{
			assert(! L"ָ����Group���ʽ��Ч");
			return FALSE;
		}
		
		if (pRowArr->empty())
			return TRUE;

		CTableDataImpl *pData = ((CRowImpl *)pRowArr->at(0).get())->getTableData();
		
		//����m_nameBatchBy
		E_FIELD eType = FIELD_E_NULL;
		if (!m_nameBatchBy.IsEmpty())
		{
			DWORD	dwIndex;
			DWORD	dwOffsetBit;
			if (!pData->findCol(m_nameBatchBy, &dwIndex, &dwOffsetBit, &eType))
			{
				assert(! L"Batch by �����ֲ�����");
				return FALSE;
			}
		}

		//ת��m_vecGroupInfo
		const CTableDataImpl::REAL_COLS *pCols = pData->getRealCols();

		std::vector<GROUP_INFO> vecGroupInfo;
		vecGroupInfo.reserve(m_vecGroupInfo.size());
		for (UINT i = 0; i <m_vecGroupInfo.size(); i++)
		{
			GROUP_INFO stInfo;
			stInfo.nGroupId = m_vecGroupInfo[i].nGroupId;
			stInfo.pMatchNode = m_vecGroupInfo[i].stExpr.convertToMatchTree(pCols);
			vecGroupInfo.push_back(stInfo);
		}

		//��ʼ���ݷ��鴦��
		DWVariant varBatchBy;

		UINT nCount = pRowArr->size();
		for (UINT i = 0; i < nCount; i++)
		{
			CRowImpl * pRowObj = (CRowImpl *)pRowArr->at(i).get();

			if (!vecGroupInfo.empty())
			{
				BOOL bMatch = FALSE;

				for (UINT j = 0; j < vecGroupInfo.size() - 1; j++)
				{
					if (pRowObj->isMatch(vecGroupInfo[j].pMatchNode))
					{
						bMatch = TRUE;
						pushRow(pRowObj, vecGroupInfo[j].nGroupId, m_nameBatchBy, eType, varBatchBy, vecResult);
						break;
					}
				}

				if (!bMatch)
					pushRow(pRowObj, vecGroupInfo.back().nGroupId, m_nameBatchBy, eType, varBatchBy, vecResult);		
			}
			else
			{
				pushRow(pRowObj, vecGroupInfo.back().nGroupId, m_nameBatchBy, eType, varBatchBy, vecResult);		
			}
		}

		//������ʱ���������ݽṹ
		for (UINT i = 0; i < vecGroupInfo.size(); i++)
			CConditionExpr::destroyMatchTree(vecGroupInfo[i].pMatchNode);
		vecGroupInfo.clear();

		return TRUE;
	}

	void CSplitDataPrivate::pushRow(CRowImpl *pRowObj, int nGroupId, const NAME_ID & nameBatchBy, E_FIELD eType, DWVariant &varBatchBy, std::vector<CSplitData::GROUP> &vecResult)
	{
		BOOL bNewBatch = FALSE;

		if (!nameBatchBy.IsEmpty())
		{
			switch (eType)
			{
			case FIELD_E_DWORD:
				{
					DWORD dwVal = pRowObj->getDword(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = varBatchBy.toUInt() != dwVal;
					varBatchBy.setValue(dwVal);
				}
				break;
			case FIELD_E_BOOL:
				{
					bool bVal = !!pRowObj->getBool(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = varBatchBy.toBool() != bVal;
					varBatchBy.setValue(bVal);
				}
				break;
			case FIELD_E_DWORD64:
				{
					DWORD64 dw64Val = pRowObj->getDword64(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = varBatchBy.toUInt64() != dw64Val;
					varBatchBy.setValue(dw64Val);
				}
				break;
			case FIELD_E_WORD:
				{
					WORD wVal = pRowObj->getWord(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = varBatchBy.toUInt() != wVal;
					varBatchBy.setValue(wVal);
				}
				break;
			case FIELD_E_BYTE:
				{
					BYTE byVal = pRowObj->getByte(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = varBatchBy.toUInt() != byVal;
					varBatchBy.setValue(byVal);
				}
				break;
			case FIELD_E_STR:
				{
					LPCWSTR lpszVal = pRowObj->getStr(nameBatchBy);
					if (!varBatchBy.isNull())
						bNewBatch = wcscmp(varBatchBy.toStr(), lpszVal) != 0;
					varBatchBy.setValue(lpszVal);
				}
				break;
			default:
				assert(0);
				break;
			}
		}

		if (bNewBatch || vecResult.empty() || vecResult.back().nGroupId != nGroupId)
		{
			CSplitData::GROUP stGroup;
			stGroup.nGroupId = nGroupId;
			vecResult.push_back(stGroup);
		}
		vecResult.back().pRowArr->push_back(pRowObj);
	}
};
