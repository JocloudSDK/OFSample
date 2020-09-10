#pragma once
#include "dwbase/IData.h"
#include "yycomponent/propBag_i.h"

Q_DECLARE_METATYPE(Data::IRowPtr);
Q_DECLARE_METATYPE(IPropBagPtr);

namespace channelmsg
{
	enum SendErrorCode
	{
		SendOk = 0,
		KeywordFilter = 1,
		ManagerForbidText = 2,                  // 管理员禁止你打字
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
		EmptyText = 16,
		ChannelNoPermissionSendImage = 17,
		ChannelUserTooMuchForSendImage = 18,
		NoPermissionSendImage = 19,
		ForbidSend = 20,
        ManagerForbidAllText = 21,              // 管理员禁止所有人打字
        ManagerForbidVisitorText = 22,          // 管理员禁止游客打字
		PrivateChatTooFastAfterJoined = 23,  //进入频道后私聊太快
		MgrRejectGuestPrivaetChat = 24,  // OW设置管理员不接受游客私聊
		

        // 以下为服务器返回的代码
        ServerRejectCode_Pass                = 0x100 + 0,
        ServerRejectCode_GlobalBlackList     = 0x100 + 1,  // 全局禁止发言的黑名单，以频道为单位
        ServerRejectCode_TidNotFound         = 0x100 + 2,  // 频道不存在
        ServerRejectCode_SidNotFound         = 0x100 + 3,  // 子频道不存在
        ServerRejectCode_UserNotExist        = 0x100 + 4,  // 用户发言子频道和实际所在子频道不一致
        ServerRejectCode_ChanDisableText     = 0x100 + 5,  // 频道被禁止所有人打字
        ServerRejectCode_UsrDisableText      = 0x100 + 6,  // 用户被禁言
        ServerRejectCode_VisitorDisalbeText  = 0x100 + 7,  // 频道被禁止游客打字
        ServerRejectCode_AccessTimeLimit     = 0x100 + 8,  // 进频道需要等待N秒发言
        ServerRejectCode_IntervalTimeLimit   = 0x100 + 9,  // 每次发言间隔
        ServerRejectCode_BindPhoneLimit      = 0x100 + 10, // 用户需要绑定手机才能发言
        ServerRejectCode_TextCounterLimited  = 0x100 + 11, // 超过后台设置的频道发言频率
        ServerRejectCode_FilterLimited       = 0x100 + 12, // 被反垃圾进程禁言
        ServerRejectCode_AnonymousUid        = 0x100 + 17, // 禁止匿名用户发言
        ServerRejectCode_ReqLimited          = 0x100 + 18, // 超过服务器所能承受最大请求
        ServerRejectCode_TextMaxLongLimited  = 0x100 + 19, // 文本过长
        ServerRejectCode_TextLengthLimited   = 0x100 + 20, // 文本长度超过限制
        ServerRejectCode_TicketOrUrlLimited  = 0x100 + 21, // 飞机票或URL限制 （拆分为29,30）
        ServerRejectCode_AdminOneChatLimited = 0x100 + 22, // 非管理员以下禁止发送私聊
        ServerRejectCode_RenPinLimit         = 0x100 + 23, // 人品值限制 // 8.49版本，人品系统下线
        ServerRejectCode_RenPinPlatformLimited          = 0x100 + 24, // 人品系统，被全平台禁言一段时间 // 8.49版本，人品系统下线
        ServerRejectCode_PoliceLimited                  = 0x100 + 25, // 400网警马甲不能发公屏
        ServerRejectCode_VipExpressionTooMuchLimited    = 0x100 + 26, // VIP表情大于5个限制
        ServerRejectCode_TicketTooMuch                  = 0x100 + 27, // 飞机票大于1
        ServerRejectCode_SpecificTicketTooMuch          = 0x100 + 28, // IMAGE yxbq yyvip 类型飞机票大于5
        ServerRejectCode_TickLimited                    = 0x100 + 29, //飞机票限制
        ServerRejectCode_UrlLimited                     = 0x100 + 30, //url限制
        ServerRejectCode_NornalTextIntervalLimited      = 0x100 + 31, //游客发言间隔限制
        ServerRejectCode_ManagerTextIntervalLimited     = 0x100 + 32, //管理员发言间隔限制 
        ServerRejectCode_RenpinUnifyRes                 = 0x100 + 34,// 人品系统统一返回码,json格式，包含人品值具体返回码 // 8.49版本，人品系统下线
		ServerRejectCode_NotMainlandIpLimitied			= 0x100 + 35,// 非大陆ip发言限制
	};

	enum ErrorLevel
	{
		None = 0, // 无类型.
		Information = 1, // 消息
		Prompt = 2, // 提示
		Warning = 3, // 警告
	};

	struct SendError
	{
		SendError():level(None),code(SendOk)
		{

		}
		ErrorLevel		level;
		SendErrorCode	code;
		QString			description;
		QVariantList	args;
	};
}

namespace channelmsg
{
	enum MessageType
	{
		KUnknowType,
		KEchoType,
		KChatType,
		KPrivateChatType,
		KAutoReplyChatType,
		KBroadCastType,
		KComponentType,
		KTableType,
		KShareBroadcastType,
		KAutoBroadcastType,
		KForbidSendType
	};

	enum MessageProp
	{
		KFullText,
		KTextFont,
		KTextColor,
		KSenderProp,
		KMyInfo, // will delete.
		KSubType,
		KEchoDetailType,
		//need to refactor.
		KNeedForceShow,
		KShowPrivateReport,
		KIconPath,
		KSenderNickColor,
		KbubbleId,
		KBubbleImagePath,
		KBubbleMargins,

        KFunnyRommType,
        KMsgNickName,
        KSenderNickFont,
        KSendTime,
		KTextMsg,
		KCustomNick, // 业务定制公屏发言昵称
		KIsYYNumVisible, // 业务定制公屏发言是否带YY号
		KIsRoleVisible, // 业务定制公屏发言是否带马甲
		KIsComponentIconVisible, // 业务定制公屏发言是否带业务图标（勋章，会员，贵族...etc）
		KIsMessageWithBkgColor,// 业务定制，贵族底纹
		KMessageBkgBeginColor,// 业务定制，贵族底纹颜色,起始色，eg.0xfffff（与KMessageBkgEndColor形成渐变色）
		KMessageBkgEndColor,// 业务定制，贵族底纹颜色，终点色，eg.0x00000
		KSubsidMessageFrom,// 消息来自的子频道（公屏转发消息，8.40）
	};

	enum ItemProp
	{
		KItemTextFont,
		KItemTextColor,
		KItemLinkKey,
		KItemLinkValue,
		KItemImageKey,
		KItemImagePath,
		KItemIsImageType, //100-200 内部用，见yycommonconst.h 
		KItemIsTicketType,
		KItemTooltip,
		KItemPriority,
		KUserProp = 10000,
        KUserPropRole = KUserProp + 1,
	};

	enum EchoDetailType
	{
		KUnknowEchoType = 0,
		RoleChangEchoType = 1 ,	//用户角色变化提示
		RoleChangeByInviteEchoType = 2, //邀请码角色变化
		StyleTypeChangeEchoType = 3 , // 频道说话模式改变
		KickOffEchoType = 4, //被T提示
		RingBusyEchoType = 5, //频道只允许5个人同时说话
		KeyActiveEchoType = 6, //设置按键通话
		ChannelUserAllowedActionEchoType = 7, // 允许/禁止提示
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
		ChannelShortIdExpired = 39, // 频道短号过期
		MgrRejectGuestPrivateChatEchonType = 40, //黄马以上管理员不接受游客私聊
		TopMaiOccupiedEchoType=41, //首麦被占，通知二麦
		OfficialRemindEchoType = 42, //进入配置白名单频道官方提示
		IpLimitedEchoType = 43, //六四境外ip限制
		BindPhoneEchoType = 44, //六四绑定手机限制

	};

	enum EchoSubType
	{
		TextTypeNull,
		TextTypeNotify,
		TextTypeNotice,
		TextTypeSpecialNotice,
		TextTypeWarning,
		TextTypeError,
		TextTypeAdvertisement,
		TextTypeWelcome,
		TextTypeAnswerQuestion,
		TextTypeEmergencies,
	};

	enum BroadCastSubType
	{
		KBroadCastTextType,
		KBroadCastVoiceType
	};

	enum PrivateChatSubType
	{
		KSendType,
		KReceiveType
	};
};

struct YYMessageItem
{
	enum ItemType
	{
		KPlanTextType,

		KLinkType,
		KAdvertisementLink,
		KUrlLinkType,
		KUserNickLinkType,

		KChannelImageType,
		KSmileType,
		KMagicSmileType,
		KNormalImageType,

		KYYTickType,
		KYYTickImageType,
		KExtraInfo,

		KNoticeLink,
		KAttentionLink,
		KWarnningLink,
		KErrorTypeLink,
		KEmergenciesLink,
		KCustomNick, // 业务定制公屏回显链接昵称
	};

	YYMessageItem( int itemType = KPlanTextType ):type( itemType )
	{
	}

	YYMessageItem( const QString& msg ):data(msg), type(KPlanTextType)
	{
	}

	int	 type;
	QString data;
	QMap<int,QVariant> propertys;
};


struct YYMessageData
{
	YYMessageData( channelmsg::MessageType t =  channelmsg::KUnknowType )
		: type(t)
	{
	}

	channelmsg::MessageType type;
	QMap<int,QVariant>	propertys;

	QList<YYMessageItem>	compositeMsg;
};

namespace channelmsg
{
	inline void setFontProp( YYMessageData& data, const QFont& font )
	{
		data.propertys[KTextFont].setValue(font);
	}

	inline QFont getFontProp( const YYMessageData& data )
	{
		return data.propertys[KTextFont].value<QFont>();
	}

	inline void setTextColorProp(YYMessageData& data, QRgb color )
	{
		data.propertys[KTextColor] = color;
	}
	inline QRgb getTextColorProp(const YYMessageData& data )
	{
		return data.propertys[KTextColor].value<QRgb>();
	}

	inline void setSenderProp(YYMessageData& data, IPropBagPtr senderProp )
	{
		data.propertys[KSenderProp].setValue(senderProp);
	}
	inline IPropBagPtr getSenderProp(const YYMessageData& data )
	{
		return data.propertys[KSenderProp].value<IPropBagPtr>();
	}

	inline void setMyProp(YYMessageData& data, Data::IRowPtr& myRowPtr )
	{
		data.propertys[KMyInfo].setValue(myRowPtr);
	}
	inline Data::IRowPtr getMyProp(const YYMessageData& data)
	{
		return data.propertys[KMyInfo].value<Data::IRowPtr>();
	}

	inline void setEchoDetailProp( YYMessageData& data, EchoDetailType detailType )
	{
		data.propertys[KEchoDetailType] = (int)detailType;
	}

	inline EchoDetailType getEchoDetailProp(const YYMessageData& data )
	{
		return (EchoDetailType)(data.propertys[KEchoDetailType].toInt());
	}

	inline void setFullTextProp(  YYMessageData& data, const QString& text )
	{
		data.propertys[KFullText] = text;
	}

	inline QString getFullTextProp(const YYMessageData& data )
	{
		return data.propertys[KFullText].toString();
	}

	inline void setEchoSubType(YYMessageData& data, EchoSubType type )
	{
		data.propertys[KSubType] = (int) type;
	}

	inline EchoSubType getEchoSubType(const YYMessageData& data)
	{
		return (EchoSubType)(data.propertys[KSubType].toInt());
	}

	inline void setBrocastSubType(YYMessageData& data, BroadCastSubType type)
	{
		data.propertys[KSubType] = (int) type;
	}

	inline BroadCastSubType getBroadCastSubType(const YYMessageData& data)
	{
		return (BroadCastSubType)(data.propertys[KSubType].toInt());
	}

	inline void setShowPrivateReport(YYMessageData& data, bool isVisible )
	{
		data.propertys[KShowPrivateReport] = isVisible;
	}

	inline bool getShowPrivateReport(const YYMessageData& messageData )
	{
		return messageData.propertys[KShowPrivateReport].toBool();
	}

    inline void setSenderNick(YYMessageData& data, const QString& nick)
    {
        nick.isEmpty() ? data.propertys.remove(KMsgNickName) : data.propertys[KMsgNickName] = nick;
    }

    inline QString getSenderNick(const YYMessageData& data)
    {
        return data.propertys.value(KMsgNickName).toString();
    }

	inline void setSenderNickColorProp(YYMessageData& data, QRgb color )
	{
		data.propertys[KSenderNickColor] = color;
	}
	inline QVariant getSenderNickColorProp(const YYMessageData& data )
	{
		return data.propertys[KSenderNickColor];
	}

    inline void setNickFont(YYMessageData& data, const QFont& fnt)
    {
        data.propertys[KSenderNickFont] = fnt;
    }

    inline bool hasNickFont(const YYMessageData& data)
    {
        QVariant var = data.propertys.value(KSenderNickFont);
        return var.isValid();
    }

    inline QFont getNickFont(const YYMessageData& data)
    {
        return data.propertys.value(KSenderNickFont).value<QFont>();
    }

	inline void setForceShow(YYMessageData& data, bool needForceShow )
	{
		data.propertys[KNeedForceShow] = needForceShow;
	}

	inline bool getForceShow(const YYMessageData& data )
	{
		return data.propertys[KNeedForceShow].toBool();
	}

	inline void setPrivateSubType(YYMessageData& data, PrivateChatSubType subType )
	{
		data.propertys[KSubType] = (int)subType;
	}

	inline PrivateChatSubType getPrivateSubType(const YYMessageData& data )
	{
		return (PrivateChatSubType) data.propertys.value(KSubType).toInt();
	}

	// 8.49版本下线功能
	inline quint32 getBubbleId(const YYMessageData& data )
	{
		return 0;
	}
	inline void setBubbleId(YYMessageData& data, const quint32 &id)
	{
	}
	inline QString getBubbleImagePath(const YYMessageData& data )
	{
		return QString();
	}
	inline void setBubbleImagePath(YYMessageData& data, const QString &imagePath)
	{
	}
	inline QMargins getBubbleMargins(const YYMessageData& data )
	{
		return QMargins();
	}
	inline void setBubbleMargins(YYMessageData& data, const QMargins &margins)
	{
	}

	//微房已下线
    inline void setFunnyRoomType(YYMessageData& data, bool isFunnyRoom)
    {
        data.propertys.remove(KFunnyRommType);
    }
    inline bool getFunnyRoomType(const YYMessageData& data)
    {
        return false;
    }

	inline void setSendTime(YYMessageData& data, uint tmt)
    {
        data.propertys[KSendTime] = tmt;
    }

    inline uint getSendTime(const YYMessageData& data)
    {
        return data.propertys.value(KSendTime).toUInt();
    }

	inline void setTextMsg(YYMessageData& data, const QString& textMsg)
	{
		data.propertys[KTextMsg] = textMsg;
	}
	inline QString getTextMsg(const YYMessageData& data)
	{
		return data.propertys.value(KTextMsg).toString();
	}

	// YY 8.31
	inline void setCustomNick(YYMessageData& data, const QString &nick)
	{
		data.propertys[KCustomNick] = nick;
	}
	inline QString getCustomNick(const YYMessageData& data)
	{
		return data.propertys.value(KCustomNick).toString();
	}

	inline void setYYNumVisible(YYMessageData& data, bool v)
	{
		data.propertys[KIsYYNumVisible] = v;
	}
	inline bool getYYNumVisible(const YYMessageData& data)
	{
		if (data.propertys.contains(KIsYYNumVisible))
		{
			return data.propertys.value(KIsYYNumVisible).toBool();
		}
		return true;
	}

	inline void setRoleVisible(YYMessageData& data, bool v)
	{
		data.propertys[KIsRoleVisible] = v;
	}
	inline bool getRoleVisible(const YYMessageData& data)
	{
		if (data.propertys.contains(KIsRoleVisible))
		{
			return data.propertys.value(KIsRoleVisible).toBool();
		}
		return true;
	}

	inline void setComponentIconVisible(YYMessageData& data, bool v)
	{
		data.propertys[KIsComponentIconVisible] = v;
	}
	inline bool getComponentIconVisible(const YYMessageData& data)
	{
		if (data.propertys.contains(KIsComponentIconVisible))
		{
			return data.propertys.value(KIsComponentIconVisible).toBool();
		}
		return true;
	}

	inline void setIsMessageWithBkgColor(YYMessageData& data, bool v)
	{
		data.propertys[KIsMessageWithBkgColor] = v;
	}
	inline bool getIsMessageWithBkgColor(const YYMessageData& data)
	{
		if (data.propertys.contains(KIsMessageWithBkgColor))
		{
			return data.propertys.value(KIsMessageWithBkgColor).toBool();
		}
		return false;
	}

	inline void setMessageBkgBeginColor(YYMessageData& data, int v)
	{
		data.propertys[KMessageBkgBeginColor] = v;
	}
	inline int getMessageBkgBeginColor(const YYMessageData& data)
	{
		if (data.propertys.contains(KMessageBkgBeginColor))
		{
			return data.propertys.value(KMessageBkgBeginColor).toInt();
		}
		return 0xf8eac3;
	}

	inline void setMessageBkgEndColor(YYMessageData& data, int v)
	{
		data.propertys[KMessageBkgEndColor] = v;
	}
	inline int getMessageBkgEndColor(const YYMessageData& data)
	{
		if (data.propertys.contains(KMessageBkgEndColor))
		{
			return data.propertys.value(KMessageBkgEndColor).toInt();
		}
		return 0xfff8e4;
	}

	inline void setSubsidMessageFrom(YYMessageData& data, quint32 v)
	{
		data.propertys[KSubsidMessageFrom] = v;
	}
	inline quint32 getSubsidMessageFrom(const YYMessageData& data)
	{
		if (data.propertys.contains(KSubsidMessageFrom))
		{
			return data.propertys.value(KSubsidMessageFrom).toUInt();
		}
		return 0;
	}
}

namespace channelmsg
{
	inline YYMessageItem makeTextItem( const QString& message )
	{
		return YYMessageItem(message);
	}

	inline YYMessageItem makeTextItem( const QString& message, const QFont& font, const QColor& color )
	{
		YYMessageItem item(YYMessageItem::KPlanTextType);
		item.data = message;
		item.propertys[KItemTextColor] = color;
		item.propertys[KItemTextFont] = font;

		return item;
	}

	inline YYMessageItem makeLinkItem(const QString& linkKey, const QString& linkValue, const QString& displayText )
	{
		YYMessageItem item(YYMessageItem::KLinkType);
		item.data = displayText;
		item.propertys[ KItemLinkKey ] = linkKey;
		item.propertys[ KItemLinkValue ] = linkValue;

		return item;
	}

	inline YYMessageItem makeAdvertisementItem(const QString& linkKey, const QString& linkValue, const QString& displayText )
	{
		YYMessageItem item(YYMessageItem::KAdvertisementLink);
		item.data = displayText;
		item.propertys[ KItemLinkKey ] = linkKey;
		item.propertys[ KItemLinkValue ] = linkValue;

		return item;
	}

	inline YYMessageItem makeNormalImageItem( const QString& imageKey, const QString& imagePath )
	{
		YYMessageItem item(YYMessageItem::KNormalImageType);
		item.propertys[KItemImageKey] = imageKey;
		item.propertys[KItemImagePath] = imagePath;
		item.propertys[KItemIsImageType]  = true;

		return item;
	}

	inline YYMessageItem makeSmileItem( const QString& simleKey )
	{
		YYMessageItem item(YYMessageItem::KSmileType);
		item.data = simleKey;
		item.propertys[KItemIsImageType]  = true;

		return item;
	}

	inline YYMessageItem makeMagicSmileItem( const QString& smileKey )
	{
		YYMessageItem item(YYMessageItem::KMagicSmileType);
		item.data = smileKey;
		item.propertys[KItemIsImageType]  = true;

		return item;
	}

	inline YYMessageItem makeYYTickItem(const QString& ticket )
	{
		YYMessageItem item(YYMessageItem::KYYTickType);
		item.propertys[KItemIsTicketType]  = true;
		item.data = ticket;
		return item;
	}

	inline bool isTicketItem( const YYMessageItem& Item )
	{
		return Item.propertys.value(KItemIsTicketType, false).toBool();
	}

	inline bool isUrlLinkItem(const YYMessageItem& item)
	{
		return item.type == YYMessageItem::KUrlLinkType;
	}
	
	inline YYMessageItem makeYYTickImageItem(const QString& ticket )
	{
		YYMessageItem item(YYMessageItem::KYYTickImageType);
		item.data = ticket;
		item.propertys[KItemIsImageType]  = true;

		return item;
	}

	//inline YYMessageItem makeUrlLinkItem( const QString& url )
	//{
	//	YYMessageItem item(YYMessageItem::KUrlLinkType);
	//	item.data = url;
	//	item.propertys[ KItemLinkKey ] = yycommon;
	//	item.propertys[ KItemLinkValue ] = linkValue;

	//	return item;
	//}

	inline YYMessageItem makeChannelImageItem( const QString& imageHashKey )
	{
		YYMessageItem item(YYMessageItem::KChannelImageType);
		item.data = imageHashKey;
		item.propertys[KItemIsImageType]  = true;

		return item;
	}

	inline bool isImageItem( const YYMessageItem& Item )
	{
		return Item.propertys.value(KItemIsImageType, false).toBool();
	}

	inline int itemTypeCount( const YYMessageData& messageData, YYMessageItem::ItemType itemType)
	{
		int count = 0;
		foreach(const YYMessageItem& item, messageData.compositeMsg )
		{
			if ( item.type == itemType )
			{
				count++;
			}
		}
		return count;
	}
};

namespace channelmsg
{
	class ItemProperty
	{
	public:
		explicit ItemProperty( const YYMessageItem& item )
			:m_item(item)
		{
		}

		QColor itemTextColor()
		{
			return m_item.propertys.value(KItemTextColor).value<QColor>();
		}

		QString itemLinkKey()
		{
			return m_item.propertys.value(KItemLinkKey).toString();
		}

		QString itemLinkValue()
		{
			return m_item.propertys.value(KItemLinkValue).toString();
		}

		QString itemImageKey()
		{
			return m_item.propertys.value(KItemImageKey).toString();
		}

		QString itemImagePath()
		{
			return m_item.propertys.value(KItemImagePath).toString();
		}
	private:
		const YYMessageItem& m_item;
	};

}





