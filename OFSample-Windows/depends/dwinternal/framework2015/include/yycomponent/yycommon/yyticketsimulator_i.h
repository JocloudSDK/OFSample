#pragma once

#include <string>
#include "dwbase/dwcomex.h"

class QString;
/**
   * @brief .
   */
DWDEFINE_INTERFACE(IYYTicketSimulator):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicket(const QString& alias, const QString &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicket(const std::wstring &alias, const std::wstring &data) = 0;

	/**
	* @brief 如果组件还没有下载下来，会缓存data信息，在下载完成后，自动再次发送.
	* @param .
	* @return -1:发送的组件是不存在的，发送失败；0：发送成功，1：发送的组件还在下载中.
	*/
	virtual int emitTicketEx(const QString& alias, const QString &data) = 0;
	/**
	* @brief 如果组件还没有下载下来，会缓存data信息，在下载完成后，自动再次发送.
	* @param .
	* @return -1:发送的组件是不存在的，发送失败；0：发送成功，1：发送的组件还在下载中.
	*/
	virtual int emitTicketEx2(const std::wstring &alias, const std::wstring &data) = 0;

	/**
	* @brief 添加指定组件飞机票是否转发给yy进程，仅适用于频道转移到独立进程的情况下
	* @param .需要转发的飞机票别名
	*/
    virtual void addRountedTicket(const QString& alias) = 0;

	/**
	* @brief 移除通过addRountedTicket添加的别名
	* @param .需要转发的飞机票别名
	*/
    virtual void removeRountedTicket(const QString& alias) = 0;    
};