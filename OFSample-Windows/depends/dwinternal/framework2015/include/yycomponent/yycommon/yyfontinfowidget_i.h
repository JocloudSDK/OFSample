#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
  * @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYFontInfoWidget): public IDWQComponent
{
/**
   * @brief . 聊天框字体、颜色和炫字等设置widget，设置一个父widget.
   * @param . parentWidget 字体设置框的父widget
   * @return .
   */
    virtual void attachWidget( DuiWidget *parentWidget ) = 0;
/**
   * @brief . 给字体设置框设置 font 与 字体颜色
   * @param . font 字体
   * @param . color 字体颜色
   * @return.
   */
    virtual void setFontInfo( const QFont& font, const QColor &color ) = 0;
/**
   * @brief . 返回字体设置框字体
   * @param .
   * @return. 字体设置框字体
   */
    virtual QFont font() = 0;
/**
   * @brief . 返回字体设置框字体颜色
   * @param .
   * @return. 字体设置框字体颜色
   */
    virtual QColor fontColor() = 0;
/*
signals:
    void fontChanged();
    void fontColorChanged();
*/
};