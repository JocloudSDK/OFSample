#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

define_interface(IChannel);
/**
* @brief �ڲ��ӿڣ�����Ƶ��ָ�룬��������.
*/
DWDEFINE_INTERFACE(IYYVideoRegulatoryInner): public IUnk
{
/**
   * @brief ����Ƶ��ָ��.
   * @param spChannel��Ƶ��ָ��.
   */
	virtual void setChannel(IChannelPtr spChannel) = 0;
};



// ����������״̬
struct CompereStatus
{
	UINT32 uid;
	UINT32 reportedCount; // �ٱ�����
	UINT32 version;
	UINT32 fine; // ������
	bool isPunished; // �Ƿ񱻴���

	CompereStatus() : uid(0), reportedCount(0), version(0), fine(0), isPunished(false){}
};

// ����Ȩ����Ϣ����������
struct ComperePrivilege
{
	UINT32 uidMaxCount; // �����Ĺۿ���������, -1��ʾ������
	UINT32 channelMaxCount; // Ƶ���Ĺۿ��������ޣ�Ŀǰֻ�����ֻ��õ������
};

struct UserImage
{
	UINT32 uid; // ���ٱ����û�
	std::vector<std::string> images; // ���ٱ��û���ͼƬ֤�ݣ����Զ���
};


DWDEFINE_INTERFACE(IYYVideoRegulatory) : public IUnk
{
	/**
	* �ṩ����ҵ��ľٱ����
	* @param appid ҵ��id
	* @param compereId ���ٱ���uid
	* @param uids ����Ƶ���û�id�������ж�������������Ƶ���������������uid
	*/
	virtual void report(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids) = 0;

	/**
	* ��Ƶ����ʱ�򣬲�ѯ������״̬ (�Ƿ񱻾ٱ����첽ȡ�ģ�ͨ����_SIG_COMPERE_STATUS����ȡ���)
	* @param uids ����Ƶ���û�id�������ж�������������Ƶ��
	*/
	virtual void queryCompereStatus(const QList<UINT32>& uids) = 0;

	/**
	* �����ı��ʱ��֪ͨ������ (�����������Ƿ���Ҫ�㲥��ͨ����_SIG_COMPERE_STATUS����ȡ���)
	* @param uids ����Ƶ���û�id�������ж�������������Ƶ��
	*/
	virtual void notifyCompereChanged(const QList<UINT32>& uids) = 0;

	// ������״̬�����ٱ���������Ϣ��
	typedef _def_boost_signals2_signal_type<void (const QList<CompereStatus>&)>::type _SIG_COMPERE_STATUS;
	virtual _SIG_COMPERE_STATUS* getCompereStatusSignal() = 0;

	// ��ѯ������Ƶ���Ĺۿ�ֱ����������
	virtual void queryComperePrivilege(UINT32 appId, UINT32 uid, UINT32 curSid) = 0;

	typedef _def_boost_signals2_signal_type<void (ComperePrivilege)>::type _SIG_COMPERE_PRIVILEGE;
	virtual _SIG_COMPERE_PRIVILEGE* getComperePrivilegeSignal() = 0;

	// ����Υ��֪ͨ����ҵ���Լ�������
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_COMPERE_PUNISHED;
	virtual _SIG_COMPERE_PUNISHED* getComperePunishedSignal() = 0;

	/**
	* �ṩ������ƽ̨�ľٱ����
	* @param appid ҵ��id
	* @param owId OW��UID
	* @param uidImages ���ٱ����û�֤�ݣ����Ծٱ�����û���ÿ���û������ж���ͼƬ��
	*/
	virtual void reportWithUserData(UINT32 appId, UINT32 owId, const std::list<UserImage>& userImages) = 0;

    // ����Υ������Ƶ���������㲥��ҵ��
    typedef _def_boost_signals2_signal_type<void (const QString&)>::type _SIG_CHANNEL_VIDEO_BAN;
    virtual _SIG_CHANNEL_VIDEO_BAN* getChannelVideoBanSignal() = 0;

	/**
	* �ṩ����ҵ��ľٱ����
	* @param appid ҵ��id
	* @param compereId ���ٱ���uid
	* @param uids ����Ƶ���û�id�������ж�������������Ƶ���������������uid
	* @param showPageIndex ��ʾҳ��������0��ʾ��ʾ��Ƶҳ��1��ʾ����ҳ.
	*/
	virtual void report2(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids,INT32 showPageIndex) = 0;

	/**
	* �ṩ����ҵ��ľٱ����
	* @param appid ҵ��id
	* @param compereId ���ٱ���uid
	* @param uids ����Ƶ���û�id�������ж�������������Ƶ���������������uid
	* @param names uid��Ӧ���ǳ�map�б�.
	* @param showPageIndex ��ʾҳ��������0��ʾ��ʾ��Ƶҳ��1��ʾ����ҳ.
	*/
	virtual void report3(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids, const QMap<UINT32,QString>& names,INT32 showPageIndex) = 0;
};