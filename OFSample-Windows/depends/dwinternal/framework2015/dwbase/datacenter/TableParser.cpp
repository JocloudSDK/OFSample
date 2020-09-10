#include "stable.h"
#include "TableParser.h"
#include "StringTable.h"
#include "../log/log.h"

#include <dwutility/ustring.h>
#include <dwutility/dwxmldocument.h>

namespace Data
{
	CTableParser::CTableParser(void)
	{
	}

	CTableParser::~CTableParser(void)
	{
	}

	BOOL CTableParser::QueryTableStruct(const NAME_ID& tableName, TABLE_STRUCT* pTableStruct) const
	{
		assert(!tableName.IsEmpty());
	
		BOOL bRet = FALSE;

		MapTblStruct::const_iterator it = m_mapTblStruct.find(tableName);
		if (it != m_mapTblStruct.end())
		{
			if (it->second.tableStruct.indexColName.IsEmpty())
			{
				assert(! L"模版可能没有装载完全，表没有索引列，可能主表没有装载!!");
				LogFinal(LOG::KDataCenter)<<L"模版可能没有装载完全，表没有索引列，可能主表没有装载!!";
			}
			else
			{
				bRet = TRUE;
				*pTableStruct = it->second.tableStruct;
				assert(pTableStruct->tableName == tableName);
			}
		}

		return bRet;
	}

	void CTableParser::QueryViewStruct(const NAME_ID &tableName, std::vector<VIEW_STRUCT> &vecViewStruct) const
	{
		assert(!tableName.IsEmpty());
		vecViewStruct.clear();

		MapViewStruct::const_iterator it = m_mapViewStruct.find(tableName);
		if (it != m_mapViewStruct.end())
		{
			vecViewStruct.reserve(it->second.size());
			for (UINT i = 0; i < it->second.size(); i++)
				vecViewStruct.push_back(it->second[i]);
		}

	}

	BOOL CTableParser::QueryFieldSetStruct(const NAME_ID& fieldSetName, FIELDSET_STRUCT* pStruct) const
	{
		assert(!fieldSetName.IsEmpty());

		MapFieldSetStruct::const_iterator it = m_mapFieldSetStruct.find(fieldSetName);
		if (it != m_mapFieldSetStruct.end())
		{
			*pStruct = it->second;
			return TRUE;
		}
		
		return FALSE;
	}

	DWORD CTableParser::LoadDataTemplate(const char *pBuffer, DWORD dwSize)
	{
		assert(NULL != pBuffer && dwSize > 0);
		
		DWORD dwReturnFlag = INVALID_FILE_FLAG;

		const std::wstring buf = DwUtility::text::toWideString(pBuffer, dwSize, CP_UTF8);

		DWXmlDocument xmlDoc;
		if (xmlDoc.loadFromString(pBuffer, dwSize))
		{
			LogFinal(LOG::KDataCenter) << "Begin to load template file";
			dwReturnFlag = LoadDataTemplateHelper(*xmlDoc.rootElement());
		}
		else
		{
			assert(FALSE);
			LogFinal(LOG::KDataCenter)<<"Failed to load template file: content=("<<buf<<")";
		} 
		
		return dwReturnFlag;
	}

	DWORD CTableParser::LoadDataTemplate(LPCTSTR szFileName)
	{
		assert(NULL != szFileName);

		DWXmlDocument xmlDoc;
		if (xmlDoc.loadFromFile(szFileName))
		{
			assert(FALSE);
			LogFinal(LOG::KDataCenter)<<"Failed to load template file("<<szFileName<<")";

			return INVALID_FILE_FLAG;
		} 
		
		LogFinal(LOG::KDataCenter)<<"Begin to load template file("<<szFileName<<")";

		return LoadDataTemplateHelper(*xmlDoc.rootElement());
	}

	DWORD CTableParser::LoadDataTemplateHelper(const DWXmlElement& rootElem)
	{
		if(rootElem.tag() != _T("tables"))//root元素
		{
			assert(FALSE);
			LogFinal(LOG::KDataCenter)<<"Failed to find xml root element(tables)";

			return INVALID_FILE_FLAG;
		}
		
		const DWORD dwFileFlag = getNewId();

		for (int i = 0; i < rootElem.childCount(); ++i)
		{
			const DWXmlElement &elem = *rootElem.child(i);

			const std::wstring strTagName = elem.tag();
			
			if (strTagName == _T("view"))
			{
				const std::wstring strViewName = elem.attribute(_T("name"));//视图名
				if (strViewName.empty())
				{
					assert(FALSE);
					LogFinal(LOG::KDataCenter)<<"Name of view is empty, skip it.";

					continue;
				}

				const std::wstring strTblName = elem.attribute(_T("owner-table"));
				if (strTblName.empty())
				{
					assert(FALSE);
					LogFinal(LOG::KDataCenter)<<"owner-table of view is empty, skip it.";

					continue;
				}

				NAME_ID tableName(strTblName.c_str());
				NAME_ID tableViewName(strViewName.c_str());

				MapViewStruct::iterator it = m_mapViewStruct.find(tableName);
				if (it != m_mapViewStruct.end())
				{
					//检查是否重复
					BOOL bExist = FALSE;

					std::vector<VIEW_STRUCT>& vecViewStruct = it->second;
					for (UINT i = 0; i < vecViewStruct.size(); i++)
					{
						if (vecViewStruct[i].viewName == tableViewName)
						{
							bExist = TRUE;
							break;
						}
					}
					
					if (bExist)
					{
						assert(FALSE);
						LogFinal(LOG::KDataCenter) << L"视图名字重复, skip it.";
					}
					else
					{
						VIEW_STRUCT viewStruct;
						viewStruct.viewName = tableViewName;
						vecViewStruct.push_back(viewStruct);
					}
				}
				else
				{
					VIEW_STRUCT viewStruct;
					viewStruct.viewName = tableViewName;
					m_mapViewStruct[tableName].push_back(viewStruct);
				}
			}
			else if (strTagName == _T("fieldset"))
			{
				const std::wstring strFieldSetName = elem.attribute(_T("name"));//视图名
				if (strFieldSetName.empty())
				{
					assert(FALSE);
					LogFinal(LOG::KDataCenter)<<"Name of fieldset is empty, skip it.";

					continue;
				}

				NAME_ID fieldSetName(strFieldSetName.c_str());

				MapFieldSetStruct::iterator it = m_mapFieldSetStruct.find(fieldSetName);
				if (it != m_mapFieldSetStruct.end())
				{
					assert(! L"fieldset名字重复");
					LogFinal(LOG::KDataCenter)<<"fieldset, skip it.";
				}
				else
				{
					std::vector<COL_PROP> vecColProp;
					if (ParseColum(elem, vecColProp))
					{
						//开始错误检查
						BOOL bCheckError = FALSE;

						//检查列是否存在重复
						std::set<NAME_ID> setUniqueCheck;
						for (UINT i = 0; i < vecColProp.size(); i++)
						{
							if (!setUniqueCheck.insert(vecColProp[i].colName).second)
							{
								bCheckError = TRUE;
								assert(! L"列名字存在重复");
								LogFinal(LOG::KDataCenter)<<"Column("<<vecColProp[i].colName.GetName()<<") already exists in fieldset.";
								break;
							}
						}

						if (!bCheckError)
						{
							FIELDSET_STRUCT fieldSetStruct;
							fieldSetStruct.fieldSetName = fieldSetName;
							fieldSetStruct.vecCol = vecColProp;
							m_mapFieldSetStruct[fieldSetName] = fieldSetStruct;
							
							LogFinal(LOG::KDataCenter)<<L"Succeed in parsing fieldset("<<strFieldSetName<<")";
						}
					}
				}
			}
			else if (strTagName == _T("table") || strTagName == _T("tableext"))
			{
				std::wstring strTblName;
				std::wstring strIndexCol;
				std::wstring strIndexType;
				E_TABLE_MODE eTblMode;
				E_INDEX_TYPE eIdxType;
				
				if (strTagName == _T("table"))//主表索引处理
				{
					eTblMode = TABLE_MAIN;
					strIndexCol = elem.attribute(_T("index-col"));
					if (strIndexCol.empty())
					{
						assert(FALSE);
						LogFinal(LOG::KDataCenter)<<"Index name of main-table("<<strTblName<<") is empty, skip it.";

						continue;
					}
					
					strIndexType = elem.attribute(_T("index-type"));
					if (0 == strIndexType.compare(_T("hash_map")))
					{
						eIdxType = INDEX_E_HASH;
					}
					else if (0 == strIndexType.compare(_T("tiny_map")))
					{
						eIdxType = INDEX_E_TINY_MAP;
					}
					else if (0 == strIndexType.compare(_T("map")))
					{
						eIdxType = INDEX_E_MAP;
					}
					else
					{
						assert(FALSE);
						LogFinal(LOG::KDataCenter)<<"Index type of main-table("<<strTblName<<") is invalid, and skip it.";

						continue;		
					}

					LogFinal(LOG::KDataCenter)<<"Begin to parse main-table("<<strTblName<<"), Index:"<<strIndexCol<<", Type:"<<strIndexType;
				}
				else//扩展表
				{
					eTblMode = TABLE_EXT;

					LogFinal(LOG::KDataCenter)<<"Begin to parse ext-table("<<strTblName<<")";
				} 

				strTblName = elem.attribute(_T("name"));//表名
				if (strTblName.empty())
				{
					assert(FALSE);
					LogFinal(LOG::KDataCenter)<<"Name of table is empty, skip it.";

					continue;
				}
				NAME_ID tableName(strTblName.c_str());


				TABLE_ITEM tempTableItem;
				MapTblStruct::const_iterator it = m_mapTblStruct.find(tableName);
				if (it != m_mapTblStruct.end())
				{
					tempTableItem = it->second;
				}
				else
				{
					tempTableItem.tableStruct.eIndexType = INDEX_E_NULL;
					tempTableItem.tableStruct.tableName = tableName;
					tempTableItem.dwFileFlag = INVALID_FILE_FLAG;
				}

				if (TABLE_MAIN == eTblMode)
				{
					//检查是否之前已经装载过主表
					if (!tempTableItem.tableStruct.indexColName.IsEmpty())
					{
						assert(! L"重复装载主表!!");
						LogFinal(LOG::KDataCenter)<<L"重复装载主表!!("<<strTblName<<").";

						continue;
					}

					tempTableItem.tableStruct.eIndexType = eIdxType;
					tempTableItem.tableStruct.indexColName = insert_name_id(strIndexCol.c_str());
					tempTableItem.dwFileFlag = dwFileFlag;	//确保主表能被Unload
				}
				else
				{
					if (tempTableItem.dwFileFlag == INVALID_FILE_FLAG)
					{
						tempTableItem.dwFileFlag = dwFileFlag; //确保无主表的扩展表也能被Unload			
					}
				}

				std::vector<COL_PROP> vecColProp;
				if (ParseColum(elem, vecColProp))
				{
					//主表的字段总是放到前面，保持内存结构一致，而不管模板文件装载顺序如何
					if (TABLE_MAIN)
						tempTableItem.tableStruct.vecCol.insert(tempTableItem.tableStruct.vecCol.begin(), vecColProp.begin(), vecColProp.end());
					else
						tempTableItem.tableStruct.vecCol.insert(tempTableItem.tableStruct.vecCol.end(), vecColProp.begin(), vecColProp.end());


					//开始错误检查
					BOOL bCheckError = FALSE;

					if (TABLE_MAIN == eTblMode)
					{
						//检查是否存在索引列
						BOOL bExistIndexCol = FALSE;
						for (UINT i = 0; i < vecColProp.size(); i++)
						{
							if (tempTableItem.tableStruct.indexColName == vecColProp[i].colName)
							{
								bExistIndexCol = TRUE;
								break;
							}
						}

						if (!bExistIndexCol)
						{
							bCheckError = TRUE;
							assert(FALSE);
							LogFinal(LOG::KDataCenter)<<"Index col is inexistent in columns. so table is invalid";
						}
					}
					
					//检查列是否存在重复
					std::set<NAME_ID> setUniqueCheck;
					for (UINT i = 0; i < tempTableItem.tableStruct.vecCol.size(); i++)
					{
						if (!setUniqueCheck.insert(tempTableItem.tableStruct.vecCol[i].colName).second)
						{
							bCheckError = TRUE;
							assert(! L"列名字存在重复");
							LogFinal(LOG::KDataCenter)<<"Column("<<tempTableItem.tableStruct.vecCol[i].colName.GetName()<<") already exists in table.";
							break;
						}
					}

					if (!bCheckError)
					{
						m_mapTblStruct[tableName] = tempTableItem;

						LogFinal(LOG::KDataCenter)<<"Succeed in parsing main-table("<<strTblName<<")";
					}
				}
			}
			else
			{
				assert(FALSE);
				LogFinal(LOG::KDataCenter)<<"Tag name("<<strTagName<<") is invalid, and continue next element.";

				continue;
			}
		}

		assert(dwFileFlag != INVALID_FILE_FLAG);
		return dwFileFlag;
	}
	
	BOOL CTableParser::ParseColum(const DWXmlElement& tableElem, std::vector<COL_PROP> &vecColProp)
	{
		vecColProp.clear();

		BOOL bError = FALSE;
		for (int i = 0; i < tableElem.childCount(); ++i)
		{
			const DWXmlElement &elem = *tableElem.child(i);
			if (elem.tag() != _T("col"))
				continue;

			COL_PROP	oColProp;

			const std::wstring strColName = elem.attribute(_T("name"));
			if (strColName.empty())
			{
				bError = TRUE;

				assert(FALSE);
				LogFinal(LOG::KDataCenter)<<"There is empty element in columns.";
				break;
			}
			oColProp.colName.m_nId = insert_name_id(strColName.c_str());
			
			const std::wstring strColType = elem.attribute(_T("type"));
			if (strColType.compare(_T("dword")) == 0) 
			{
				oColProp.eColType = FIELD_E_DWORD;
			} 
			else if (strColType.compare(_T("word")) == 0)
			{
				oColProp.eColType = FIELD_E_WORD;
			} 
			else if (strColType.compare(_T("string")) == 0)
			{
				oColProp.eColType = FIELD_E_STR;
			}
			else if (strColType.compare(_T("blob")) == 0)
			{
				oColProp.eColType = FIELD_E_BLOB;
			}	
			else if (strColType.compare(_T("byte")) == 0)
			{
				oColProp.eColType = FIELD_E_BYTE;
			} 
			else if (strColType.compare(_T("table")) == 0)
			{
				oColProp.eColType = FIELD_E_TABLE;
			}
			else if (strColType.compare(_T("bool")) == 0)
			{
				oColProp.eColType = FIELD_E_BOOL;
			} 
			else if (strColType.compare(_T("dword64")) == 0)
			{
				oColProp.eColType = FIELD_E_DWORD64;
			} 
			else
			{
				bError = TRUE;

				assert(FALSE);
				LogFinal(LOG::KDataCenter)<<"Type("<<strColType<<") of Column is invalid.";
				break;
			}

			const std::wstring strColVirtual = elem.attribute(_T("virtual"));
			if (strColVirtual.compare(_T("true")) == 0) 
			{
				oColProp.bVirtual = TRUE;
			}
			else if (strColVirtual.empty() || strColVirtual.compare(_T("false")) == 0) 
			{
				oColProp.bVirtual = FALSE;
			}
			else
			{
				bError = TRUE;

				assert(FALSE);
				LogFinal(LOG::KDataCenter)<<"Virtual("<<strColVirtual<<") of Column is invalid.";
				break;
			}

			vecColProp.push_back(oColProp);
		}

		if (bError)
			vecColProp.clear();

		return !bError;
	}

	void CTableParser::UnloadDataTemplate(DWORD dwFileFlag, std::vector<NAME_ID>& vecTableName)
	{
		vecTableName.clear();

		for (MapTblStruct::iterator iter = m_mapTblStruct.begin(); iter != m_mapTblStruct.end(); )
		{
			if (dwFileFlag == iter->second.dwFileFlag)
			{
				vecTableName.push_back(iter->second.tableStruct.tableName);
				m_mapTblStruct.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	DWORD CTableParser::getNewId()
	{
		static DWORD s_dwCount = 0;
		return ::InterlockedIncrement((LONG *)&s_dwCount);
	}
};
