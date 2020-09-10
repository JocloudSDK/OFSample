#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/yyipcwindowhost_i.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
class DwBinaryStream;
struct IPcMsg;

/*
signals:

void connected();
void closed();
void dataArrived(const DwBinaryStream &msg);
void dataArrived2(const RootMsg &msg);
void dataArrived3(const std::string &strMsg); // ���ź�dataArrived����һ��,���ź�dataArrived2��RootMsg.msg����һ�����Ƽ�ʹ��
void navigateCompleted();
void errored(int statusCode);
void retrying(bool &handled);
*/

/* demo
IYYIPCCefHostPtr cefHostWrap;
coCreateComInstance(KSERVICEIPCCefWidget, cefHostWrap);
parentCef->addItem(cefHostWrap->hostWidget());
cefHostWrap->setUrl("http://www.yy.com");
*/

/**
* @brief IYYIPCCefHost�ӿ�.
*/

DWDEFINE_INTERFACE( IYYIPCCefHost ) : public IDWQComponent
{
	enum CEF_Type
	{
		CT_None = 0,
		CT_QtWidgetType = 1,
		CT_RealWndType = 2,
		CT_OSF_RealWndType = 3,
	};

	/**
	* @brief ���ط�װ��cefhostwidget.
	* @return type DuiWidget.
	*/
	virtual DuiWidget* hostWidget() = 0;

	/**
	* @brief ����������ҳ.
	* @param type const QString& ��ҳurl.
	*/
	virtual void setUrl(const QString& url) = 0;

	/**
	* @brief ����������ҳ.
	* @return type QString ��ҳurl.
	*/
	virtual QString url() = 0;

	/**
	* @brief ����cefwidget��ʾģʽ.
	* @param type CEF_Type cefwidget��ʾģʽ����������widget�������洰�ڻ������֣�.
	*/
	virtual void setCefWidgetType(CEF_Type type) = 0;

	/**
	* @brief ��������cefwidget��ʾģʽ.
	* @return type CEF_Type.
	*/
	virtual CEF_Type cefWidgetType() = 0;

	/**
	* @brief �������ݵ�host����.
	* @param type IPcMsg.
	*/
	virtual void send(const IPcMsg& msg) = 0;

	/**
	* @brief �������ݵ�host����.
	* @param type DwBinaryStream.
	*/
	virtual void send(DwBinaryStream &msg) = 0;

	/**
	* @brief �����Ƿ�����״̬.
	* @return type bool.
	*/
	virtual bool isConnected() = 0;

	/**
	* @brief ��ֹ����.
	*/
	virtual void stop() = 0;

	/**
	* @brief ˢ����ҳ.
	*/
	virtual void refresh() = 0;

	/**
	* @brief ����maskͼƬ.
	*/
	virtual void setMaskImage( const QString &mask ) = 0;

	/**
	* @brief �Ƿ������϶�����
	*/
	virtual void setEnableDrag(bool enable) = 0;

	/**
	* @brief ���������϶���������
	*/
	virtual void setEnableDragRect(QRect &rc) = 0;

	/**
	* @brief ���������϶���������
	*/
	virtual void setEnableAlpha(bool) = 0;

};

/*
signals:

void connected();
void closed();
void dataArrived(const DwBinaryStream &msg);
void dataArrived2(const RootMsg &msg);
void navigateCompleted();
void errored(int statusCode);
void retrying(bool &handled);
*/

/* demo
IYYCompatibleHostPtr compatibleHost;
coCreateComInstance(KSERVICECompatibleHostWidget, compatibleHost);
parentCef->addItem(compatibleHost->hostWidget());
compatibleHost->setCompatibleHostType(IYYCompatibleHost::CHT_Auto);
if (compatibleHost->getCefHost() != NULL)
{
compatibleHost->getCefHost()->setCefWidgetType(IYYIPCCefHost::CT_OSF_RealWndType);
}

if (compatibleHost->getIeHost() != NULL)
{
compatibleHost->getIeHost()->setWebScrollBarOff(false);
}

compatibleHost->setUrl("http://www.yy.com");
*/
/**
* @brief IYYCompatibleHost�ӿ�.
*/
DWDEFINE_INTERFACE( IYYCompatibleHost ) : public IDWQComponent
{
	enum CompatibleHostType
	{
		CHT_Auto = 0,
		CHT_IE,
		CHT_CEF,
	};

	/**
	* @brief ���ط�װ��cefhostwidget.
	* @return type DuiWidget.
	*/
	virtual DuiWidget* hostWidget() = 0;

	/**
	* @brief ����������ҳ.
	* @param type const QString& ��ҳurl.
	*/
	virtual void setUrl(const QString& url) = 0;

	/**
	* @brief ����������ҳ.
	* @return type QString ��ҳurl.
	*/
	virtual QString url() = 0;

	/**
	* @brief ���ü����Ե�ģʽ.
	* @param type CompatibleHostType ����ģʽ�������Զ�����cefģʽ��ieģʽ���֣�.
	*/
	virtual void setCompatibleHostType(CompatibleHostType forceType) = 0;

	/**
	* @brief ���ü����Ե�ģʽ.
	* @return type CompatibleHostType ����ģʽ�������Զ�����cefģʽ��ieģʽ���֣�.
	*/
	virtual CompatibleHostType compatibleHostType() = 0;


	/**
	* @brief �������ݵ�host����.
	* @param type IPcMsg.
	*/
	virtual void send(const IPcMsg& msg) = 0;

	/**
	* @brief �������ݵ�host����.
	* @param type DwBinaryStream.
	*/
	virtual void send(DwBinaryStream &msg) = 0;

	/**
	* @brief ��ֹ����.
	*/
	virtual void stop() = 0;

	/**
	* @brief ˢ����ҳ.
	*/
	virtual void refresh() = 0;

	/**
	* @brief �����Ƿ�����״̬.
	* @return type bool.
	*/
	virtual bool isConnected() = 0;

	/**
	* @brief ����cefhost��װ����ӿڣ������cefwidget��ʾ������£�.
	* @return type IYYIPCCefHostPtr.
	*/
	virtual IYYIPCCefHostPtr getCefHost() = 0;

	/**
	* @brief ����iehost��װ����ӿڣ������ie��ʾ������£�.
	* @return type IYYIPCCefHostPtr.
	*/
	virtual IYYIPCWindowHostExPtr getIeHost() = 0;
};