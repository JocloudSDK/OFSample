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
	 * 最最低层的包（开发者可能收到进程包）
	 * （2014-09-24） 将废弃
	 *
	 * @param uri @see (in yy-api-protocol.h)(suggest not using again, use onRecvTransferData instead) 
	*/
	virtual void onRecv(const unsigned int id, unsigned int uri, const BYTE* data, size_t length) = 0;
	/** 进程与进程之间的通信（业务数据包）
	 *
	 * @param id 应用插件id
	 * @param dataType @see yy::TransferType(in yy-api-transfer-datatype.h)
	 * @param data 业务数据
	*/
	virtual void onRecvByProcess(const unsigned int id,int dataType, const char* data , size_t len) = 0;
	/**
	 * @param eventid @see eAppEvent
	 */
	virtual void onRecvWindowEvent(const unsigned int id,int eventid) = 0;
	/**
	 * @brief YY收到进程外的data-transfer类型的包，解包后将包中的dataType和data数据以信号形式emit，本函数响应该信号
	 * @param id 应用插件id
	 * @param dataType @see TransferType(in dw-api-transfer-datatype.h)
	 * @param data 业务数据
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
	int  platformVersion; ///< 平台接口版本（新，旧） @see ePlatformVersion
};

/** UI回调器　*/
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
 * 模板接口
 *
 * 步骤使用　
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

	/** 跨进程时，主要启动或加载的文件(DLL,或Flash,WEB)*/
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
	 * 设置属性值,默认值　KKAttribute...
	*/
	virtual bool setAttribute(const char* name, const QVariant &value) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QVariant getAttribute(const char* name) const = 0;

	/** 模板的路径*/
	virtual void setTemplatePath(const wchar_t* path) = 0;
	virtual const wchar_t* getTemplatePath() const = 0;

	/** 模板的版本号*/
	virtual void setTemplateVersion(unsigned int version) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getTemplateVersion() const = 0;

	// IPC connect interface,主要建立跨进程通信链路

	/** 1 创建链路对象，但没有启动，需要调用ct_start方可启动链路
	 *
	 * @param prover 链路协议版本（业务包），为了兼容开放平台老版本，不同于　TemplateProcess::platformVersion 值
	 * @param addonId 插件ID,因为模板里面可以有N个插件,如果模板中没有插件,那addonId = templateId, can zero.
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
	 * @param info 如果有值,它可以用来启动不同的插件(模板中可能有N个插件)
	*/
	virtual bool ct_start(unsigned int addonId) = 0;
	/** IPC connect , go to close */
    virtual void ct_close(unsigned int addonId) = 0;
	/** IPC connect , is connected */
	virtual bool ct_isRunning(unsigned int addonId) const = 0;
	/** IPC connect , send data */
	virtual bool ct_send(const DwMarshallable& msg,unsigned int addonId ) = 0;

	/** 发送数据到另一个进程去(业务数据) 
	 * @param dataType @see yy-api-transfer-datatype.h
	 *                  = yy::TransferNone , 使用开放平台旧的协议() ,!= 使用新的
	 *
	*/
	virtual bool ct_sendByProcess(int dataType,const char* data, size_t len,unsigned int addonId) = 0;

	/** IPC connect , window 变化情况
	 * 此接口，提供给＂具体模板业务＂调用，把窗体变化通知另一进程(如yyexternal.exe)
	 * @param eventid @see eAppEvent
	*/
    virtual void ct_send_windowevent(unsigned int addonId, int eventid, const TemplateWindowPosition* position) = 0;

	/** 收到WINDOW事件
	 * 此接口，主要提供给＂开放平台openplatform.dll＂那边调用
	 * openplatform收到WINDOW相当包后，调用此接口通知到具体模板业务(IChannelTemplateConnectReceiver::onRecvWindowEvent)
	 *
	 * @param eventid @see eAppEvent
	*/
	virtual void ct_recv_windowevent(unsigned int addonId, int eventid) = 0;
	// JS support

	/** JS create engine 一个模板中可以支持N个JS插件．
	 * 
	 * @param JSPath JS包的绝对路径
	 * @param platformVersion 平台接口版本 @see ePlatformVersion
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
		ThreeColumnStyle, ///< 三栏风格
		TwoColumnStyle,
		EndStyle,
	};
	enum eSubStyle
	{
		SubUnkStyle = 0,
		SubToolbarStyle = 0x01,///< for 三栏风格带模板工具条
		SubHideRightStyle = 0x02, ///< for 三栏风格,初始化为2栏模板
		SubHideLeftStyle = 0x04, ///< 不支持
		SubHideMidStyle = 0x08, ///< 只有三栏风格才有效．
		SubLeftNoAnimationStyle = 0x10,	//暂不支持
		SubRightNoAnimationStyle = 0x20,//暂不支持
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

	/** 选择样式后，可设置显示区域大小尺寸*/
	virtual void lt_setMinSize(int cx , int cy) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void lt_setMaxSize(int cx , int cy) = 0;
	/** 有窗体(DuiWidget)支持，会创建一个区域出来,如白板，游戏直播,娱乐套件
	 *  内部会创建一个DuiWidget
	 *
	 * @param parent != NULL,就会把内部的DuiWidget的QGraphicsItem指定它;
	 *               == NULL,就会依靠selectStyle方式内部选一个QGraphicsItem;
	 *
	 * @param callback UI变化回调 can is set NULL
	 
	 * @return >=0 successed, == 0 new object; > 0 object existed ; < 0 failure
	*/
	virtual int lt_createWindow(DuiWidget* parent, IChannelTemplateUICallBack* callback) = 0;
	/** 有真实窗体支持,内部会创建一个DuiWidget，且会创建一个hwnd window
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

	/** 模板特有接口，一个模板中窗体可以有N插件的拥有者(如：教育模板中有白板插件，桌面共享)
	 *
	 * @param addonId (10051,10054...) , = 0,默认templateId
	*/
	virtual void lt_setWindowOwnerById(unsigned int addonId) = 0;
	enum eStateCode
	{
		SC_ERR_FAIL = -1000 ,
		SC_ERR_CREATED_FAILURE, //
		SC_ERR_DISCONNECT,//断开连接
		SC_ERR_PEERQUITED,//退出连接
		
		SC_CREATING, // 正在创建
		SC_TEMPLATE_LOADING, ///< 针对5.0模板情况
		SC_OK = 0,   // ok,创建成功
		SC_CREATED,  // 创建成功
		SC_CONNECTED,// 连接成功
	};
	/** 显示状态界面[跨进程启动失败，可以再重试],必须有窗体支持才行
	 * @param errcode 参考　eStateCode
	*/
	virtual bool lt_showState(int errcode) = 0;
	/** 全屏支持，必依靠真实窗体支持 会收到 IChannelTemplateUICallBack::onFullScreen(...) */
	virtual bool lt_setFullScreen(bool screen) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool lt_isFullScreen(void) const = 0;
	/** 容器是否可见*/
    virtual void lt_setViewVisible(bool visible) = 0;
	/** 更新布局*/
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

	/** 目前无用（可当存取值）*/
	virtual void lt_setHeight(int cy) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  lt_getHeight() const = 0;

	/** JS 向JS包发消息 */
	virtual bool js_send(const char* data, size_t len,unsigned int addonId) = 0;

	/** 执行一些特定的命令 */
	virtual int  handlerCommand(const char* name, const QVariant &value) = 0;

	/** 单独起一个yyexternal*/
	virtual bool ct_startEx(unsigned int addonId, bool seperateExe) = 0;

	/**
	* @brief 取得上一个模版id
	* @return 上一个模版id
	*/
	virtual unsigned int previousTemplateId() = 0;

	virtual void  lt_setMinSize_ChannelWindow(int width, int height) = 0;

    /** 注册音量控制
    * @brief 模板ID作为key
    * @return 如果已经被注册过，false，否则返回true
    */
    virtual void registerAudioVolumeController(unsigned int templateid) = 0;

    /** 反注册音量控制
    * @brief 模板ID作为key
    * @return 如果已完全反注册，返回true，否则返回false
    */
    virtual void unregisterAudioVolumeController(unsigned int templateid) = 0;
/**
signal:
   sigWindow(unsigned int appId, int eventid, WId  wid, int  x, int  y, int  width, int  height, bool visible);

   sigWidgetChanged(const QString& objname, int eventid);
*/
};

/** 模板业务需要实现的接口*/
DWDEFINE_INTERFACE(ITemplateEntry): public IUnk
{
	/** 是为了在进行接口切换时，是否需要重新加载
	*
	* @param oldSid 前一个频道ID
	* @param newSid 新的频道ID 
	* @param name 属性值
	*
	* @return 属性值
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
