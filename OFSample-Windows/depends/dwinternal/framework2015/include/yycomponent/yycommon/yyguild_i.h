#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/propbagsetwapper_i.h"



enum JOIN_GUILD_RES_CODE //protocol::uinfo::ApplyResult
{
	JOIN_GUILD_RES_OK,
	JOIN_GUILD_RES_NOT_ENOUGH_JIFEN,
	JOIN_GUILD_RES_IN_QUIT_COOLDOWN,
	JOIN_GUILD_RES_EXCEED_MAX_JOIN_COUNT,
	JOIN_GUILD_RES_ALREADY_IS_MEMBER,
	JOIN_GUILD_RES_WAIT_FOR_APPROVAL,
};

enum INVITE_CODE_APPLY_RES_CODE //protocol::uinfo::InviteCodeApplyResult
{
	CODE_APPLY_SUCCESS,
	CODE_OUT_OF_DATE, //ÑûÇëÂë¹ýÆÚ
	CODE_HAVE_USED,   //ÑûÇëÂëÒÑ¾­Ê¹ÓÃ
	CODE_HANDIN_FALSE,//ÑûÇëÂëÌá½»Ê§°Ü
	CODE_ERROR		  //ÑûÇëÂë´íÎó
};

DWDEFINE_INTERFACE(IYYGuild) : IUnk
{
	//my guilds
	virtual IPropBagSetWrapperPtr getMyJoinedGuildList() = 0;
	virtual void reSyncMyJoiningGuildList() = 0;
	virtual void reSyncMyRecentQuitedGuildList() = 0;

	//joining
	virtual void asyncCheckJoinGuild(UINT32 sid) = 0;
	virtual void asyncJoinGuild(UINT32 sid, LPCWSTR reason) = 0;
	virtual void asyncAbandonJoiningGuild(UINT32 sid) = 0;
	virtual void asyncInviteCodeApply(UINT32 sid, LPCWSTR inviteCode) = 0;

	//quiting
	virtual bool canQuitGuild(UINT32 sid) = 0;
	virtual void asyncQuitGuild(UINT32 sid) = 0;

	//settling
	virtual bool isSettleCooldown() = 0;
	virtual bool canSetSettledGuild(UINT32 sid) = 0;
	virtual void asyncSetSettledGuild(UINT32 sid) = 0;


	//other's guilds
	virtual IPropBagSetWrapperPtr getUserJoinedGuildListOf(UINT32 uid) = 0;

	//my joined guild list
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_MY_JOINED_GUILD_LIST_CHANGED;
	virtual _SIG_MY_JOINED_GUILD_LIST_CHANGED *getSigMyJoinedGuildListChanged() = 0;

	//my joining guilds
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL_MY_JOINING_GUILDS;
	virtual _SIG_REMOVE_ALL_MY_JOINING_GUILDS *getSigRemoveAllMyJoiningGuilds() = 0;
	
	typedef _def_boost_signals2_signal_type<void (IPropBagSetWrapperPtr spJoiningGuilds)>::type	_SIG_ADD_MY_JOINING_GUILDS;
	virtual _SIG_ADD_MY_JOINING_GUILDS *getSigAddMyJoiningGuilds() = 0;

	//my recent quited guilds
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL_MY_RECENT_QUITED_GUILDS;
	virtual _SIG_REMOVE_ALL_MY_RECENT_QUITED_GUILDS *getSigRemoveAllMyRecentQuitedGuilds() = 0;
	
	typedef _def_boost_signals2_signal_type<void (IPropBagSetWrapperPtr spRecentQuitedGuilds)>::type	_SIG_ADD_MY_RECENT_QUITED_GUILDS;
	virtual _SIG_ADD_MY_RECENT_QUITED_GUILDS *getSigAddMyRecentQuitedGuilds() = 0;

	//join
	typedef _def_boost_signals2_signal_type<void (UINT32 sid, JOIN_GUILD_RES_CODE resCode)>::type	_SIG_ON_CHECK_JOIN_GUILD_RES;
	virtual _SIG_ON_CHECK_JOIN_GUILD_RES *getSigOnCheckJoinGuildRes() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 sid, JOIN_GUILD_RES_CODE resCode)>::type	_SIG_ON_JOIN_GUILD_RES;
	virtual _SIG_ON_JOIN_GUILD_RES *getSigOnJoinGuildRes() = 0;
	
	typedef _def_boost_signals2_signal_type<void (bool fApproved, IPropBagWrapperPtr spGuildInfo)>::type	_SIG_JOINING_GUILD_ANSWER_BY_ADMIN;
	virtual _SIG_JOINING_GUILD_ANSWER_BY_ADMIN *getSigJoiningGuildAnswerByAdmin() = 0;
	
	typedef _def_boost_signals2_signal_type<void (UINT32 sid, LPCWSTR corpName, ROLE_IN_CHANNEL role, INVITE_CODE_APPLY_RES_CODE resCode)>::type	_SIG_ON_INVITE_CODE_APPLY_RES;
	virtual _SIG_ON_INVITE_CODE_APPLY_RES *getSigOnInviteCodeApplyRes() = 0;

	//settle
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_ON_SET_SETTLED_GUILD_FAILED;
	virtual _SIG_ON_SET_SETTLED_GUILD_FAILED *getSigOnSetSettledGuildFailed() = 0;
};

DWDEFINE_INTERFACE(IYYGuildList) : IUnk
{
	virtual IPropBagSetWrapperPtr getMyGuildList() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagSetWrapperPtr spChannelInfos)>::type	_SIG_MY_GUILD_LIST_CHANGED;
	virtual _SIG_MY_GUILD_LIST_CHANGED* sigMyJoinedGuildListChanged() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagSetWrapperPtr spChannelInfos)>::type _SIG_MY_GUILD_LIST_ADDED;
	virtual _SIG_MY_GUILD_LIST_ADDED* sigMyJoinedGuildListAdded() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagSetWrapperPtr spChannelInfos)>::type _SIG_MY_GUILD_LIST_REMOVED;
	virtual _SIG_MY_GUILD_LIST_REMOVED* sigMyJoinedGuildListRemoved() = 0;
};