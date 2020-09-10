#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yychanneldef.h"
#include <string>
#include <vector>

class DuiWidget;
class QString;
class QVariant;
class QPoint;

enum eAddonStatus
{		
	ADDON_S_DISCONNECT = 0,
	ADDON_S_CREATE = 1,
	ADDON_S_REMOVE = 2,
};
const quint32 KBaseBulletinId = 6001; //基础公告 appid(openplatform server config)
const quint32 KDefBulletinId = 6002; //上麦名片,onaircard
/** 插件的状态 */
enum eAddonState
{
	AS_Create = 0,
	AS_Closed = 1,
	AS_FullScreen = 2,
	AS_NormalScreen = 3,
	AS_SetFocus = 4,
	AS_KillFocus = 5,
	AS_ReStart = 6,
	AS_Hided = 7,
	AS_Showed = 8,

	AS_View_visible = 9,
	AS_View_invisible = 10,
};
enum ToolbarStyle 
{
    kTS_Vert = 0,
    kTS_Horz,
    kTS_Count
};

/** 插件过滤器(添加时)*/
class IAddonFilter
{
public:
	virtual ~IAddonFilter(){};

	enum eFilter
	{
		Filter_unk = 0,
		Filter_belongto,///< 可以使用
		Filter_invalid, ///< 无效,不能使用
	};
	enum eFrom
	{
		From_unk = 0,
		From_add, ///< add addon from server 
		From_run, ///< before run addon 
	};
	/**
	 * @param from @see eFrom
	 *
	 * @return true,过滤这个应用
	*/
	virtual int  addonFilter(const quint32 id, quint32 type,int from) = 0;
};

class IAddonHandler;
/** 
* @brief 插件界面顯示區接口.
*/

class IAddonViewWidget
{
public:
	virtual ~IAddonViewWidget(){};

/**
   * @brief 这个界面区域是否属于这个插件.
   * @param id，插件id.
   * @return 是否属于这个插件.
   */
	virtual bool belongTo(const quint32 id) = 0;

/**
   * @brief 设置插件处理器.
   * @param handler，处理器.
   */
	virtual void setAddonHandler(IAddonHandler* handler) = 0;
/**
   * @brief 设置是否全屏显示.
   * @param id，插件id.
   * @param full，是否全屏显示.
  */
	virtual void setFullScreen(const quint32 id,bool full) = 0;
/**
   * @brief 设置是否显示全屏按钮.
   * @param id，插件id.
   * @param show，是否显示.
   */
	virtual void showFullScreenButton(const quint32 id, bool show) = 0;
/**
   * @brief 设置全屏按钮是否有效.
   * @param id，插件id.
   * @return enable，是否有效.
   */
	virtual void enableFullScreenButton(const quint32 id , bool enable) = 0;


	/**
	* @brief 隐藏整个相关联对象(如工具条中的按钮).
	* @param id，插件id.
	* @param item，相关联对象.
	*/
	virtual void hideItem(const quint32 id, DuiWidget* item) = 0;

	/**
	* @brief 隐藏顯示區对象.
	* @param id，插件id.
	* @param item，相关联对象.
	*/
	virtual void hideItemOfView(const quint32 id, DuiWidget* item) = 0;
/**
   * @brief 显示某个widget.
   * @param id，插件id.
   * @param item，要显示的widget.
   * @param index，显示位置.
   * @return .
   */
	virtual void showItem(const quint32 id, DuiWidget* item, int index) = 0;
	
	/**
	* @brief 添加widget.
	* @param id，插件id.
	* @param item，要添加的widget.
	* @param text，显示的文本.
	* @param imagePath，图标路劲.
	* @param tip，tooltip.
	* @return index，位置.
	*/
	virtual void createItem(const quint32 id, DuiWidget* item, const QString& text, const QString& imagePath, const QString &tip, int index) = 0;
	/** 删除 */
	/**
	* @brief 移除插件.
	* @param id，插件id.
	* @param item，对应的widget.
	*/
	virtual void removeItem(const quint32 id,DuiWidget* item) = 0;
	

	/**
	* @brief 調整順序.
	* @param order，顺序列表.
	*/
	virtual void orderItem(const std::vector<quint32>& order) = 0;

	/**
	* @brief 获取应用widget.
	* @return 应用widge.
	*/
	virtual DuiWidget* getAppWidget() = 0;
/**
   * @brief 是否显示.
   * @return 是否显示.
   */
	virtual bool isShow() const = 0;
	/**
	* @brief 设置是否显示.
	* @param show，是否显示.
	*/
	virtual void showSelf(bool show) = 0;

/**
   * @brief 设置是否按下状态.
   * @param id，插件id.
   * @param check，是否按下状态.
   */
	virtual void checkButton(const quint32 id, bool check) = 0;	

    /**
	* @brief 设置滚工具条风格.
	* @param type, 水平 or 竖直.
	*/
    virtual void setToolbarStyle(ToolbarStyle type) = 0;
};

class IAddonHandler
{
public:
	virtual ~IAddonHandler(){}

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerViewWidget(const std::string& name,IAddonViewWidget* view) =0;
/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual void unRegisterViewWidget(const std::string& name) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void activeViewWidget(const std::string& name) =0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerFilter(const std::string& name,IAddonFilter* filter) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unRegisterFilter(const std::string& name) = 0;

	/** 指定VIEW是否可见
	 *
	 * @name ="",指当前VIEW显示区
	*/
    virtual bool isViewVisible(const std::string& name) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void setViewVisible(IAddonViewWidget* view,bool visible) = 0;

	virtual QObject* object() = 0;

	/** 这个插件是否属于这个处理器(一个插件只能存在一个处理器) */
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool belongTo(const quint32 id, quint32 type) = 0;
	/** 插入到cache中，但不创建显示区, Asynchronous wait connect state!
	 *
	 * @param isConnect =true,需要与链路保持信号
	*/
    virtual bool add(const quint32 id, const QString& text, bool connected, quint32 type, int source) = 0;

	/** 创建显示区　*/
	virtual bool create(const quint32 id, const QString& text, const QString& imagePath, const QString &tip) = 0;
	/* show AND startup
    ** 接口弃用。create的时候会调一次startup，
    ** ！！注意！！置顶公告区插件请用setCurrentApp
    ** YYXMII-53106, by yy8.50
    */
	virtual void startup(const quint32 id) = 0;

	/** 关闭显示区，但不从cache中删除　*/
	virtual bool close(const quint32 id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool hide(const quint32 id) = 0;
	/** 关闭显示区
	 *@param destroy =true 从cache中删除
	*/
	virtual bool remove(const quint32 id, bool destroy, int source) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void removeAll() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setError(const quint32 id , int errcode) = 0;

	/** @return */
	virtual bool  find(const quint32 id) = 0;
	virtual std::list<quint32> getIds(void) = 0;

	/** addon list is null  */
	virtual bool isEmpty(void) const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isFullScreen(const quint32 id) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setFullScreen(const quint32 id,bool full) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 getCurAppId(void) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 getLastAppId(void) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 getAppIdByOW(void) const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual bool setAttribute(const quint32 id , const QString& name, const QVariant &value) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QVariant getAttribute(const quint32 id , const char* name) const = 0;

	virtual DuiWidget* findWidget(const quint32 id) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */    
	virtual void clickedButton(const QString& name) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */	
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void enableSwitch(bool enable) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  getHeight(const quint32 id) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setHeight(const quint32 id,int height) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  getWidth(const quint32 id) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setWidth(const quint32 id,int width) = 0;

/**
   * @brief .置顶公告区插件，即激活
   * @param .
   * @return .
   */
	virtual void setCurrentApp(const quint32 curid) = 0;


    /**
   * @brief .通过名字查找注册了（registerViewWidget）的view
   * @param .
   * @return .
   */
    virtual IAddonViewWidget* findViewWidget(const std::string& name) const = 0;

    /**
    * @brief .设置公告区插件显示优先级
    * @param .
    * @return .
    */
    virtual void addAddonPriority(quint32 addonid, int priority) = 0;
    virtual void removeAddonPriority(quint32 addonid) = 0;
};

class IPannelButtonFilter
{
public:
	virtual ~IPannelButtonFilter(){};

	enum ePBSource
	{		
		PBSource_create = 0,
		PBSource_destroy,
	};

	enum ePBFilter
	{
		PBFilter_unk = 0,
		/** 针对 PBSource_create */
		PBFilter_create, ///< 需要创建
		PBFilter_null, ///< 创建空值

		/** 针对 PBSource_destroy*/
		PBFilter_delete,
	};

	enum ePBState
	{
		PBState_unk = 0,
		// 一组
		PBState_show = 1,
		PBState_hide = 2,
		// 一组
		PBState_disable = 0x10,
		PBState_enable  = 0x20,
	};
	/** 
	 * 创建之前需要询问一下,可以改变初始值.
	 *
	 * @param source @see ePBSource
     *
	 * @param id [in/out] 输入输出
	 * @param text [in/out]
	 * @param icon [in/out]
	 * @param state 状态 @see ePBState
	 *
	*/
	virtual bool preCreatePannelButton(quint32& id, QString& objname, QString& text, QString &icon, QString& tip, int& state) = 0;

	/**
	 * 在消毁之前询问一下,如果不想删除,可以根据state调整对象状态
	 *
	 * @return PBFilter_unk OR PBFilter_delete
	*/
	virtual int  preDestroyPannelButton(quint32 id, int& state) = 0;
	/**
	 *
	 *
	 * @return @see ePBFilter 
	 *
	*/
	virtual int  checkPannelButton(int source, quint32 id, const QString& text, const QString &icon) const = 0;
};

class IPannelButton : public IPannelButtonFilter
{
public:
	virtual ~IPannelButton(){}

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerFilter(const std::string& name, IPannelButtonFilter* filter) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unRegisterFilter(const std::string& name) = 0;

	virtual QObject* createPannelButton(const quint32 id, const QString& text, const QString& icon, const QString &tip) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool destroyPannelButton(const quint32 id, QObject *obj) = 0;
};

class IPannelButtonMagr
{
public:
	virtual ~IPannelButtonMagr(){}
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerPannelButton(const std::string& name,IPannelButton* pannelbutton) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unRegisterPannelButton(const std::string& name) = 0;
	virtual IPannelButton* getPannelButton(const std::string& name) = 0;
	/** 还原频道最初始化的状态　*/
	virtual void resetPannelButton(const std::string& name) = 0;
};

/** 公告区处理器(JS的YYBulletinScriptObjectProxy处理类) */
class IBulletinHandler
{
public:
	virtual ~IBulletinHandler(){}

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool belongTo(const quint32 id) = 0;

	virtual DuiWidget* addPage(const QString &icon, const QString &text, unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void removePage(unsigned int id) = 0;
/**
   * @brief .bella,勿用，请改用IAddonHandler的setCurrentApp, by yy8.50
   * @param .
   * @return .
   */
	virtual void showPage(unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void hidePage(unsigned int id) = 0;

/**
   * @brief .bella,勿用，请改用IAddonHandler的setCurrentApp, by yy8.50
   * @param .
   * @return .
   */
	virtual void switchPage(unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void enableSwitch(unsigned int id,bool b) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString getBulletin() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QPoint getBulletinPos() = 0;

    // wanna to set prefer bulletin's height, setHeight before lock
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void lockBulletinHeight(bool b = true) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int getBulletinHeight() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setBulletinHeight(int height) = 0;

/**
   * @brief .bella,勿用，请改用IAddonHandler的setCurrentApp, by yy8.50
   * @param .
   * @return .
   */
	virtual void showPageByOW() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getPageByOW() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getCurrentPage() const = 0;
};


/** 频道中公告区(默认) */
class IBulletinView
{
public:
	virtual ~IBulletinView(){}

    /** wanna to set prefer bulletin's height, setHeight before lock
	 * 锁住高度,但如果已经被别的人锁住,就会失败
	 *
	 * @param lockname 执行锁的名称
	*/
	virtual bool lockBulletinHeight(const std::string& lockname,bool b) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int getBulletinHeight() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setBulletinHeight(int height)  = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void changedBulletinHeight() = 0;

	virtual DuiWidget* getBulletinWidget() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showBigBulletinWidget(bool b)  = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void addBulletinView(const std::string& name,DuiWidget* item) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void removeBulletinView(const std::string& name,DuiWidget* item) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void activeBulletinView(const std::string& name) = 0;
};

/** 公告区管理器 */
class IBulletinMagr
{
public:
	virtual ~IBulletinMagr(){}
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void registerHandler(const std::string& name,IBulletinHandler* handler) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unRegisterHandler(const std::string& name) = 0;
	
	virtual IBulletinHandler* belongTo(const quint32 id) const = 0;
	virtual IBulletinView* bulletinView() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void addFilterBulletinApp(const quint32 id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void removeFilterBulletinApp(const quint32 id) = 0;
	virtual IBulletinHandler* handlerByString(const std::string& name) = 0;
	virtual QObject* object() = 0;
	virtual void loadApps() = 0;

    /*
    * @brief 隐藏频道直播墙
    */
    virtual void hideDefaultChannelShower() = 0;
};

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IChannelAddonHandler) : public IDWQComponent
{
/**
   * @brief .设置插件默认处理器
   * @param .handler，插件处理器
   * @return .无
   */
	virtual void setDefaultHandler(IAddonHandler* handler) = 0;
/**
   * @brief .注册插件处理器
   * @param .name，插件名字； handler,插件处理器
   * @return .无
   */
	virtual void registerHandler(const std::string& name,IAddonHandler* handler) =0;
/**
   * @brief .反注册插件处理器
   * @param .name，插件名字
   * @return .无
   */
	virtual void unRegisterHandler(const std::string& name) = 0;

/** 
   *@brief，通过名字获取相应插件处理器
   * @param .name，插件名字
   * @return 如果不存在，返回默认处理器;
   */
	virtual IAddonHandler* handler(const std::string& name) const = 0;

/** 
	*@brief，查询id 属于那个处理器
	* @param .name，插件id
	* @return 如果不存在，返回NULL;
	*/
	virtual IAddonHandler* handlerById(const quint32 id) const = 0;
/** 
	*@brief，查询id 属于那个处理器
	* @param .name，插件id；type，插件类型
	* @return 如果不存在，返回NULL;
	*/
	virtual IAddonHandler* belongTo(const quint32 id,quint32 type) const = 0;
/** 
	*@brief，获取默认处理器
	* @param .无
	* @return 如果不存在，返回NULL;
	*/
	virtual IAddonHandler* handler() const = 0;

/** 
	*@brief，获取公告面板按钮管理器
	* @param .无
	* @return 如果不存在，返回NULL;
	*/
	virtual IPannelButtonMagr* pannelButtonMagr() const = 0;
/** 
	*@brief，获取公告管理器
	* @param 无
	* @return 如果不存在，返回NULL;
	*/
	virtual IBulletinMagr* bulletinMagr() const = 0;
/** 
	*@brief，获取公告插件（应用）列表
	* @param 无
	* @return 公告插件（应用）列表
	*/
	virtual QList<unsigned int> bulletinAppList() = 0;
	virtual void add(const quint32 id, const QString& text, bool connected, quint32 type, int source, quint32 version) = 0;
	virtual void remove(const quint32 id, bool destroy, int source) = 0;

    /** 
	*@brief，获取默认view
	* @param .无
	* @return 如果不存在，返回NULL;
	*/
    virtual IAddonViewWidget* addonView() const = 0;
/*
signals:
    void sigAppState(unsigned int id ,int state);
    void sigAppWindow(unsigned int id, int state, WId  wid, int  x, int  y, int  width, int  height, bool visible);
	void bulletinAppAdded(unsigned int appid);
	void bulletinAppRemoved(unsigned int appid);
	void sigCurBulletinAppChanged(unsigned int oldAppid, unsigned int newAppid);
*/
};

DWDEFINE_INTERFACE(IChannelAddonHandlerInner):public IUnk
{
	virtual void setAddonHandler() = 0;
	virtual void setAddonPannelButtonMagr(IPannelButtonMagr* pannel) = 0;
	virtual void setBulletinMagr(IBulletinMagr* bulletinMgr) = 0;
};