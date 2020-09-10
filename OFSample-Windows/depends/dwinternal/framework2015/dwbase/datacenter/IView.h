#pragma once

#include "ITable.h"

namespace Data
{
	//��ͼ�ӿ�
	interface NOVTABLE IView : IUnk
	{
		//���Զ�θı�Filter��ÿ���޸�Filter�ᷢ��Reset�¼�
		//�벻Ҫ����Filter�����ܻᵼ������ʹ��view�û�������������Ľ��
		//�������������ѯ����ʹ��ITable��select������
		virtual void				setFilter(const CFilter *pFilter) = 0;

		//����ָ���ļ��һ�����߶�����޸ģ�Ĭ�ϼ��ȫ�����У�
		virtual void				setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount) = 0;

		virtual IRowPtr				row(const KEY &key) = 0;
		virtual BOOL				hasRow(const KEY &key) = 0;
		
		//������Ϊ�գ����ذ��������ݵ�Array������NULL��
		virtual IRowArrayPtr		all() = 0;
						
		virtual const NAME_ID		getViewName() = 0;
		virtual ITablePtr			getOwner() = 0;

		//�¼�
		//!!��ֹUI��������һ���ź�
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() = 0;
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() = 0;
		
		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() = 0;

		//�����ų�ĳЩ������
		virtual void				setFilterExcept(const NAME_IDS &pColNames) = 0;
	};
	typedef comptr<IView> IViewPtr;
};
