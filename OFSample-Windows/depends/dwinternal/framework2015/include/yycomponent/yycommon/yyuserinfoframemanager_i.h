#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiSettingPageBase;
/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
  *IYYUserInfoFrameManagerPtr im_tree_data = root->getServiceInterface<IYYUserInfoFrameManager>(KSERVICECommon);
*/
DWDEFINE_INTERFACE(IYYUserInfoFrameManager): public IUnk
{
/**
   * @brief 显示好友或者陌生人的个人信息框.
   * @param uid 用户uid.
   * @param type 默认显示哪页，共两页，0表示"Y友信息"，1表示"公会信息".
   * @return.
   */
    virtual void showFrame(UINT64 uid, quint32 type) = 0;

/**
   * @brief 显示好友或者陌生人的个人信息框.
   * @param strArgs 参数格式{uid:xxx,src:xxx,bigT:xxx}, uid表示用户uid; src表示来源，显示在添加好友时的“来自于xxx”上;bigT用于加好友时统计的id.
   * @param type 默认显示哪页，共两页，0表示"Y友信息"，1表示"公会信息".
   * @return.
   */
    virtual void showFrame2(const std::wstring& strArgs, quint32 type) = 0;
};

class DuiWidget;
DWDEFINE_INTERFACE(IYYUserInfoFrameManagerEx): public IDWQComponent
{
	/**
	* @brief 显示好友或者陌生人的个人信息框.
	* @param uid 用户uid.
	* @param type 默认显示哪页，共两页，0表示"Y友信息"，1表示"公会信息".
	* @return.
	*/
	virtual void showFrame(UINT64 uid, quint32 type) = 0;

	/**
	* @brief 显示好友或者陌生人的个人信息框.
	* @param strArgs 参数格式{uid:xxx,src:xxx,bigT:xxx}, uid表示用户uid; src表示来源，显示在添加好友时的“来自于xxx”上;bigT用于加好友时统计的id.
	* @param type 默认显示哪页，共两页，0表示"Y友信息"，1表示"公会信息".
	*/
	virtual void showFrame2(const std::wstring& strArgs, quint32 type) = 0;

	//以后需要的话，再把单个userinfoFrame接口开出来
	enum UserInfoPos
	{
		POS_USERINFO_BEHIND_IMID = 0
	};
	virtual bool insertWidget(UINT32 uid, DuiWidget* widget, UserInfoPos pos) = 0;

    virtual bool insertPage(UINT32 uid, const QString& strTabText, const QString& strIconPath, const QString& strPageName, DuiSettingPageBase* page) = 0;

	/**
	* @brief 设置大资料卡皮肤.
	* @param uid 用户uid.
	* @param imagePath 图片本地路径
	* @return.
	*/
	virtual void setInfocardImage(quint32 uid, const QString &imagePath) = 0;
};
