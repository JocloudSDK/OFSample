#pragma once

#include "ITablePub.h"

namespace Data
{
	interface NOVTABLE IFieldSet;
	interface NOVTABLE ITable;

	interface NOVTABLE IFieldSetData : IUnk
	{
		//���ֶ�
		virtual BOOL			hasData(const NAME_ID &name) = 0; //�������������ֶΣ������ֶ�Ӧ��getVirtualFieldȻ���ж����ص�DWVariant�Ƿ�Ϊ��
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;
		virtual comptr<ITable>	getSubTable(const NAME_ID &name) = 0;
		virtual BOOL			isModified(const NAME_ID &name) = 0; //���������ݸı��¼�ʱ�ж��ֶ��Ƿ��޸Ĺ�, �������������ֶ�
	
		//���ڵ������ºܷ���Ĳ鿴����
		virtual void			dump() = 0;

		virtual comptr<IFieldSet>	getEdit() = 0;
		virtual const NAME_ID	getFieldSetName() = 0;
	};
	typedef comptr<IFieldSetData> IFieldSetDataPtr;
};
