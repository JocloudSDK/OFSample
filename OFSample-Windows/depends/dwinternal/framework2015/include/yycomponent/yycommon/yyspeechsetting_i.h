#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
   * @brief ����ok������������ýӿ�.
   */
DWDEFINE_INTERFACE(IYYSpeechSetting) : public IUnk
{
/**
   * @brief ���ÿ���ok������·��.
   */
    virtual void setKaraokePlayerPath(const QString& path ) = 0;

	/**
   * @brief ��ȡ������·��.
   */
    virtual QString karaokePlayerPath() const = 0;

	/**
   * @brief �����������.
   */
    virtual void setSoftwareMixEnable(bool ) = 0;

	/**
   * @brief �Ƿ������������.
   */
    virtual bool isSoftwareMixEnable() const = 0;
};