#pragma once
#include "duifw/duiimagelist.h"

/**
* @brief �Զ���ģ��ͼ����Ϣ.
*/
struct TemplateInfomation
{
	quint32 id;			//ģ��id
	QString name;		//ģ�����֣�����tooltip��ʾ
	QString iconPath;	//�Զ����ģ��ͼ�꣬���ṩһ������ǰƵ��״̬ͼ��ͷ�ǰƵ��״̬ͼ�����һ���ͼƬ
};

/**
* @brief ����Ƶ����ʾӦ��ͼ��.
*/
struct AppIconInformation
{
	quint32 appid;		//Ӧ��id
	double priority;	//��ʾ���ȼ������������Ʒ���������ȼ�
	QString appName;	//Ӧ�����֣�����tooltip��ʾ
	QString iconPath;	//��ʾͼ�꣬���ṩһ������ǰƵ��״̬ͼ��ͷ�ǰƵ��״̬ͼ�����һ���ͼƬ
	
};

DWDEFINE_INTERFACE(IYYTemplateInfo): public IUnk
{
/**
   * @brief ��ȡģ������.
   * @param templateId��ģ��id.
   * @return ģ������.
   */
	virtual QString getTemplateName(UINT32 templateId) = 0;

/**
   * @brief ����ģ����Ϣ�����ƣ�ͼ�꣩.
   * @param infoList����Ϣ�б���ϸ��TemplateInfomation.
   */
	virtual void appendTemplateInfo(const QList<TemplateInfomation>& infoList) = 0;

	/**
	* @brief ������Ƶ��ͼ��,Ӧ��ͼ���������ģ��ͼ�꣬ҵ�����Ƶ���Ƿ���ģ������Ƿ�������method.
	* @param sid����Ƶ��id.
	* @param appIconInfo����ϸ��AppIconInformation.
	*/
	virtual void addChannelAppIcon( quint32 sid, const  AppIconInformation&  appIconInfo) = 0;


	/**
	* @brief �Ƴ����õ����б�Ӧ��Ӱ�����Ƶ��ͼ��.
	* @param appid��Ӧ��id.
	*/
	virtual void removeChannelAppIcon(quint32 appid) = 0;

	/**
	* @brief ��Ƶ���Ѿ����õ�ͼ��.
	* @param sid����Ƶ��id.
	* @param �Ѿ����õ�ͼ�����Ϣ�б�.
	*/
	virtual QList<AppIconInformation> channelAppIcons( quint32 sid) = 0; 

};

struct YYSubChannelIconInfo
{
	QList<TemplateInfomation> templateInfo;
	QList<QPair<quint32,AppIconInformation>> appInfo;
};

DWBIND_INTERFACE(YYSubChannelIconInfo, DWMAKE_INTERFACE_UUID_NAME(YYSubChannelIconInfo))
