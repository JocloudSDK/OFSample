#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief IM config interface.
   */	
DWDEFINE_INTERFACE( IYYIMConfig ) : public IUnk
{
/**
   * @brief 查找im设置.
   * @param strTagName tab字段.
   * @param defvalue 默认内容.
   * @return 查找的tab的内容.
   */	
	virtual QVariant findConfiger(QString strTagName, QVariant defvalue) = 0;
/**
   * @brief im设置清理.
   * @param . 
   * @return .
   */
	virtual void Clear() = 0;
/**
   * @brief 设置im配置.
   * @param strTag tab字段.
   * @param value 内容.
   * @return.
   */
	virtual void setConfig(QString strTag, QVariant value) = 0;
/**
   * @brief 得到默认的im字体font.
   * @return. 
   */
	virtual QFont getDefFont() = 0;
/**
   * @brief 得到一段字的长度.
   * @param strText 内容.
   * @param font font内容.
   * @return 返回字长度.
   */
	virtual int getTextWidth(QString strText,const QFont &font) = 0;
};
