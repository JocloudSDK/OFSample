#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duitextedit.h"

class DuiFrameWindow;
class DuiWidget;
class DuiTextAbstractIntellisenseInput;

DWDEFINE_INTERFACE(IYYDiscussionWidget) : public IDWQComponent
{
	/**
	* @brief 添加文本解析器
	* @brief
	*/
	virtual void addIntellisenseInput(DuiTextAbstractIntellisenseInput *) = 0;
	virtual void insertIntellisenseInput( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input ) = 0;

	/*
	* @ brief 获取消息显示editor
	*/
	virtual DuiTextEdit* displayEditor()=0;
};