#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


//����actionr��Ӱ�����ȼ����򡣵���������ͬһ��λ��ʱ��Ч
const char KPriority[] = "action_priority";
//menuActionNameָ����Ҫ�����λ��,����menuActionName��ǰ��
const char KMenuItemName[]  = "menuItemName";

namespace YYMenuConst
{
	const char KMainFrameMenu[] = "MainFrameMenu";
	const char KTrayIconMenu[] = "TrayIconMenu";
}

/**
  * @brief IYYMenuPrepare�ӿ�(Ƶ���˵�/��������½ǰ�ť�˵�/���̲˵� ).
  */
DWDEFINE_INTERFACE( IYYMenuPrepare ) : public IDWQComponent
{
/* 
signals:
    void prepareMenu( const QString& menuItemName, QMenu* menu, const QVariantList &args );

	//����DuiMenu������public����������const, ������ﲻ��const DuiMenu*
	//ע�ⲻҪ��menu�����κ��޸ĵĲ���
	void prepareMenuEx(const QString& menuName, DuiMenu* menu, const QVariantList &args);
*/

	/**
	* @brief ����ǰ�˵����Action.
	* @param action ��ӵ�action.
	*/
	virtual void addAction(QAction *action) = 0;

	/**
	 * @brief ����ǰ�˵�����Ӳ˵�Menu.
	 * @param Menu ��ӵ��Ӳ˵�.
	 */
	virtual void addMenu(QMenu *menu) = 0;
};



/*
	����Ƶ���Ĳ˵������¼��������ˣ�����Բ�ѯ����Ҫ�����λ��

	**************
	���ά����ʵʱ����Ϊûר�˸�����׼ȷ��objectName�����layoutxxxx.xml
	**************

    use getObject function, have to release the smart pointer when component onUnload
	
	* Ƶ��logMenu, objectName="channelIconMenu"
	viewSubChannelInfo
	guildManagement
	applyGuild
	<Separator/>���ָ��ߣ�
	inviteFriends(����3�������Ķ����˵�)
		inviteQQFriends
		inviteWeiboFans
		inviteYYFriends
	addToFavor
	removeFromFavor
	addDesktopShortcut
	<Separator/>
	quitChannel


	* Ƶ������������Ա�б�˵�, objectName="itemContextMenu"
	sendChatMsg
	<Separator />
	personalInfo
	guildInfo
	addFriend
	joinHisChannel
	moveToMyChannel
	<Separator />
	memberManage
		addTempVIP
		addVIP
		addMember
		addCA
		addSA
		addViceOwner
		<Separator />
		removeViceOwner
		removeSA
		removeCA
		removeMember
		removeVIP
		removeTempVIP
	<Separator />
	addMicOrderList
	setToKeyVoice
	<Separator />
	disableVoice
	enableVoice
	disableText
	enableText
	ignoreText
	unignoreText
	ignoreVoice
	unignoreVoice
	kickOutChannel
	addToBlacklist
	<Separator />
	quitGuild
	<Separator />
	appMenuGroup

	* �����б�˵�, objectName="maixuListMenu"
	notifySecondMaixu
	<Separator/>
	doublingTime
	removeFromMaixu
	<Separator/>
	clearMaixu
	<Separator/>
	moveUp
	moveDown
	move2Top
	<Separator/>
	mutiMaiInvite
	kickFromMutiMai
	push2MutiMai
	managerkickFromMutiMai
	<Separator/>
	sendChatMsg
	personalInfo
	guildInfo
	addFriend


*/