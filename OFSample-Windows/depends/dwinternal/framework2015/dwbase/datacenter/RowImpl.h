#pragma once

#include "ITable.h"
#include "Bitmap.h"
#include "DataCenterPub.h"
#include "PropData.h"
#include "FieldString.h"
#include "TableDataImpl.h"

namespace Data
{
	class CRowImpl : public IRow
	{
	public:
		enum E_ROW_STATE
		{
			ROW_STATE_E_ALLOC,	//新分配
			ROW_STATE_E_PENGING,//已经插入但可能尚未结束据编辑，表示处于未完全明确的状态
			ROW_STATE_E_INTABLE,//已经插入表中
			ROW_STATE_E_REMOVE,	//已经从表中移除
		};

		static CRowImpl * createInstance(CTableDataImpl *pTableData, BOOL bByClone = FALSE);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef *		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		//读字段
		virtual BOOL			hasData(const NAME_ID &name);
		virtual LPCWSTR			getStr(const NAME_ID &name);
		virtual DWORD64			getDword64(const NAME_ID &name);
		virtual DWORD			getDword(const NAME_ID &name);
		virtual WORD			getWord(const NAME_ID &name);
		virtual BYTE			getByte(const NAME_ID &name);
		virtual BOOL			getBool(const NAME_ID &name);
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize);
		virtual comptr<ITable>	getSubTable(const NAME_ID &name);
		virtual BOOL			isModified(const NAME_ID &name);
		virtual BOOL			isMatch(const CFilter *pFilter);
		virtual BOOL			getModifyNameIds(NAME_IDS &nameIds);

		virtual void			dump();//用于调试器下很方便的查看Row的内容

		//读虚拟字段	
		virtual const DWVariant	getVirtualField(const NAME_ID &name);

		//读属性
		virtual BOOL			hasProp(const NAME_ID &name);
		virtual LPCWSTR			getPropStr(const NAME_ID &name);
		virtual DWORD64			getPropDword64(const NAME_ID &name);
		virtual DWORD			getPropDword(const NAME_ID &name);
		virtual WORD			getPropWord(const NAME_ID &name);
		virtual BYTE			getPropByte(const NAME_ID &name);
		virtual BOOL			getPropBool(const NAME_ID &name);
		virtual comptr<IUnk>	getPropUnk(const NAME_ID &name);

		//写属性
		virtual BOOL			setPropStr(const NAME_ID &name, LPCWSTR lpwszValue);
		virtual BOOL			setPropDword64(const NAME_ID &name, DWORD64 dw64Value);
		virtual BOOL			setPropDword(const NAME_ID &name, DWORD dwValue);
		virtual BOOL			setPropWord(const NAME_ID &name, WORD wValue);
		virtual BOOL			setPropByte(const NAME_ID &name, BYTE byValue);
		virtual BOOL			setPropBool(const NAME_ID &name, BOOL bValue);
		virtual BOOL			setPropUnk(const NAME_ID &name, IUnk *pUnk);
		virtual BOOL			removeProp(const NAME_ID &name);


		//写字段，给CRowEditImpl用
		BOOL					setStr(const NAME_ID &name, LPCWSTR lpwszValue);
		BOOL					setDword64(const NAME_ID &name, DWORD64 dw64Value);
		BOOL					setDword(const NAME_ID &name, DWORD dwValue);
		BOOL					setWord(const NAME_ID &name, WORD wValue);
		BOOL					setByte(const NAME_ID &name, BYTE byValue);
		BOOL					setBool(const NAME_ID &name, BOOL bValue);
		BOOL					setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize);
		BOOL					setSubTable(const NAME_ID &name, ITableEdit *pSubTable);
		BOOL					removeData(const NAME_ID &name);
		
		CRowImpl *				clone() const; //不会复制修改状态
		BOOL					getKey(KEY *pKey) const;
		BOOL					setKey(const KEY & key);

		inline DWORD			getRefCount() const { return m_dwRef; }//给CRowEditImpl用
		inline BOOL				isModified() const { const CTableDataImpl::REAL_COLS *pCols = m_pTableData->getRealCols(); return !Bitmap::isAllZero(getModifiedBitPtr(pCols), pCols->dwModifiedBitSize); }
		inline const void *		getModifiedBit() const { return getModifiedBitPtr(m_pTableData->getRealCols()); }

		inline BOOL				isMatch(const MATCH_NODE *pMatchNode) const { const CTableDataImpl::REAL_COLS *pCols = m_pTableData->getRealCols(); return isMatchHelper(pMatchNode, getValuePtr(), getExistBitPtr(pCols), getModifiedBitPtr(pCols)); }
		BOOL					isLess(const CRowImpl *pRowDst, const ORDER_CASE *pOrderCases, UINT nCount) const;
		
		void					getContentStr(std::wstring &wstrOut);

		inline CTableDataImpl *	getTableData() const { return m_pTableData; }

		void					setRowState(E_ROW_STATE eRowState);
		inline E_ROW_STATE		getRowState() const { return (E_ROW_STATE)m_cRowState; }

		template<class T>
		static BOOL	__fastcall	matchEqualSimple(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(T) == 0); return dwMatchDum == *(T *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchEqualDword64(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(DWORD64) == 0); return *(DWORD64 *)dwMatchDum == *(DWORD64 *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchEqualStr(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)			{ assert(dwOffsetBit % sizeof(void *) == 0); return wcscmp((LPCWSTR)dwMatchDum, (*(const CFieldStringW **)((BYTE *)pValue + (dwOffsetBit >> 3)))->getStr()) == 0; }
		static BOOL	__fastcall	matchEqualBool(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)			{ return (BOOL)dwMatchDum == Bitmap::getBit(pValue, dwOffsetBit); }

		template<class T>
		static BOOL	__fastcall	matchNotEqualSimple(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)	{ assert(dwOffsetBit % sizeof(T) == 0); return dwMatchDum != *(T *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchNotEqualDword64(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)	{ assert(dwOffsetBit % sizeof(DWORD64) == 0); return *(DWORD64 *)dwMatchDum != *(DWORD64 *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchNotEqualStr(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(void *) == 0); return wcscmp((LPCWSTR)dwMatchDum, (*(const CFieldStringW **)((BYTE *)pValue + (dwOffsetBit >> 3)))->getStr()) != 0; }
		static BOOL	__fastcall	matchNotEqualBool(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ return (BOOL)dwMatchDum != Bitmap::getBit(pValue, dwOffsetBit); }
		
		template<class T>
		static BOOL	__fastcall	matchGreaterSimple(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(T) == 0); return dwMatchDum < *(T *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchGreaterDword64(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)	{ assert(dwOffsetBit % sizeof(DWORD64) == 0); return *(DWORD64 *)dwMatchDum < *(DWORD64 *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchGreaterStr(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(void *) == 0); return wcscmp((LPCWSTR)dwMatchDum, (*(const CFieldStringW **)((BYTE *)pValue + (dwOffsetBit >> 3)))->getStr()) < 0; }
		static BOOL	__fastcall	matchGreaterBool(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ return dwMatchDum < (DWORD)Bitmap::getBit(pValue, dwOffsetBit); }
		
		template<class T>
		static BOOL	__fastcall	matchLessSimple(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(T) == 0); return dwMatchDum > *(T *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchLessDword64(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)		{ assert(dwOffsetBit % sizeof(DWORD64) == 0); return *(DWORD64 *)dwMatchDum > *(DWORD64 *)((BYTE *)pValue + (dwOffsetBit >> 3)); }
		static BOOL	__fastcall	matchLessStr(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)			{ assert(dwOffsetBit % sizeof(void *) == 0); return wcscmp((LPCWSTR)dwMatchDum, (*(const CFieldStringW **)((BYTE *)pValue + (dwOffsetBit >> 3)))->getStr()) > 0; }
		static BOOL	__fastcall	matchLessBool(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit)			{ return dwMatchDum > (DWORD)Bitmap::getBit(pValue, dwOffsetBit); }
		

	private:
		CRowImpl(CTableDataImpl *pTableData, BOOL bByClone);
		~CRowImpl(void);
		
		inline void				setExistBit(DWORD dwIndex, BOOL bSet) { return Bitmap::setBit(getExistBitPtr(m_pTableData->getRealCols()), dwIndex, bSet); }
		inline BOOL				getExistBit(DWORD dwIndex) const { return Bitmap::getBit(getExistBitPtr(m_pTableData->getRealCols()), dwIndex); }
	
		inline void				setModifiedBit(DWORD dwIndex, BOOL bSet) { return Bitmap::setBit(getModifiedBitPtr(m_pTableData->getRealCols()), dwIndex, bSet); }
		inline BOOL				getModifiedBit(DWORD dwIndex) const { return Bitmap::getBit(getModifiedBitPtr(m_pTableData->getRealCols()), dwIndex); }

		inline void *			getValuePtr() const { return (void *)(this + 1); }
		inline void *			getExistBitPtr(const CTableDataImpl::REAL_COLS *pCols) const { return (BYTE *)getValuePtr() + pCols->dwValuesSize; }
		inline void *			getModifiedBitPtr(const CTableDataImpl::REAL_COLS *pCols) const { return (BYTE *)getExistBitPtr(pCols) + pCols->dwExistBitSize; }
	
		static BOOL				isMatchHelper(const MATCH_NODE *pMatchNode, const void *pValue, const void *pExistBit, const void *pModifiedBit);
		static BOOL				isMatchCaseItem(const MATCH_CASE & caseItem, const void *pValue, const void *pExistBit, const void *pModifiedBit);

		BOOL					findColWithVerify(const NAME_ID &name, E_FIELD eType, DWORD *lpIndex, DWORD *lpOffsetBit) const;
		void					finishSetXXX(BOOL bValueChanged, DWORD dwIndex);

		template<class T> 
		inline T getXXXDirect(DWORD dwOffsetBit) const { assert(dwOffsetBit % sizeof(T) == 0); return  *(T *)((BYTE *)getValuePtr() + (dwOffsetBit >> 3)); }
		
		template<class T> 
		inline T * getXXXDirectPtr(DWORD dwOffsetBit) const { assert(dwOffsetBit % sizeof(T) == 0); return  (T *)((BYTE *)getValuePtr() + (dwOffsetBit >> 3)); }

		DWORD					m_dwRef;
		CPropData *				m_pPropData;		//保持了引用计数
		CTableDataImpl*			m_pTableData;		//保持了引用计数
		BYTE					m_cRowState;		//占1个字节，对齐前面的m_wPropCount
		std::vector<DWORD>		m_modifyIndexs;		//修改过的数据
	};
};
