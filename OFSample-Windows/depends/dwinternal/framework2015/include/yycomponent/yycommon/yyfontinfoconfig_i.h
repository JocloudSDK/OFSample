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
	* @brief . 加载聊天框字体等信息配置文件
	* @param .
	* @return.
	*/
	virtual void loadFontConfig( ) = 0;
	/**
	* @brief . 保存聊天框字体等信息配置
	* @param .
	* @return.
	*/
	virtual void saveFontConfig( ) = 0;

	/**
	* @brief . 设置聊天框的DuiTextEdit，把DuiTextEdit里面的font、字体颜色等信息导出到配置文件里面
	* @param . editor 设置进去的DuiTextEdit
	* @return.
	*/
	virtual void setEditorFont( DuiTextEdit *editor ) = 0;

	/**
	* @brief . 设置字体font
	* @param . font 字体font
	* @return.
	*/
	virtual void setFont( const QFont &font ) = 0;
	/**
	* @brief . 设置字的颜色
	* @param . color 字体颜色
	* @return.
	*/
	virtual void setFontColor( const QColor &color ) = 0;

	/**
	* @brief . 返回字体font
	* @param .
	* @return. 字体font
	*/
	virtual const QFont& font() const = 0;

	/**
	* @brief . 返回字体的颜色
	* @param .
	* @return. 字体的颜色
	*/
	virtual const QColor& fontColor() const = 0;
};