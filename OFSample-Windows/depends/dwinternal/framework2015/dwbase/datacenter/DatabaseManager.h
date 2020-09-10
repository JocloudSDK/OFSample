#pragma once

#include <map>
#include <vector>
#include "IDatabase.h"

namespace Data 
{
	class CDatabaseManager
	{
	private:
		CDatabaseManager();
		~CDatabaseManager();

	public:
		static CDatabaseManager* GetInstance();

		IDatabasePtr	createDatabase(DWORD dwId);
		IDatabasePtr	queryDatabase(DWORD dwId);
		void			bindAnotherDatabaseId(DWORD dwId, DWORD dwAnotherId);
		void			dropDatabase(DWORD dwId);
		
	public:
		xsignals::signal<void (DWORD)> m_sigDatabaseCreated;

	private:
		//����������-1��ʾû���ҵ�
		DWORD			findItem(DWORD dwId);

		struct DATABASE
		{
			//forƵ�����̺ţ�������id
			DWORD			dwId_1;
			DWORD			dwId_2;
			IDatabasePtr	pDatabase;
		};

		DATABASE				m_queryCache;
		std::vector<DATABASE>	m_vecDatabase;
		CRITICAL_SECTION		m_cri;
	};

};