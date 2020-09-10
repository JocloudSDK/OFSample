#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


//设置actionr，影响优先级排序。当多个组件插同一个位置时生效
const char KPriority[] = "action_priority";
//menuActionName指定你要插入的位置,插在menuActionName的前面
const char KMenuItemName[]  = "menuItemName";

namespace YYMenuConst
{
	const char KMainFrameMenu[] = "MainFrameMenu";
	const char KTrayIconMenu[] = "TrayIconMenu";
}

/**
  * @brief IYYMenuPrepare接口(频道菜单/主面板右下角按钮菜单/托盘菜单 ).
  */
DWDEFINE_INTERFACE( IYYMenuPrepare ) : public IDWQComponent
{
/* 
signals:
    void prepareMenu( const QString& menuItemName, QMenu* menu, const QVariantList &args );

	//由于DuiMenu的所有public方法都不是const, 因此这里不能const DuiMenu*
	//注意不要对menu进行任何修改的操作
	void prepareMenuEx(const QString& menuName, DuiMenu* menu, const QVariantList &args);
*/

	/**
	* @brief 往当前菜单添加Action.
	* @param action 添加的action.
	*/
	virtual void addAction(QAction *action) = 0;

	/**
	 * @brief 往当前菜单添加子菜单Menu.
	 * @param Menu 添加的子菜单.
	 */
	virtual void addMenu(QMenu *menu) = 0;
};



/*
	现在频道的菜单有以下几个开放了，表可以查询你需要插入的位置

	**************
	这表维护不实时，因为没专人跟进，准确的objectName请查找layoutxxxx.xml
	**************

    use getObject function, have to release the smart pointer when component onUnload
	
	* 频道logMenu, objectName="channelIconMenu"
	viewSubChannelInfo
	guildManagement
	applyGuild
	<Separator/>（分割线）
	inviteFriends(下面3个是他的二级菜单)
		inviteQQFriends
		inviteWeiboFans
		inviteYYFriends
	addToFavor
	removeFromFavor
	addDesktopShortcut
	<Separator/>
	quitChannel


	* 频道树、公屏人员列表菜单, objectName="itemContextMenu"
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

	* 麦序列表菜单, objectName="maixuListMenu"
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