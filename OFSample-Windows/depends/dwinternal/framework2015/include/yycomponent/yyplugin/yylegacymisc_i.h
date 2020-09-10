#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief ��Ϸ��Ϣ�ӿ�.
   */
DWDEFINE_INTERFACE(IYYGameInfo): public IUnk
{
/**
   * @brief ��ȡ��Ϸ����.
   * @param strGameID ��Ϸid.
   * @return ��Ϸ����.
   */
	virtual QString getGameName(const QString& strGameID) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void filterStringWithLegacyTicket(QString &str) = 0;
};