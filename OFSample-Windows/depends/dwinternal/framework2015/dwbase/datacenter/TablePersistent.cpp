#include "TablePersistent.h"
#include "DatabaseContextImpl.h"
#include "DatabaseManager.h"
#include "IDataUtil.h"
#include "dwutility/dwxmldocument.h"
#include "dwutility/dwxmlelement.h"

using namespace Data;

//------------------------------------------------------------------------------------------
static const WCHAR XML_ELEMENT_TABLE[] = L"table";
	static const WCHAR XML_ATTR_TABLE_NAME[] = L"name";
	static const WCHAR XML_ELEMENT_ROW[] = L"row";
		static const WCHAR XML_ELEMENT_COL[] = L"col";
//------------------------------------------------------------------------------------------

template <E_FIELD>
struct _FiledPersistent
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return FALSE;
	}
};

template<>
struct _FiledPersistent<FIELD_E_STR>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			return spRow->getStr(fieldName);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setStr(fieldName, val);
	}
};

template<>
struct _FiledPersistent<FIELD_E_DWORD64>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			WCHAR buf[32] = { 0 };
			return _i64tow(spRow->getDword64(fieldName), buf, 10);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setDword64(fieldName, (DWORD64)_wtoi64(val));
	}
};

template <>
struct _FiledPersistent<FIELD_E_DWORD>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			WCHAR buf[16] = { 0 };
			return _ultow(spRow->getDword(fieldName), buf, 10);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setDword(fieldName, (DWORD)_wtol(val));
	}
};

template <>
struct _FiledPersistent<FIELD_E_WORD>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			WCHAR buf[16] = { 0 };
			return _ultow(spRow->getWord(fieldName), buf, 10);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setWord(fieldName, (WORD)_wtol(val));
	}
};

template <>
struct _FiledPersistent<FIELD_E_BYTE>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			WCHAR buf[16] = { 0 };
			return _ultow(spRow->getByte(fieldName), buf, 10);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setByte(fieldName, (BYTE)_wtol(val));
	}
};

template <>
struct _FiledPersistent<FIELD_E_BOOL>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			WCHAR buf[16] = { 0 };
			return _ultow(spRow->getBool(fieldName) ? 0 : 1, buf, 10);
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		return spRow->setBool(fieldName, (BOOL)_wtol(val));
	}
};

template<>
struct _FiledPersistent<FIELD_E_BLOB>
{
	static std::wstring read(IRowPtrCR spRow, NAME_ID fieldName)
	{
		if (spRow->hasData(fieldName))
		{
			DWORD len = 0;
			const BYTE* blob = spRow->getBlob(fieldName, &len);
			if (len > 0 && blob)
			{
				return bin2hex(blob, len);
			}
		}
		return L"";
	}
	static BOOL write(IRowEditPtr spRow, NAME_ID fieldName, LPCWSTR val)
	{
		std::string blob = hex2bin(val);
		return spRow->setBlob(fieldName, (const BYTE*)blob.data(), blob.length());
	}

private:
	static std::wstring bin2hex(const void * _src, size_t len)
	{
		const WCHAR * Hex = (const WCHAR*)L"0123456789abcdef"; 
		const unsigned char * src = (const unsigned char *)_src; 

		std::wstring hex;
		hex.resize(len << 1);

		for (size_t i = 0; i < len; i++)
		{
			hex[i*2] = Hex[src[i] >> 4 ];
			hex[i*2+1] = Hex[src[i] & 0xF];
		}

		return hex;
	}

	static std::string hex2bin(LPCWSTR hex)
	{
		const BYTE Bin[] =
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x00 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x10 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x20 */
			0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,        /* 0x30 */
			0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,  /* 0x40 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x50 */
			0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,  /* 0x60 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x80 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0x90 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xa0 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xb0 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xc0 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xd0 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xe0 */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 0xf0 */
		};

		size_t len = wcslen(hex) >> 1;
		std::string bin;
		bin.resize(len);

		for (size_t i = 0; i < len; ++i)
		{
			bin[i] = (Bin[(BYTE)hex[i * 2]] << 4) | Bin[(BYTE)hex[i * 2 + 1]];
		}

		return bin;
	}
};

//------------------------------------------------------------------------------------------
CTablePersistent::CTablePersistent(CDatabaseContextImpl* pDatabaseContextImpl)
	: m_pDatabaseContextImpl(pDatabaseContextImpl)
{

}
CTablePersistent::~CTablePersistent()
{
	m_pDatabaseContextImpl = NULL;
}

BOOL CTablePersistent::load(ITableEditPtr spTable, LPCWSTR lpszTableDataPath)
{
	DWXmlDocument xmlDoc;
	if (!xmlDoc.loadFromFile(lpszTableDataPath))
	{
		return FALSE;
	}
	TABLE_STRUCT tableStruct;
	if (!m_pDatabaseContextImpl->queryTableStruct(spTable->getTableName(), &tableStruct))
	{
		return FALSE;
	}
	if (!_loadTable(spTable, &tableStruct, xmlDoc.rootElement()))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTablePersistent::save(ITablePtr spTable, LPCWSTR lpszTableDataPath)
{
	DWXmlDocument xmlDoc;
	DWXmlElement* pTableElement = xmlDoc.createElementByPath(XML_ELEMENT_TABLE);
	if (pTableElement == NULL)
	{
		return FALSE;
	}
	TABLE_STRUCT tableStruct;
	if (!m_pDatabaseContextImpl->queryTableStruct(spTable->getTableName(), &tableStruct))
	{
		return FALSE;
	}
	if (!_saveTable(spTable, &tableStruct, pTableElement))
	{
		return FALSE;
	}
	return !!xmlDoc.saveToFile(lpszTableDataPath);
}

BOOL CTablePersistent::_saveTable(ITablePtr spTable, const TABLE_STRUCT* pTableStruct, DWXmlElement* pTableElement)
{
	pTableElement->setAttribute(XML_ATTR_TABLE_NAME, spTable->getTableName().GetName());
	IRowArrayPtr spRows = spTable->all();
	for (UINT i = 0; i < spRows->size(); ++i)
	{
		DWXmlElement* pRowElement = new DWXmlElement(XML_ELEMENT_ROW);
		if (_saveRow(spRows->at(i), pTableStruct, pRowElement))
		{
			pTableElement->addChild(pRowElement);
		}
		else
		{
			delete pRowElement;
		}
	}
	return TRUE;
}

BOOL CTablePersistent::_saveRow(IRowPtrCR spRow, const TABLE_STRUCT* pTableStruct, DWXmlElement* pRowElement)
{
	for (UINT i = 0; i < pTableStruct->vecCol.size(); ++i)
	{
		COL_PROP colProp = pTableStruct->vecCol[i];
		if (colProp.bVirtual)
		{
			continue;
		}

		DWXmlElement* pColElement = new DWXmlElement(XML_ELEMENT_COL);
		BOOL bSuccessed = TRUE;
		switch(colProp.eColType)
		{
		case FIELD_E_STR:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_STR>::read(spRow, colProp.colName));
			break;
		case FIELD_E_DWORD64:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_DWORD64>::read(spRow, colProp.colName));
			break;
		case FIELD_E_DWORD:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_DWORD>::read(spRow, colProp.colName));
			break;
		case FIELD_E_WORD:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_WORD>::read(spRow, colProp.colName));
			break;
		case FIELD_E_BYTE:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_BYTE>::read(spRow, colProp.colName));
			break;
		case FIELD_E_BOOL:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_BOOL>::read(spRow, colProp.colName));
			break;
		case FIELD_E_BLOB:
			pColElement->setAttribute(colProp.colName.GetName(), _FiledPersistent<FIELD_E_BLOB>::read(spRow, colProp.colName));
			break;
		case FIELD_E_TABLE:
			bSuccessed = _saveSubTable(spRow, colProp.colName, pColElement);
			break;

		//暂时不支持
		case FIELD_E_UNK:
		default:
			break;
		}

		if (bSuccessed)
		{
			pRowElement->addChild(pColElement);
		}
		else
		{
			delete pRowElement;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CTablePersistent::_saveSubTable(IRowPtrCR spRow, NAME_ID subTableName, DWXmlElement* pColElement)
{
	ITablePtr spSubTable = spRow->getSubTable(subTableName);
	if (spSubTable == NULL)
	{
		return FALSE;
	}

	DWXmlElement* pSubTableElement = new DWXmlElement(XML_ELEMENT_TABLE);
	TABLE_STRUCT subTableStruct;
	if (m_pDatabaseContextImpl->queryTableStruct(subTableName, &subTableStruct)
		&& _saveTable(spSubTable, &subTableStruct, pSubTableElement))
	{
		pColElement->setAttribute(subTableName.GetName(), subTableName.GetName());
		pColElement->addChild(pSubTableElement);
		return TRUE;
	}
	else
	{
		delete pSubTableElement;
		return FALSE;
	}
}

BOOL CTablePersistent::_loadTable(ITableEditPtr spTable, const TABLE_STRUCT* pTableStruct, const DWXmlElement* pTableElement)
{
	std::wstring tableName = pTableElement->attribute(XML_ATTR_TABLE_NAME);
	if (tableName.compare(spTable->getTableName().GetName()) != 0)
	{
		return FALSE;
	}

	COL_MAP colMap;
	for (std::vector<COL_PROP>::const_iterator itCol = pTableStruct->vecCol.begin();
		itCol != pTableStruct->vecCol.end(); ++itCol)
	{
		if (!itCol->bVirtual)
		{
			colMap.insert(std::make_pair(itCol->colName, itCol->eColType));
		}
	}

	for (int i = 0; i < pTableElement->childCount(); ++i)
	{
		const DWXmlElement* pRowElement = pTableElement->child(i);
		CEditLock oLock(spTable);
		IRowEditPtr spRowEdit = spTable->allocRow();
		if (_loadRow(spRowEdit, colMap, pRowElement))
		{
			spTable->insert(spRowEdit);
		}	
	}
	return TRUE;
}

BOOL CTablePersistent::_loadRow(IRowEditPtr spRow, const COL_MAP& colMap, const DWXmlElement* pRowElement)
{
	for (int i = 0; i < pRowElement->childCount(); ++i)
	{
		const DWXmlElement* pColElement = pRowElement->child(i);
		if (pColElement->attributeCount() != 1)
		{
			return FALSE;
		}
		NAME_ID fieldName(pColElement->attributeName(0).c_str());
		COL_MAP::const_iterator itCol = colMap.find(fieldName);
		if (itCol == colMap.end())
		{
			continue;
		}

		BOOL bSuccessed = TRUE;
		switch(itCol->second)
		{
		case FIELD_E_STR:
			bSuccessed = _FiledPersistent<FIELD_E_STR>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_DWORD64:
			bSuccessed = _FiledPersistent<FIELD_E_DWORD64>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_DWORD:
			bSuccessed = _FiledPersistent<FIELD_E_DWORD>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_WORD:
			bSuccessed = _FiledPersistent<FIELD_E_WORD>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_BYTE:
			bSuccessed = _FiledPersistent<FIELD_E_BYTE>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_BOOL:
			bSuccessed = _FiledPersistent<FIELD_E_BOOL>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_BLOB:
			bSuccessed = _FiledPersistent<FIELD_E_BLOB>::write(spRow, fieldName, pColElement->attribute(0).c_str());
			break;
		case FIELD_E_TABLE:
			bSuccessed = _loadSubTable(spRow, pColElement);
			break;

			//暂时不支持
		case FIELD_E_UNK:
		default:
			break;
		}

		if (!bSuccessed)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CTablePersistent::_loadSubTable(IRowEditPtr spRow, const DWXmlElement* pColElement)
{
	if (pColElement->childCount() != 1)
	{
		return FALSE;
	}

	NAME_ID subTableName(pColElement->attribute(0).c_str());
	const DWXmlElement* pSubTableElement = pColElement->child(0);
	IDatabasePtr spDatabase = CDatabaseManager::GetInstance()->queryDatabase(m_pDatabaseContextImpl->getDatabaseId());
	ITableEditPtr spSubTable = spDatabase->allocTable(subTableName);

	TABLE_STRUCT subTableStruct;
	if (m_pDatabaseContextImpl->queryTableStruct(subTableName, &subTableStruct)
		&& _loadTable(spSubTable, &subTableStruct, pSubTableElement))
	{
		spRow->setSubTable(subTableName, spSubTable);
		return TRUE;
	}

	return FALSE;
}