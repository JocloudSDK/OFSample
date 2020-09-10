#pragma once

#include "dwbase/dwcomex.h"
#include "dwbase/dwcominterface_i.h"

class DuiWidget;

DWDEFINE_INTERFACE(IOpenPlatformEntryTemplate): public IDWComponentEntry
{

};

DWDEFINE_INTERFACE(IOpenPlatformTemplate) : public IDWQComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual DuiWidget *getTemplateFrame() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int getTemplateId() = 0;
};

DWDEFINE_INTERFACE(IOpenPlatformTemplateMgr) : public IDWQComponent
{
	struct IOpenPlatformTemplateEvent
	{
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void onLoad() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void onUnLoad() = 0;
	};

	//virtual void setEvent(IOpenPlatformTemplateEvent *pEvent) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void load(int componentId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unLoad() = 0;
};