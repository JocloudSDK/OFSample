#pragma once

#include "dwglobal.h"

#include "ITable.h"
#include "ITableEdit.h"
#include "IView.h"
#include "IFieldSet.h"
#include "IFieldSetData.h"
#include "IDatabase.h"
#include "IDataUtil.h"	//�������ĵ�һЩ���鹤���ຯ��
#include "SplitData.h"


namespace Data
{
	DWBASE_EXPORTS IDatabasePtr		getGlobalDatabase();

	//dwIdָ�����ݿ��id����Ҫ��֤Ψһ�������ʧ�ܷ���NULL��
	DWBASE_EXPORTS IDatabasePtr		createDatabase(DWORD dwId);
	DWBASE_EXPORTS IDatabasePtr		queryDatabase(DWORD dwId);
	DWBASE_EXPORTS void				bindAnotherDatabaseId(DWORD dwId, DWORD dwAnotherId);
	DWBASE_EXPORTS void				dropDatabase(DWORD dwId);
	DWBASE_EXPORTS xsignals::signal<void (DWORD)>&	sigDatabaseCreated();
};
