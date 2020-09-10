#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
signals:
	void aboutToClose(int reason);
	void connected();
	void closed();
	void crashed();
    void receivedData(const std::string &data);
    void navigateTimeout();
*/
class DuiWidget;
class QString;

/**
   * @brief IYYIPCWindowHost接口.
   */
DWDEFINE_INTERFACE( IYYIPCWindowHost ) : public IDWQComponent
{
/**
   * @brief 启动.
   */
	virtual bool start() = 0;
/**
   * @brief 停止.
   */
	virtual void stop() = 0;
/**
   * @brief 设置显示的url.
   * @param url url.
   */
	virtual void setBulletinUrl( const QString& url) = 0;
/**
   * @brief 设置类型.
   * @param type 显示类型.
   */
	virtual void setBulletinType( int type ) = 0;
/**
   * @brief 设置应用程序名字.
   * @param name 名字.
   */
	virtual void setApplicationName(const QString& name) = 0;
/**
   * @brief 设置web type.
   * @param type 类型.
   */
	virtual void setWebType(int type) = 0;
/**
   * @brief 是否连上.
   * @return .
   */
	virtual bool isConnected() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebScrollBarOff(bool off) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebContextMenuOff(bool off) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebMessageBoxOff(bool off) = 0;
/**
   * @brief .
   * @param .
   */
	virtual  void setNewWebPageOff(bool off) = 0;
/**
   * @brief 设置parent widget.
   * @param parent parent widget.
   */
	virtual void setParentWidget( DuiWidget* parent ) = 0;
};






/*
signals:

void aboutToClose(int reason);
void connected();
void closed();
void crashed();

void dataArrived(DwBinaryStream *msg);
void dataArrived2(const RootMsg &msg);
void errored();
void navigateHistoryStateChanged(quint32 backOrForward, quint32 state);

// receive from web close window signal 
void sigIPCHostCloseWindow(const QString&);

// receive from web close dialog signal
void sigIPCHostCloseDialog(int reason);
void retrying(bool &handled);
*/

/*
网页调c++，window.external.sendCommand(Cmd, Arg1, Arg2,..);
c++连接dataArrived2(const RootMsg &)这个信号。RootMsg里的string是个json，格式为
{ "Cmd" : "传入的cmd", "Args" : ["arg1", "arg2"...] }

内置已经有接口如下：
打开url: window.external.sendCommand("openUrl","{\"url\":\"http://www.yy.com\",\"needTicket\":1}");
*/

class DwBinaryStream;

/**
   * @brief IYYIPCWindowHostEx扩展接口.
   */
DWDEFINE_INTERFACE(IYYIPCWindowHostEx) : public IDWQComponent
{
/**
   * @brief 发送消息。
   * @param 如果是调用网页里的js，请使用以下json：{ "Cmd": "executeJs", "Args": [ "foo(\"arg1\")" ] }
   */
	virtual void sendMessage(DwBinaryStream *msg) = 0 ;

/**
   * @brief 注意，webType必须是KCommonWebType调用才会成功.
   */
	virtual void goBack() = 0 ;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void goForward() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void refresh() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void enableDropTarget(bool enable) = 0 ;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showLoading() = 0 ;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool start()=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stop()=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setBulletinUrl(const QString& url)=0;

	/*
	Type:
	{
	Invalid = 0x0,
	Flash = 0x01,
	Web = 0x02,
	Media = 0x03
	};
	*/
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setBulletinType(int type)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setApplicationName(const QString& name)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebType(int type)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isConnected() const=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebScrollBarOff(bool off)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebContextMenuOff(bool off)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWebMessageBoxOff(bool off)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setNewWebPageOff(bool off)=0;	//禁止在新页面中打开
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool sendMessageByString(const QString& msg)=0;

	virtual DuiWidget* hostWidget() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void adjustWidgetSize() = 0; //重新布局窗口
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showHideWidget(bool b) = 0;	//显示隐藏自己的widget
};