#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/* use for getObject() */
define_interface(IPropBagSetWrapper);


/**
  * @brief 频道列表信息接口（频道收藏相关）,通过root->getServiceInterface<IYYChannelListInfo>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE( IYYChannelListInfo ) : public IUnk
{
	typedef _def_boost_signals2_signal_type<void (const std::vector<IPropBagWrapperPtr> &spChannelInfos)>::type	_SIG_REFRESH_FAVOR;
/**
  * @brief get favor refresh signal.
  */
	virtual _SIG_REFRESH_FAVOR* getRefreshFavorSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL;
/**
  * @brief get remove all signal.
  */
	virtual _SIG_REMOVE_ALL* getRemoveAllSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagWrapperPtrCR spChannelInfo)>::type	_SIG_ADD_FAVOR_CHANNEL;
/**
  * @brief get add-favor signal.
  */
	virtual _SIG_ADD_FAVOR_CHANNEL* getAddFavorChannelSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagWrapperPtrCR spChannelInfo)>::type	_SIG_REMOVE_FAVOR_CHANNEL;
/**
  * @brief get remove-favor signal.
  */
	virtual _SIG_REMOVE_FAVOR_CHANNEL* getRemoveFavorChannelSignal() = 0;

	/**
	* @brief 能否将一个频道加入收藏.
	* @param sid，频道id.
	* @return 可以返回true，否则返回false.
	*/
	virtual bool canAddToFavor(quint32 sid) = 0;
	
	/**
	* @brief 将一个频道加入收藏.
	* @param sid，频道id.
	* @return 成功返回0，否则-1，其他未定.
	*/
	virtual int addToFavor(quint32 sid) = 0;

	/**
	* @brief 将一个频道移出收藏.
	* @param sid，频道id.
	* @return 成功返回0，否则-1，其他未定.
	*/
	virtual int removeFromFavor(quint32 sid) = 0;
	/**
	* @brief 获取收藏列表.
	* @return 收藏列表.
	*/
	virtual IPropBagSetWrapperPtr favorList() = 0;


	typedef _def_boost_signals2_signal_type<void (UINT32 sid, LPCWSTR remarkName)>::type	_SIG_FAVOR_CHANNEL_REMARKNAME_CHANGED;
	virtual _SIG_FAVOR_CHANNEL_REMARKNAME_CHANGED* sigFavorChannelRemarkNameChanged() = 0;

	virtual bool canRemoveFavor(quint32 sid) = 0;
};