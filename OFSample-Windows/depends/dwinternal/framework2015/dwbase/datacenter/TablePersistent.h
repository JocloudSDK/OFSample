#pragma once
#include "ITablePub.h"
#include "ITable.h"
#include "ITableEdit.h"
#include "DataCenterPub.h"

class DWXmlElement;
namespace Data
{
	class CDatabaseContextImpl;
	class CTablePersistent
	{
		typedef std::map<NAME_ID, E_FIELD> COL_MAP;

	public:
		CTablePersistent(CDatabaseContextImpl* pDatabaseContextImpl);
		~CTablePersistent();
	public:
		BOOL load(ITableEditPtr spTable, LPCWSTR lpszTableDataPath);
		BOOL save(ITablePtr spTable, LPCWSTR lpszTableDataPath);

	private:
		BOOL _saveTable(ITablePtr spTable, const TABLE_STRUCT* pTableStruct, DWXmlElement* pTableElement);
		BOOL _saveRow(IRowPtrCR spRow, const TABLE_STRUCT* pTableStruct, DWXmlElement* pRowElement);
		BOOL _saveSubTable(IRowPtrCR spRow, NAME_ID subTableName, DWXmlElement* pColElement);
		BOOL _loadTable(ITableEditPtr spTable, const TABLE_STRUCT* pTableStruct, const DWXmlElement* pTableElement);
		BOOL _loadRow(IRowEditPtr spRow, const COL_MAP& colMap, const DWXmlElement* pRowElement);
		BOOL _loadSubTable(IRowEditPtr spRow, const DWXmlElement* pColElement);

	private:
		CDatabaseContextImpl* m_pDatabaseContextImpl;
	};
};