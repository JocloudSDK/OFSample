#pragma once
#include "duifw/duiimagelist.h"

/**
* @brief 自定义模版图标信息.
*/
struct TemplateInfomation
{
	quint32 id;			//模版id
	QString name;		//模板名字，用作tooltip显示
	QString iconPath;	//自定义的模板图标，请提供一个将当前频道状态图标和非前频道状态图标合在一起的图片
};

/**
* @brief 在子频道显示应用图标.
*/
struct AppIconInformation
{
	quint32 appid;		//应用id
	double priority;	//显示优先级，请与基础产品组申请优先级
	QString appName;	//应用名字，用作tooltip显示
	QString iconPath;	//显示图标，请提供一个将当前频道状态图标和非前频道状态图标合在一起的图片
	
};

DWDEFINE_INTERFACE(IYYTemplateInfo): public IUnk
{
/**
   * @brief 获取模板名称.
   * @param templateId，模板id.
   * @return 模板名称.
   */
	virtual QString getTemplateName(UINT32 templateId) = 0;

/**
   * @brief 设置模板信息（名称，图标）.
   * @param infoList，信息列表，详细见TemplateInfomation.
   */
	virtual void appendTemplateInfo(const QList<TemplateInfomation>& infoList) = 0;

	/**
	* @brief 设置子频道图标,应用图标会优先于模版图标，业务根据频道是否有模版决定是否调用这个method.
	* @param sid，子频道id.
	* @param appIconInfo，详细见AppIconInformation.
	*/
	virtual void addChannelAppIcon( quint32 sid, const  AppIconInformation&  appIconInfo) = 0;


	/**
	* @brief 移除设置的所有被应用影响的子频道图标.
	* @param appid，应用id.
	*/
	virtual void removeChannelAppIcon(quint32 appid) = 0;

	/**
	* @brief 子频道已经设置的图标.
	* @param sid，子频道id.
	* @param 已经设置的图标的信息列表.
	*/
	virtual QList<AppIconInformation> channelAppIcons( quint32 sid) = 0; 

};

struct YYSubChannelIconInfo
{
	QList<TemplateInfomation> templateInfo;
	QList<QPair<quint32,AppIconInformation>> appInfo;
};

DWBIND_INTERFACE(YYSubChannelIconInfo, DWMAKE_INTERFACE_UUID_NAME(YYSubChannelIconInfo))
