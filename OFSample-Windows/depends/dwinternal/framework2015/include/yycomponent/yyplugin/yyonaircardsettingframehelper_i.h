#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief ������Ƭ���ýӿ�.
   */
DWDEFINE_INTERFACE(IYYOnAirCardSettingFrameHelper) : public IDWQComponent
{
/**
   * @brief ��ʾ������Ƭ���ô���.
   * @param parent parent widget.
   */
	virtual void showSettingFrame(QWidget *parent) = 0;
/**
   * @brief ��ȡ������Ƭ���ô���.
   */
	virtual QWidget *settingFrame() const = 0;

//signals:
//	void settingFrameCreated();
};
