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
   * @brief . ��������塢��ɫ�����ֵ�����widget������һ����widget.
   * @param . parentWidget �������ÿ�ĸ�widget
   * @return .
   */
    virtual void attachWidget( DuiWidget *parentWidget ) = 0;
/**
   * @brief . ���������ÿ����� font �� ������ɫ
   * @param . font ����
   * @param . color ������ɫ
   * @return.
   */
    virtual void setFontInfo( const QFont& font, const QColor &color ) = 0;
/**
   * @brief . �����������ÿ�����
   * @param .
   * @return. �������ÿ�����
   */
    virtual QFont font() = 0;
/**
   * @brief . �����������ÿ�������ɫ
   * @param .
   * @return. �������ÿ�������ɫ
   */
    virtual QColor fontColor() = 0;
/*
signals:
    void fontChanged();
    void fontColorChanged();
*/
};