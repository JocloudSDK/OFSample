#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYBigBulletinLinkd) : public IDWQComponent
{
	struct  YYBigBulletinLinkdCallback
	{
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void onRecv(UINT32 uri, const std::string &data) = 0;
	};

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setCallback(YYBigBulletinLinkdCallback *callback) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool clearCallback() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int send(UINT32 uri, const std::string &data) = 0;
};