#pragma once

#include "dwglobal.h"

#include "ITable.h"
#include "ITableEdit.h"
#include "IView.h"
#include "IFieldSet.h"
#include "IFieldSetData.h"
#include "IDatabase.h"
#include "IDataUtil.h"	//数据中心的一些帮组工具类函数
#include "SplitData.h"


namespace Data
{
	DWBASE_EXPORTS IDatabasePtr		getGlobalDatabase();

	//dwId指定数据库的id，需要保证唯一，否则会失败返回NULL。
	DWBASE_EXPORTS IDatabasePtr		createDatabase(DWORD dwId);
	DWBASE_EXPORTS IDatabasePtr		queryDatabase(DWORD dwId);
	DWBASE_EXPORTS void				bindAnotherDatabaseId(DWORD dwId, DWORD dwAnotherId);
	DWBASE_EXPORTS void				dropDatabase(DWORD dwId);
	DWBASE_EXPORTS xsignals::signal<void (DWORD)>&	sigDatabaseCreated();
};
