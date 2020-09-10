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
const quint32 KBaseBulletinId = 6001; //�������� appid(openplatform server config)
const quint32 KDefBulletinId = 6002; //������Ƭ,onaircard
/** �����״̬ */
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

/** ���������(���ʱ)*/
class IAddonFilter
{
public:
	virtual ~IAddonFilter(){};

	enum eFilter
	{
		Filter_unk = 0,
		Filter_belongto,///< ����ʹ��
		Filter_invalid, ///< ��Ч,����ʹ��
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
	 * @return true,�������Ӧ��
	*/
	virtual int  addonFilter(const quint32 id, quint32 type,int from) = 0;
};

class IAddonHandler;
/** 
* @brief ��������@ʾ�^�ӿ�.
*/

class IAddonViewWidget
{
public:
	virtual ~IAddonViewWidget(){};

/**
   * @brief ������������Ƿ�����������.
   * @param id�����id.
   * @return �Ƿ�����������.
   */
	virtual bool belongTo(const quint32 id) = 0;

/**
   * @brief ���ò��������.
   * @param handler��������.
   */
	virtual void setAddonHandler(IAddonHandler* handler) = 0;
/**
   * @brief �����Ƿ�ȫ����ʾ.
   * @param id�����id.
   * @param full���Ƿ�ȫ����ʾ.
  */
	virtual void setFullScreen(const quint32 id,bool full) = 0;
/**
   * @brief �����Ƿ���ʾȫ����ť.
   * @param id�����id.
   * @param show���Ƿ���ʾ.
   */
	virtual void showFullScreenButton(const quint32 id, bool show) = 0;
/**
   * @brief ����ȫ����ť�Ƿ���Ч.
   * @param id�����id.
   * @return enable���Ƿ���Ч.
   */
	virtual void enableFullScreenButton(const quint32 id , bool enable) = 0;


	/**
	* @brief �����������������(�繤�����еİ�ť).
	* @param id�����id.
	* @param item�����������.
	*/
	virtual void hideItem(const quint32 id, DuiWidget* item) = 0;

	/**
	* @brief �����@ʾ�^����.
	* @param id�����id.
	* @param item�����������.
	*/
	virtual void hideItemOfView(const quint32 id, DuiWidget* item) = 0;
/**
   * @brief ��ʾĳ��widget.
   * @param id�����id.
   * @param item��Ҫ��ʾ��widget.
   * @param index����ʾλ��.
   * @return .
   */
	virtual void showItem(const quint32 id, DuiWidget* item, int index) = 0;
	
	/**
	* @brief ���widget.
	* @param id�����id.
	* @param item��Ҫ��ӵ�widget.
	* @param text����ʾ���ı�.
	* @param imagePath��ͼ��·��.
	* @param tip��tooltip.
	* @return index��λ��.
	*/
	virtual void createItem(const quint32 id, DuiWidget* item, const QString& text, const QString& imagePath, const QString &tip, int index) = 0;
	/** ɾ�� */
	/**
	* @brief �Ƴ����.
	* @param id�����id.
	* @param item����Ӧ��widget.
	*/
	virtual void removeItem(const quint32 id,DuiWidget* item) = 0;
	

	/**
	* @brief �{�����.
	* @param order��˳���б�.
	*/
	virtual void orderItem(const std::vector<quint32>& order) = 0;

	/**
	* @brief ��ȡӦ��widget.
	* @return Ӧ��widge.
	*/
	virtual DuiWidget* getAppWidget() = 0;
/**
   * @brief �Ƿ���ʾ.
   * @return �Ƿ���ʾ.
   */
	virtual bool isShow() const = 0;
	/**
	* @brief �����Ƿ���ʾ.
	* @param show���Ƿ���ʾ.
	*/
	virtual void showSelf(bool show) = 0;

/**
   * @brief �����Ƿ���״̬.
   * @param id�����id.
   * @param check���Ƿ���״̬.
   */
	virtual void checkButton(const quint32 id, bool check) = 0;	

    /**
	* @brief ���ù����������.
	* @param type, ˮƽ or ��ֱ.
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

	/** ָ��VIEW�Ƿ�ɼ�
	 *
	 * @name ="",ָ��ǰVIEW��ʾ��
	*/
    virtual bool isViewVisible(const std::string& name) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
    virtual void setViewVisible(IAddonViewWidget* view,bool visible) = 0;

	virtual QObject* object() = 0;

	/** �������Ƿ��������������(һ�����ֻ�ܴ���һ��������) */
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool belongTo(const quint32 id, quint32 type) = 0;
	/** ���뵽cache�У�����������ʾ��, Asynchronous wait connect state!
	 *
	 * @param isConnect =true,��Ҫ����·�����ź�
	*/
    virtual bool add(const quint32 id, const QString& text, bool connected, quint32 type, int source) = 0;

	/** ������ʾ����*/
	virtual bool create(const quint32 id, const QString& text, const QString& imagePath, const QString &tip) = 0;
	/* show AND startup
    ** �ӿ����á�create��ʱ����һ��startup��
    ** ����ע�⣡���ö��������������setCurrentApp
    ** YYXMII-53106, by yy8.50
    */
	virtual void startup(const quint32 id) = 0;

	/** �ر���ʾ����������cache��ɾ����*/
	virtual bool close(const quint32 id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool hide(const quint32 id) = 0;
	/** �ر���ʾ��
	 *@param destroy =true ��cache��ɾ��
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
   * @brief .�ö������������������
   * @param .
   * @return .
   */
	virtual void setCurrentApp(const quint32 curid) = 0;


    /**
   * @brief .ͨ�����ֲ���ע���ˣ�registerViewWidget����view
   * @param .
   * @return .
   */
    virtual IAddonViewWidget* findViewWidget(const std::string& name) const = 0;

    /**
    * @brief .���ù����������ʾ���ȼ�
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
		/** ��� PBSource_create */
		PBFilter_create, ///< ��Ҫ����
		PBFilter_null, ///< ������ֵ

		/** ��� PBSource_destroy*/
		PBFilter_delete,
	};

	enum ePBState
	{
		PBState_unk = 0,
		// һ��
		PBState_show = 1,
		PBState_hide = 2,
		// һ��
		PBState_disable = 0x10,
		PBState_enable  = 0x20,
	};
	/** 
	 * ����֮ǰ��Ҫѯ��һ��,���Ըı��ʼֵ.
	 *
	 * @param source @see ePBSource
     *
	 * @param id [in/out] �������
	 * @param text [in/out]
	 * @param icon [in/out]
	 * @param state ״̬ @see ePBState
	 *
	*/
	virtual bool preCreatePannelButton(quint32& id, QString& objname, QString& text, QString &icon, QString& tip, int& state) = 0;

	/**
	 * ������֮ǰѯ��һ��,�������ɾ��,���Ը���state��������״̬
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
	/** ��ԭƵ�����ʼ����״̬��*/
	virtual void resetPannelButton(const std::string& name) = 0;
};

/** ������������(JS��YYBulletinScriptObjectProxy������) */
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
   * @brief .bella,���ã������IAddonHandler��setCurrentApp, by yy8.50
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
   * @brief .bella,���ã������IAddonHandler��setCurrentApp, by yy8.50
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
   * @brief .bella,���ã������IAddonHandler��setCurrentApp, by yy8.50
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


/** Ƶ���й�����(Ĭ��) */
class IBulletinView
{
public:
	virtual ~IBulletinView(){}

    /** wanna to set prefer bulletin's height, setHeight before lock
	 * ��ס�߶�,������Ѿ����������ס,�ͻ�ʧ��
	 *
	 * @param lockname ִ����������
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

/** ������������ */
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
    * @brief ����Ƶ��ֱ��ǽ
    */
    virtual void hideDefaultChannelShower() = 0;
};

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IChannelAddonHandler) : public IDWQComponent
{
/**
   * @brief .���ò��Ĭ�ϴ�����
   * @param .handler�����������
   * @return .��
   */
	virtual void setDefaultHandler(IAddonHandler* handler) = 0;
/**
   * @brief .ע����������
   * @param .name��������֣� handler,���������
   * @return .��
   */
	virtual void registerHandler(const std::string& name,IAddonHandler* handler) =0;
/**
   * @brief .��ע����������
   * @param .name���������
   * @return .��
   */
	virtual void unRegisterHandler(const std::string& name) = 0;

/** 
   *@brief��ͨ�����ֻ�ȡ��Ӧ���������
   * @param .name���������
   * @return ��������ڣ�����Ĭ�ϴ�����;
   */
	virtual IAddonHandler* handler(const std::string& name) const = 0;

/** 
	*@brief����ѯid �����Ǹ�������
	* @param .name�����id
	* @return ��������ڣ�����NULL;
	*/
	virtual IAddonHandler* handlerById(const quint32 id) const = 0;
/** 
	*@brief����ѯid �����Ǹ�������
	* @param .name�����id��type���������
	* @return ��������ڣ�����NULL;
	*/
	virtual IAddonHandler* belongTo(const quint32 id,quint32 type) const = 0;
/** 
	*@brief����ȡĬ�ϴ�����
	* @param .��
	* @return ��������ڣ�����NULL;
	*/
	virtual IAddonHandler* handler() const = 0;

/** 
	*@brief����ȡ������尴ť������
	* @param .��
	* @return ��������ڣ�����NULL;
	*/
	virtual IPannelButtonMagr* pannelButtonMagr() const = 0;
/** 
	*@brief����ȡ���������
	* @param ��
	* @return ��������ڣ�����NULL;
	*/
	virtual IBulletinMagr* bulletinMagr() const = 0;
/** 
	*@brief����ȡ��������Ӧ�ã��б�
	* @param ��
	* @return ��������Ӧ�ã��б�
	*/
	virtual QList<unsigned int> bulletinAppList() = 0;
	virtual void add(const quint32 id, const QString& text, bool connected, quint32 type, int source, quint32 version) = 0;
	virtual void remove(const quint32 id, bool destroy, int source) = 0;

    /** 
	*@brief����ȡĬ��view
	* @param .��
	* @return ��������ڣ�����NULL;
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