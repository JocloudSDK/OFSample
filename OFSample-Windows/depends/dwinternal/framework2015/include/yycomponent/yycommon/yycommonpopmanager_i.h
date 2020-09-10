#pragma once

#include "yycomponent/yycomclsid.h"
#include "dwbase/dwcomex.h"

/**
* @brief 通过root->getServiceInterface<IYYCommonPopManager>(KSERVICECommon)来获取.
*/
DWDEFINE_INTERFACE(IYYCommonPopManager) : public IDWQComponent
{
	/**
	* @brief . 通用泡泡清理工作
	* @param .
	* @return .
	*/
	virtual void clear() = 0;
	/**
	* @brief . 得到泡泡显示的区域
	* @param .
	* @return. 
	*/
	virtual QRect workArea() = 0;
	/**
	* @brief . 向服务器上报系统推窗点击消息，版本：4.15，需要等4.15版本普及才使用此接口
	* @param .
	* @return.
	*/
	virtual void sendUrlClickNotify(const UINT32 msgid, const UINT32 uid) = 0; 
	/**
	* @brief .向服务器上报UI统计消息，需要调用的时候一般在窗口显示的时候调用，接口加入版本：5.0
	* @param .
	* @return.
	*/
	virtual void markMsgSuccess(const UINT32 msgid, const UINT32 uid) = 0; 
	/*
	signals:
	void sigPopMsgArrived( const std::wstring& clsId, const std::wstring& xml );
	*/
};