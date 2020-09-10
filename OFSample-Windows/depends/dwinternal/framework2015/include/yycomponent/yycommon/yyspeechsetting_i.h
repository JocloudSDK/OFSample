#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
   * @brief 卡拉ok和软件混音设置接口.
   */
DWDEFINE_INTERFACE(IYYSpeechSetting) : public IUnk
{
/**
   * @brief 设置卡拉ok播放器路径.
   */
    virtual void setKaraokePlayerPath(const QString& path ) = 0;

	/**
   * @brief 获取播放器路径.
   */
    virtual QString karaokePlayerPath() const = 0;

	/**
   * @brief 启动软件混音.
   */
    virtual void setSoftwareMixEnable(bool ) = 0;

	/**
   * @brief 是否启动软件混音.
   */
    virtual bool isSoftwareMixEnable() const = 0;
};