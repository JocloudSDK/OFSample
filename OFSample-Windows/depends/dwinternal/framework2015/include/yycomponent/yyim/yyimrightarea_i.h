#pragma once

#include "dwbase/dwcomex.h"

class DuiWidget;

//各业务显示优先级
/*
*客服 0XF,最高优先级
*友缘 0xFF
*频道活动 0XFFFF
*时装秀 默认0xFFF,配置不优先显示后是0xFFFF
*游戏角色 0xFFFF
使用方法：
IYYImChatFrameManagerPtr frameMgr = comRoot->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
if(NULL == frameMgr)
	return;
IYYImChatWidgetPtr chatFrame = frameMgr->findImWidget(userId);
if(NULL == chatFrame)
	return;
IYYImRightAreaPtr rightMgr;
if ( !chatFrame->queryInterface(rightMgr) )
{
	DW_ASSERT(false);
	return;
}
*/
DWDEFINE_INTERFACE(IYYImRightArea) : public IDWQComponent
{
	/*
	* 给好友展示增加一个页面,优先显示nPriority小的页面，同一数值轮播显示。普通页面用0xFFFF。
	* 8.4以上，包括8.4，不支持这个接口了.
	*/
	virtual void insertBuddyViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/*
	* 给展示自己区域增加一个页面,优先显示nPriority小的页面，同一数值轮播显示。普通页面用0xFFFF。
	* 8.4以上，包括8.4，不支持这个接口了.
	*/
	virtual void insertMyViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/*
	*通过xml创建page,如果xml中含有DuiTextEdit,应用这个方法创建。
	* strObjectName是返回对应的widget的widget.
	* 8.4以上，包括8.4，不支持这个接口了.
	*/
	virtual DuiWidget* insertBuddyViewPageEx(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;
	virtual DuiWidget* insertMyViewPageEx(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;

    /*
    * 去掉一个widget,会在接口里面删除
    */
    virtual void removeBuddyWidget(DuiWidget* page) = 0;
    virtual void removeMyWidget(DuiWidget* page) = 0;

    /*
    *  设置优先级,8.4以上，包括8.4，不支持这个接口了.
    */
    virtual void setBuddyWidgetPriority(DuiWidget* widget, UINT32 nPriority) = 0;
    virtual void setMyWidgetPriority(DuiWidget* widget, UINT32 nPriority) = 0;

    /*
    *  设置展示区域的高度,8.4以上，包括8.4，不支持这个接口了.
    *  对应两个信号
    *  signals:
    *      void myViewHeightUpdate(int newHeight, int oldHeight);
    *      void buddyViewHeightUpdate(int newHeight, int oldHeight);
    */
    virtual void setMyWidgetMinHeight(DuiWidget* widget, int minHeight) = 0;

	/**
	  * @brief 在Y秀区域上增加一个页面，版本:8.4。
	  * @param page 要增加的页面。
	  * @param nPriority 优先显示nPriority小的页面，同一数值轮播显示。普通页面用0xFFFF。
	  * @return  true表示成功，false表示失败。
	  */
	virtual bool insertPersonViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/**
	  * @brief 在Y秀区域上,通过xml创建page,如果xml中含有DuiTextEdit,应用这个方法创建，版本:8.4。
	  * @param strPath 要增加的页面。
	  * @param strObjectName 返回对应的widget的widget。
	  * @param nPriority 优先显示nPriority小的页面，同一数值轮播显示。普通页面用0xFFFF。
	  * @return  非0表示成功，NULL表示失败。
	  */
	virtual DuiWidget* insertPersonViewPageByXml(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;

	/**
	  * @brief 在Y秀区域上删除一个页面，版本:8.4。
	  * @param page 要删除的页面。
	  * @return  true表示成功，false表示失败。
	  */
	virtual bool removePersonViewPage(DuiWidget* page) = 0;

	/**
	  * @brief 设置页面优先级，版本:8.4。
	  * @param page 要设置的页面。
	  * @return  true表示成功，false表示失败。
	  */
	virtual bool setPersonViewPagePriority(DuiWidget* page, UINT32 nPriority) = 0;
};