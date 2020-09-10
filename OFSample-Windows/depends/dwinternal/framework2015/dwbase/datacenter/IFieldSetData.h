#pragma once

#include "ITablePub.h"

namespace Data
{
	interface NOVTABLE IFieldSet;
	interface NOVTABLE ITable;

	interface NOVTABLE IFieldSetData : IUnk
	{
		//读字段
		virtual BOOL			hasData(const NAME_ID &name) = 0; //不能用于虚拟字段，虚拟字段应该getVirtualField然后判定返回的DWVariant是否为空
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;
		virtual comptr<ITable>	getSubTable(const NAME_ID &name) = 0;
		virtual BOOL			isModified(const NAME_ID &name) = 0; //用于在数据改变事件时判断字段是否被修改过, 不能用于虚拟字段
	
		//用于调试器下很方便的查看内容
		virtual void			dump() = 0;

		virtual comptr<IFieldSet>	getEdit() = 0;
		virtual const NAME_ID	getFieldSetName() = 0;
	};
	typedef comptr<IFieldSetData> IFieldSetDataPtr;
};
