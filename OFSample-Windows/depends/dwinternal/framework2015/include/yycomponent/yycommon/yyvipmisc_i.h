#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/**
   * @brief vip������ݽӿ�.
   */
DWDEFINE_INTERFACE(IYYVipMisc) : public IUnk
{
	/**
   * @brief ��ȡ����vip��Ϣ�����ź�.
   * @param .
   * @return vip����汾�Ÿ���֪ͨ�ź�.
   */
	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32)>::type _SIG_MY_VIPINFO_CHANGED;
	virtual _SIG_MY_VIPINFO_CHANGED *myVipInfoChangedSignal() = 0;

	/**
   * @brief ��ȡvip���������.
   * @param .
   * @return vip�������������֪ͨ�ź�.
   */
	typedef _def_boost_signals2_signal_type<void (std::vector<UINT32>)>::type _SIG_VIP_WHITELIST_RES;
	virtual _SIG_VIP_WHITELIST_RES *vipWhiteListResSignal() = 0;

	/**
   * @brief �첽��ȡvip�����ƹ��б�.
   * @param .
   * @return . 
   */
	virtual void asyncGetPopularize() = 0;

	/**
   * @brief �첽��ȡvip���������.
   * @param .
   * @return . 
   */
	virtual void asyncGetVipExpWhiteList(const std::vector<UINT32>& vecUids) = 0;

	/**
   * @brief  ��ȡvip�����ƹ��б�.
   * @param 
   * @return vip�����ƹ��б����֪ͨ�ź�.
   */
	struct PopularizeAttr
	{
		int     m_uGrpId;         // �������ID
		std::wstring  m_strKey;         // ����key
	};
	typedef std::vector<PopularizeAttr> PopularizeAttrList;

	typedef _def_boost_signals2_signal_type<void (const PopularizeAttrList&)>::type _SIG_POPULARIZE_RES;
	virtual _SIG_POPULARIZE_RES *popularizeResSignal() = 0;

	virtual void getUserOffLineScore( const UINT32 uid,const UINT32 from ) = 0;

	typedef _def_boost_signals2_signal_type<void ( const UINT32& uid,const UINT32& score,const UINT32& add_score )>::type _SIG_GET_USER_OFFLINE_SCORE_FOR_CHANNEL;
	virtual _SIG_GET_USER_OFFLINE_SCORE_FOR_CHANNEL *getSigGetOfflineScoreForChannel() = 0;


	// ��ѯ����վ����
	virtual void asyncGetRecycleBuddy(UINT32 uLastPos, UINT32 uCount) = 0;

	// �ָ�����
	virtual void asyncRestoreRecycleBuddy(const std::vector<UINT32>& vecBid) = 0;

	// ��ѯ����վ������Ӧ
	typedef _def_boost_signals2_signal_type<void (UINT32 totalCount)>::type _SIG_RECYCLE_BUDDIES_READY;
	virtual _SIG_RECYCLE_BUDDIES_READY *recycleBuddiesReadySignal() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 currentPos, IPropBagCollPtrCR buddiesUinfo)>::type _SIG_RECYCLE_BUDDIES_RECEIVED;
	virtual _SIG_RECYCLE_BUDDIES_RECEIVED* recycleBuddiesReceivedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 resCode)>::type _SIG_RECYCLE_BUDDIES_RECEIVED_ERROR;
	virtual _SIG_RECYCLE_BUDDIES_RECEIVED_ERROR *recycleBuddiesReceivedErrorSignal() = 0;

	// �ָ�������Ӧ
	typedef _def_boost_signals2_signal_type<void (UINT32 uRescode, const std::vector<UINT32>& vecBidSuc, const std::vector<UINT32>& vecBidFailed)>::type _SIG_RESTORE_BUDDIES_RES;
	virtual _SIG_RESTORE_BUDDIES_RES *restoreBuddiesResSignal() = 0;

    // ��Ա��Ϣ
    typedef _def_boost_signals2_signal_type<void (UINT32 id, const std::string& msg)>::type _SIG_VIP_MSG;
    virtual _SIG_VIP_MSG *vipMsgSignal() = 0;

	virtual std::wstring queryUrl(const std::wstring &key) = 0;

    struct PurpleInfo
    {
        UINT8 grade;
        UINT8 type;
        PurpleInfo() { grade = type = 0; }
    };
    virtual PurpleInfo getPurpleInfo(UINT32 uid) = 0;  // �����л�������Ϸ���
    // grade 0 �������Ա  1 ~ 7 ,  type 0 �·�  1 ���
    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 grade, UINT32 type)>::type _SIG_PURPLE_INFO_RES;
    virtual _SIG_PURPLE_INFO_RES *purpleInfoSignal() = 0;

    virtual UINT8 myPurpleGrade() = 0;
    virtual UINT8 myPurpleType() = 0;

    enum EPurplePrivi
    {
        E_PURPLE_NICK = 0,       // ��ɫ�ǳ�
        E_PURPLE_SUPER,          // ��������
        E_PURPLE_EXVIP,          // �����Ա
        E_PURPLE_GAME,           // ��Ϸ����
		E_SUPER_VIP,             //������Ա
    };

    virtual int getPurplePrivi(UINT32 uid, EPurplePrivi privi) = 0;  // �����л�������Ϸ���, -1��ʾ������

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, EPurplePrivi privi, int value)>::type _SIG_PURPLE_PRIVI_RES;
    virtual _SIG_PURPLE_PRIVI_RES *purplePriviSignal() = 0;

    virtual void GetOwVipInfo(UINT32 uid) = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT8 isOwvip)>::type _SIG_OWVIP_RES;
    virtual _SIG_OWVIP_RES *owvipInfoSignal() = 0;

    virtual UINT8 isOwvip() = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 score, UINT32 addScore, UINT32 type)>::type _SIG_VIP_PURPLE_SCORE_RES;
    virtual _SIG_VIP_PURPLE_SCORE_RES *getSigPurpleScore() = 0; 

    virtual void getPurpleScore(UINT32 type, UINT32 uid) = 0;

    enum { PrettyNumAppId = 6 };
    virtual QString miniLightInfo(UINT32 appId, UINT32 dengji, const QString& key) = 0;

   /**
   * @brief  ��ȡƵ���������Ż���ͼ����Ϣ.
   * @param	dengji�����Ż���ͼ��ȼ�
   * @return ���Ż���ͼ����Ϣ.
   */
	virtual IPropBagWrapperPtr getVipPrettyNumInfo(UINT32 dengji) = 0;
};