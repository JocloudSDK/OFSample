#pragma once

#include "ITablePub.h"
#include "IFieldSetData.h"
#include <vector>

namespace Data
{
	interface NOVTABLE ITableEdit;
	
	interface NOVTABLE IFieldSet : IUnk
	{
		virtual IFieldSetDataPtr	getData() = 0;

		//写字段
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
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;
		virtual comptr<ITableEdit>	getSubTable(const NAME_ID &name) = 0;
	
		//用于调试器下很方便的查看Row的内容
		virtual void			dump() = 0;

		virtual void			beginEdit() = 0;
		virtual void			endEdit() = 0;

		virtual const NAME_ID	getFieldSetName() = 0;
		virtual void			getCols(std::vector<COL_PROP> & cols) = 0;

		//事件信号
		virtual xsignals::signal<void(comptr<IFieldSetData>)> & getSigChanged() = 0;
	};
	typedef comptr<IFieldSet> IFieldSetPtr;
};
