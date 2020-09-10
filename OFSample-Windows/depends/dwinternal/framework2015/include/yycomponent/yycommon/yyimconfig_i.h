#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief IM config interface.
   */	
DWDEFINE_INTERFACE( IYYIMConfig ) : public IUnk
{
/**
   * @brief ����im����.
   * @param strTagName tab�ֶ�.
   * @param defvalue Ĭ������.
   * @return ���ҵ�tab������.
   */	
	virtual QVariant findConfiger(QString strTagName, QVariant defvalue) = 0;
/**
   * @brief im��������.
   * @param . 
   * @return .
   */
	virtual void Clear() = 0;
/**
   * @brief ����im����.
   * @param strTag tab�ֶ�.
   * @param value ����.
   * @return.
   */
	virtual void setConfig(QString strTag, QVariant value) = 0;
/**
   * @brief �õ�Ĭ�ϵ�im����font.
   * @return. 
   */
	virtual QFont getDefFont() = 0;
/**
   * @brief �õ�һ���ֵĳ���.
   * @param strText ����.
   * @param font font����.
   * @return �����ֳ���.
   */
	virtual int getTextWidth(QString strText,const QFont &font) = 0;
};
