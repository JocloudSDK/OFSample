#include "stable.h"
#include "ConditionExpr.h"
#include "RowImpl.h"

namespace Data
{
	CConditionExpr::CConditionExpr()
	{
		m_header.eMode = COMBINE_E_NULL;
	}
	
	CConditionExpr::CConditionExpr(E_COMBINE_MODE eMode)
	{
		m_header.eMode = eMode;
	}

	CConditionExpr::~CConditionExpr()
	{
		clearCachedMatchTree();
	}

	void CConditionExpr::equal(const NAME_ID & name, const DWVariant & val)
	{
		addCase(name, val, CASE_E_EQUAL);
	}

	void CConditionExpr::notEqual(const NAME_ID & name, const DWVariant & val)
	{
		addCase(name, val, CASE_E_NOT_EQUAL);
	}

	void CConditionExpr::less(const NAME_ID & name, const DWVariant & val)
	{
		addCase(name, val, CASE_E_LESS);
	}

	void CConditionExpr::greater(const NAME_ID & name, const DWVariant & val)
	{
		addCase(name, val, CASE_E_GREATER);
	}

	void CConditionExpr::null(const NAME_ID & name)
	{
		addCase(name, DWVariant(), CASE_E_IS_NULL);
	}

	void CConditionExpr::notNull(const NAME_ID & name)
	{
		addCase(name, DWVariant(), CASE_E_NOT_NULL);
	}

	void CConditionExpr::modified(const NAME_ID & name)
	{
		addCase(name, DWVariant(), CASE_E_MODIFIED);
	}

	void CConditionExpr::addCase(const NAME_ID & name, const DWVariant &val, E_MATCH_CASE eCase)
	{
		RAW_MATCH_NODE &node = m_header.vecChild.empty() ? m_header : m_header.vecChild[0];
		
		//先看看是否存在
		for (UINT i = 0; i < node.vecCase.size(); i++)
		{
			if (node.vecCase[i].name == name && node.vecCase[i].eCase == eCase)
			{
				assert(! L"过滤条件设置出现冲突");
				return;
			}
		}

		RAW_MATCH_CASE stCase;
		stCase.eCase = eCase;
		stCase.name = name;
		stCase.val = val;

		node.vecCase.push_back(stCase);

		clearCachedMatchTree();
	}

	void CConditionExpr::clearCachedMatchTree()
	{
		for (std::map<void*, MATCH_NODE*>::iterator it = m_pCachedMatchTree.begin();
			it != m_pCachedMatchTree.end(); ++it)
		{
			destroyMatchTree(it->second);
		}
		m_pCachedMatchTree.clear();
	}

	void CConditionExpr::combine(const CConditionExpr *pExpr)
	{
		if (pExpr == this)
		{
			assert(! L"combine参数指定自己啦");
			return;
		}

		if (m_header.eMode == COMBINE_E_OR)
		{
			assert(m_header.vecCase.empty());
			m_header.vecChild.push_back(pExpr->m_header);
		}
		else if (m_header.eMode == COMBINE_E_AND)
		{
			if (m_header.vecChild.empty())//当前是叶子节点，需要分裂为非叶子节点
			{
				RAW_MATCH_NODE node = m_header;
				m_header.vecCase.clear();

				m_header.vecChild.push_back(node);
			}
			
			m_header.vecChild.push_back(pExpr->m_header);
		}
		else
		{
			assert(! L"Filter模式不能为空");
		}

		clearCachedMatchTree();
	}

	DWORD CConditionExpr::calcMatchTreeAllocSize(const RAW_MATCH_NODE *pNode)
	{
		DWORD dwSize = sizeof(MATCH_NODE);
	
		if (pNode->vecChild.empty() && pNode->vecCase.empty())
			assert(! L"警告：指定了空的条件表达式，可能乱费性能哦");

		if (!pNode->vecChild.empty() && !pNode->vecCase.empty())
			assert(! L"CConditionExpr内部出现逻辑错误"); 

		if (pNode->vecChild.empty())//属于叶子节点
		{
			for (UINT i = 0; i < pNode->vecCase.size(); i++)
			{
				dwSize += sizeof(MATCH_CASE);

				switch (pNode->vecCase[i].val.getType())
				{
				case DWVariant::TYPE_E_INT:
				case DWVariant::TYPE_E_UINT:
				case DWVariant::TYPE_E_BOOL:
				case DWVariant::TYPE_E_NULL:
					break;
				case DWVariant::TYPE_E_STR:
					dwSize += (wcslen(pNode->vecCase[i].val.toStr()) + 1) * sizeof(wchar_t);
					break;
				case DWVariant::TYPE_E_INT64:
				case DWVariant::TYPE_E_UINT64:
					dwSize += sizeof(DWORD64);
					break;
				default:
					assert(! L"calcMatchTreeAllocSize时出现不支持字段类型");
					break;
				}
			}
		}
		else
		{
			for (UINT i = 0; i < pNode->vecChild.size(); i++)
				dwSize += calcMatchTreeAllocSize(&pNode->vecChild[i]);
		}

		return dwSize;
	}

	MATCH_NODE* CConditionExpr::getMatchTree(const CTableDataImpl::REAL_COLS *pCols) const
	{
		MATCH_NODE* pMatchTree = NULL;
		std::map<void*, MATCH_NODE*>::const_iterator itFind = m_pCachedMatchTree.find((void*)pCols);
		if (itFind == m_pCachedMatchTree.end()) {
			pMatchTree = convertToMatchTree(pCols);
			m_pCachedMatchTree.insert(std::make_pair((void*)pCols, pMatchTree));
		}
		else {
			pMatchTree = itFind->second;
		}
		return pMatchTree;
	}

	MATCH_NODE * CConditionExpr::convertToMatchTree(const CTableDataImpl::REAL_COLS *pCols) const
	{
		DWORD dwAllocSize = calcMatchTreeAllocSize(&m_header);
		BYTE *pBuf = new BYTE[dwAllocSize];
		memset(pBuf, 0, dwAllocSize);

		MATCH_NODE *pNode = (MATCH_NODE *)pBuf;
		DWORD dwSizeUsed = sizeof(MATCH_NODE);

		BOOL bResult = convertToMatchTreeHelper(pCols, &m_header, pNode, pBuf, &dwSizeUsed);
		if (bResult)
		{
			assert(dwSizeUsed == dwAllocSize);
		}
		else
		{
			delete [] pBuf;
			pBuf = NULL;
		}
		
		return (MATCH_NODE *)pBuf;
	}

	BOOL CConditionExpr::convertToMatchTreeHelper(const CTableDataImpl::REAL_COLS *pCols, 
												  const RAW_MATCH_NODE *pNodeSrc, 
												  MATCH_NODE *pNodeDst, 
												  BYTE * pBuf,
												  /*[in][out]*/DWORD *pSizeUsed)
	{	
		//是否为空的条件表达式
		if (pNodeSrc->vecChild.empty() && pNodeSrc->vecCase.empty())
		{
			pNodeDst->eMode = pNodeSrc->eMode;
			pNodeDst->arChild = NULL;
			pNodeDst->dwChildCount = 0;
			pNodeDst->arMatchCase = NULL;
			pNodeDst->dwMatchCaseCount = 0;

			return TRUE;
		}

		if (pNodeSrc->vecChild.empty())//属于叶子节点
		{
			BOOL bCheckError = FALSE;
			
			const DWORD dwCount = pNodeSrc->vecCase.size();
			MATCH_CASE * pCases = (MATCH_CASE *)((PBYTE)pBuf + *pSizeUsed);
			*pSizeUsed += sizeof(MATCH_CASE) * dwCount;

			for (UINT i = 0; i < dwCount && !bCheckError; i++)
			{				
				DWORD dwIndex = -1;
				DWORD dwOffsetBit;
				E_FIELD eType;

				for (UINT j = 0; j < pCols->dwColCount; j++)
				{
					if (pCols->arCol[j].dwId == pNodeSrc->vecCase[i].name.m_nId)
					{
						dwIndex = j;
						dwOffsetBit = (E_FIELD)pCols->arCol[j].wOffsetBit;
						eType = (E_FIELD)pCols->arCol[j].wType;
						break;
					}
				}

				if (dwIndex != -1)
				{
					DWORD dwDum = 0;

					switch (pNodeSrc->vecCase[i].val.getType())
					{
					case DWVariant::TYPE_E_INT:
					case DWVariant::TYPE_E_UINT:
						{
							dwDum = pNodeSrc->vecCase[i].val.toUInt();
							
							if (eType == FIELD_E_BOOL) //外面可能传入TRUE/FALSE，错误的认为是TYPE_E_INT/TYPE_E_UINT
							{
								if (dwDum != FALSE && dwDum != TRUE)
									bCheckError = TRUE;
							}
							else if (eType != FIELD_E_DWORD && eType != FIELD_E_WORD && eType != FIELD_E_BYTE)
							{
								bCheckError = TRUE;
							}
						}
						break;
					case DWVariant::TYPE_E_BOOL:
						{
							dwDum = pNodeSrc->vecCase[i].val.toBool();

							if (eType != FIELD_E_BOOL)
								bCheckError = TRUE;
						}
						break;
					case DWVariant::TYPE_E_STR:
						{
							LPCWSTR pStrSrc = pNodeSrc->vecCase[i].val.toStr();
							DWORD dwSizeBytes = (wcslen(pStrSrc) + 1) * sizeof(wchar_t);
							wchar_t *pStrDst = (wchar_t *)((PBYTE)pBuf + *pSizeUsed);
							memcpy(pStrDst, pStrSrc, dwSizeBytes);
							*pSizeUsed += dwSizeBytes;

							dwDum = (DWORD)pStrDst;

							if (eType != FIELD_E_STR)
								bCheckError = TRUE;
						}
						break;
					case DWVariant::TYPE_E_INT64:
					case DWVariant::TYPE_E_UINT64:
						{
							DWORD64 *p = (DWORD64 *)((PBYTE)pBuf + *pSizeUsed);
							*p = pNodeSrc->vecCase[i].val.toUInt64();
							*pSizeUsed += sizeof(DWORD64);

							dwDum = (DWORD)p;
							if (eType != FIELD_E_DWORD64)
								bCheckError = TRUE;
						}
						break;
					case DWVariant::TYPE_E_NULL:
						{
							if (pNodeSrc->vecCase[i].eCase != CASE_E_IS_NULL && pNodeSrc->vecCase[i].eCase != CASE_E_NOT_NULL && pNodeSrc->vecCase[i].eCase != CASE_E_MODIFIED)
								bCheckError = TRUE;
						}
						break;
					default:
						bCheckError = TRUE;
						assert(! L"select时出现不支持字段类型");
						break;
					}

					if (bCheckError)
					{
						LPCWSTR lpszName = pNodeSrc->vecCase[i].name.GetName();
						Error::OutputError(_T("筛选条件指定了%s字段类型错误！！"), lpszName ? lpszName : _T("空"));
					}

					pCases[i].dwColIndex = dwIndex;
					pCases[i].dwOffsetBit = dwOffsetBit;
					pCases[i].dwDum = dwDum;
					pCases[i].eMatchCase = pNodeSrc->vecCase[i].eCase;
					pCases[i].pfnMatch = NULL;

					//设置回调函数指针
					switch (pNodeSrc->vecCase[i].eCase)
					{
					case CASE_E_EQUAL:
						{
							switch(eType)
							{
							case FIELD_E_DWORD://最大可能的排前面，提高性能
								pCases[i].pfnMatch = &CRowImpl::matchEqualSimple<DWORD>;
								break;
							case FIELD_E_BOOL:
								pCases[i].pfnMatch = &CRowImpl::matchEqualBool;
								break;
							case FIELD_E_WORD:
								pCases[i].pfnMatch = &CRowImpl::matchEqualSimple<WORD>;
								break;
							case FIELD_E_BYTE:
								pCases[i].pfnMatch = &CRowImpl::matchEqualSimple<BYTE>;
								break;
							case FIELD_E_STR:
								pCases[i].pfnMatch = &CRowImpl::matchEqualStr;
								break;
							case FIELD_E_DWORD64:
								pCases[i].pfnMatch = &CRowImpl::matchEqualDword64;
								break;
							default:
								bCheckError = TRUE;
								assert(! L"筛选条件指定了%s字段类型没有筛选函数！！");
								break;
							}
						}
						break;
					case CASE_E_NOT_EQUAL:
						{
							switch(eType)
							{
							case FIELD_E_DWORD:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualSimple<DWORD>;
								break;		
							case FIELD_E_BOOL:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualBool;
								break;		
							case FIELD_E_WORD:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualSimple<WORD>;
								break;		
							case FIELD_E_BYTE:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualSimple<BYTE>;
								break;
							case FIELD_E_STR:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualStr;
								break;
							case FIELD_E_DWORD64:
								pCases[i].pfnMatch = &CRowImpl::matchNotEqualDword64;
								break;
							default:
								bCheckError = TRUE;
								assert(! L"筛选条件指定了%s字段类型没有筛选函数！！");
								break;
							}
						}
						break;
					case CASE_E_GREATER:
						{
							switch(eType)
							{
							case FIELD_E_DWORD:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterSimple<DWORD>;
								break;
							case FIELD_E_BOOL:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterBool;
								break;
							case FIELD_E_WORD:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterSimple<WORD>;
								break;
							case FIELD_E_BYTE:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterSimple<BYTE>;
								break;
							case FIELD_E_STR:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterStr;
								break;
							case FIELD_E_DWORD64:
								pCases[i].pfnMatch = &CRowImpl::matchGreaterDword64;
								break;
							default:
								bCheckError = TRUE;
								assert(! L"筛选条件指定了%s字段类型没有筛选函数！！");
								break;
							}
						}
						break;
					case CASE_E_LESS:
						{
							switch(eType)
							{
							case FIELD_E_DWORD:
								pCases[i].pfnMatch = &CRowImpl::matchLessSimple<DWORD>;
								break;
							case FIELD_E_BOOL:
								pCases[i].pfnMatch = &CRowImpl::matchLessBool;
								break;
							case FIELD_E_WORD:
								pCases[i].pfnMatch = &CRowImpl::matchLessSimple<WORD>;
								break;
							case FIELD_E_BYTE:
								pCases[i].pfnMatch = &CRowImpl::matchLessSimple<BYTE>;
								break;
							case FIELD_E_STR:
								pCases[i].pfnMatch = &CRowImpl::matchLessStr;
								break;
							case FIELD_E_DWORD64:
								pCases[i].pfnMatch = &CRowImpl::matchLessDword64;
								break;
							default:
								bCheckError = TRUE;
								assert(! L"筛选条件指定了%s字段类型没有筛选函数！！");
								break;
							}
						}
						break;
					case CASE_E_IS_NULL:
					case CASE_E_NOT_NULL:
					case CASE_E_MODIFIED:
						break;
					default:
						bCheckError = TRUE;
						assert(! L"不支持的筛选模式");
						break;
					}
				}
				else
				{
					LPCWSTR lpszName = pNodeSrc->vecCase[i].name.GetName();
					Error::OutputError(_T("筛选条件指定了不存在的列%s！！"), lpszName ? lpszName : _T("空"));

					bCheckError = TRUE;
				}
			}

			pNodeDst->eMode = pNodeSrc->eMode;
			pNodeDst->arChild = NULL;
			pNodeDst->dwChildCount = 0;
			pNodeDst->arMatchCase = pCases;
			pNodeDst->dwMatchCaseCount = dwCount;
			
			if (bCheckError)
				return FALSE;
		}
		else //非叶子节点
		{
			pNodeDst->eMode = pNodeSrc->eMode;
			pNodeDst->arChild = (MATCH_NODE *)((PBYTE)pBuf + *pSizeUsed);
			*pSizeUsed += sizeof(MATCH_NODE) * pNodeSrc->vecChild.size();
			pNodeDst->dwChildCount = pNodeSrc->vecChild.size();			
			pNodeDst->arMatchCase = NULL;
			pNodeDst->dwMatchCaseCount = 0;

			for (UINT i = 0; i < pNodeSrc->vecChild.size(); i++)
			{
				if (!convertToMatchTreeHelper(pCols, &pNodeSrc->vecChild[i], &pNodeDst->arChild[i], pBuf, pSizeUsed))
					return FALSE;
			}
		}

		return TRUE;
	}

	void CConditionExpr::destroyMatchTree(MATCH_NODE *pHeader)
	{
		delete [] (BYTE *)pHeader;
	}

	BOOL CConditionExpr::isEqualHelper(const RAW_MATCH_NODE *pLeft, const RAW_MATCH_NODE *pRight)
	{
		if (pLeft->eMode != pRight->eMode)
			return FALSE;

		if (pLeft->vecChild.empty())//属于叶子节点
		{
			assert(!pLeft->vecCase.empty());
			
			if (pLeft->vecCase.size() != pRight->vecCase.size())
				return FALSE;

			for (UINT i = 0; i < pLeft->vecCase.size(); i++)
			{
				BOOL bExist = FALSE;
				for (UINT j = 0; j < pRight->vecCase.size(); j++)
				{
					if (pLeft->vecCase[i].eCase == pRight->vecCase[j].eCase
						&& pLeft->vecCase[i].name == pRight->vecCase[j].name
						&& pLeft->vecCase[i].val == pRight->vecCase[j].val)
					{
						bExist = TRUE;
						break;
					}
				}

				if (!bExist)
					return FALSE;
			}

		}
		else
		{
			assert(pLeft->vecCase.empty());
			
			if (pLeft->vecChild.size() != pRight->vecChild.size())
				return FALSE;

			for (UINT i = 0; i < pLeft->vecChild.size(); i++)
			{
				if (!isEqualHelper(&pLeft->vecChild[i], &pRight->vecChild[i]))
					return FALSE;
			}
		}
		
		return TRUE;
	}

	BOOL CConditionExpr::isEqual(const CConditionExpr * pExpr) const
	{
		if (pExpr == this)
			return TRUE;

		return isEqualHelper(&m_header, &pExpr->m_header);
	}
};
