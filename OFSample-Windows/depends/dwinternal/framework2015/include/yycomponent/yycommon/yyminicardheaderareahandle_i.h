#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yyminicardindicatorhandle_i.h"

struct IYYMiniCardHeaderAreaHandle
{
	/**
	* @brief �Ƿ���Ҫ��ʾ.
	* @param cardInfo �û���Ϣ.
	* @return.
	*/
	virtual bool isShown(IMiniCardInfo* cardInfo) = 0;

	/**
	* @brief ��ȡ��ʾ���ȼ�.
	* @return ֵ���������ʾ.
	*/
	virtual int getPriority() = 0;

	/**
	* @brief ������Ҫ��ʾ��widget.
	* @param cardInfo �û���Ϣ.
	* @return ��ʾ��widget.
	*/
	virtual DuiWidget* widget(IMiniCardInfo* cardInfo) = 0;

	/**
	* @brief ָ��˭ӵ��widget������Ȩ.
	* @return true���ӿڸ������٣���֮��������.
	*/
	virtual bool isOwner() = 0;
};