#pragma once
#include "ITablePub.h"
#include <map>
#include <vector>
#include "DataCenterPub.h"

#define INVALID_FILE_FLAG 0

class DWXmlElement;
namespace Data
{
	class CTableParser
	{
	public:
		CTableParser(void);
		~CTableParser(void);
		
		DWORD				LoadDataTemplate(LPCTSTR szFileName);
		DWORD				LoadDataTemplate(const char *pBuffer, DWORD dwSize);
		void				UnloadDataTemplate(DWORD dwFileFlag, std::vector<NAME_ID>& vecTableName);
		BOOL				QueryTableStruct(const NAME_ID& tableName, TABLE_STRUCT* pTableStruct) const;
		void				QueryViewStruct(const NAME_ID &tableName, std::vector<VIEW_STRUCT> &vecViewStruct) const;
		BOOL				QueryFieldSetStruct(const NAME_ID& fieldSetName, FIELDSET_STRUCT* pStruct) const;
	
	
	private:
		DWORD				LoadDataTemplateHelper(const DWXmlElement& rootElem);
		static BOOL			ParseColum(const DWXmlElement& tableElem, std::vector<COL_PROP> &vecColProp);
		static DWORD		getNewId();	

		enum E_TABLE_MODE
		{
			TABLE_MAIN = 1,
			TABLE_EXT
		};

		struct TABLE_ITEM
		{
			TABLE_STRUCT	tableStruct;
			DWORD			dwFileFlag;
		};

		typedef std::map<NAME_ID, TABLE_ITEM> MapTblStruct;
		MapTblStruct m_mapTblStruct;	//´æ´¢±í½á¹¹

		typedef std::map<NAME_ID, std::vector<VIEW_STRUCT> > MapViewStruct;
		MapViewStruct m_mapViewStruct;

		typedef std::map<NAME_ID, FIELDSET_STRUCT> MapFieldSetStruct;
		MapFieldSetStruct m_mapFieldSetStruct;	
	};
};

