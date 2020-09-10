#pragma once

#include "dwbase/dwcomex.h"
#include "dwbase/dwcominterface_i.h"
#include "yycomponent/yychannel/yychanneldef.h"

class DwMarshallable;
class DuiWidget;
class QVariant;

/** IPC connect ,data receiver */
class IChannelTemplateConnectReceiver
{
public:
	virtual ~IChannelTemplateConnectReceiver(){}

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onConnected(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onDisconnected(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onPeerQuited(const unsigned int id,int errcode) = 0;
	/**
	 * ����Ͳ�İ��������߿����յ����̰���
	 * ��2014-09-24�� ������
	 *
	 * @param uri @see (in yy-api-protocol.h)(suggest not using again, use onRecvTransferData instead) 
	*/
	virtual void onRecv(const unsigned int id, unsigned int uri, const BYTE* data, size_t length) = 0;
	/** ���������֮���ͨ�ţ�ҵ�����ݰ���
	 *
	 * @param id Ӧ�ò��id
	 * @param dataType @see yy::TransferType(in yy-api-transfer-datatype.h)
	 * @param data ҵ������
	*/
	virtual void onRecvByProcess(const unsigned int id,int dataType, const char* data , size_t len) = 0;
	/**
	 * @param eventid @see eAppEvent
	 */
	virtual void onRecvWindowEvent(const unsigned int id,int eventid) = 0;
	/**
	 * @brief YY�յ��������data-transfer���͵İ�������󽫰��е�dataType��data�������ź���ʽemit����������Ӧ���ź�
	 * @param id Ӧ�ò��id
	 * @param dataType @see TransferType(in dw-api-transfer-datatype.h)
	 * @param data ҵ������
	*/
	virtual void onRecvTransferData(const unsigned int id, UINT32 dataType, const BYTE* data, size_t length) = 0;
};

struct TemplateWindowPosition
{
	bool visible;
	int  x;
	int  y;
	int  width;
	int  height;
	WId  wid;
};

struct TemplateProcess
{
    const wchar_t* apptype;
	const wchar_t* apppath;
	const wchar_t* appname;
	const wchar_t* appfile;
	const wchar_t* appicon;
	const wchar_t* vip;
	int  platformVersion; ///< ƽ̨�ӿڰ汾���£��ɣ� @see ePlatformVersion
};

/** UI�ص�����*/
class IChannelTemplateUICallBack
{
public:
	virtual ~IChannelTemplateUICallBack(){}

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onRestart(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onFullScreen(const unsigned int id, bool screen, WId fullWId) = 0;
	// TO DO:
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onCreate(const unsigned int id,const TemplateWindowPosition* position) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onShow(const unsigned int id,const TemplateWindowPosition* position) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onHide(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onDestroy(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onSize(const unsigned int id,const TemplateWindowPosition* position) = 0;
};

/** 
 * ģ��ӿ�
 *
 * ����ʹ�á�
   1 call  base interface     
   2 IPC support (select)
   3 JS  support (select)
   4 Window support
     4.1 init  --- lt_selectStyle , lt_setMinSize
	 4.2 create--- lt_createWindow OR lt_createRealWindow
	 4.3 update--- lt_update

 * @author by huqiuyun
*/
DWDEFINE_INTERFACE(IChannelTemplate): public IDWQComponent
{
	// base interface
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateTypeByString(const wchar_t* type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateType(const unsigned int type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual unsigned int getTemplateType() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateId(const unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getTemplateId(void) const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateName(const wchar_t* name) = 0;
	virtual const wchar_t* getTemplateName() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateEntry(const wchar_t* entry) = 0;
	virtual const wchar_t* getTemplateEntry() const = 0;

	/** �����ʱ����Ҫ��������ص��ļ�(DLL,��Flash,WEB)*/
	virtual void setTemplateFile(const wchar_t* file) = 0;
	virtual const wchar_t*  getTemplateFile() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateSIcon(const wchar_t* icon) = 0;
	virtual const wchar_t* getTemplateSIcon() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setTemplateDesc(const wchar_t* desc) = 0;
	virtual const wchar_t* getTemplateDesc() const = 0;
	/**
	 * ��������ֵ,Ĭ��ֵ��KKAttribute...
	*/
	virtual bool setAttribute(const char* name, const QVariant &value) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QVariant getAttribute(const char* name) const = 0;

	/** ģ���·��*/
	virtual void setTemplatePath(const wchar_t* path) = 0;
	virtual const wchar_t* getTemplatePath() const = 0;

	/** ģ��İ汾��*/
	virtual void setTemplateVersion(unsigned int version) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getTemplateVersion() const = 0;

	// IPC connect interface,��Ҫ���������ͨ����·

	/** 1 ������·���󣬵�û����������Ҫ����ct_start����������·
	 *
	 * @param prover ��·Э��汾��ҵ�������Ϊ�˼��ݿ���ƽ̨�ϰ汾����ͬ�ڡ�TemplateProcess::platformVersion ֵ
	 * @param addonId ���ID,��Ϊģ�����������N�����,���ģ����û�в��,��addonId = templateId, can zero.
	 * 
	*/
	virtual bool ct_create(unsigned int addonId ,int prover , const TemplateProcess* info) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void ct_destroy(unsigned int addonId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void ct_destroyAll() = 0;

	/** 2 IPC connect , receive data */
	virtual void ct_receiver(IChannelTemplateConnectReceiver* receiver,unsigned int addonId) = 0;
	/** 3 IPC connect ,go to start , and before call it ,need to call ct_create
	 *
	 * @param info �����ֵ,����������������ͬ�Ĳ��(ģ���п�����N�����)
	*/
	virtual bool ct_start(unsigned int addonId) = 0;
	/** IPC connect , go to close */
    virtual void ct_close(unsigned int addonId) = 0;
	/** IPC connect , is connected */
	virtual bool ct_isRunning(unsigned int addonId) const = 0;
	/** IPC connect , send data */
	virtual bool ct_send(const DwMarshallable& msg,unsigned int addonId ) = 0;

	/** �������ݵ���һ������ȥ(ҵ������) 
	 * @param dataType @see yy-api-transfer-datatype.h
	 *                  = yy::TransferNone , ʹ�ÿ���ƽ̨�ɵ�Э��() ,!= ʹ���µ�
	 *
	*/
	virtual bool ct_sendByProcess(int dataType,const char* data, size_t len,unsigned int addonId) = 0;

	/** IPC connect , window �仯���
	 * �˽ӿڣ��ṩ��������ģ��ҵ�񣢵��ã��Ѵ���仯֪ͨ��һ����(��yyexternal.exe)
	 * @param eventid @see eAppEvent
	*/
    virtual void ct_send_windowevent(unsigned int addonId, int eventid, const TemplateWindowPosition* position) = 0;

	/** �յ�WINDOW�¼�
	 * �˽ӿڣ���Ҫ�ṩ��������ƽ̨openplatform.dll���Ǳߵ���
	 * openplatform�յ�WINDOW�൱���󣬵��ô˽ӿ�֪ͨ������ģ��ҵ��(IChannelTemplateConnectReceiver::onRecvWindowEvent)
	 *
	 * @param eventid @see eAppEvent
	*/
	virtual void ct_recv_windowevent(unsigned int addonId, int eventid) = 0;
	// JS support

	/** JS create engine һ��ģ���п���֧��N��JS�����
	 * 
	 * @param JSPath JS���ľ���·��
	 * @param platformVersion ƽ̨�ӿڰ汾 @see ePlatformVersion
	*/
	virtual bool js_create(const wchar_t* JSPath,unsigned int addonId, int platformVersion) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void js_destroy(unsigned int addonId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void js_destroyAll() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void js_start(unsigned int addonId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void js_stop(unsigned int addonId) = 0;
    
	enum eStyle
	{
		UnkStyle = 0, 
		DefaultStyle,
		ThreeColumnStyle, ///< �������
		TwoColumnStyle,
		EndStyle,
	};
	enum eSubStyle
	{
		SubUnkStyle = 0,
		SubToolbarStyle = 0x01,///< for ��������ģ�幤����
		SubHideRightStyle = 0x02, ///< for �������,��ʼ��Ϊ2��ģ��
		SubHideLeftStyle = 0x04, ///< ��֧��
		SubHideMidStyle = 0x08, ///< ֻ������������Ч��
		SubLeftNoAnimationStyle = 0x10,	//�ݲ�֧��
		SubRightNoAnimationStyle = 0x20,//�ݲ�֧��
	};
	// layout interface
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool lt_selectStyleByString(const wchar_t* style) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool lt_selectStyle(int style) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual void lt_setDefalutStyle() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_setSubStyleByString(const wchar_t* style) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_setSubStyle(int style) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  lt_getSubStyle() const = 0;

	/** ѡ����ʽ�󣬿�������ʾ�����С�ߴ�*/
	virtual void lt_setMinSize(int cx , int cy) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_setMaxSize(int cx , int cy) = 0;
	/** �д���(DuiWidget)֧�֣��ᴴ��һ���������,��װ壬��Ϸֱ��,�����׼�
	 *  �ڲ��ᴴ��һ��DuiWidget
	 *
	 * @param parent != NULL,�ͻ���ڲ���DuiWidget��QGraphicsItemָ����;
	 *               == NULL,�ͻ�����selectStyle��ʽ�ڲ�ѡһ��QGraphicsItem;
	 *
	 * @param callback UI�仯�ص� can is set NULL
	 
	 * @return >=0 successed, == 0 new object; > 0 object existed ; < 0 failure
	*/
	virtual int lt_createWindow(DuiWidget* parent, IChannelTemplateUICallBack* callback) = 0;
	/** ����ʵ����֧��,�ڲ��ᴴ��һ��DuiWidget���һᴴ��һ��hwnd window
	 * @return >=0 successed, == 0 new object; > 0 object existed ; < 0 failure
	*/
	virtual int lt_createRealWindow(DuiWidget* parent, IChannelTemplateUICallBack* callback) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_destroyWindow() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_showWindow(bool show) = 0;
	virtual DuiWidget* lt_getWindow() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool lt_getWindowPosition(TemplateWindowPosition& position) = 0;

	/** ģ�����нӿڣ�һ��ģ���д��������N�����ӵ����(�磺����ģ�����аװ��������湲��)
	 *
	 * @param addonId (10051,10054...) , = 0,Ĭ��templateId
	*/
	virtual void lt_setWindowOwnerById(unsigned int addonId) = 0;
	enum eStateCode
	{
		SC_ERR_FAIL = -1000 ,
		SC_ERR_CREATED_FAILURE, //
		SC_ERR_DISCONNECT,//�Ͽ�����
		SC_ERR_PEERQUITED,//�˳�����
		
		SC_CREATING, // ���ڴ���
		SC_TEMPLATE_LOADING, ///< ���5.0ģ�����
		SC_OK = 0,   // ok,�����ɹ�
		SC_CREATED,  // �����ɹ�
		SC_CONNECTED,// ���ӳɹ�
	};
	/** ��ʾ״̬����[���������ʧ�ܣ�����������],�����д���֧�ֲ���
	 * @param errcode �ο���eStateCode
	*/
	virtual bool lt_showState(int errcode) = 0;
	/** ȫ��֧�֣���������ʵ����֧�� ���յ� IChannelTemplateUICallBack::onFullScreen(...) */
	virtual bool lt_setFullScreen(bool screen) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool lt_isFullScreen(void) const = 0;
	/** �����Ƿ�ɼ�*/
    virtual void lt_setViewVisible(bool visible) = 0;
	/** ���²���*/
    virtual bool lt_update() = 0;

	/** style == ThreeColumnStyle ,subStyle == SubToolbarStyle ,send pannel tool bar region widget */
	virtual DuiWidget* lt_getWidget(const std::string& objname) const = 0;
	virtual DuiWidget* lt_getWidgetByState() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_setWidth(int cx) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  lt_getWidth() const = 0;

	/** Ŀǰ���ã��ɵ���ȡֵ��*/
	virtual void lt_setHeight(int cy) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  lt_getHeight() const = 0;

	/** JS ��JS������Ϣ */
	virtual bool js_send(const char* data, size_t len,unsigned int addonId) = 0;

	/** ִ��һЩ�ض������� */
	virtual int  handlerCommand(const char* name, const QVariant &value) = 0;

	/** ������һ��yyexternal*/
	virtual bool ct_startEx(unsigned int addonId, bool seperateExe) = 0;

	/**
	* @brief ȡ����һ��ģ��id
	* @return ��һ��ģ��id
	*/
	virtual unsigned int previousTemplateId() = 0;

	virtual void  lt_setMinSize_ChannelWindow(int width, int height) = 0;

    /** ע����������
    * @brief ģ��ID��Ϊkey
    * @return ����Ѿ���ע�����false�����򷵻�true
    */
    virtual void registerAudioVolumeController(unsigned int templateid) = 0;

    /** ��ע����������
    * @brief ģ��ID��Ϊkey
    * @return �������ȫ��ע�ᣬ����true�����򷵻�false
    */
    virtual void unregisterAudioVolumeController(unsigned int templateid) = 0;
/**
signal:
   sigWindow(unsigned int appId, int eventid, WId  wid, int  x, int  y, int  width, int  height, bool visible);

   sigWidgetChanged(const QString& objname, int eventid);
*/
};

/** ģ��ҵ����Ҫʵ�ֵĽӿ�*/
DWDEFINE_INTERFACE(ITemplateEntry): public IUnk
{
	/** ��Ϊ���ڽ��нӿ��л�ʱ���Ƿ���Ҫ���¼���
	*
	* @param oldSid ǰһ��Ƶ��ID
	* @param newSid �µ�Ƶ��ID 
	* @param name ����ֵ
	*
	* @return ����ֵ
	*/
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QVariant getAttribute(unsigned int oldSid,unsigned int newSid,const char* name) = 0;
};

DWBIND_INTERFACE(TemplateWindowPosition, DWMAKE_INTERFACE_UUID_NAME(TemplateWindowPosition))
DWBIND_INTERFACE(IChannelTemplateUICallBack, DWMAKE_INTERFACE_UUID_NAME(IChannelTemplateUICallBack))
