#pragma once
#include "dwbase/dwcomex.h"
#include "dwbase/yycomclsid.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/yyvideosdkinfo_i.h"
#include "biz/biz_types.h"
#include "qstring"

class QString;


/*
一般，State指return true或者不中断执行流程的(不return，继续执行)
Error对应return false;
*/

enum YYComStoreCode
{
	YYComStoreCode_Begin = 0,
	YYComStoreCode_CreatePathFailed,
	YYComStoreCode_NoPluginInXml,
	YYComStoreCode_NoMatchVersionInXml,
	YYComStoreCode_NotExistLocalVersion,
	YYComStoreCode_DownloadXmlFailed,
	YYComStoreCode_Un7zXmlFailed,
	YYComStoreCode_DelTempFileFailed,
	YYComStoreCode_SaveTempFileFailed,
	YYComStoreCode_DelDestXmlFailed,
	YYComStoreCode_RenameDestXmlFailed,
	YYComStoreCode_XmlNoComponent,
	YYComStoreCode_XmlProcessFinish,

	YYComStoreCode_ComponentOk,
	YYComStoreCode_ComponentOKOfExist,

	YYComStoreCode_Error_Begin = 2000,
	YYComStoreCode_Error_ActivePending  = YYComStoreCode_Error_Begin,
	YYComStoreCode_Error_ActiveDownload,
	YYComStoreCode_Error_ActiveInstall,
	YYComStoreCode_Error_ActiveChecking,
};

enum DataReportType
{
	// click
	ComButtonClickByMainApp = 100,
	ComButtonClickByChannelChat = ComButtonClickByMainApp + 1,
	ComButtonClickByChannelTool = ComButtonClickByMainApp + 2,
	ComButtonClickByAppView		= ComButtonClickByMainApp + 3,

};

DWDEFINE_INTERFACE(IYYComStoreTask):public IDWQComponent
{
	virtual bool start() = 0;	//强制启动下载，走不限速的CDN。
	virtual bool stop() = 0;
	virtual void showProgress(bool b, int x, int y) = 0;
	virtual void showCompleteUI(bool b) = 0;
	virtual void reportDelayLoad() = 0;
	
	virtual void queryProgress(int &total, int &current) = 0;

	typedef _def_boost_signals2_signal_type<void ( int total, int current )>::type	_SIG_PROGRESS;
	virtual _SIG_PROGRESS *getProgressSignal() = 0;

	virtual void setTicketData(const QString &data, bool binary) = 0;
/*
signals:
    void internal_finish(const QString& name);
    void install_status(const QString& name, int step, int total, int current);
    void install_complete(const QString& name, int code, const QString& instPath);
*/
};

DWDEFINE_INTERFACE(IYYComStoreLoginState) : public IUnk
{
	virtual void setYYLogined(bool logined) = 0;
	virtual void quitApp(int returnCode) = 0;
};

/** 
 *  可以连接IYYComStore内部以下信号，以便跟踪流程中的各种状态：
 *    void yycomstorecode(int code);
 */

/**
 *  可以连接安装组件任务的以下两个信号，通过IYYComStoreTask接口连接：
 *    void install_status(const QString&, int, int, int);
 *    void install_complete(const QString&, int, const QString&);
 */

/**
 * IYYComStoreTask接口指针只能使用时获取，不能长期持有
 */

DWDEFINE_INTERFACE(IYYComStore) : public IDWQComponent
{
	typedef IYYVideoSDKInfo::VersionData VersionData;	
	/** 初始化动态下载
	 *  [return]      : 成功true,失败false
	 */
	virtual bool init() = 0;

	/** YY登录下载yycomstore.xml文件，之前在init里面下载，后来由于一些内置组件在登录前加载IYYComStore需要先初始化，故分开
	 * 
	 */
	virtual void realYYLogin(unsigned int uid);

	/** 加载组件
	 *  pTask    : 返回一个IYYComStoreTask接口，可以连接信号和控制任务
	 *  name     : 组件名称
	 *  force    : 强制下载组件，即不使用本地已经存在的组件
	 *  channelId: 有的组件是采用频道灰度的
	 *  [return] : 成功true,失败false
	 */
	/*******************************************************
	*****  IYYComStoreTask指针只能使用时获取，不能持有  ****
	*******************************************************/
	virtual IYYComStoreTaskWeakPtr install(
		const std::wstring& name,
		bool force/* = false*/,
		unsigned int channelId
	) = 0;

	/** 查询本地组件所在文件夹
	 *  name     : 组件名称
	 *  [return] : 查询到的路径，没有则为空
	 *  仅查询适合的版本，不一定是最新的。
	 */
	virtual std::wstring findComponentDir(
		const std::wstring& name
	) = 0;

	/** 查询本地组件entryDll
	*  name     : 组件名称
	*  [return] : 查询到的路径，没有则为空
	*  仅查询适合的版本，不一定是最新的。
	*/
	virtual std::wstring findComponentEntryDll(
		const std::wstring& name
		) = 0;

	/** 获取组件任务接口
	 *  name     : 组件名称
	 *  [return] : IYYComStoreTask接口指针
	 */
	/*******************************************************
	*****  IYYComStoreTask指针只能使用时获取，不能持有  ****
	*******************************************************/
	virtual IYYComStoreTaskWeakPtr getTask(
		const std::wstring& name
	) = 0;

	/** 上报点击数据
	 *  name     : 组件名称
	 *  version  : 组件版本号，外面可能取不到
	 *  type     : 上报类型
	 *  [return] : 无
	 */
	virtual void reportClick(
		const std::wstring& name,
		unsigned int version,
		int type
	) = 0;

	/** 上报点击数据
	 *  id       : 组件ID
	 *  version  : 组件版本号，外面可能取不到
	 *  type     : 上报类型
	 *  param    : 自定义的数据
	 *  other    : 附加数据
	 *  [return] : 无
	 */
	virtual void reportClickById(
		unsigned long id,
		unsigned int version,
		int type,
		unsigned int param,
		const std::string& other
	) = 0;

	virtual IUnkPtr componentEntry(int id) = 0;
	virtual bool	isNewComponent(int id) = 0;
	virtual int		loadComponentEx(const char* name, bool download, bool start) = 0;
	virtual int		startComponent( const char* componentAlias, const char* args ) = 0;
	virtual bool	getAllVersions(unsigned int id, unsigned int channelId, std::vector<VersionData> &ret) = 0;

    /** 获取组件扩展字段信息
	 *  id       : 组件ID
	 *  tagName  : 扩展字段名称
	 */
    virtual std::string	getComponentExpand(int id, const char *tagName) = 0;

    /** 设置组件自定义控制信息
	 *  json  : 控制信息json
	 */
    virtual void    setControl(const char* json) = 0;

	virtual void addAppService(const char* data, int length) = 0;
	virtual void addAppComponent(const char* data, int length) = 0;

	/** Component被点击后调用
	  *  key	: 传递的信息
	  */
	virtual void triggerComponentByKey(const char *key) = 0;
};

DWDEFINE_INTERFACE(IYYTemplateSupport): public IUnk
{
	enum Stage
	{
		Download,
		Load
	};
	typedef _def_boost_signals2_signal_type<void ( const QString &name, quint32 id, Stage s, IYYComStoreTaskPtr task)>::type	_SIG_TEMPLETE_LOAD;
	virtual _SIG_TEMPLETE_LOAD *getTemplateLoadSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (const QString &name, quint32 id, Stage s, int error)>::type _SIG_TEMPLETE_LOAD_ERROR;
	virtual _SIG_TEMPLETE_LOAD_ERROR *getTemplateLoadErrorSignal() = 0;
};
