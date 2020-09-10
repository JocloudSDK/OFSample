#pragma once

#include "protocolsdk/login_i.h"
#include "protocolsdk/pipserver_i.h"
#include "protocolsdk/session_i.h"

struct IProtocolSdk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual protocolsdk::ILoginPtr loginProxy() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual protocolsdk::IPipServerPtr pipServerProxy() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual protocolsdk::ISessionPtr sessionProxy() = 0;
};

extern "C" __declspec(dllexport) IProtocolSdk *protocolSdk();
extern "C" __declspec(dllexport) void releaseSDK(IProtocolSdk *);