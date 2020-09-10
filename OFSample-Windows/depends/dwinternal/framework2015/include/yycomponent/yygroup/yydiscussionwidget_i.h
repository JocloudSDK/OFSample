#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duitextedit.h"

class DuiFrameWindow;
class DuiWidget;
class DuiTextAbstractIntellisenseInput;

DWDEFINE_INTERFACE(IYYDiscussionWidget) : public IDWQComponent
{
	/**
	* @brief ����ı�������
	* @brief
	*/
	virtual void addIntellisenseInput(DuiTextAbstractIntellisenseInput *) = 0;
	virtual void insertIntellisenseInput( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input ) = 0;

	/*
	* @ brief ��ȡ��Ϣ��ʾeditor
	*/
	virtual DuiTextEdit* displayEditor()=0;
};