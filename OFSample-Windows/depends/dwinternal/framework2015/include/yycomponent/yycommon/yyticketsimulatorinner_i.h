#pragma once

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
#include <string>
#include "dwbase/dwcomex.h"

class QString;
/**
   * @brief .
   */
DWDEFINE_INTERFACE(IYYTicketSimulatorInner):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicketByComponentName(const QString& name, const QString &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitTicketByComponentName2(const std::wstring &name, const std::wstring &data) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int emitDataTicketByComponentName(const QString& name, const QString &data) = 0;

/**
   * @brief . 通过组件名字发飞机票；如果不存在，会下载组件再转发
   * @param .
   * @return .
   */
    virtual int emitTicketByComponentNameEx(const QString& name, const QString& data) = 0;

    /**
    * @brief . 添加指定组件飞机票是否转发给yy进程，仅适用于频道转移到独立进程的情况下
    * @param . 需要转发的组件名字
    */
    virtual void addRountedComponent(const QString& name) = 0;

    /**
    * @brief . 移除addRountedComponent添加的名字
    * @param . 需要转发的组件名字
    */
    virtual void removeRountedComponent(const QString& name) = 0;
};

#endif	//所有组件都会定义这个宏
