#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yychannelmessageedit_i.h"
#include "sysbase/tool/xsignals.h"

namespace sendError
{
	enum SendErrorCode
	{
		SendOk = 0,
		KeywordFilter = 1,
		ManagerForbidText = 2,
		SayTooFastAfterJoined = 3,
		SendTooFast = 4,
		SendTextLong = 5,
		NoPermissionSendYYTick = 6,
		NoPermissionSendUrl = 7,
		SendRepeatMessage = 8,
		TooMuchYYLink = 9,
		TooMuchMagicSmile = 10,
		TooMuchTicketImage = 11,
		CantSendImagePrivate = 12,
		SendImageTooFast = 13,
		TextTransformed = 14,
		NeedBindMobile = 15,
		EmptyText = 16
	};

	enum ErrorLevel
	{
		None // ������.
		,Information // ��Ϣ
		,Prompt // ��ʾ
		,Warning // ����
	};

	struct SendError
	{
		SendError()
			:level(None)
			,code(SendOk)
		{}
		typedef QVariantList Args;
		ErrorLevel level;
		SendErrorCode code;
		QString description;
		Args args;
	};

}
/**
* @brief ������Ϣ.
*/
DWDEFINE_INTERFACE(IYYChannelMessage) : public IUnk
{
/**
   * @brief ������Ϣ.
   * @param message����Ϣ����.
   * @return ��SendErrorCode.
   */
	virtual int sendMessage( const std::wstring& message ) = 0;
};

enum MessageListType
{
	DefaultMessageList,
	UserMessageList
};

enum ChannelSystemMessageType
{
	NoneType = 0,

	RoleChangeEchoType = 1 ,	//�û���ɫ�仯��ʾ
	RoleChangeByInviteEchoType = 2, //�������ɫ�仯

	StyleTypeChangeEchoType = 3 , // Ƶ��˵��ģʽ�ı�

	KickOffEchoType = 4, //��T��ʾ
	RingBusyType = 5,
	KeyActiveType = 6, //���ð���ͨ��
	ChannelUserAllowedActionType = 7, // ����/��ֹ��ʾ
	TuoRenEchoType = 8,	// ���˵�Ƶ����ʾ
	TextStatusChangeEchoType = 9, //��������״̬�ı�
	TextLimitedEchoType = 10,		//��������״̬�ı�
	SubChannelCreateOrRemoveEchoType = 11, // ����/ɾ����Ƶ��
	BulletinChangeEchoType = 12,		//�������ݸı�

	ReceptionAlterEchoType = 13,				//����/ȡ���Ӵ���
	ReceptionMessageEchoType = 14,			//�Ӵ���

	MaixuChangedEchoType = 15,		//����Ա�ı�����
	MicrophoneTimeChangeEchoType = 16, //Ƶ������ʱ�����
	DoubleMicTimeEchoType = 17, // ��������ʱ��
	AllowJoinMaixuEchoType = 18, // ���Լ�������
	ForbidJoinMaixuEchoType = 19,// ���ܼ�������

	KongMaiEchoType = 20,			// ������ʾ
	FangMaiEchoType = 21,			// ������ʾ

	JoinChannelEchoType = 22,		// ĳ�˼���Ƶ��
	LeaveChannelEchoType = 23,	// ĳ���뿪Ƶ��
	EndPrivateChatEchoType = 24,	// ����˽��
	ApplyJoinGuildEchoType = 25,	// �������

	MutiMaiInviteEchoType = 26,	//������������
	MutiMaiOverEchoType = 27,		//�������󳬹�����
	TopMutiMaiLeaveEchoType = 28, // 1���������������ѽ���
	ReplyMutiMaiInviteEchoType = 29, //������������ظ�

	SafeLeveLowEchoType = 30,			//��ȫ�ȼ�����

	CodecTypeChangeEchoType = 31, // ������/��ͨ������ʾ
	GuestVoiceEchoType = 32,			//�οͲ���˵��
	OperateFailResultEchoType = 33,	//����Ա����ʧ����ʾ
	StopRecordEchoType = 34,		// ֹͣ¼��
	SortIDRetractEchoType = 35,	// ��λID�����
	AutoAnswerEchoType = 36,		//�Զ��ظ�
	AudioErrorEchoType = 37,			//������Ƶģ�����

	TemplateChangeEchoType = 38, //ģ��ı�
};

struct SystemMessageInfo
{
	int			type;
	QString	message;
};

enum SendErrorTipsPostion 
{
   TipsPosDefult=0,         //��ʾ����������·�
   TipsPosPrivateChatTitle, //ʹ��Ĭ��ģ���µĽ�ֹ˽����ʾ�����ʽ
};
/**
* @brief �����л����л���Ĭ������ : switchPublicMessage(DefaultMessageList, NULL). ϵͳ��Ϣ���Զ����뵽�༭����.\n
*����ϵͳ��Ϣ�źţ�\n
*switchPublicMessage(UserMessageList,NULL);\n
*switchPrivateMessage(UserMessageList,NULL);\n
*���ñ༭������Ϊ�û����ͣ����Ҵ���ָ�롣 ���ָ��ǿգ�ϵͳ��Ϣ��Ĭ�ϲ���ָ���ı༭����\n

*/


DWDEFINE_INTERFACE(IYYChannelMessageEx) : public IUnk
{

/**
   * @brief �����л����л���Ĭ������ : switchPublicMessage(DefaultMessageList, NULL). ϵͳ��Ϣ���Զ����뵽�༭����.
   * @param type,��MessageListType.
   * @param spPublicMessage,��������ָ��.
   */
	virtual void switchPublicMessage(MessageListType type, IChannelMessageEditPtr spPublicMessage) = 0;
/**
   * @brief ˽���л�.ϵͳ��Ϣ���Զ����뵽�༭����.
   * @param type,��MessageListType.
   * @param spPublicMessage,����˽����ָ��.
   */
	virtual void switchPrivateMessage(MessageListType type, IChannelMessageEditPtr spPublicMessage) = 0;

/**
   * @brief ���͹�����Ϣ.
   * @param message����Ϣ�ı�����.
   * @param message����Ϣ�ı�����.
   * @param textColor����Ϣ�ı���ɫ.
   * @return ���ͽ�������룬��ϸ��SendErrorCode.
   */
	virtual int	sendNormalMessage( const QString& message , const QFont& font, const QColor& textColor ) = 0;

	/**
	* @brief ����˽����Ϣ.
	* @param message����Ϣ�ı�����.
	* @param message����Ϣ�ı�����.
	* @param textColor����Ϣ�ı���ɫ.
	* @return ���ͽ�������룬��ϸ��SendErrorCode.
	*/
	virtual int	sendPrivateMessage( UINT32 uid, const QString& message, const QFont& font, const QColor& textColor ) = 0;

	typedef xsignals::signal<void (MessageInfo&)> SIG_MESSAGE_ARRIVED;
/**
   * @brief ��ȡ������Ϣ�����ź�.  ע�⣺���źű������switchPublicMessageʹ��
   * @return ������Ϣ�����ź�.
   */
	virtual SIG_MESSAGE_ARRIVED *getNormalMessageArrivedSignal() = 0;
/**
   * @brief ��ȡ˽����Ϣ�����ź�.  ע�⣺���źű������switchPrivateMessageʹ��
   * @return ˽����Ϣ�����ź�.
   */
	virtual SIG_MESSAGE_ARRIVED *getPrivateMessageArrivedSignal() = 0;

	/**
	* @brief ϵͳ��Ϣ�źŶ���.
	* @param SystemMessageInfo,ϵͳ��ϢĬ����ʾ�ı�.
	*/
	typedef xsignals::signal<void (const SystemMessageInfo& ) > SIG_SYSTEM_MESSAGE_ARRIVED;

	/**
	* @brief ϵͳ��Ϣ�źŶ���.
	* @param YYMessageData,ϵͳ��Ϣ.
	*/
	typedef xsignals::signal<void(const YYMessageData&) > SIG_SYSTEM_MESSAGEEX_ARRIVED;

	/**
	* @brief ��ȡϵͳ��Ϣ�ź�.  ע�⣺���źű������switchPublicMessage��switchPrivateMessageʹ��
	* @return ϵͳ��Ϣ�ź�.
	*/
	virtual SIG_SYSTEM_MESSAGE_ARRIVED *getSystemMessageArrivedSignal() = 0;


	/**
   * @brief ��ȡ���������͵�Ƶ��������Ϣ�����ź�.
		�źŷ��صĲ������uidΪ�Լ���ʱ��spSenderInfoû�а�������
   * @return ������Ϣ�����ź�.
   */
	virtual SIG_MESSAGE_ARRIVED *channelNormalMessageArrivedSignal() = 0;

	/**
   * @brief ��ȡ����������˽����Ϣ�����ź�.
		�źŷ��صĲ������uidΪ�Լ���ʱ��spSenderInfoû�а�������
   * @return ˽����Ϣ�����ź�.
   */
	virtual SIG_MESSAGE_ARRIVED *channelPrivateMessageArrivedSignal() = 0;

	/**
   * @brief ��ȡ����������ϵͳ��Ϣ�����ź�.
   * @return ϵͳ��Ϣ�����ź�.
   */
	virtual SIG_SYSTEM_MESSAGE_ARRIVED *channelSystemMessageArrivedSignal() = 0;


	
	/**
   * @brief ��ȡ������Ϣ�����ź�.
   * @return ������Ϣ�����ź�.
   */
	typedef xsignals::signal<void (const sendError::SendError& ) > SIG_SENDERROR;

	virtual SIG_SENDERROR *sendErrorSignal() = 0;

	virtual void sendEchoMessage(const MessageInfo& msg) = 0;

	virtual SIG_MESSAGE_ARRIVED *channelEchoMessageArrivedSignal() = 0;

	/**
	* @brief ���ô�����Ϣ��ʾ�����ʾλ��.
	* @param pos, ��ʾ��λ�� 
	*/
	virtual void setSendErrorTipsPosition(const SendErrorTipsPostion& pos) = 0;
    
	/**
	* @brief ��ȡ������Ϣ��ʾ���λ��.
	* @return ������Ϣ��ʾ���λ��.
	*/
	virtual SendErrorTipsPostion sendErrorTipsPosition() = 0;

	/**
	* @brief ��ȡ����������ϵͳ��Ϣ�����ź�.
	* @return ϵͳ��Ϣ�����ź�.
	*/
	virtual SIG_SYSTEM_MESSAGEEX_ARRIVED *channelSystemMessageExArrivedSignal() = 0;

	/**
	* @brief Ƶ���㲥��Ϣ�źŶ���.
	* @param YYMessageData,Ƶ���㲥��Ϣ.
	*/
	typedef xsignals::signal<void(const YYMessageData&) > SIG_BROADCAST_MESSAGE_ARRIVED;

	virtual SIG_BROADCAST_MESSAGE_ARRIVED *channelBroadcastMessageArrivedSignal() = 0;

	typedef xsignals::signal<void(const YYMessageData&) > SIG_TRANSFER_MESSAGE_ARRIVED;
	virtual SIG_TRANSFER_MESSAGE_ARRIVED *getTransferNormalMessageArrivedSignal() = 0;//����getNormalMessageArrivedSignal��ֻ��������Ƶ��ת������
};