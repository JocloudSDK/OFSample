#include "stable.h"
#include "FieldSetDataImpl.h"
#include "EventDispatch.h"
#include "FieldBlob.h"
#include "DatabaseLeakDetector.h"

namespace Data
{
	CFieldSetDataImpl::CFieldSetDataImpl(CFieldSetImpl *pSetEdit)
	{
		m_dwRef = 1;
		m_pSetEdit = pSetEdit;

		m_pValue = NULL;
	}

	CFieldSetDataImpl::~CFieldSetDataImpl(void)
	{
		assert(m_dwRef == 0);

		const CFieldSetImpl::COLS *pCols = m_pSetEdit->getCols();
		for (UINT i = 0; i < pCols->dwCount; i++)
		{
			switch(pCols->arType[i])
			{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				break;
			case FIELD_E_STR:
				{
					CFieldStringW * p = (CFieldStringW *)m_pValue[i];
					if (p)
						p->release();
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob * p = (CFieldBlob *)m_pValue[i];
					if (p)
						p->release();
				}
				break;
			case FIELD_E_DWORD64:
				{
					DWORD64 *p = (DWORD64 *)m_pValue[i];
					if (p)
						mp_free(p);
				}
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit *p = (ITableEdit *)m_pValue[i];
					if (p)
						p->release();
				}
				break;
			default:
				assert(! L"不支持的数据类型");
				break;				
			}
		}
		
		delete [] m_pValue;
		m_pValue = NULL;

		m_pSetEdit = NULL;
	}

	CFieldSetDataImpl * CFieldSetDataImpl::createInstance(CFieldSetImpl *pSetEdit)
	{
		return new CFieldSetDataImpl(pSetEdit);
	}

	void CFieldSetDataImpl::init()
	{
		DWORD dwCount = m_pSetEdit->getCols()->dwCount;
		assert(dwCount > 0);
		m_pValue = new DWORD[dwCount];
		memset(m_pValue, 0, sizeof(DWORD) * dwCount);

		m_bitExist.init(dwCount);
		m_bitModified.init(dwCount);
	}

	void CFieldSetDataImpl::addRef()
	{
		m_pSetEdit->addRef();

		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CFieldSetDataImpl::release()
	{
		CFieldSetImpl *pTemp = m_pSetEdit;

		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;

		pTemp->release();
	}

	IWeakRef* CFieldSetDataImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CFieldSetDataImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	DWORD CFieldSetDataImpl::internalrelease()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);

		DWORD dwRet = ::InterlockedDecrement((LONG *)&m_dwRef);
		if (dwRet == 0)
			delete this;

		return dwRet;
	}

	BOOL CFieldSetDataImpl::getXXX(const NAME_ID &name, E_FIELD type, DWORD *lpdwDum)
	{
		DWORD dwIndex;
		E_FIELD eTypeRes;

		if (!m_pSetEdit->findCol(name, &dwIndex, &eTypeRes))
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		if (eTypeRes != type)
		{

			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时字段 %s 类型错误！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		*lpdwDum = m_pValue[dwIndex];

		return TRUE;
	}

	LPCWSTR	CFieldSetDataImpl::getStr(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_STR, &dwValue) && dwValue != 0)
			return ((CFieldStringW *)dwValue)->getStr();
		return _T("");
	}

	const BYTE * CFieldSetDataImpl::getBlob(const NAME_ID &name, DWORD * lpdwSize)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_BLOB, &dwValue) && dwValue != 0)
		{
			CFieldBlob * pBlob = (CFieldBlob *)dwValue;
			if (lpdwSize)
				*lpdwSize = pBlob->getSize();
			return pBlob->getPtr();
		}

		if (lpdwSize)
			*lpdwSize = 0;
		return NULL;
	}

	DWORD64	CFieldSetDataImpl::getDword64(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_DWORD64, &dwValue) && dwValue != 0)
			return *(DWORD64 *)dwValue;
		return 0;
	}

	DWORD CFieldSetDataImpl::getDword(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_DWORD, &dwValue))
			return dwValue;
		return 0;
	}

	WORD CFieldSetDataImpl::getWord(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_WORD, &dwValue))
			return (WORD)dwValue;
		return 0;
	}

	BYTE CFieldSetDataImpl::getByte(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_BYTE, &dwValue))
			return (BYTE)dwValue;
		return 0;
	}

	BOOL CFieldSetDataImpl::getBool(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());
	
		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_BOOL, &dwValue))
			return (BOOL)dwValue;
		return FALSE;
	}

	comptr<ITable> CFieldSetDataImpl::getSubTable(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());

		DWORD dwValue;
		if (m_pSetEdit->isStructInitialized() && getXXX(name, FIELD_E_TABLE, &dwValue))
		{
			ITableEdit *pEdit = (ITableEdit *)dwValue;
			if (pEdit)
				return pEdit->getTable();
		}
		return NULL;
	}

	BOOL CFieldSetDataImpl::hasData(const NAME_ID &name)
	{
		if (!m_pSetEdit->isStructInitialized())
			return FALSE;

		BOOL bRet = FALSE;

		DWORD dwIndex;
		E_FIELD eTypeRes;
		if (m_pSetEdit->findCol(name, &dwIndex, &eTypeRes))
		{
			bRet = m_bitExist.getBit(dwIndex);
		}
		else
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
		}

		return bRet;
	}

	BOOL CFieldSetDataImpl::isModified(const NAME_ID &name)
	{
		assert(m_pSetEdit->isStructInitialized());
		if (!m_pSetEdit->isStructInitialized())
			return FALSE;

		BOOL bRet = FALSE;

		DWORD dwIndex;
		E_FIELD eTypeRes;
		if (m_pSetEdit->findCol(name, &dwIndex, &eTypeRes))
		{
			bRet = m_bitModified.getBit(dwIndex);
		}
		else
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
		}

		return bRet;
	}

	BOOL CFieldSetDataImpl::removeData(const NAME_ID &name)
	{
		DWORD dwIndex;
		E_FIELD eTypeRes;
		if (!m_pSetEdit->findCol(name, &dwIndex, &eTypeRes))
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("remove数据中心时字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		const BOOL isExist = m_bitExist.getBit(dwIndex);
		switch (eTypeRes)
		{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				{
					assert(isExist || m_pValue[dwIndex] == 0);
					m_pValue[dwIndex] = 0;
				}
				break;
			case FIELD_E_STR:
				{
					CFieldStringW *pStr = (CFieldStringW *)m_pValue[dwIndex];
					assert(isExist == (pStr != NULL));
					if (pStr)
					{
						pStr->release();
						m_pValue[dwIndex] = 0;
					}
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob *pBlob = (CFieldBlob *)m_pValue[dwIndex];
					assert(isExist == (pBlob != NULL));
					if (pBlob)
					{
						pBlob->release();
						m_pValue[dwIndex] = 0;
					}
				}
				break;
			case FIELD_E_DWORD64:
				{
					DWORD64 *p = (DWORD64 *)m_pValue[dwIndex];
					assert(isExist == (p != NULL));
					if (p)
					{
						mp_free(p);
						m_pValue[dwIndex] = 0;
					}
				}
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit *p = (ITableEdit *)m_pValue[dwIndex];
					assert(isExist == (p != NULL));
					if (p)
					{
						p->release();
						m_pValue[dwIndex] = 0;
					}
				}
				break;
			default:
				assert(! L"不支持的字段类型");
				break;
		}

		if (isExist)
		{
			m_bitExist.setBit(dwIndex, FALSE);
			m_bitModified.setBit(dwIndex, TRUE);
		}

		return isExist;
	}

	BOOL CFieldSetDataImpl::setXXX(const NAME_ID &name, E_FIELD type, DWORD dwDum1, DWORD dwDum2)
	{
		DWORD dwIndex;
		E_FIELD eTypeRes;

		if (!m_pSetEdit->findCol(name, &dwIndex, &eTypeRes))
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("修改数据中心时字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		if (eTypeRes != type)
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("修改数据中心时字段 %s 类型错误！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		BOOL bValueChanged = FALSE;//数据是否发生改变？

		switch (type)
		{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				{
					bValueChanged = m_pValue[dwIndex] != (DWORD)dwDum1;
					if (bValueChanged)
						m_pValue[dwIndex] = (DWORD)dwDum1;
				}
				break;
			case FIELD_E_STR:
				{
					DWORD cchSrc = wcslen((LPCWSTR)dwDum1);

					CFieldStringW *pStr = (CFieldStringW *)m_pValue[dwIndex];
					if (pStr)
					{
						bValueChanged = !pStr->isEqual((LPCWSTR)dwDum1, cchSrc);
						if (bValueChanged)
						{
							pStr->release();
							pStr = NULL;
						}
					}
					else
					{
						bValueChanged = TRUE;
					}

					if (bValueChanged)
					{
						CFieldStringW *pNewStr = CFieldStringW::createInstance((LPCWSTR)dwDum1, cchSrc);
						m_pValue[dwIndex] = (DWORD)pNewStr;
					}
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob *pBlob = (CFieldBlob *)m_pValue[dwIndex];
					if (pBlob)
					{
						bValueChanged = !pBlob->isEqual((const BYTE *)dwDum1, dwDum2);
						if (bValueChanged)
						{
							pBlob->release();
							pBlob = NULL;
						}
					}
					else
					{
						bValueChanged = TRUE;
					}

					if (bValueChanged)
					{
						CFieldBlob *pNewBlob = CFieldBlob::createInstance((const BYTE *)dwDum1, dwDum2);
						m_pValue[dwIndex] = (DWORD)pNewBlob;
					}
				}
				break;	
			case FIELD_E_DWORD64:
				{
					if (m_pValue[dwIndex] != 0)
					{
						bValueChanged = *(DWORD64 *)dwDum1 != *(DWORD64 *)m_pValue[dwIndex];
					}
					else
					{
						bValueChanged = TRUE;
						m_pValue[dwIndex] = (DWORD)mp_alloc(sizeof(DWORD64));
					}

					if (bValueChanged)
						*(DWORD64 *)m_pValue[dwIndex] = *(DWORD64 *)dwDum1;
				}
				break;
			case FIELD_E_TABLE:
				{
					bValueChanged = dwDum1 != m_pValue[dwIndex];

					if (bValueChanged)
					{
						ITableEdit *p = (ITableEdit *)m_pValue[dwIndex];
						if (p)
							p->release();

						((ITableEdit *)dwDum1)->addRef();
						m_pValue[dwIndex] = (DWORD)dwDum1;
					}
				}
				break;
			default:
				assert(! L"不支持的字段类型");
				break;
		}

		BOOL bDataChanged = bValueChanged || !m_bitExist.getBit(dwIndex);
		if (bDataChanged)
		{
			m_bitExist.setBit(dwIndex, TRUE);
			m_bitModified.setBit(dwIndex, TRUE);
		}
		else
		{
			assert(m_bitExist.getBit(dwIndex) == TRUE);//原来应该是存在这个字段的
		}

		return TRUE;
	}

	BOOL CFieldSetDataImpl::setStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		return setXXX(name, FIELD_E_STR, (DWORD)lpwszValue);
	}

	BOOL CFieldSetDataImpl::setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize)
	{
		return setXXX(name, FIELD_E_BLOB, (DWORD)pbData, cbSize);
	}

	BOOL CFieldSetDataImpl::setDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		return setXXX(name, FIELD_E_DWORD64, (DWORD)&dw64Value);
	}

	BOOL CFieldSetDataImpl::setDword(const NAME_ID &name, DWORD dwValue)
	{
		return setXXX(name, FIELD_E_DWORD, (DWORD)dwValue);
	}

	BOOL CFieldSetDataImpl::setWord(const NAME_ID &name, WORD wValue)
	{
		return setXXX(name, FIELD_E_WORD, (DWORD)wValue);
	}
	
	BOOL CFieldSetDataImpl::setByte(const NAME_ID &name, BYTE byValue)
	{
		return setXXX(name, FIELD_E_BYTE, (DWORD)byValue);
	}

	BOOL CFieldSetDataImpl::setBool(const NAME_ID &name, BOOL bValue)
	{
		return setXXX(name, FIELD_E_BOOL, (DWORD)bValue);
	}

	BOOL CFieldSetDataImpl::setSubTable(const NAME_ID &name, ITableEdit *pSubTable)
	{
		return setXXX(name, FIELD_E_TABLE, (DWORD)pSubTable);
	}

	CFieldSetDataImpl * CFieldSetDataImpl::clone() const
	{
		CFieldSetDataImpl *pNew = CFieldSetDataImpl::createInstance(m_pSetEdit);
		pNew->init();

		const CFieldSetImpl::COLS *pCols = m_pSetEdit->getCols();
		
		memcpy(pNew->m_pValue, m_pValue, sizeof(DWORD) * pCols->dwCount);

		for (UINT i = 0; i < pCols->dwCount; i++)
		{
			switch(pCols->arType[i])
			{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				break;
			case FIELD_E_STR:
				{
					CFieldStringW *pStr = (CFieldStringW *)m_pValue[i];
					if (pStr)
						pStr->addRef();
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob *pBlob = (CFieldBlob *)m_pValue[i];
					if (pBlob)
						pBlob->addRef();
				}
				break;
			case FIELD_E_DWORD64:
				{
					DWORD64 *p = (DWORD64 *)mp_alloc(sizeof(DWORD64));
					*p = *(DWORD64 *)m_pValue[i];
					
					pNew->m_pValue[i] = (DWORD)p;
				}
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit *p = (ITableEdit *)m_pValue[i];
					if (p)
						p->addRef();
				}
				break;
			default:
				assert(! L"不支持的字段类型");
				break;				
			}
		}

		pNew->m_bitExist = m_bitExist;

		//注意m_bitModified不需要复制
		return pNew;
	}
	
	void CFieldSetDataImpl::getContentStr(std::wstring &wstrOut)
	{
		wstrOut = _T("");

		TCHAR szTemp[1024] = {0};
		wstrOut.reserve(_countof(szTemp));
	
		const CFieldSetImpl::COLS *pCols = m_pSetEdit->getCols();			
		for (UINT i = 0; i < pCols->dwCount; i++)
		{
			const NAME_ID colName = pCols->arId[i];

			if (hasData(colName))
			{
				switch (pCols->arType[i])
				{
				case FIELD_E_STR:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%s, "), colName.GetName(), getStr(colName));
					break;
				case FIELD_E_BLOB:
					{
						DWORD dwSize;
						const BYTE *pData = getBlob(colName, &dwSize);
						_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=ptr:0x%08x size:%d, "), colName.GetName(), (unsigned int) pData, dwSize);
					}
					break;
				case FIELD_E_DWORD64:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%I64u, "), colName.GetName(), getDword64(colName));
					break;
				default:
					DWORD dwDum = 0;
					getXXX(colName, (E_FIELD)pCols->arType[i], &dwDum);
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%u, "), colName.GetName(), dwDum);
					break;
				}
			}
			else
			{
				_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=NULL, "), colName.GetName());
			}

			wstrOut += szTemp;
		}

		assert(wstrOut.size() > 2);
		if (wstrOut.size() > 2)
			wstrOut.resize(wstrOut.size() - 2);
	}

	void CFieldSetDataImpl::dump()
	{
		assert(m_pSetEdit->isStructInitialized());
		if (!m_pSetEdit->isStructInitialized())
			return;

		std::wstring strContent;
		getContentStr(strContent);

		Error::OutputDebug(_T("fieldset dump: name=%s, ptr=0x%08X(%u), ref=%d, %s"), 
			getFieldSetName().GetName(), 
			this,
			this,
			m_dwRef,
			strContent.c_str());
	}
};
