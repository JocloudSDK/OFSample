#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief 游戏信息接口.
   */
DWDEFINE_INTERFACE(IYYGameInfo): public IUnk
{
/**
   * @brief 获取游戏名字.
   * @param strGameID 游戏id.
   * @return 游戏名字.
   */
	virtual QString getGameName(const QString& strGameID) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void filterStringWithLegacyTicket(QString &str) = 0;
};