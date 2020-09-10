#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/* use for getObject() */
define_interface(IPropBagSetWrapper);


/**
  * @brief Ƶ���б���Ϣ�ӿڣ�Ƶ���ղ���أ�,ͨ��root->getServiceInterface<IYYChannelListInfo>(KSERVICECommon)����ȡ.
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
	* @brief �ܷ�һ��Ƶ�������ղ�.
	* @param sid��Ƶ��id.
	* @return ���Է���true�����򷵻�false.
	*/
	virtual bool canAddToFavor(quint32 sid) = 0;
	
	/**
	* @brief ��һ��Ƶ�������ղ�.
	* @param sid��Ƶ��id.
	* @return �ɹ�����0������-1������δ��.
	*/
	virtual int addToFavor(quint32 sid) = 0;

	/**
	* @brief ��һ��Ƶ���Ƴ��ղ�.
	* @param sid��Ƶ��id.
	* @return �ɹ�����0������-1������δ��.
	*/
	virtual int removeFromFavor(quint32 sid) = 0;
	/**
	* @brief ��ȡ�ղ��б�.
	* @return �ղ��б�.
	*/
	virtual IPropBagSetWrapperPtr favorList() = 0;


	typedef _def_boost_signals2_signal_type<void (UINT32 sid, LPCWSTR remarkName)>::type	_SIG_FAVOR_CHANNEL_REMARKNAME_CHANGED;
	virtual _SIG_FAVOR_CHANNEL_REMARKNAME_CHANGED* sigFavorChannelRemarkNameChanged() = 0;

	virtual bool canRemoveFavor(quint32 sid) = 0;
};