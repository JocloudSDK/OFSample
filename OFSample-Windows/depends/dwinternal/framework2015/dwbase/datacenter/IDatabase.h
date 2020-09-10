#pragma once

#include "ITable.h"
#include "ITableEdit.h"
#include "IView.h"
#include "IFieldSet.h"

namespace Data
{
	interface NOVTABLE IDatabase : IUnk
	{
		//����DataBase��һ����������ݼ���û���򴴽�
		virtual ITablePtr			getTable(const NAME_ID& name) = 0;
		virtual ITableEditPtr		getTableEdit(const NAME_ID& name) = 0;
		
		virtual IFieldSetPtr		getFieldSet(const NAME_ID& name) = 0;

		//ɾ��һ��һ����
		virtual void				deleteTable(const NAME_ID& name) = 0;
		virtual void				deleteFieldSet(const NAME_ID& name) = 0;

		//����һ���±�һ������Ϊ������ʹ��
		virtual ITableEditPtr		allocTable(const NAME_ID & name) = 0;

		//�����ļ���ʶ��0��ʾʧ��
		virtual DWORD				loadDataTemplate(LPCTSTR lpszFilePath) = 0;
		virtual DWORD				loadDataTemplateFromRes(LPCTSTR lpszResouceName) = 0;
		virtual void				unloadDataTemplate(DWORD dwFileFlag) = 0;

		//��ĳ־û��洢
		virtual ITablePtr			loadTable(const NAME_ID& name, LPCTSTR lpszTableDataPath) = 0;
		virtual BOOL				saveTable(const NAME_ID& name, LPCTSTR lpszTableDataPath) = 0;

		//����DataBase��������һ����
		virtual void				getTables(NAME_IDS &tables) = 0;
		virtual void				getFieldSets(NAME_IDS &fieldSets) = 0;

		//���ر�ģ���¼�֪ͨ
		virtual xsignals::signal<void (DWORD)> & sigLoadDataTemplated() = 0;
	};

	typedef comptr<IDatabase> IDatabasePtr;
};
