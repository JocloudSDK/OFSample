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
   * @brief IYYIPCWindowHost�ӿ�.
   */
DWDEFINE_INTERFACE( IYYIPCWindowHost ) : public IDWQComponent
{
/**
   * @brief ����.
   */
	virtual bool start() = 0;
/**
   * @brief ֹͣ.
   */
	virtual void stop() = 0;
/**
   * @brief ������ʾ��url.
   * @param url url.
   */
	virtual void setBulletinUrl( const QString& url) = 0;
/**
   * @brief ��������.
   * @param type ��ʾ����.
   */
	virtual void setBulletinType( int type ) = 0;
/**
   * @brief ����Ӧ�ó�������.
   * @param name ����.
   */
	virtual void setApplicationName(const QString& name) = 0;
/**
   * @brief ����web type.
   * @param type ����.
   */
	virtual void setWebType(int type) = 0;
/**
   * @brief �Ƿ�����.
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
   * @brief ����parent widget.
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
��ҳ��c++��window.external.sendCommand(Cmd, Arg1, Arg2,..);
c++����dataArrived2(const RootMsg &)����źš�RootMsg���string�Ǹ�json����ʽΪ
{ "Cmd" : "�����cmd", "Args" : ["arg1", "arg2"...] }

�����Ѿ��нӿ����£�
��url: window.external.sendCommand("openUrl","{\"url\":\"http://www.yy.com\",\"needTicket\":1}");
*/

class DwBinaryStream;

/**
   * @brief IYYIPCWindowHostEx��չ�ӿ�.
   */
DWDEFINE_INTERFACE(IYYIPCWindowHostEx) : public IDWQComponent
{
/**
   * @brief ������Ϣ��
   * @param ����ǵ�����ҳ���js����ʹ������json��{ "Cmd": "executeJs", "Args": [ "foo(\"arg1\")" ] }
   */
	virtual void sendMessage(DwBinaryStream *msg) = 0 ;

/**
   * @brief ע�⣬webType������KCommonWebType���òŻ�ɹ�.
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
	virtual void setNewWebPageOff(bool off)=0;	//��ֹ����ҳ���д�
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
	virtual void adjustWidgetSize() = 0; //���²��ִ���
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showHideWidget(bool b) = 0;	//��ʾ�����Լ���widget
};