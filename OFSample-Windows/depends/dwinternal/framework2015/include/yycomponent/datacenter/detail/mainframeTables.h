#pragma once

#include <tchar.h>
#include "dwbase/IData.h"

namespace Data
{
	DEFINE_NAMEID(viewMyGuild);
	DEFINE_NAMEID(viewMyFavorChannel);
	DEFINE_NAMEID(viewMyRecentChannel);
	DEFINE_NAMEID(tblChannelList);

	namespace ChannelList
	{
		DEFINE_NAMEID(dwSid); // KSID
		DEFINE_NAMEID(dwSubSid); // KSubSID
		DEFINE_NAMEID(dwAliasId); // KASId
		DEFINE_NAMEID(strName); // Qt::DisplayRole
		DEFINE_NAMEID(dwGuildLogo); // KLogoIndex
		DEFINE_NAMEID(strGuildLogoUrl); // KLogoUrl
		DEFINE_NAMEID(dwUserCount); // KOnline
		DEFINE_NAMEID(bIsSettle); // KIsSettle
		DEFINE_NAMEID(dwRole); // KRoleIndex
		DEFINE_NAMEID(bIsLimit); // KLimit
		DEFINE_NAMEID(strRemark); // KRemarkName
		DEFINE_NAMEID(btJoinType); // KJoinType
		DEFINE_NAMEID(subTblActivities);
		DEFINE_NAMEID(bIsMyGuild);
		DEFINE_NAMEID(bIsMyFavorChannel);
		DEFINE_NAMEID(bIsMyRecentChannel);
		DEFINE_NAMEID(dw64LastVisit);

		namespace Activities
		{
			DEFINE_NAMEID(dwSid); // subid
			DEFINE_NAMEID(strName); // name
			DEFINE_NAMEID(dwUserCount); // users
		}
	}
}
