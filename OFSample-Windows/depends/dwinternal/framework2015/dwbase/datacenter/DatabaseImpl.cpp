
#include "stable.h"
#include "DatabaseImpl.h"
#include "TableEditImpl.h"
#include "ViewImpl.h"
#include "FieldSetImpl.h"
#include "../log/log.h"
#include <assert.h>
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

namespace Data
{
	CDatabaseImpl::CDatabaseImpl(DWORD dwId)
	{
		m_dwRef = 1;
		m_tblCache.pTable = NULL;
		m_pContext = CDatabaseContextImpl::createInstance(dwId);
	}

	CDatabaseImpl::~CDatabaseImpl(void)
	{
		assert(m_dwRef == 0);

		m_tblCache.pTable = NULL;
		m_mapTableEdit.clear();
		m_pContext->release();
		m_pContext = NULL;
	}

	CDatabaseImpl * CDatabaseImpl::createInstance(DWORD dwId)
	{
		return new CDatabaseImpl(dwId);
	}

	void CDatabaseImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CDatabaseImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CDatabaseImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CDatabaseImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	ITablePtr CDatabaseImpl::getTable(const NAME_ID& tableName)
	{
		CAutoLock lock(m_csLock);

		ITableEditPtr pTableEdit = getTableEdit(tableName);
		if (pTableEdit)
			return pTableEdit->getTable();
		
		return NULL;
	}

	ITableEditPtr CDatabaseImpl::getTableEdit(const NAME_ID& tableName)
	{
		CAutoLock lock(m_csLock);

		assert(!tableName.IsEmpty());
		if (tableName.IsEmpty())
			return NULL;

		ITableEditPtr pTableEdit = queryTableEdit(tableName);
		if (!pTableEdit)
		{
			pTableEdit = allocTable(tableName);
			assert(pTableEdit);
			if (pTableEdit)
				m_mapTableEdit[tableName] = pTableEdit;
		}

		return pTableEdit;
	}

	ITableEditPtr CDatabaseImpl::queryTableEdit(const NAME_ID& tableName)
	{
		if (m_tblCache.oName == tableName)
			return m_tblCache.pTable;

		MapTableEdit::iterator iter = m_mapTableEdit.find (tableName);
		if (iter != m_mapTableEdit.end ())
		{
			//更新Cache
			m_tblCache.oName = tableName;
			m_tblCache.pTable = iter->second;

			return iter->second;
		} 

		return NULL;
	}

	ITableEditPtr CDatabaseImpl::allocTable(const NAME_ID & tableName)
	{
		assert(!tableName.IsEmpty());
		if (!tableName.IsEmpty())
		{
			ITableEditPtr pTableEditPtr;
			pTableEditPtr.attach(CTableEditImpl::createInstance(tableName, m_pContext));
			return pTableEditPtr;
		}
		return NULL;
	}

	void CDatabaseImpl::deleteTable(const NAME_ID& tableName)
	{
		CAutoLock autoLock(m_csLock);

		assert(!tableName.IsEmpty());
		if (tableName.IsEmpty())
			return;

		if (tableName == m_tblCache.oName)
		{
			m_tblCache.oName = INVALID_NAME_ID;
			m_tblCache.pTable = NULL;
		}
		m_mapTableEdit.erase(tableName);
	}

	void CDatabaseImpl::deleteFieldSet(const NAME_ID& name)
	{
		CAutoLock autoLock(m_csLock);

		assert(!name.IsEmpty());
		if (name.IsEmpty())
			return;

		m_mapFieldSet.erase(name);
	}

	IFieldSetPtr CDatabaseImpl::getFieldSet(const NAME_ID& name)
	{
		CAutoLock lock(m_csLock);

		assert(!name.IsEmpty());
		if (name.IsEmpty())
			return NULL;

		IFieldSetPtr pFieldSet;

		MapFieldSetEdit::const_iterator it = m_mapFieldSet.find(name);
		if (it != m_mapFieldSet.end())
		{
			pFieldSet = it->second;
		}
		else
		{
			CFieldSetImpl * pObj = CFieldSetImpl::createInstance(name, m_pContext);
			m_mapFieldSet[name] = pObj;
			pFieldSet.attach(pObj);
		}

		return pFieldSet;
	}

	DWORD CDatabaseImpl::loadDataTemplate(LPCTSTR lpszFilePath)
	{
		LogFinal(LOG::KDataCenter) << L"开始装载模板, path=" << lpszFilePath;
		return m_pContext->loadDataTemplate(lpszFilePath);

		//HANDLE hFile = CreateFile(lpszFilePath,               // file to open
		//	GENERIC_READ,          // open for reading
		//	FILE_SHARE_READ,       // share for reading
		//	NULL,                  // default security
		//	OPEN_EXISTING,         // existing file only
		//	FILE_ATTRIBUTE_NORMAL, // normal file
		//	NULL);                 // no attr. template

		//if (hFile == INVALID_HANDLE_VALUE) 
		//{ 
		//	return 0; 
		//}

		//DWORD fileSize = 0;
		//fileSize = ::GetFileSize(hFile, NULL);

		//std::string readBuf;
		//readBuf.resize(fileSize);
		//DWORD dwBytesRead;
		//if(!ReadFile(hFile, (LPVOID)readBuf.data(), readBuf.size(), &dwBytesRead, NULL))
		//{
		//	return 0;
		//}

		//::CloseHandle(hFile);

		//return  m_pContext->loadDataTemplate(readBuf.data(), readBuf.size());
	}

	DWORD CDatabaseImpl::loadDataTemplateFromRes(LPCTSTR lpszResouceName)
	{
		LogFinal(LOG::KDataCenter) << L"开始装载模板, resouce=" << lpszResouceName;

		MEMORY_BASIC_INFORMATION mi = {0};
		void *addr = _ReturnAddress();
		if (::VirtualQuery(addr, &mi, sizeof(mi)))
		{
			TCHAR szName[_MAX_PATH] = {0};
			HMODULE hModule = (HMODULE)mi.AllocationBase;
			if (GetModuleFileName(hModule, szName, _countof(szName)) != 0)
			{
				HRSRC hResInfo = ::FindResource(hModule, lpszResouceName, _T("DATATEMPLATE"));
				if (hResInfo)
				{
					const void *pBuffer = NULL;
					DWORD dwSize = NULL;
					HGLOBAL hRes = ::LoadResource(hModule, hResInfo);
					assert(hRes != NULL);
					if (hRes != NULL)
					{
						dwSize = ::SizeofResource(hModule, hResInfo);
						assert(dwSize > 0);

						pBuffer = ::LockResource(hRes);
						assert(pBuffer);
					}

					if (pBuffer)
					{
						DWORD ret = m_pContext->loadDataTemplate((const char*)pBuffer, dwSize);
						m_sigLoadDataTemplated(ret);
						return ret;
					}
					else
					{
						DWORD dwError = ::GetLastError();
						LogFinal(LOG::KDataCenter) << L"LoadResource失败, module=" << szName << L", error=" << dwError;
					}
				}
				else
				{
					DWORD dwError = ::GetLastError();
					assert(! L"FindResource没有找到，是否模板没有作为资源编译进模块");
					LogFinal(LOG::KDataCenter) << L"FindResource没有找到，是否模板没有作为资源编译进模块, module=" << szName << L", error=" << dwError;
				}
			}
			else
			{
				DWORD dwError = ::GetLastError();
				assert(! L"GetModuleFileName失败，不可能的事情!!");
				LogFinal(LOG::KDataCenter) << L"GetModuleFileName失败, addr=" << addr << L", error=" << dwError;
			}
		}
		else
		{
			DWORD dwError = ::GetLastError();
			assert(! L"VirtualQuery失败，不可能的事情!!");
			LogFinal(LOG::KDataCenter) << L"VirtualQuery失败, addr=" << addr << L", error=" << dwError;
		}
		
		m_sigLoadDataTemplated(INVALID_FILE_FLAG);
		return INVALID_FILE_FLAG;
	}

	void CDatabaseImpl::unloadDataTemplate(DWORD dwFileFlag)
	{
		CAutoLock autoLock(m_csLock);

		std::vector<NAME_ID> vecName;
		m_pContext->unloadDataTemplate(dwFileFlag, vecName);
		
		for (UINT i = 0; i < vecName.size(); i++)
		{
			m_mapTableEdit.erase(vecName[i]);

			//更新Cache
			if (m_tblCache.oName == vecName[i])
			{
				m_tblCache.oName = INVALID_NAME_ID;
				m_tblCache.pTable = NULL;
			}
		}
	}

	ITablePtr CDatabaseImpl::loadTable(const NAME_ID& name, LPCTSTR lpszTableDataPath)
	{
		ITableEditPtr spTableEdit = getTableEdit(name);
		if (spTableEdit)
		{
			CAutoLock autoLock(m_csLock);
			if (m_pContext->loadTable(spTableEdit, lpszTableDataPath))
			{
				return spTableEdit->getTable();
			}
		}
		return NULL;
	}

	BOOL CDatabaseImpl::saveTable(const NAME_ID& name, LPCTSTR lpszTableDataPath)
	{
		ITablePtr spTable = getTable(name);
		if (!spTable)
		{
			return FALSE;
		}
		CAutoLock autoLock(m_csLock);
		return m_pContext->saveTable(spTable, lpszTableDataPath);
	}


	void CDatabaseImpl::getTables( NAME_IDS &tables )
	{
		CAutoLock lock(m_csLock);

		MapTableEdit::const_iterator iter = m_mapTableEdit.begin();
		while (iter != m_mapTableEdit.end ())
		{
			tables << iter->first;

			++iter;
		}
	}

	void CDatabaseImpl::getFieldSets( NAME_IDS &fieldSets )
	{
		CAutoLock lock(m_csLock);

		IFieldSetPtr pFieldSet;

		MapFieldSetEdit::const_iterator it = m_mapFieldSet.begin();
		if (it != m_mapFieldSet.end())
		{
			fieldSets << it->first;
		}
	}
}
