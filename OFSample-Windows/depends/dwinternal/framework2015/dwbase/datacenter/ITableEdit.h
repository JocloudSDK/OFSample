#pragma once

#include "ITablePub.h"
#include <vector>

namespace Data
{
	interface NOVTABLE IRow;
	interface NOVTABLE ITable;
	interface NOVTABLE ITableEdit;

	//�б༭�ӿ�
	interface NOVTABLE IRowEdit : IUnk
	{
		//д�ֶΣ�����ֶη����޸ģ�����TRUE�����ʧ�ܻ����ֶ�δ�ı䣬����FALSE
		virtual BOOL			setStr(const NAME_ID &name, LPCWSTR lpwszValue) = 0;
		virtual BOOL			setDword64(const NAME_ID &name, DWORD64 dw64Value) = 0;
		virtual BOOL			setDword(const NAME_ID &name, DWORD dwValue) = 0;
		virtual BOOL			setWord(const NAME_ID &name, WORD wValue) = 0;
		virtual BOOL			setByte(const NAME_ID &name, BYTE byValue) = 0;
		virtual BOOL			setBool(const NAME_ID &name, BOOL bValue) = 0;
		virtual BOOL			setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize) = 0;
		virtual BOOL			setSubTable(const NAME_ID &name, ITableEdit *pSubTable) = 0;
		virtual BOOL			removeData(const NAME_ID &name) = 0;

		//���ֶ�
		virtual BOOL			hasData(const NAME_ID &name) = 0;
		virtual LPCWSTR			getStr(const NAME_ID &name) = 0;
		virtual DWORD64			getDword64(const NAME_ID &name) = 0;
		virtual DWORD			getDword(const NAME_ID &name) = 0;
		virtual WORD			getWord(const NAME_ID &name) = 0;
		virtual BYTE			getByte(const NAME_ID &name) = 0;
		virtual BOOL			getBool(const NAME_ID &name) = 0;
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize) = 0;//�������lpdwSize��ΪNULL���������ݿ�ĳ���
		virtual comptr<ITableEdit>	getSubTable(const NAME_ID &name) = 0;
	
		//���ڵ������ºܷ���Ĳ鿴Row������
		virtual void			dump() = 0;

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
	};
	typedef comptr<IRowEdit> IRowEditPtr;
	typedef __comptr_cr<IRowEdit> IRowEditPtrCR;

	
	//������
	struct INSERT_RES
	{
		BOOL		bInserted;	//�Ƿ���Ĳ�����?
		IRowEditPtr pRow;		//���ָ��row�Ѿ����ڣ������Ѿ����ڵĶ��󣬷��򷵻��²����row����
	};
	
	//�����ֶμ��㺯��
	//��ѵ������Ϊ���IVirtualFieldHandler������װһ�������������ڽ���Table��
	struct IVirtualFieldHandler : public IUnk
	{
		//��������������ֶη���TRUE�����򷵻�FALSE
		virtual BOOL onCalcField(DWORD dwDatabaseId	/*in*/, 
								 const NAME_ID & tableName /*in*/, 
								 IRow *pRow /*in*/, 
								 const NAME_ID & fieldName /*in*/, 
								 DWVariant *pVarResult /*out*/) = 0;
	};

	//�м���
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

	//��༭�ӿ�
	interface NOVTABLE ITableEdit : IUnk
	{
		virtual comptr<ITable>		getTable() = 0;

		//�༭
		virtual void				beginEdit() = 0;
		virtual void				endEdit() = 0;
		virtual IRowEditPtr			row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;

		virtual IRowEditPtr			allocRow() = 0;
		virtual BOOL				insert(IRowEdit *pRowEdit) = 0;
		virtual INSERT_RES			insert(const KEY &key) = 0;
		virtual BOOL				remove(const KEY &key) = 0;
		virtual void				reset() = 0; //ǿ�����Table���������row

		//������Ϊ�գ����ؿ����ݵ�Array���󣨲���NULL��
		virtual IRowEditArrayPtr	all() = 0;
		

		//���������ǿ�ѡ�ģ�pFilter==NULL��ʾֻ����CFilterOption==NULL��ʾֻ��Ҫ���ˣ����ͬʱָ�������ȹ��ˣ�������
		//��������Ϊ�գ���������Ϊ�յ�Array�����Ƿ���NULL��
		//select���Լ�ƫ��ɸѡ����Ҫ��Ч�ܶ�, �뾡��ʹ��
		virtual IRowEditArrayPtr	select(const CFilter *pFilter, const CFilterOption * pFilterOption) = 0;
		
		virtual DWORD				size() = 0;	//row count


		/*�����ֶ�	
			���һ���ֶι�����Row֮��������ֶΣ�������ֶθò������������ֶ��أ�
			Ӧ������ʵ���ֶΣ����ɣ�
			A. �ȼ����������ֶ�ʵ�֣���Ӧ�ÿ������Ӷ���˵��Ӧ�ÿ�������Ҫ��ע������ֶ����������������ֶΣ������Ƿ����ı䣬���ֹ�����Emit�׳��ı��¼��������ʵ���ֶ�ʵ�֣��ֹ�ȥ�޸�����ֶΣ��ڲ��Զ������¼���������û��ʲô���
			B. �����ֶ�û�л��棬�޷�֪���ֶ��Ƿ���ĸı䣬ÿ�δ���һ���¼������㣬��ʵ���ֶ��޸�ʱ���������ڲ���Ƚϣ�ֻ����ĸı䣬�Ŵ����¼���
			C. ���߳����⣬��Ϊ�����ֶ���UI�߳�callback�����һ���ֶι�����row֮������ݣ�row�ڲ��ǰ�ȫ�ģ�����Ҫ����Щ���ݿ��Ƕ��̰߳�ȫ��
		*/
		virtual void				setVirtualFieldHandler(IVirtualFieldHandler *pHandler) = 0;
		virtual void				removeVirtualFieldHandler(IVirtualFieldHandler *pHandler) = 0;
	
		//������
		virtual const NAME_ID		getTableName() = 0;
		virtual const NAME_ID		getIndexColName() = 0; 
		virtual E_INDEX_TYPE		getIndexType() = 0; 
		virtual void				getCols(std::vector<COL_PROP> & cols) = 0;

		virtual void				dump() = 0;
	};
	typedef comptr<ITableEdit> ITableEditPtr;
};
