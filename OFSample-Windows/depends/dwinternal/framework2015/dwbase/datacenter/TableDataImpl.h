#pragma once

#include <vector>
#include "ITablePub.h"
#include "ITableEdit.h"
#include "StringTable.h"
#include "EventCollect.h"
#include "CriticalSection.h"
#include "DatabaseContextImpl.h"

namespace Data
{	
	interface NOVTABLE IRow;
	class CRowImpl;
	class CViewDataImpl;

	class CTableDataImpl : public IUnk
	{
	public:
		struct COL
		{
			DWORD dwId;
			WORD  wOffsetBit;
			WORD  wType;
		};

		struct REAL_COLS
		{
			COL *			arCol;
			DWORD			dwColCount;
			DWORD			dwValuesSize;
			DWORD			dwExistBitSize;
			DWORD			dwModifiedBitSize;
		};

		struct VIRTUAL_COLS
		{
			BYTE * 			arType;
			WORD * 			arId;
			DWORD			dwColCount;
		};

		static CTableDataImpl * createInstance(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef *		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		void					beginEditMode();
		void					endEditMode();

		BOOL					init(const TABLE_STRUCT *pTableStruct);
		
		void					getCols(std::vector<COL_PROP> & cols);	
		CDatabaseContextImpl *	getDatabaseContext() { return m_pDatabaseContext; }
		const NAME_ID			getTableName() const { return m_tableName; }

		inline const NAME_ID	getIndexColName() const { return m_indexColName; }
		inline E_FIELD			getIndexColType() const { return m_eIndexColType; }
		inline DWORD			getIndexColIndex() const { return m_dwIndexColIndex; }
		inline DWORD			getIndexColOffsetBit() const { return m_dwIndexColOffsetBit; }

		BOOL					findCol(const NAME_ID &col, DWORD *lpIndex, DWORD *lpOffsetBit, E_FIELD *lpType);
		BOOL					findVirtualCol(const NAME_ID &col, DWORD *lpIndex, E_FIELD *lpType);
		
		inline void				enterEditCri() { ::EnterCriticalSection(&m_criEdit); }
		inline void				leaveEditCri() { ::LeaveCriticalSection(&m_criEdit); }
		inline CRITICAL_SECTION * getEditCri() { return &m_criEdit; }
		BOOL					verifyEditEnable();
		BOOL					verifyEditIsLocked() const;

		inline const REAL_COLS * getRealCols() const { return &m_realCols; }
		inline UINT				getRealColCount() const { return m_realCols.dwColCount; }

		CCriticalLock&			getPropEditLock() { return m_csPropEdit; }
	
		//�����������Table�༭�������صĶ��󲻴����ü���
		CViewDataImpl *			queryView(const NAME_ID& viewName);
		CViewDataImpl *			createView(const NAME_ID& viewName);

		inline BOOL				isStructInitialized() const { return m_bStructInitialized; }
		inline BOOL				isCurrentEditRow(const CRowImpl *pRow) const { return pRow == m_pRowLastEdited; }

		//�¼�֪ͨ
		void					notifyRowAdd(CRowImpl *pRow);
		void					notifyRowDel(CRowImpl *pRow);

		inline void				notifyRowEditActionBegin(CRowImpl *pRowBackup, CRowImpl *pRowCur) { assert(m_pRowBackup == NULL && m_pRowLastEdited == NULL); m_pRowBackup = pRowBackup; m_pRowLastEdited = pRowCur; }
		void					notifyRowEditActionEnd();
		void					notifyTableReset(const std::vector<CRowImpl *> & vecRow);

		//�����ֶμ���֪ͨ
		BOOL					notifyCalcField(IRow *pRow, const NAME_ID &name, DWVariant *pVar);


		//�¼�����
		xsignals::signal<void (IRowArrayPtr)> * getSigAddList() { return m_eventCollect.getSigAddList(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigDelList() { return m_eventCollect.getSigDelList(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigChangedList() { return m_eventCollect.getSigChangedList(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getSigReset() { return m_eventCollect.getSigReset(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigAddList()  { return m_eventCollect.getQueuedSigAddList(&m_criEdit); } 
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigDelList() { return m_eventCollect.getQueuedSigDelList(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigChangedList() { return m_eventCollect.getQueuedSigChangedList(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigReset() { return m_eventCollect.getQueuedSigReset(&m_criEdit); }
	
		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getSigChangedListEx() { return m_eventCollect.getSigChangedListEx(&m_criEdit); }
		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getQueuedSigChangedListEx() { return m_eventCollect.getQueuedSigChangedListEx(&m_criEdit); }

		void						setVirtualFieldHandler(IVirtualFieldHandler *pHandler);
		void						removeVirtualFieldHandler(IVirtualFieldHandler *pHandler);
	
	private:
		CTableDataImpl(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext);
		~CTableDataImpl(void);

		//ʵ��������
		inline static UINT getColWeight(const COL_PROP & col)
		{
			switch (col.eColType)
			{
			case FIELD_E_DWORD64:	return 1;
			case FIELD_E_DWORD:		return 2;
			case FIELD_E_WORD:		return 3;
			case FIELD_E_BYTE:		return 4;
			case FIELD_E_BOOL:		return 5;
			case FIELD_E_STR:		return 6;
			case FIELD_E_BLOB:		return 7;
			case FIELD_E_TABLE:		return 8;
			default:				assert(! L"��֧�ֵ��ֶ�����");
			}
			return -1;
		}

		inline static bool	colComp(const COL_PROP & left, const COL_PROP & right) { return getColWeight(left) < getColWeight(right); }

		//pRowOrig����������Row�ı��¼���Ч
		void				rowEvent(CRowImpl *pRowCur, E_ROW_EVENT eRowEvent, CRowImpl * pRowOrig);

		DWORD				m_dwRef;
		BOOL				m_bStructInitialized;
		CRITICAL_SECTION	m_criEdit;
		CCriticalLock		m_csPropEdit;	//���ڱ༭����ʱͬ��,���ű�ʹ��һ��

		CDatabaseContextImpl * m_pDatabaseContext;	//���������ü���
		NAME_ID				m_tableName;

		//������
		NAME_ID				m_indexColName;
		DWORD				m_dwIndexColIndex;
		DWORD				m_dwIndexColOffsetBit;
		E_FIELD				m_eIndexColType;
		
		DWORD				m_dwFindColCacheIndex; //��Ҫ���������ϲ����������
		
		REAL_COLS			m_realCols;
		VIRTUAL_COLS		m_virtualCols;
		
		std::set<comptr<IVirtualFieldHandler> > m_setVirtualFieldCB;
		CRITICAL_SECTION	m_criVirtualFieldCB;	

		CEventCollect		m_eventCollect;	//�����¼��ռ�

		//���������޸��¼��ϲ�
		CRowImpl *			m_pRowBackup;//���������ü�������Ҫ����internalrelease�ͷ�
		CRowImpl*			m_pRowLastEdited; //û�б�������

		//��ͼ
		std::vector<CViewDataImpl *> m_vecViewData;
	};
};
