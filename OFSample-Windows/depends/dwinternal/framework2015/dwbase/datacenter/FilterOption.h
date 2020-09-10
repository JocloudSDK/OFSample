#pragma once

#include "StringTable.h"

namespace Data 
{
	class CFilterOptionPrivate;
	class DWBASE_EXPORTS CFilterOption
	{
	public:
		CFilterOption();
		~CFilterOption();

		//����ָ������ֶΣ�����������
		CFilterOption & orderBy(const NAME_ID &name, BOOL bAscending = TRUE);
		
		//���������Ŀ�����������ȡ������ͷ100��Ԫ�أ�Ĭ�ϲ�������
		CFilterOption & setLimit(DWORD dwLimitCount);
		
		operator const CFilterOption *() const;

		const CFilterOptionPrivate * _private() const;
	
	private:
		CFilterOptionPrivate * m_pri;
	};
};