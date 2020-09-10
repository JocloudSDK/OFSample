#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duitextedit.h"
#include "duifw/duitextobject.h"
#include "duifw/duitextcursor.h"
#include "duifw/duitextabstractintellisenseinput.h"

//取im的接口
/*
	IYYImChatFrameManagerPtr frameMgr = Root->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	if (frameMgr == NULL || !frameMgr->isOpenChatWnd(uid))
	{
		return;
	}
	IYYImChatFramePtr chatFrame = frameMgr->findImFrame( uid );
	if (chatFrame == NULL)
	{
		return;
	}

	IYYImChatTextEditPtr textBoxPtr;
	chatFrame->queryInterface( textBoxPtr );
*/

DWDEFINE_INTERFACE(IYYImChatTextEdit) : public IDWQComponent
{
	/**
	* @brief . 
	* @param .
	* @return.
	*/
	virtual DuiTextEdit* textEdit() = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual int registerHandler( DuiTextAbstractIntellisenseInput* intellisenseInput, DuiTextObject* textObject, QString& descInfo ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual int getUniqueIntellisenseId() = 0;
};