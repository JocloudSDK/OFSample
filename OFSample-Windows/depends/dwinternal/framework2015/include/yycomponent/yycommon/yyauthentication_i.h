#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "propbagsetwapper_i.h"

struct IPropBagWrapper;

/**
  * @brief YY认证接口，通过root->getServiceInterface<IYYAuthentication>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IYYAuthentication):public IUnk
{
/**
   * @brief get hex cookie.
   * @return hex cookie.
   */
	virtual std::wstring hexCookie() const = 0;
/**
   * @brief get cookie.
   * @return get cookie.
   */
	virtual std::string cookie() const = 0;
/**
   * @brief get password.
   * @return get password.
   */
	virtual std::string password() const = 0;
/**
   * @brief get passport.
   * @return get passport.
   */	
	virtual std::wstring passport() const = 0;
/**
   * @brief get account.
   * @return get account.
   */
	virtual std::wstring account() const = 0;

/**
   * @brief 是否用email登录.
   * @return true means use email to login.
   */
	virtual	bool isEmailLogin() const = 0;
/**
   * @brief 是否用电信网络.
   */
	virtual bool isDXnet() const = 0;
/**
   * @brief get my uid.
   * @return my uid.
   */
	virtual unsigned int getMyUid() const = 0;
/**
   * @brief get WAN ip.
   * @return ip.
   */
	virtual unsigned int getWANIp() const = 0;

/**
   * @brief .
   * @return .
   */
	virtual time_t syncGetServerTime() const = 0;
/**
   * @brief .
   * @return .
   */
	virtual std::string getPasswordHC() const = 0;
/**
   * @brief get my guilds.
   * @return my guilds.
   */
	virtual IPropBagSetWrapperPtr getMyGuild() const = 0;
/**
   * @brief get client isp type.
   * @return isp type.
   */
	virtual BIZ_ISPType getIspTypeAll() const = 0;
};
