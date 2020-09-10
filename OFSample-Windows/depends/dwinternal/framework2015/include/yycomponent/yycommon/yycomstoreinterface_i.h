#pragma once
#include "dwbase/dwcomex.h"
#include "dwbase/yycomclsid.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/yyvideosdkinfo_i.h"
#include "biz/biz_types.h"
#include "qstring"

class QString;


/*
һ�㣬Stateָreturn true���߲��ж�ִ�����̵�(��return������ִ��)
Error��Ӧreturn false;
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
	virtual bool start() = 0;	//ǿ���������أ��߲����ٵ�CDN��
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
 *  ��������IYYComStore�ڲ������źţ��Ա���������еĸ���״̬��
 *    void yycomstorecode(int code);
 */

/**
 *  �������Ӱ�װ�����������������źţ�ͨ��IYYComStoreTask�ӿ����ӣ�
 *    void install_status(const QString&, int, int, int);
 *    void install_complete(const QString&, int, const QString&);
 */

/**
 * IYYComStoreTask�ӿ�ָ��ֻ��ʹ��ʱ��ȡ�����ܳ��ڳ���
 */

DWDEFINE_INTERFACE(IYYComStore) : public IDWQComponent
{
	typedef IYYVideoSDKInfo::VersionData VersionData;	
	/** ��ʼ����̬����
	 *  [return]      : �ɹ�true,ʧ��false
	 */
	virtual bool init() = 0;

	/** YY��¼����yycomstore.xml�ļ���֮ǰ��init�������أ���������һЩ��������ڵ�¼ǰ����IYYComStore��Ҫ�ȳ�ʼ�����ʷֿ�
	 * 
	 */
	virtual void realYYLogin(unsigned int uid);

	/** �������
	 *  pTask    : ����һ��IYYComStoreTask�ӿڣ����������źźͿ�������
	 *  name     : �������
	 *  force    : ǿ���������������ʹ�ñ����Ѿ����ڵ����
	 *  channelId: �е�����ǲ���Ƶ���Ҷȵ�
	 *  [return] : �ɹ�true,ʧ��false
	 */
	/*******************************************************
	*****  IYYComStoreTaskָ��ֻ��ʹ��ʱ��ȡ�����ܳ���  ****
	*******************************************************/
	virtual IYYComStoreTaskWeakPtr install(
		const std::wstring& name,
		bool force/* = false*/,
		unsigned int channelId
	) = 0;

	/** ��ѯ������������ļ���
	 *  name     : �������
	 *  [return] : ��ѯ����·����û����Ϊ��
	 *  ����ѯ�ʺϵİ汾����һ�������µġ�
	 */
	virtual std::wstring findComponentDir(
		const std::wstring& name
	) = 0;

	/** ��ѯ�������entryDll
	*  name     : �������
	*  [return] : ��ѯ����·����û����Ϊ��
	*  ����ѯ�ʺϵİ汾����һ�������µġ�
	*/
	virtual std::wstring findComponentEntryDll(
		const std::wstring& name
		) = 0;

	/** ��ȡ�������ӿ�
	 *  name     : �������
	 *  [return] : IYYComStoreTask�ӿ�ָ��
	 */
	/*******************************************************
	*****  IYYComStoreTaskָ��ֻ��ʹ��ʱ��ȡ�����ܳ���  ****
	*******************************************************/
	virtual IYYComStoreTaskWeakPtr getTask(
		const std::wstring& name
	) = 0;

	/** �ϱ��������
	 *  name     : �������
	 *  version  : ����汾�ţ��������ȡ����
	 *  type     : �ϱ�����
	 *  [return] : ��
	 */
	virtual void reportClick(
		const std::wstring& name,
		unsigned int version,
		int type
	) = 0;

	/** �ϱ��������
	 *  id       : ���ID
	 *  version  : ����汾�ţ��������ȡ����
	 *  type     : �ϱ�����
	 *  param    : �Զ��������
	 *  other    : ��������
	 *  [return] : ��
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

    /** ��ȡ�����չ�ֶ���Ϣ
	 *  id       : ���ID
	 *  tagName  : ��չ�ֶ�����
	 */
    virtual std::string	getComponentExpand(int id, const char *tagName) = 0;

    /** ��������Զ��������Ϣ
	 *  json  : ������Ϣjson
	 */
    virtual void    setControl(const char* json) = 0;

	virtual void addAppService(const char* data, int length) = 0;
	virtual void addAppComponent(const char* data, int length) = 0;

	/** Component����������
	  *  key	: ���ݵ���Ϣ
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
