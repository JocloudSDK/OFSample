#include "stable.h"
#include "PropData.h"
#include "DataCenterPub.h"
#include "../mempool/MemPool.h"

namespace Data
{
	CPropData * CPropData::createInstance()
	{
		return new (mp_alloc(sizeof(CPropData))) CPropData();
	}

	CPropData::CPropData()
	{
		m_dwRef = 1;
		m_pData = NULL;
	}

	CPropData::~CPropData()
	{
		if (m_pData)
		{
			assert(m_pData->dwCount > 0);

			for (UINT i = 0; i < m_pData->dwCount; i++)
			{
				PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
				switch (pProps[i].type)
				{
					case FIELD_E_STR:
					case FIELD_E_DWORD64:
						mp_free((void *)pProps[i].value);
						break;
					case FIELD_E_UNK:
						{
							IUnk *p = (IUnk *)pProps[i].value;
							if (p)
								p->release();
						}
						break;
					default:
						break;
				}
			}
			
			mp_free(m_pData);
			m_pData = NULL;
		}
	}

	BOOL CPropData::getPropXXX(const NAME_ID &name, E_FIELD type, DWORD *lpdwDum) const 
	{
		if (m_pData)
		{
			assert(m_pData->dwCount > 0);
			
			const PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
			for (UINT i = 0; i < m_pData->dwCount; i++)
			{
				if (pProps[i].id == name.m_nId)
				{
					if (pProps[i].type != type)
					{
						LPCWSTR lpszName = name.GetName();
						Error::OutputError(_T("访问数据中心时属性 %s 类型错误！！"), lpszName ? lpszName : _T("空"));

						return FALSE;
					}

					*lpdwDum = pProps[i].value;
					return TRUE;
				}
			}
		}
		
		return FALSE;
	}

	LPCWSTR CPropData::getPropStr(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_STR, &value))
			return (LPCWSTR)value;
		return L"";
	}

	DWORD64 CPropData::getPropDword64(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_DWORD64, &value))
			return *(DWORD64 *)value;
		return 0;
	}

	DWORD CPropData::getPropDword(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_DWORD, &value))
			return value;
		return 0;
	}

	WORD CPropData::getPropWord(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_WORD, &value))
			return (WORD)value;
		return 0;
	}

	BYTE CPropData::getPropByte(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_BYTE, &value))
			return (BYTE)value;
		return 0;
	}

	BOOL CPropData::getPropBool(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_BOOL, &value))
			return (BOOL)!!value;
		return FALSE;
	}

	comptr<IUnk> CPropData::getPropUnk(const NAME_ID &name) const
	{
		DWORD value;
		if (getPropXXX(name, FIELD_E_UNK, &value))
			return (IUnk *)value;
		return NULL;
	}

	BOOL CPropData::hasProp(const NAME_ID &name) const
	{
		if (m_pData)
		{
			const PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
			for (UINT i = 0; i < m_pData->dwCount; i++)
			{
				if (pProps[i].id == name.m_nId)
					return TRUE;
			}
		}
		return FALSE;
	}

	BOOL CPropData::setPropXXX(const NAME_ID &name, E_FIELD type, DWORD dwDum)
	{
		UINT iIndexProp = -1;
		
		//如果存在则清掉
		if (m_pData)
		{
			PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
			for (UINT i = 0; i < m_pData->dwCount; i++)
			{
				if (pProps[i].id == name.m_nId)
				{
					if (pProps[i].type != type)
					{
						LPCWSTR lpszName = name.GetName();
						Error::OutputError(_T("写入数据中心时属性 %s 类型错误！！"), lpszName ? lpszName : _T("空"));

						return FALSE;
					}

					iIndexProp = i;
					switch (pProps[i].type)
					{
						case FIELD_E_STR:
						case FIELD_E_DWORD64:
							mp_free((void *)pProps[i].value);
							break;
						case FIELD_E_UNK:
							{
								IUnk *p = (IUnk *)pProps[i].value;
								if (p)
									p->release();
							}
							break;
						default:
							break;
					}

					pProps[i].value = 0;
					pProps[i].type = 0;
					pProps[i].id = 0;

					break;
				}
			}
		}
		
		if (iIndexProp == -1)
		{
			if (m_pData)
			{
				PROP_DATA *pNewData = (PROP_DATA *)mp_alloc(sizeof(PROP_DATA) + sizeof(PROP_ITEM) * (m_pData->dwCount + 1));
				pNewData->dwCount = m_pData->dwCount + 1;
				memcpy(pNewData + 1, m_pData + 1, sizeof(PROP_ITEM) * m_pData->dwCount);

				mp_free(m_pData);
				m_pData = pNewData;
				
				iIndexProp = m_pData->dwCount - 1;
			}
			else
			{
				m_pData = (PROP_DATA *)mp_alloc(sizeof(PROP_DATA) + sizeof(PROP_ITEM));
				m_pData->dwCount = 1;
				
				iIndexProp = 0;
			}
		}

		//保存新的属性
		PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
		pProps[iIndexProp].id = name.m_nId;
		pProps[iIndexProp].type = type;
		switch (type)
		{
			case FIELD_E_STR:
				{
					if (dwDum != 0)
					{
						DWORD cbSize = (wcslen((LPCWSTR)dwDum) + 1) * sizeof(wchar_t);
						wchar_t * p = (wchar_t *)mp_alloc(cbSize);
						memcpy(p, (LPCWSTR)dwDum, cbSize);

						pProps[iIndexProp].value = (DWORD)p;
					}
					else
					{
						pProps[iIndexProp].value = 0;
					}
				}
				break;
			case FIELD_E_DWORD64:
				{
					DWORD64 *p = (DWORD64 *)mp_alloc(sizeof(DWORD64));
					*p = *(DWORD64 *)dwDum;
					pProps[iIndexProp].value = (DWORD)p;
				}
				break;
			case FIELD_E_UNK:
				{
					((IUnk *)dwDum)->addRef();
					pProps[iIndexProp].value = dwDum;
				}
				break;
			default:
				pProps[iIndexProp].value = (DWORD)dwDum;
				break;
		}

		return TRUE;
	}

	BOOL CPropData::setPropStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		return setPropXXX(name, FIELD_E_STR, (DWORD)lpwszValue);
	}

	BOOL CPropData::setPropDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		return setPropXXX(name, FIELD_E_DWORD64, (DWORD)&dw64Value);
	}

	BOOL CPropData::setPropDword(const NAME_ID &name, DWORD dwValue)
	{
		return setPropXXX(name, FIELD_E_DWORD, (DWORD)dwValue);
	}

	BOOL CPropData::setPropWord(const NAME_ID &name, WORD wValue)
	{
		return setPropXXX(name, FIELD_E_WORD, (DWORD)wValue);
	}

	BOOL CPropData::setPropByte(const NAME_ID &name, BYTE byValue)
	{
		return setPropXXX(name, FIELD_E_BYTE, (DWORD)byValue);
	}

	BOOL CPropData::setPropBool(const NAME_ID &name, BOOL bValue)
	{
		return setPropXXX(name, FIELD_E_BOOL, (DWORD)bValue);
	}

	BOOL CPropData::setPropUnk(const NAME_ID &name, IUnk *pUnk)
	{
		return setPropXXX(name, FIELD_E_UNK, (DWORD)pUnk);
	}

	BOOL CPropData::removeProp(const NAME_ID &name)
	{
		UINT iIndexProp = -1;

		if (m_pData)
		{
			PROP_ITEM *pProps = (PROP_ITEM *)(m_pData + 1);
			for (UINT i = 0; i < m_pData->dwCount; i++)
			{
				if (pProps[i].id == name.m_nId)
				{
					iIndexProp = i;
					switch (pProps[i].type)
					{
						case FIELD_E_STR:
						case FIELD_E_DWORD64:
							mp_free((void *)pProps[i].value);
							break;
						case FIELD_E_UNK:
							{
								IUnk *p = (IUnk *)pProps[i].value;
								if (p)
									p->release();
							}
							break;
						default:
							break;
					}
					break;
				}
			}
		}
		
		if (iIndexProp != -1)
		{
			if (m_pData->dwCount == 1)
			{
				mp_free(m_pData);
				m_pData = NULL;
			}
			else
			{
				PROP_DATA *pNewData = (PROP_DATA *)mp_alloc(sizeof(PROP_DATA) + sizeof(PROP_ITEM) * (m_pData->dwCount - 1));
				pNewData->dwCount = m_pData->dwCount - 1;
				
				UINT iSrc = 0; 
				UINT iDst = 0; 
				while (iSrc < m_pData->dwCount)
				{
					if (iSrc == iIndexProp)
					{
						iSrc++;
					}
					else
					{
						((PROP_ITEM*)(m_pData + 1))[iSrc] = ((PROP_ITEM*)(pNewData + 1))[iDst];
						iSrc++;
						iDst++;
					}
				}
				
				mp_free(m_pData);
				m_pData = pNewData;
			}

			return TRUE;
		}

		return FALSE;
	}
};
