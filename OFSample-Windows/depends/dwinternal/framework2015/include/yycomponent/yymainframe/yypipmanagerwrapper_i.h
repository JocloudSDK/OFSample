#pragma once

/**
  *@brief pip manager wrapper interface,通过Root->getServiceInterface<IPipManagerWrapper>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IPipManagerWrapper) : IUnk
{
/**
   * @brief finalize.
   */
	virtual void finalize() = 0;

/**
   * @brief async request open id.
   * @param buff buffer.
   * @param len length.
   * @param type type.
   */
	virtual void asyncReqOpenId(const char* buff, int len, int type) = 0;

public: // sig
	typedef _def_boost_signals2_signal_type<void (int, const char*, int)>::type	_SIG_OPENID_RES;
/**
   * @brief get open id response signal.
   * length, buffer and type.
   */
	virtual _SIG_OPENID_RES *getSigOpenIdRes() = 0;
};