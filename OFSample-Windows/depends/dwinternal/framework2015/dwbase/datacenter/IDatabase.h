#pragma once

#include "ITable.h"
#include "ITableEdit.h"
#include "IView.h"
#include "IFieldSet.h"

namespace Data
{
	interface NOVTABLE IDatabase : IUnk
	{
		//返回DataBase里一级表或者数据集，没有则创建
		virtual ITablePtr			getTable(const NAME_ID& name) = 0;
		virtual ITableEditPtr		getTableEdit(const NAME_ID& name) = 0;
		
		virtual IFieldSetPtr		getFieldSet(const NAME_ID& name) = 0;

		//删除一张一级表
		virtual void				deleteTable(const NAME_ID& name) = 0;
		virtual void				deleteFieldSet(const NAME_ID& name) = 0;

		//分配一张新表，一般是作为二级表使用
		virtual ITableEditPtr		allocTable(const NAME_ID & name) = 0;

		//返回文件标识，0表示失败
		virtual DWORD				loadDataTemplate(LPCTSTR lpszFilePath) = 0;
		virtual DWORD				loadDataTemplateFromRes(LPCTSTR lpszResouceName) = 0;
		virtual void				unloadDataTemplate(DWORD dwFileFlag) = 0;

		//表的持久化存储
		virtual ITablePtr			loadTable(const NAME_ID& name, LPCTSTR lpszTableDataPath) = 0;
		virtual BOOL				saveTable(const NAME_ID& name, LPCTSTR lpszTableDataPath) = 0;

		//返回DataBase里面所有一级表
		virtual void				getTables(NAME_IDS &tables) = 0;
		virtual void				getFieldSets(NAME_IDS &fieldSets) = 0;

		//加载表模板事件通知
		virtual xsignals::signal<void (DWORD)> & sigLoadDataTemplated() = 0;
	};

	typedef comptr<IDatabase> IDatabasePtr;
};
