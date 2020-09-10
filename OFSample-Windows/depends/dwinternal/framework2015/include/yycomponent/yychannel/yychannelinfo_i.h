#pragma once
#include <vector>
#include "dwbase/dwcomex.h"
#include "dwbase/IData.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/* use for getObject() */
struct ChannelSimpleInfo
{
	quint32 id;
	quint32 parentId;
	QString name;
};

/**
* @brief Ƶ����Ϣ�ӿڣ���ȡƵ��������Ϣ�ֶ����ø�����YYChannelInfoHelper
*/
DWDEFINE_INTERFACE( IYYChannelInfomation ) : public IUnk
{
/**
   * @brief ��ȡһ����Ƶ������Ϣ���ֶμ�sessiontables.h.
   * @param sid����Ƶ��id.
   * @return ָ����Ƶ����Ϣ��ָ��.
   */
	virtual Data::IRowPtr getChannelRow( UINT32 sid ) = 0;
/**
   * @brief ��ȡƵ����λId.
   * @return Ƶ����λId����û�У�����Ƶ��id.
   */
	virtual UINT32 getAsid() = 0;
/**
   * @brief ��ȡƵ��id.
   * @return Ƶ��id.
   */
	virtual UINT32 getSid() = 0;
/**
   * @brief ��ȡ��ǰ��Ƶ��id.
   * @return ��ǰ��Ƶ��id.
   */
	virtual UINT32 getCurSubSid() = 0;
/**
   * @brief ��ȡƵ������.
   * @return Ƶ������.
   */
	virtual QString getChannelName() = 0;
/**
   * @brief ��ȡƵ��ģʽ.
   * @return Ƶ��ģʽ.
   */	
	virtual CHANNEL_STYLE getCurChannelStyle() = 0;
/**
	* @brief ��ȡ��ǰ��Ƶ���ı��ź�.
	* @param newSid���µ���Ƶ��id.
	* @return ��ǰ��Ƶ���ı��ź�.
	*/	
	typedef _def_boost_signals2_signal_type<void (UINT32 newSid)>::type	_SIG_CUR_SUB_SID_CHANGED;
	virtual _SIG_CUR_SUB_SID_CHANGED* getCurSubSidChangedSignal() = 0;

/**
	* @brief ��ȡƵ�������ź�.
	* @return Ƶ�������ź�.
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_JOINED;
	virtual _SIG_JOINED* getJoinedSignal() = 0;
	
/**
	* @brief ��ȡƵ���뿪�ź�.
	* @return Ƶ���뿪�ź�.
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LEAVED;
	virtual _SIG_LEAVED* getLeavedSignal() = 0;

/**
	* @brief ��ȡƵ��ģʽ�ı��ź�.
	* @param admin���ı�Ƶ��ģʽ�Ĺ���ԱUid�� newChannelStyle���µ�Ƶ��ģʽ.
	* @return Ƶ��ģʽ�ı��ź�.
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 admin, UINT32 newChannelStyle)>::type	_SIG_CUR_CHANNEL_STYLE_CHANGED;
	virtual _SIG_CUR_CHANNEL_STYLE_CHANGED* getCurChannelStyleChangedSignal() = 0;

/**
	* @brief ��ȡ͸�����ݵ����ź�.
	* @param ͸��������.
	* @return ͸�����ݵ����ź�.
	*/
	typedef _def_boost_signals2_signal_type<void (const QString&)>::type	_SIG_DATA_ARRIVED;
	virtual _SIG_DATA_ARRIVED* getDataArrivedSignal() = 0;

/**
	* @brief ��ȡ��Ƶ���б�.
	* @return ��Ƶ��id�б�.
	*/
	virtual QVector<ChannelSimpleInfo> getChannelSimpleInfos() = 0;

	virtual QString getDisplayName(UINT32 sid) = 0;

	virtual bool hasJiedai() = 0;
	virtual bool isJieDaiSubSid(UINT32 subSid) = 0;
	virtual bool hasJieDaiSubSid(UINT32 subSid) = 0;
	virtual UINT32 getSpeakerRtt(UINT32 uid) = 0;
	virtual UINT32 getPing() = 0; // �����ӳ�

	typedef _SIG_CUR_SUB_SID_CHANGED _SIG_CUR_CHANNEL_INFO_CHANGED;
	virtual _SIG_CUR_CHANNEL_INFO_CHANGED* getCurChannelInfoChangedSignal() = 0;

    // spErrInfoĿǰCPROP_ErrorHandled��ʾ��Ӵ����signal���ͻ��˲��ٴ���
	typedef _def_boost_signals2_signal_type<void (CHANNEL_ERROR_CODE, IPropBagWrapperPtr spErrInfo)>::type	_SIG_ERROR;
	virtual _SIG_ERROR *getErroredSignal() = 0;
	virtual void leave() = 0;

	typedef _def_boost_signals2_signal_type<void (DATA_NOTIFY_TYPE dataNotifyType, Data::IRowArrayPtr spNewRowArray, Data::IRowArrayPtr spOldRowArray)>::type	_SIG_DATA_NOTIFY;
	virtual _SIG_DATA_NOTIFY* getSigChannelTable() = 0;
	virtual _SIG_DATA_NOTIFY* getSigManagerList() = 0;
	typedef _def_boost_signals2_signal_type<void (Data::IFieldSetDataPtr spFieldSet)>::type	_SIG_FIELDSET_NOTIFY;
	virtual _SIG_FIELDSET_NOTIFY* getSigChannelFieldSet() = 0;


	//��Ƶ���Ż�
	virtual bool hasMoreUsers(UINT32 sid) = 0;
	virtual void asynGetMoreUsers(UINT32 sid) = 0;
	virtual void notifyUpdateUsers(const std::vector<BCUser>& users) = 0;
	virtual void getSearchResult(const std::wstring& keyword) = 0;


	typedef _def_boost_signals2_signal_type<void (const std::vector<UINT32>& results, const std::wstring& keyword,UINT8 resWithAuth)>::type _SIG_SEARCHRESULT_RES;
	virtual _SIG_SEARCHRESULT_RES* getSearchResultResSignal() = 0;

	virtual Data::IFieldSetPtr getChannelFieldSet() = 0;

	/**
	* @brief ��ȡ��ǰƵ����ʵ��������.
	*/
	virtual UINT32 getTotalUserCount() = 0;

	virtual Data::ITablePtr getChannelTable() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SUBSCRIBE_UIDS_READY;
	virtual _SIG_SUBSCRIBE_UIDS_READY *getSigSubscribeUidsReady() = 0;

	typedef _def_boost_signals2_signal_type<void (const std::vector<ETEntry>& res_entries, int status, UINT32 locateUid)>::type _SIG_SCROLLBAR_INFO;
	virtual _SIG_SCROLLBAR_INFO *getSigScrollbarInfo() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_CUR_SID_CONFIRMED;
	virtual _SIG_CUR_SID_CONFIRMED *getSigCurSidConfirmed() = 0;

	typedef _def_boost_signals2_signal_type<void (LPCTSTR,UINT32)>::type	_SIG_IMAGE_URL_RECEIVED;
	virtual _SIG_IMAGE_URL_RECEIVED*		getSigImageUrlReceived() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type _SIG_KICKEDFROMVIPCHN;
	virtual _SIG_KICKEDFROMVIPCHN* getSigKickedFromVipChannel() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type _SIG_VIPCHNREACH;
	virtual _SIG_VIPCHNREACH* getSigVipChannelReachable() = 0;

	/**
	* @brief ��ȡƵ����������Ȩ������.
	* @return 1��ʾ���������˴��֣�2��ʾ��ֹ�����˴��֣�3��ʾ��ֹ�οʹ���.
	*/
	virtual UINT32 getChannelTextStatus(UINT32 sid) = 0;

	virtual bool isExist(UINT32 uid , UINT32 sid) = 0;
	virtual bool isFolder(UINT32 sid) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 subSid)>::type _SIG_JUMP_SUB_CHANNEL_NEED_PASSWORD;
	virtual _SIG_JUMP_SUB_CHANNEL_NEED_PASSWORD *getSigJumpSubChannelNeedPassword() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_JUMP_SUB_CHANNEL_FAIL;
	virtual _SIG_JUMP_SUB_CHANNEL_FAIL *getSigJumpSubChannelFail() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 subSid, LPCWSTR tips)>::type _SIG_JUMP_SUB_CHANNEL_NEED_CHARGE;
	virtual _SIG_JUMP_SUB_CHANNEL_NEED_CHARGE *getSigJumpSubChannelNeedCharge() = 0;

	virtual bool isHugeChannel() const = 0;

	typedef _def_boost_signals2_signal_type<void (LPCWSTR, UINT32)>::type	_SIG_STAT_CHANNEL_REPORT;
	virtual _SIG_STAT_CHANNEL_REPORT*  sigStatChannelReport() = 0;

    virtual void reportJoinFromByString( const std::string& tag ) = 0;

    virtual _SIG_DATA_NOTIFY* getSigChannelTableEx(const NAME_IDS* exceptFieldFilters) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::string &strSeq, const std::string &internalId, const std::string &strImg, const std::string &strReason) >::type _SIG_NEED_VERIFY_PICCODE;
	virtual _SIG_NEED_VERIFY_PICCODE* sigNeedVerifyPicCode() = 0;

	virtual void queryAppUInfo(const std::vector<UINT32>& uids) = 0;
	typedef _def_boost_signals2_signal_type<void (const std::vector<IPropBagPtr>& res_Uinfos, int retCode)>::type _SIG_APP_QUERY_UINFO_RES;
	virtual _SIG_APP_QUERY_UINFO_RES* getSigAppQueryUinfoRes() = 0;

	virtual void getUserLocation(UINT32 uid)=0;
	typedef _def_boost_signals2_signal_type<void (const UINT32 uid, UINT8 resCode)>::type _SIG_USERLOCATION_RES;
	virtual _SIG_USERLOCATION_RES* getSigUserLocationRes() = 0;

	virtual void getSearchMoreResult(const std::wstring& keyword, UINT32 lastSearchedUid)=0;

	/**
	  * @brief ��õ�ǰƵ������Ƶ������Ա�б�
	  * @return Data::IRowArrayPtr �б���
	  */
	virtual Data::IRowArrayPtr getManagerList() = 0;

	/**
	  * @brief �����ض���Ƶ������Ա�б�
	  * @param subSid ��Ҫ��õ���Ƶ��id
	  */
	virtual void querySubChannelManagerList(const UINT32 subSid) = 0;

	/**
	  * @brief �ض���Ƶ���б����󷵻�
	  * @param subSid ��Ƶ��id
	  * @param managerColl ����Ա���Լ���
	  */
	DWDECLARE_SIGNAL(SigSubChannelManagerListResponsed, void(const UINT32 subSid, IPropBagCollPtrCR managerColl));

	/**
	* @brief ��ȡ��ǰƵ��ҵ��������������.
	* @return ����-1��ʾû�����ù�����.
	*/
	virtual INT64 getAppTotalUserCount(void) = 0;

	/**
	* @brief �ṩ��ҵ�����õ�ǰƵ����������.
	* @param nCount ��������,ֵΪ-1ʱ����ʾȡ����ǰ�����ã�Ƶ�����ڱ�����ԭ��ʾ��ʵ������.
	*/
	virtual bool setAppTotalUserCount(INT64 nCount) = 0;

	/**
	* @brief ��UED��֤��code����������.
	* @param strCode UED��֤��code.
	*/
	virtual void sendSwitchTemplateVerifyCode(const QString& strCode) = 0;

	/**
	* @brief �л�ģ����֤�ź�.
	* @param nCode  �μ�protocol::session::ETSesspOperateAuthRes::OperateResCode.
	* @param strUrl UED��֤��url.
	*/
	typedef _def_boost_signals2_signal_type<void (int nCode,const QString &strUrl) >::type _SIG_SWITCH_TEMPLATE_VERIFY;
	virtual _SIG_SWITCH_TEMPLATE_VERIFY* getSigSwitchTemplateVerify() = 0;

	/**
	* @brief �첽��ȡping ����.
	* @param 
	*/
	virtual void asynGetPing() = 0;
	
	typedef _def_boost_signals2_signal_type<void (UINT32 value)>::type _SIG_GET_PING;
	virtual _SIG_GET_PING* getSigGetPing() = 0;

	/**
	* @brief �첽��ȡSpeakerRtt ����.
	* @param 
	*/
	virtual void asynGetSpeakerRtt(UINT32 uid) = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 value)>::type _SIG_GET_SPEAKERRTT;
	virtual _SIG_GET_SPEAKERRTT* getSigGetSpeakerRtt() = 0;

    /**
    * @brief �����Ƿ���Ի���ģ����ʾ�ġ������й�����ip����ʾ
    * @param true��ʾ���ԣ�����ģ����ʽ����ʾ����ʾ��
    */
    virtual void setIgnoreLimitChinaIpDefaultTips(bool setting) = 0;
    /*
    * @brief ��ȡsetIgnoreLimitChinaIpDefaultTips���õ�ֵ
    */
    virtual bool isIgnoreLimitChinaIpDefaultTips() = 0;
    typedef _def_boost_signals2_signal_type<void ()>::type _SIG_LIMIT_CHINA_IP;
    virtual _SIG_LIMIT_CHINA_IP* getSigLimitChinaIp() = 0;


	/*
	* @brief ��ȡģ�������˳�Ƶ�����Զ��嵯���İ�
	*     ���İ�Ϊ�գ����Ե�����ʾ��
	*/
	virtual void setLeaveChannelRemindStr(const std::wstring& str) = 0;
	virtual void restoreLeaveChannelRemindStr() = 0;
	virtual std::wstring getLeaveChannelRemindString() = 0;
	
	// �������û������˷��
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_ANONYMOUS_USER;
	virtual _SIG_ANONYMOUS_USER* getSigAnonymousUser() = 0;

	/* 
	    �������/Ƶ��/�������� �Ŀ���������ڵ�������ֱ����/ǩԼƵ��channel
		���û����ڰ�ֱ����channel �ڣ��׳����ź�
		���û���ǰ���ڰ�ֱ����channel����ͨ���ɻ�Ʊ����Ƶ������Я��userData��Ϣ: {startLiveKey:0}
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 sid, UINT32 ssid)>::type _SIG_IN_CHANNEL_ALREADY;
	virtual _SIG_IN_CHANNEL_ALREADY* getSigInChannelAlready() = 0;

    /*
    ** @brief �������ѽ��루��ǰ�ն˼��������Ƶ�������ն˴�Ƶ�����߳�
    ** @param(s) now_from, froms ��getSigChannelCheckKickoff��KConfirmPCJoinChannel2�õ�
    */
    virtual void kickoff(UINT8 now_from, const std::vector<UINT8>& froms) = 0;

    /*
    ** @brief   �źš�
    **          ���ź��ڽ��볢�Խ���Ƶ���󣬼�⵽��ͬ�˺��������˽�����Ƶ��ʱ����
    **          �յ����źź�����Ҫȷ�ϴӵ�ǰ�ն˽���Ƶ��������kickoff�ӿڣ�����ֱ�Ӵ���
    **          ����Ӧ�ôӵ�ǰ�ն��˳���ǰƵ��
    **          ��ͬ��KConfirmPCJoinChannel2
    ** @param now_from ��ʾ��ǰ���Խ���Ƶ�����ն�id
    ** @param froms    ��ʾ�ѽ�����Ƶ���������ն�id�б�
    */
    typedef _def_boost_signals2_signal_type<void (UINT8 now_from, const std::vector<UINT8>& froms)>::type _SIG_CHANNEL_CHECK_KICKOFF;
    virtual _SIG_CHANNEL_CHECK_KICKOFF* getSigChannelCheckKickoff() = 0;

	/*
	* @brief �˳�Ƶ���Զ��嵯������(���ñ�־�󣬾ͻ��׳��ź�_SIG_LEAVE_CHANNEL_TAKEOVER)
	* @param tmpId: ģ��ID �������ID������Ϊ0��������Ч�������ڲ��ҵ��ĸ�ҵ�������˸ñ�־��
	*/
	virtual void setLeaveChannelTakeoverFlag(UINT32 tmpId) = 0;
	/*
	*@brief ��resetLeaveChannelTakeoverFlag����ʹ�ã����ñ�־λ0
	*/
	virtual void resetLeaveChannelTakeoverFlag() = 0;
	virtual bool getLeaveChannelTakeoverFlag() = 0;
	
	/*
	* @brief ҵ������_SIG_LEAVE_CHANNEL_TAKEOVER�źź󣬻ص��˽ӿڽ��ɿͻ��˴���
	* ҵ����ô˻ص��ӿڣ��ͻ��˻����resetLeaveChannelTakeoverFlag���ñ�־������
	*/
	virtual void leaveChannelTakeoverCallback(const QMap<int,QVariant>&) = 0;

	/*
	* @brief ׼���˳�Ƶ���źţ�ͨ��setLeaveChannelTakeoverFlag�ӿ����ñ�־��Ż��׳�
	*/
	typedef _def_boost_signals2_signal_type<void (const QMap<int,QVariant>&)>::type _SIG_LEAVE_CHANNEL_TAKEOVER;
	virtual _SIG_LEAVE_CHANNEL_TAKEOVER* getSigLeaveChannelTakeover() = 0;
	
	virtual bool isSuperChannel() const = 0;
	virtual bool isChannelTreeReady() const = 0;
};

DWDEFINE_SIGNAL(IYYChannelInfomation::SigSubChannelManagerListResponsed);
