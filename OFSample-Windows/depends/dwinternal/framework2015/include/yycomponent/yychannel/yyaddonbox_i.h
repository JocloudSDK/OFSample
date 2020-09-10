#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiFrameWindow;
class DuiWidget;
/*
    use getObject function, have to release the smart pointer when component onUnload
*/

DWDEFINE_INTERFACE(IYYAddonBox) : public IDWQComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual DuiFrameWindow *getAddonBoxWindow() = 0;
   	//频道应用
	// 请用新接口 addChannelAddonItem2。
/**
   * @brief .
   * @param .
   * @return .
   */ 
	virtual quint32 addChannelAddonItem(const QString &addonName, const QString &addonIcon, const QString &addonToolTips) = 0; //添加
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool removeChannelAddonItem(quint32 addonId) = 0; //移除
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonProgressBarVisiable(quint32 addonId, bool bVisiable) = 0; //是否显示进度条
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonProgressBarValue(quint32 addonId, int nValue) = 0; //设置进度
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonDownloadError(quint32 addonId) = 0; //下载出错
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonDownloadConplete(quint32 addonId) = 0; //下载完成
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonShowGray(quint32 addonId, bool bShow) = 0; //是否显示灰度图
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonImage(quint32 itemId, const QString &imagePath) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool showChannelAddonDownloadStateImage(quint32 itemId, const QString &imagePath) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool hideChannelAddonDownlaodStateImage(quint32 itemId) = 0;

	//个人应用
	// 请用新接口 addUserAddonItem2。
/**
   * @brief .
   * @param .
   * @return .
   */ 
	virtual quint32 addUserAddonItem(const QString &addonName, const QString &addonIcon, const QString &addonToolTips) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool removeUserAddonItem(quint32 addonId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonProgressBarVisiable(quint32 addonId, bool bVisiable) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonProgressBarValue(quint32 addonId, int nValue) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonDownloadError(quint32 addonId) = 0; //下载出错
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonDownloadConplete(quint32 addonId) = 0; //下载完成
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonShowGray(quint32 addonId, bool bShow) = 0; //是否显示灰度图
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonImage(quint32 itemId, const QString &imagePath) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool showUserAddonDownloadStateImage(quint32 itemId, const QString &imagePath) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool hideUserAddonDownlaodStateImage(quint32 itemId) = 0;
	
	// 请用新接口 addRecentlyAddonItem2。
/**
   * @brief .
   * @param .
   * @return .
   */ 
	virtual quint32 addRecentlyAddonItem(const QString &addonName, const QString &addonIcon, const QString &addonToolTips) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool removeRecentlyAddonItem(quint32 addonId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonImage(quint32 itemId, const QString &imagePath) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonProgressBarVisiable(quint32 addonId, bool bVisiable) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonProgressBarValue(quint32 addonId, int nValue) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonDownloadError(quint32 addonId) = 0; //下载出错
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonDownloadConplete(quint32 addonId) = 0; //下载完成
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentlyAddonShowGray(quint32 addonId, bool bShow) = 0; //是否显示灰度图

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool showRecentAddonDownloadStateImage(quint32 itemId, const QString &imagePath) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool hideRecentAddonDownlaodStateImage(quint32 itemId) = 0;
	
	//应用消息
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual DuiWidget *getAddonMessagePage() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setAddonMsgNum(int num) = 0;

	//当弹出一个messagebox时，此窗口不希望失去焦点而影藏
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setDeactivatedHide(bool bHide) = 0;

	//测试与更新图标
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonTestIconVisible(quint32 itemId, bool bVisible) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonUpdateIconVisible(quint32 itemId, bool bVisible) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonTestIconVisible(quint32 itemId, bool bVisible) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonUpdateIconVisible(quint32 itemId, bool bVisible) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentAddonTestIconVisible(quint32 itemId, bool bVisible) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setRecentAddonUpdateIconVisible(quint32 itemId, bool bVisible) = 0;

	//有消息来了
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setAddonMsgNotifyVisible(bool bShow, int aboutToShowTab) = 0;

	//设置tips
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setChannelAddonTooltips(quint32 itemId, const QString &tooltips) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setUserAddonTooltips(quint32 itemId, const QString &tooltips) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setRecentAddonTooltips(quint32 itemId, const QString &tooltips) = 0;

	//排序
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setChannelAddonsSortFlags(quint32 itemId, quint32 sortFlags) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setUserAddonsSortFlags(quint32 itemId, quint32 sortFlags) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool sortChannelAddons() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool sortUserAddons() = 0;


	//允许添加到模版
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 addChannelAddonItem2(quint32 addonId, const QString &addonName, const QString &addonIcon, const QString &addonToolTips, const QString& userData ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 addUserAddonItem2(quint32 addonId, const QString &addonName, const QString &addonIcon, const QString &addonToolTips, const QString& userData ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 addRecentlyAddonItem2(quint32 addonId, const QString &addonName, const QString &addonIcon, const QString &addonToolTips, const QString& userData ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void clickAddon( quint32 itemId ) = 0;
	virtual QList<quint32> queryItemId( quint32 addonId ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString userData( quint32 itemId ) = 0;

	/*
	signals:
	void aboutToClose();
	void channelAddonClicked(quint32 itemId,int type);
	void userAddonClicked(quint32 itemId,int type);
	void recentAddonClicked(quint32 itemId,int type);
	void addonMessagePageSelected();

	void channelAddonRightclicked(quint32 itemId, int addonType);
	void userAddonRightclicked(quint32 itemId, int addonType);
	void recentAddonRightclicked(quint32 itemId, int addonType);

	void showAddonStore(int from);


	void addonItemAdded( quint32 itemId, quint32 addonId, const QString& addonName, 
									const QString& addonIcon, const QString& addonToolTips, const QString& userdata );

	void addonItemRemoved( quint32 itemId, quint32 addonId );

	void itemTipsChanged( quin32 itemId, const QString& tips );

	*/
};

//由于应用盒子的改版，上面的接口已经不适用了，现在提供一套新的接口，上面的接口依然保留，但是不推荐
class IAddonBoxItem;

DWDEFINE_INTERFACE(IYYAddonBoxEx) : public IDWQComponent
{
	//获取窗口
	virtual DuiFrameWindow *getAddonBoxWindow() = 0;

	//添加删除
	virtual IAddonBoxItem* addItem(int firstType, int secondType) = 0;
	virtual bool removeItem(IAddonBoxItem* item) = 0;
	//virtual bool removeItem(int id) = 0;
	//virtual IAddonBoxItem* getItem(int id) = 0;

	//排序:排序规则改变后，要调用这个函数才能重新排序
	//virtual void adjustAllFrame() = 0;
	virtual void adjustFrame(int FirstType, int SecondType) = 0;

	//类型转化
	virtual int GetFirstTypeFromMajorCategory(quint32 majorCategory) = 0;
	virtual int GetSencondTypeFromMinorCategory(quint32 minorCategory) = 0;

	//各种信号
};