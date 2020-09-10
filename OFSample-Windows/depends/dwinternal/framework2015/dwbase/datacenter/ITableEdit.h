#pragma once

#include "ITablePub.h"
#include <vector>

namespace Data
{
	interface NOVTABLE IRow;
	interface NOVTABLE ITable;
	interface NOVTABLE ITableEdit;

	//行编辑接口
	interface NOVTABLE IRowEdit : IUnk
	{
		//写字段，如果字段发生修改，返回TRUE，如果失败或者字段未改变，返回FALSE
		virtual BOOL			setStr(const NAME_ID &name, LPCWSTR lpwszValue) = 0;
		virtual BOOL			setDword64(const NAME_ID &name, DWORD64 dw64Value) = 0;
		virtual BOOL			setDword(const NAME_ID &name, DWORD dwValue) = 0;
		virtual BOOL			setWord(const NAME_ID &name, WORD wValue) = 0;
		virtual BOOL			setByte(const NAME_ID &name, BYTE byValue) = 0;
		virtual BOOL			setBool(const NAME_ID &name, BOOL bValue) = 0;
		virtual BOOL			setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize) = 0;
		virtual BOOL			setSubTable(const NAME_ID &name, ITableEdit *pSubTable) = 0;
		virtual BOOL			removeData(const NAME_ID &name) = 0;

		//读字段
		virtual BOOL			hasData(const NAME_ID &name) = 0;
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;//如果参数lpdwSize不为NULL，返回数据块的长度
		virtual comptr<ITableEdit>	getSubTable(const NAME_ID &name) = 0;
	
		//用于调试器下很方便的查看Row的内容
		virtual void			dump() = 0;

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
	};
	typedef comptr<IRowEdit> IRowEditPtr;
	typedef __comptr_cr<IRowEdit> IRowEditPtrCR;

	
	//插入结果
	struct INSERT_RES
	{
		BOOL		bInserted;	//是否真的插入了?
		IRowEditPtr pRow;		//如果指定row已经存在，返回已经存在的对象，否则返回新插入的row对象
	};
	
	//虚拟字段计算函数
	//最佳的设计是为这个IVirtualFieldHandler独立封装一个对象，生命周期交给Table。
	struct IVirtualFieldHandler : public IUnk
	{
		//如果处理了虚拟字段返回TRUE，否则返回FALSE
		virtual BOOL onCalcField(DWORD dwDatabaseId	/*in*/, 
								 const NAME_ID & tableName /*in*/, 
								 IRow *pRow /*in*/, 
								 const NAME_ID & fieldName /*in*/, 
								 DWVariant *pVarResult /*out*/) = 0;
	};

	//行集合
	interface NOVTABLE IRowEditArray : IUnk
	{
		virtual UINT			size() = 0;
		virtual IRowEditPtrCR	at(UINT i) = 0;
		virtual void			push_back(IRowEdit *pRow) = 0;
		virtual IRowEditPtrCR	front() = 0;
		virtual IRowEditPtrCR	back() = 0;
		virtual void			reserve(UINT count) = 0;
		virtual void			swap(IRowEditArray *pArrRow) = 0;
		virtual void			clear() = 0;
		virtual BOOL			empty() = 0;
	};
	typedef comptr<IRowEditArray> IRowEditArrayPtr;

	//表编辑接口
	interface NOVTABLE ITableEdit : IUnk
	{
		virtual comptr<ITable>		getTable() = 0;

		//编辑
		virtual void				beginEdit() = 0;
		virtual void				endEdit() = 0;
		virtual IRowEditPtr			row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;

		virtual IRowEditPtr			allocRow() = 0;
		virtual BOOL				insert(IRowEdit *pRowEdit) = 0;
		virtual INSERT_RES			insert(const KEY &key) = 0;
		virtual BOOL				remove(const KEY &key) = 0;
		virtual void				reset() = 0; //强制清空Table里面的所有row

		//如果结果为空，返回空内容的Array对象（不是NULL）
		virtual IRowEditArrayPtr	all() = 0;
		

		//两个参数是可选的，pFilter==NULL表示只排序，CFilterOption==NULL表示只需要过滤，如果同时指定则是先过滤，再排序
		//如果结果集为空，返回内容为空的Array（不是返回NULL）
		//select比自己偏历筛选性能要高效很多, 请尽量使用
		virtual IRowEditArrayPtr	select(const CFilter *pFilter, const CFilterOption * pFilterOption) = 0;
		
		virtual DWORD				size() = 0;	//row count


		/*虚拟字段	
			如果一个字段关联了Row之外的其它字段，则这个字段该不该做成虚拟字段呢？
			应该做成实体字段，理由：
			A. 先假设用虚拟字段实现，从应用开发复杂度来说，应用开发必须要关注跟这个字段相关联的其它多个字段，当它们发生改变，得手工调用Emit抛出改变事件。这跟用实体字段实现，手工去修改这个字段（内部自动触发事件）工作量没有什么差别。
			B. 虚拟字段没有缓存，无法知道字段是否真的改变，每次触发一个事件不划算，而实体字段修改时数据中心内部会比较，只有真的改变，才触发事件。
			C. 多线程问题，因为计算字段在UI线程callback，如果一个字段关联了row之外的数据（row内部是安全的），就要对这些数据考虑多线程安全。
		*/
		virtual void				setVirtualFieldHandler(IVirtualFieldHandler *pHandler) = 0;
		virtual void				removeVirtualFieldHandler(IVirtualFieldHandler *pHandler) = 0;
	
		//表属性
		virtual const NAME_ID		getTableName() = 0;
		virtual const NAME_ID		getIndexColName() = 0; 
		virtual E_INDEX_TYPE		getIndexType() = 0; 
		virtual void				getCols(std::vector<COL_PROP> & cols) = 0;

		virtual void				dump() = 0;
	};
	typedef comptr<ITableEdit> ITableEditPtr;
};
