#pragma once

#include "ITablePub.h"

namespace Data
{
	interface NOVTABLE ITable;
	interface NOVTABLE IView;
	interface NOVTABLE ITableEdit;

	interface NOVTABLE IRow : IUnk
	{
		//���ֶ�
		virtual BOOL			hasData(const NAME_ID &name) = 0; //�������������ֶΣ������ֶ�Ӧ��getVirtualFieldȻ���ж����ص�DWVariant�Ƿ�Ϊ��
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;//�������lpdwSize��ΪNULL���������ݿ�ĳ���
		virtual comptr<ITable>	getSubTable(const NAME_ID &name) = 0;
		virtual BOOL			isModified(const NAME_ID &name) = 0; //���������ݸı��¼�ʱ�ж��ֶ��Ƿ��޸Ĺ�, �������������ֶ�
		virtual BOOL			isMatch(const CFilter *pFilter) = 0;
	
		//���ڵ������ºܷ���Ĳ鿴Row������
		virtual void			dump() = 0;

		//�������ֶ�	
		virtual const DWVariant	getVirtualField(const NAME_ID &name) = 0;

		//������
		virtual BOOL			hasProp(const NAME_ID &name) = 0;
		virtual LPCWSTR			getPropStr(const NAME_ID &name) = 0;
		virtual DWORD64			getPropDword64(const NAME_ID &name) = 0;
		virtual DWORD			getPropDword(const NAME_ID &name) = 0;
		virtual WORD			getPropWord(const NAME_ID &name) = 0;
		virtual BYTE			getPropByte(const NAME_ID &name) = 0;
		virtual BOOL			getPropBool(const NAME_ID &name) = 0;
		virtual comptr<IUnk>	getPropUnk(const NAME_ID &name) = 0;

		//д����
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

	//�м���
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

	//��ӿ�
	interface NOVTABLE ITable : IUnk
	{
		virtual IRowPtr				row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;
		
		//������Ϊ�գ����ذ��������ݵ�Array������NULL��
		virtual IRowArrayPtr		all() = 0;
		
		//���������ǿ�ѡ�ģ�pFilter==NULL��ʾֻ����CFilterOption==NULL��ʾֻ��Ҫ���ˣ����ͬʱָ�������ȹ��ˣ�������
		//��������Ϊ�գ���������Ϊ�յ�Array�����Ƿ���NULL��
		//select���Լ�ƫ��ɸѡ����Ҫ��Ч�ܶ�, �뾡��ʹ��
		virtual IRowArrayPtr		select(const CFilter *pFilter, const CFilterOption * pFilterOption) = 0;
				
		virtual comptr<ITableEdit>	getEdit() = 0;

		virtual comptr<IView>		getView(const NAME_ID& viewName) = 0;

		virtual const NAME_ID		getTableName() = 0;

		virtual void				dump() = 0;

		//�¼�
		//!!��ֹUI��������һ���ź�
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() = 0;
		
		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() = 0;
	};
	typedef comptr<ITable> ITablePtr;
};
