#pragma once

#include "dwbase/dwcomex.h"
#include <dwbase/IData.h>

DWDEFINE_INTERFACE(IYYLocalChannelList) : IUnk
{
	typedef Data::IRowArrayPtr IRowArrayPtr;

	/**
	  * @brief 获得我的公会列表
	  * @return IRowArrayPtr IRowPtr可使用YYLocalChannelListfoHelper读取
	  */
	virtual IRowArrayPtr myGuilds() = 0;

	/**
	  * @brief 获得我收藏的频道列表
	  * @return IRowArrayPtr IRowPtr可使用YYLocalChannelListfoHelper读取
	  */
	virtual IRowArrayPtr myFavorChannels() = 0;

	/**
	  * @brief 获得最近访问频道列表
	  * @return IRowArrayPtr IRowPtr可使用YYLocalChannelListfoHelper读取
	  */
	virtual IRowArrayPtr recentChannels() = 0;

	/**
	  * @brief 从最近访问频道列表中删除一个频道
	  * @param sid 频道Sid
	  * @return bool 是否删除成功
	  */
	virtual bool removeRecentChannel(UINT32 sid) = 0;

	/**
	  * @brief 清空最近访问频道
	  * @return void 
	  */
	virtual void clearRecentChannel() = 0;

	DWDECLARE_SIGNAL(SigMyGuildAdded, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigMyGuildDeleted, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigMyGuildChanged, void (IRowArrayPtr newRows, IRowArrayPtr oldRows));

	DWDECLARE_SIGNAL(SigMyFavorChannelAdded, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigMyFavorChannelDeleted, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigMyFavorChannelChanged, void (IRowArrayPtr newRows, IRowArrayPtr oldRows));

	DWDECLARE_SIGNAL(SigRecentChannelAdded, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigRecentChannelDeleted, void (IRowArrayPtr rows));
	DWDECLARE_SIGNAL(SigRecentChannelChanged, void (IRowArrayPtr newRows, IRowArrayPtr oldRows));
};

DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyGuildAdded);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyGuildDeleted);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyGuildChanged);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyFavorChannelAdded);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyFavorChannelDeleted);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigMyFavorChannelChanged);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigRecentChannelAdded);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigRecentChannelDeleted);
DWDEFINE_SIGNAL(IYYLocalChannelList::SigRecentChannelChanged);
