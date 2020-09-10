#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiTextEdit;

/**
* @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYFontInfoConfig) : public IUnk
{
	/**
	* @brief . ����������������Ϣ�����ļ�
	* @param .
	* @return.
	*/
	virtual void loadFontConfig( ) = 0;
	/**
	* @brief . ����������������Ϣ����
	* @param .
	* @return.
	*/
	virtual void saveFontConfig( ) = 0;

	/**
	* @brief . ����������DuiTextEdit����DuiTextEdit�����font��������ɫ����Ϣ�����������ļ�����
	* @param . editor ���ý�ȥ��DuiTextEdit
	* @return.
	*/
	virtual void setEditorFont( DuiTextEdit *editor ) = 0;

	/**
	* @brief . ��������font
	* @param . font ����font
	* @return.
	*/
	virtual void setFont( const QFont &font ) = 0;
	/**
	* @brief . �����ֵ���ɫ
	* @param . color ������ɫ
	* @return.
	*/
	virtual void setFontColor( const QColor &color ) = 0;

	/**
	* @brief . ��������font
	* @param .
	* @return. ����font
	*/
	virtual const QFont& font() const = 0;

	/**
	* @brief . �����������ɫ
	* @param .
	* @return. �������ɫ
	*/
	virtual const QColor& fontColor() const = 0;
};