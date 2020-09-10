#include "stable.h"
#include "RowEditImpl.h"
#include "RowImpl.h"
#include "TableEditImpl.h"
#include "DatabaseLeakDetector.h"

namespace Data
{
	const DWORD c_dwMaxDumpCount = 1000;

	CDatabaseLeakDetector::CDatabaseLeakDetector()
		: m_dw64InsertCount(0)
	{
		m_dwRef = 1;
	}

	CDatabaseLeakDetector::~CDatabaseLeakDetector(void)
	{
		assert(m_dwRef == 0);
	}

	CDatabaseLeakDetector * CDatabaseLeakDetector::createInstance()
	{
		return new CDatabaseLeakDetector();
	}

	void CDatabaseLeakDetector::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CDatabaseLeakDetector::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CDatabaseLeakDetector::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CDatabaseLeakDetector::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	void CDatabaseLeakDetector::InsertPtr(PVOID ptr, DETECTOR_E_TYPE eObjType)
	{
		CAutoLock autoLock(m_csLock);

		m_dw64InsertCount++;

		BOOL bResult = m_mapLeak.insert(std::make_pair(DWORD(ptr), eObjType)).second;
		assert(bResult);
		(void)bResult;
	}

	void CDatabaseLeakDetector::RemovePtr(PVOID ptr)
	{
		CAutoLock autoLock(m_csLock);
		
		BOOL bResult = m_mapLeak.erase((DWORD)ptr);
		assert(bResult);
		(void)bResult;
	}

	void CDatabaseLeakDetector::StartDetector()
	{
		addRef();

		DWORD dwThreadId = 0;
		HANDLE hThread = (HANDLE)::CreateThread(NULL,
						0,
						&CDatabaseLeakDetector::DetectorThreadProc,
						this,
						0,
						&dwThreadId);
		assert(hThread);
		::CloseHandle(hThread);
	}

	DWORD CDatabaseLeakDetector::DetectorThreadProc(void* thiz)
	{
		CDatabaseLeakDetector* pDetector = (CDatabaseLeakDetector*)thiz;
		
        for(int i = 0; i < 15; i++)
        {
		    Sleep(1000);
        }

		pDetector->OnDetector();
		
		pDetector->release();

		return 0;
	}
	
	void CDatabaseLeakDetector::OnDetector() const
	{
		CAutoLock autoLock(m_csLock);
				
		if (!m_mapLeak.empty())
		{
			Error::OutputDebug(_T("检测到数据中心未释放的数据%d条，使用过的对像总数：%u64"), m_mapLeak.size(), m_dw64InsertCount);
			Error::OutputDebug(_T("begin dump =========================================================================="));

			//先 Dump ITable
			for (LeakDetectorMap::const_iterator it = m_mapLeak.begin(); it != m_mapLeak.end(); ++it)
			{
				if (it->second == DETECTOR_E_TYPE_TABLEEDIT)
					((CTableEditImpl *)it->first)->dumpSummary();
			}

			//Dump IRow
			UINT i = 0;
			for (LeakDetectorMap::const_iterator it = m_mapLeak.begin(); it != m_mapLeak.end() && i < c_dwMaxDumpCount; ++it, ++i)
			{			
				if (it->second == DETECTOR_E_TYPE_ROW)
					((IRow *)it->first)->dump();
			}
			if (i == c_dwMaxDumpCount)
				Error::OutputDebug(_T("there are more..., are omitted"));

			//Dump IRowEdit
			i = 0;
			for (LeakDetectorMap::const_iterator it = m_mapLeak.begin(); it != m_mapLeak.end() && i < c_dwMaxDumpCount; ++it, ++i)
			{			
				if (it->second == DETECTOR_E_TYPE_ROWEDIT)
					((IRowEdit *)it->first)->dump();
			}

			if (i == c_dwMaxDumpCount)
				Error::OutputDebug(_T("there are more..., are omitted"));
			
			Error::OutputDebug(_T("end dump ==========================================================================\r\n"));
			Error::OutputWarning(_T("检测到数据中心未释放的数据%d条，使用过的对像总数：%u64, 可以在Output窗口中查看所有泄漏记录\n"), m_mapLeak.size(), m_dw64InsertCount );
		}
	}
}