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
void dataArrived3(const std::string &strMsg); // 与信号dataArrived内容一样,与信号dataArrived2的RootMsg.msg内容一样，推荐使用
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
* @brief IYYIPCCefHost接口.
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
	* @brief 返回封装的cefhostwidget.
	* @return type DuiWidget.
	*/
	virtual DuiWidget* hostWidget() = 0;

	/**
	* @brief 启动加载网页.
	* @param type const QString& 网页url.
	*/
	virtual void setUrl(const QString& url) = 0;

	/**
	* @brief 启动加载网页.
	* @return type QString 网页url.
	*/
	virtual QString url() = 0;

	/**
	* @brief 设置cefwidget显示模式.
	* @param type CEF_Type cefwidget显示模式（包括离屏widget绘制与真窗口绘制两种）.
	*/
	virtual void setCefWidgetType(CEF_Type type) = 0;

	/**
	* @brief 返回设置cefwidget显示模式.
	* @return type CEF_Type.
	*/
	virtual CEF_Type cefWidgetType() = 0;

	/**
	* @brief 发送数据到host进程.
	* @param type IPcMsg.
	*/
	virtual void send(const IPcMsg& msg) = 0;

	/**
	* @brief 发送数据到host进程.
	* @param type DwBinaryStream.
	*/
	virtual void send(DwBinaryStream &msg) = 0;

	/**
	* @brief 返回是否连接状态.
	* @return type bool.
	*/
	virtual bool isConnected() = 0;

	/**
	* @brief 中止服务.
	*/
	virtual void stop() = 0;

	/**
	* @brief 刷新网页.
	*/
	virtual void refresh() = 0;

	/**
	* @brief 设置mask图片.
	*/
	virtual void setMaskImage( const QString &mask ) = 0;

	/**
	* @brief 是否启用拖动窗口
	*/
	virtual void setEnableDrag(bool enable) = 0;

	/**
	* @brief 设置启用拖动窗口区域
	*/
	virtual void setEnableDragRect(QRect &rc) = 0;

	/**
	* @brief 设置启用拖动窗口区域
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
* @brief IYYCompatibleHost接口.
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
	* @brief 返回封装的cefhostwidget.
	* @return type DuiWidget.
	*/
	virtual DuiWidget* hostWidget() = 0;

	/**
	* @brief 启动加载网页.
	* @param type const QString& 网页url.
	*/
	virtual void setUrl(const QString& url) = 0;

	/**
	* @brief 启动加载网页.
	* @return type QString 网页url.
	*/
	virtual QString url() = 0;

	/**
	* @brief 设置兼容性的模式.
	* @param type CompatibleHostType 兼容模式（包括自动优先cef模式与ie模式两种）.
	*/
	virtual void setCompatibleHostType(CompatibleHostType forceType) = 0;

	/**
	* @brief 设置兼容性的模式.
	* @return type CompatibleHostType 兼容模式（包括自动优先cef模式与ie模式两种）.
	*/
	virtual CompatibleHostType compatibleHostType() = 0;


	/**
	* @brief 发送数据到host进程.
	* @param type IPcMsg.
	*/
	virtual void send(const IPcMsg& msg) = 0;

	/**
	* @brief 发送数据到host进程.
	* @param type DwBinaryStream.
	*/
	virtual void send(DwBinaryStream &msg) = 0;

	/**
	* @brief 中止服务.
	*/
	virtual void stop() = 0;

	/**
	* @brief 刷新网页.
	*/
	virtual void refresh() = 0;

	/**
	* @brief 返回是否连接状态.
	* @return type bool.
	*/
	virtual bool isConnected() = 0;

	/**
	* @brief 返回cefhost封装组件接口（如果是cefwidget显示的情况下）.
	* @return type IYYIPCCefHostPtr.
	*/
	virtual IYYIPCCefHostPtr getCefHost() = 0;

	/**
	* @brief 返回iehost封装组件接口（如果是ie显示的情况下）.
	* @return type IYYIPCCefHostPtr.
	*/
	virtual IYYIPCWindowHostExPtr getIeHost() = 0;
};