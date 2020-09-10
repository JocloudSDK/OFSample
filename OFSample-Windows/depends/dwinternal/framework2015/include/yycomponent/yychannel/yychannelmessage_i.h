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
		None // 无类型.
		,Information // 消息
		,Prompt // 提示
		,Warning // 警告
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
* @brief 发送消息.
*/
DWDEFINE_INTERFACE(IYYChannelMessage) : public IUnk
{
/**
   * @brief 发送消息.
   * @param message，消息内容.
   * @return 见SendErrorCode.
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

	RoleChangeEchoType = 1 ,	//用户角色变化提示
	RoleChangeByInviteEchoType = 2, //邀请码角色变化

	StyleTypeChangeEchoType = 3 , // 频道说话模式改变

	KickOffEchoType = 4, //被T提示
	RingBusyType = 5,
	KeyActiveType = 6, //设置按键通话
	ChannelUserAllowedActionType = 7, // 允许/禁止提示
	TuoRenEchoType = 8,	// 拖人到频道提示
	TextStatusChangeEchoType = 9, //文字聊天状态改变
	TextLimitedEchoType = 10,		//文字限制状态改变
	SubChannelCreateOrRemoveEchoType = 11, // 创建/删除子频道
	BulletinChangeEchoType = 12,		//公告内容改变

	ReceptionAlterEchoType = 13,				//设置/取消接待语
	ReceptionMessageEchoType = 14,			//接待语

	MaixuChangedEchoType = 15,		//管理员改变麦序
	MicrophoneTimeChangeEchoType = 16, //频道麦序时间调整
	DoubleMicTimeEchoType = 17, // 增加麦序时间
	AllowJoinMaixuEchoType = 18, // 可以加入麦序
	ForbidJoinMaixuEchoType = 19,// 不能加入麦序

	KongMaiEchoType = 20,			// 控麦提示
	FangMaiEchoType = 21,			// 放麦提示

	JoinChannelEchoType = 22,		// 某人加入频道
	LeaveChannelEchoType = 23,	// 某人离开频道
	EndPrivateChatEchoType = 24,	// 结束私聊
	ApplyJoinGuildEchoType = 25,	// 申请入会

	MutiMaiInviteEchoType = 26,	//多人连麦邀请
	MutiMaiOverEchoType = 27,		//多人连麦超过限制
	TopMutiMaiLeaveEchoType = 28, // 1号麦已下麦，连麦已结束
	ReplyMutiMaiInviteEchoType = 29, //多人连麦邀请回复

	SafeLeveLowEchoType = 30,			//安全等级过低

	CodecTypeChangeEchoType = 31, // 高音质/普通音质提示
	GuestVoiceEchoType = 32,			//游客不能说话
	OperateFailResultEchoType = 33,	//管理员操作失败提示
	StopRecordEchoType = 34,		// 停止录音
	SortIDRetractEchoType = 35,	// 短位ID快过期
	AutoAnswerEchoType = 36,		//自动回复
	AudioErrorEchoType = 37,			//加载音频模块错误

	TemplateChangeEchoType = 38, //模版改变
};

struct SystemMessageInfo
{
	int			type;
	QString	message;
};

enum SendErrorTipsPostion 
{
   TipsPosDefult=0,         //提示框在输入框下方
   TipsPosPrivateChatTitle, //使用默认模板下的禁止私聊提示框的样式
};
/**
* @brief 公屏切换，切换到默认设置 : switchPublicMessage(DefaultMessageList, NULL). 系统消息会自动插入到编辑框上.\n
*接收系统消息信号：\n
*switchPublicMessage(UserMessageList,NULL);\n
*switchPrivateMessage(UserMessageList,NULL);\n
*设置编辑器类型为用户类型，并且传空指针。 如果指针非空，系统消息会默认插入指定的编辑器。\n

*/


DWDEFINE_INTERFACE(IYYChannelMessageEx) : public IUnk
{

/**
   * @brief 公屏切换，切换到默认设置 : switchPublicMessage(DefaultMessageList, NULL). 系统消息会自动插入到编辑框上.
   * @param type,见MessageListType.
   * @param spPublicMessage,代表公屏的指针.
   */
	virtual void switchPublicMessage(MessageListType type, IChannelMessageEditPtr spPublicMessage) = 0;
/**
   * @brief 私屏切换.系统消息会自动插入到编辑框上.
   * @param type,见MessageListType.
   * @param spPublicMessage,代表私屏的指针.
   */
	virtual void switchPrivateMessage(MessageListType type, IChannelMessageEditPtr spPublicMessage) = 0;

/**
   * @brief 发送公聊消息.
   * @param message，消息文本内容.
   * @param message，消息文本字体.
   * @param textColor，消息文本颜色.
   * @return 发送结果返回码，详细见SendErrorCode.
   */
	virtual int	sendNormalMessage( const QString& message , const QFont& font, const QColor& textColor ) = 0;

	/**
	* @brief 发送私聊消息.
	* @param message，消息文本内容.
	* @param message，消息文本字体.
	* @param textColor，消息文本颜色.
	* @return 发送结果返回码，详细见SendErrorCode.
	*/
	virtual int	sendPrivateMessage( UINT32 uid, const QString& message, const QFont& font, const QColor& textColor ) = 0;

	typedef xsignals::signal<void (MessageInfo&)> SIG_MESSAGE_ARRIVED;
/**
   * @brief 获取公屏消息到达信号.  注意：该信号必须配合switchPublicMessage使用
   * @return 公屏消息到达信号.
   */
	virtual SIG_MESSAGE_ARRIVED *getNormalMessageArrivedSignal() = 0;
/**
   * @brief 获取私聊消息到达信号.  注意：该信号必须配合switchPrivateMessage使用
   * @return 私聊消息到达信号.
   */
	virtual SIG_MESSAGE_ARRIVED *getPrivateMessageArrivedSignal() = 0;

	/**
	* @brief 系统消息信号定义.
	* @param SystemMessageInfo,系统消息默认显示文本.
	*/
	typedef xsignals::signal<void (const SystemMessageInfo& ) > SIG_SYSTEM_MESSAGE_ARRIVED;

	/**
	* @brief 系统消息信号定义.
	* @param YYMessageData,系统消息.
	*/
	typedef xsignals::signal<void(const YYMessageData&) > SIG_SYSTEM_MESSAGEEX_ARRIVED;

	/**
	* @brief 获取系统消息信号.  注意：该信号必须配合switchPublicMessage和switchPrivateMessage使用
	* @return 系统消息信号.
	*/
	virtual SIG_SYSTEM_MESSAGE_ARRIVED *getSystemMessageArrivedSignal() = 0;


	/**
   * @brief 获取服务器发送到频道公屏消息到达信号.
		信号返回的参数里的uid为自己的时候，spSenderInfo没有包含属性
   * @return 公屏消息到达信号.
   */
	virtual SIG_MESSAGE_ARRIVED *channelNormalMessageArrivedSignal() = 0;

	/**
   * @brief 获取服务器发送私聊消息到达信号.
		信号返回的参数里的uid为自己的时候，spSenderInfo没有包含属性
   * @return 私聊消息到达信号.
   */
	virtual SIG_MESSAGE_ARRIVED *channelPrivateMessageArrivedSignal() = 0;

	/**
   * @brief 获取服务器发送系统消息到达信号.
   * @return 系统消息到达信号.
   */
	virtual SIG_SYSTEM_MESSAGE_ARRIVED *channelSystemMessageArrivedSignal() = 0;


	
	/**
   * @brief 获取发送消息错误信号.
   * @return 发送消息错误信号.
   */
	typedef xsignals::signal<void (const sendError::SendError& ) > SIG_SENDERROR;

	virtual SIG_SENDERROR *sendErrorSignal() = 0;

	virtual void sendEchoMessage(const MessageInfo& msg) = 0;

	virtual SIG_MESSAGE_ARRIVED *channelEchoMessageArrivedSignal() = 0;

	/**
	* @brief 设置错误消息提示框的显示位置.
	* @param pos, 提示框位置 
	*/
	virtual void setSendErrorTipsPosition(const SendErrorTipsPostion& pos) = 0;
    
	/**
	* @brief 获取错误消息提示框的位置.
	* @return 错误消息提示框的位置.
	*/
	virtual SendErrorTipsPostion sendErrorTipsPosition() = 0;

	/**
	* @brief 获取服务器发送系统消息到达信号.
	* @return 系统消息到达信号.
	*/
	virtual SIG_SYSTEM_MESSAGEEX_ARRIVED *channelSystemMessageExArrivedSignal() = 0;

	/**
	* @brief 频道广播消息信号定义.
	* @param YYMessageData,频道广播消息.
	*/
	typedef xsignals::signal<void(const YYMessageData&) > SIG_BROADCAST_MESSAGE_ARRIVED;

	virtual SIG_BROADCAST_MESSAGE_ARRIVED *channelBroadcastMessageArrivedSignal() = 0;

	typedef xsignals::signal<void(const YYMessageData&) > SIG_TRANSFER_MESSAGE_ARRIVED;
	virtual SIG_TRANSFER_MESSAGE_ARRIVED *getTransferNormalMessageArrivedSignal() = 0;//类似getNormalMessageArrivedSignal，只是由其他频道转发过来
};