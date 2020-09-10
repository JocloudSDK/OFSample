#pragma once

#include "ITablePub.h"

namespace Data
{
	interface NOVTABLE ITable;
	interface NOVTABLE IView;
	interface NOVTABLE ITableEdit;

	interface NOVTABLE IRow : IUnk
	{
		//读字段
		virtual BOOL			hasData(const NAME_ID &name) = 0; //不能用于虚拟字段，虚拟字段应该getVirtualField然后判定返回的DWVariant是否为空
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;//如果参数lpdwSize不为NULL，返回数据块的长度
		virtual comptr<ITable>	getSubTable(const NAME_ID &name) = 0;
		virtual BOOL			isModified(const NAME_ID &name) = 0; //用于在数据改变事件时判断字段是否被修改过, 不能用于虚拟字段
		virtual BOOL			isMatch(const CFilter *pFilter) = 0;
	
		//用于调试器下很方便的查看Row的内容
		virtual void			dump() = 0;

		//读虚拟字段	
		virtual const DWVariant	getVirtualField(const NAME_ID &name) = 0;

		//读属性
		virtual BOOL			hasProp(const NAME_ID &name) = 0;
		virtual LPCWSTR			getPropStr(const NAME_ID &name) = 0;
		virtual DWORD64			getPropDword64(const NAME_ID &name) = 0;
		virtual DWORD			getPropDword(const NAME_ID &name) = 0;
		virtual WORD			getPropWord(const NAME_ID &name) = 0;
		virtual BYTE			getPropByte(const NAME_ID &name) = 0;
		virtual BOOL			getPropBool(const NAME_ID &name) = 0;
		virtual comptr<IUnk>	getPropUnk(const NAME_ID &name) = 0;

		//写属性
		virtual BOOL			setPropStr(const NAME_ID &name, LPCWSTR lpwszValue) = 0;
		virtual BOOL			setPropDword64(const NAME_ID &name, DWORD64 dw64Value) = 0;
		virtual BOOL			setPropDword(const NAME_ID &name, DWORD dwValue) = 0;
		virtual BOOL			setPropWord(const NAME_ID &name, WORD wValue) = 0;
		virtual BOOL			setPropByte(const NAME_ID &name, BYTE byValue) = 0;
		virtual BOOL			setPropBool(const NAME_ID &name, BOOL bValue) = 0;
		virtual BOOL			setPropUnk(const NAME_ID &name, IUnk *pUnk) = 0;
		virtual BOOL			removeProp(const NAME_ID &name) = 0;

		virtual BOOL			getModifyNameIds(NAME_IDS &nameIds) = 0;
	};
	typedef comptr<IRow>		IRowPtr;
	typedef __comptr_cr<IRow>	IRowPtrCR;

	//行集合
	interface NOVTABLE IRowArray : IUnk
	{
		virtual UINT			size() = 0;
		virtual IRowPtrCR		at(UINT i) = 0;
		virtual void			push_back(IRow *pRow) = 0;
		virtual IRowPtrCR		front() = 0;
		virtual IRowPtrCR		back() = 0;
		virtual void			reserve(UINT count) = 0;
		virtual void			swap(IRowArray *pArrRow) = 0;
		virtual void			clear() = 0;
		virtual BOOL			empty() = 0;
		virtual comptr<IRowArray> clone(BOOL bCloneRows) = 0;
		virtual NAME_ID			getTableName()= 0;
	};
	typedef comptr<IRowArray> IRowArrayPtr;

	//表接口
	interface NOVTABLE ITable : IUnk
	{
		virtual IRowPtr				row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;
		
		//如果结果为空，返回包含空内容的Array（不是NULL）
		virtual IRowArrayPtr		all() = 0;
		
		//两个参数是可选的，pFilter==NULL表示只排序，CFilterOption==NULL表示只需要过滤，如果同时指定则是先过滤，再排序
		//如果结果集为空，返回内容为空的Array（不是返回NULL）
		//select比自己偏历筛选性能要高效很多, 请尽量使用
		virtual IRowArrayPtr		select(const CFilter *pFilter, const CFilterOption * pFilterOption) = 0;
				
		virtual comptr<ITableEdit>	getEdit() = 0;

		virtual comptr<IView>		getView(const NAME_ID& viewName) = 0;

		virtual const NAME_ID		getTableName() = 0;

		virtual void				dump() = 0;

		//事件
		//!!禁止UI连接下面一组信号
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() = 0;
		
		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() = 0;
	};
	typedef comptr<ITable> ITablePtr;
};
