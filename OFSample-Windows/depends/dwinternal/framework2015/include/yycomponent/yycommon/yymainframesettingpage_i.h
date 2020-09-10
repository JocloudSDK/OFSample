#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiSettingPageBase;
class DuiWidget;

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYMainFrameSettingPage) : public IDWQComponent
{
/* signals:
    void pageActivity(DuiSettingPageBase* page, UINT32 uid);
    void pageLeave(DuiSettingPageBase* page, UINT32 uid);
    bool validatePage(DuiSettingPageBase* page, UINT32 uid);
    void pageSave(DuiSettingPageBase* page,UINT32 uid, DuiSettingPageBase::SaveEvent);
    void pageRevert(DuiSettingPageBase* page, UINT32 uid);
	void settingActivity(DuiSettingFrame* frame,UINT32 uid);
*/	
};



/*
!! page name

!!blockName:
	"giftBlock"
*/

/*
void pageAction( const QString& pageName, Action );
*/

/**
   * @brief YY个人设置页面接口.
   */
class DuiSettingPageBase;
DWDEFINE_INTERFACE(IYYSettingPage): public IDWQComponent 
{
	enum Action {NoAction = 0, OKAction = 1, ApplyAction = 2};

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void insertWidget( const QString& blockName, DuiWidget* widget ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void removeWidget( const QString& blockName, DuiWidget* widget ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setDataChange( const QString& pageName ) = 0;

	/*
	 * 在pos位置插入widget,widget按顺序排列
	*/
	enum MyInfoWidgetPos
	{
		POS_MYINFO_BEHIND_YYID = 0,
		POS_SYSTEMSETTING_BEHIND_CHECKBOX = 1
	};
	virtual void insertWidgetEx( DuiWidget* widget, MyInfoWidgetPos pos) = 0;

	virtual void setDataChangeEx(MyInfoWidgetPos pos) = 0;

    virtual bool insertPage(const QString& strTabText, const QString& strIconPath, const QString& strPageName, DuiSettingPageBase* page) = 0;

/* signals:
    YY设置窗口
    void yySettingFrameCreated();
    void yySettingFrameClosing();
*/	
};