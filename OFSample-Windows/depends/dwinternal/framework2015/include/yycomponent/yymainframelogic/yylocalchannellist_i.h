#pragma once

#include "dwbase/dwcomex.h"
#include <dwbase/IData.h>

DWDEFINE_INTERFACE(IYYLocalChannelList) : IUnk
{
	typedef Data::IRowArrayPtr IRowArrayPtr;

	/**
	  * @brief ����ҵĹ����б�
	  * @return IRowArrayPtr IRowPtr��ʹ��YYLocalChannelListfoHelper��ȡ
	  */
	virtual IRowArrayPtr myGuilds() = 0;

	/**
	  * @brief ������ղص�Ƶ���б�
	  * @return IRowArrayPtr IRowPtr��ʹ��YYLocalChannelListfoHelper��ȡ
	  */
	virtual IRowArrayPtr myFavorChannels() = 0;

	/**
	  * @brief ����������Ƶ���б�
	  * @return IRowArrayPtr IRowPtr��ʹ��YYLocalChannelListfoHelper��ȡ
	  */
	virtual IRowArrayPtr recentChannels() = 0;

	/**
	  * @brief ���������Ƶ���б���ɾ��һ��Ƶ��
	  * @param sid Ƶ��Sid
	  * @return bool �Ƿ�ɾ���ɹ�
	  */
	virtual bool removeRecentChannel(UINT32 sid) = 0;

	/**
	  * @brief ����������Ƶ��
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
