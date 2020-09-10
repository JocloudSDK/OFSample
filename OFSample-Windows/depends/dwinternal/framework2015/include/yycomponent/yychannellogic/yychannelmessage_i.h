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
		ManagerForbidText = 2,                  // ����Ա��ֹ�����
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
        ManagerForbidAllText = 21,              // ����Ա��ֹ�����˴���
        ManagerForbidVisitorText = 22,          // ����Ա��ֹ�οʹ���
		PrivateChatTooFastAfterJoined = 23,  //����Ƶ����˽��̫��
		MgrRejectGuestPrivaetChat = 24,  // OW���ù���Ա�������ο�˽��
		

        // ����Ϊ���������صĴ���
        ServerRejectCode_Pass                = 0x100 + 0,
        ServerRejectCode_GlobalBlackList     = 0x100 + 1,  // ȫ�ֽ�ֹ���Եĺ���������Ƶ��Ϊ��λ
        ServerRejectCode_TidNotFound         = 0x100 + 2,  // Ƶ��������
        ServerRejectCode_SidNotFound         = 0x100 + 3,  // ��Ƶ��������
        ServerRejectCode_UserNotExist        = 0x100 + 4,  // �û�������Ƶ����ʵ��������Ƶ����һ��
        ServerRejectCode_ChanDisableText     = 0x100 + 5,  // Ƶ������ֹ�����˴���
        ServerRejectCode_UsrDisableText      = 0x100 + 6,  // �û�������
        ServerRejectCode_VisitorDisalbeText  = 0x100 + 7,  // Ƶ������ֹ�οʹ���
        ServerRejectCode_AccessTimeLimit     = 0x100 + 8,  // ��Ƶ����Ҫ�ȴ�N�뷢��
        ServerRejectCode_IntervalTimeLimit   = 0x100 + 9,  // ÿ�η��Լ��
        ServerRejectCode_BindPhoneLimit      = 0x100 + 10, // �û���Ҫ���ֻ����ܷ���
        ServerRejectCode_TextCounterLimited  = 0x100 + 11, // ������̨���õ�Ƶ������Ƶ��
        ServerRejectCode_FilterLimited       = 0x100 + 12, // �����������̽���
        ServerRejectCode_AnonymousUid        = 0x100 + 17, // ��ֹ�����û�����
        ServerRejectCode_ReqLimited          = 0x100 + 18, // �������������ܳ����������
        ServerRejectCode_TextMaxLongLimited  = 0x100 + 19, // �ı�����
        ServerRejectCode_TextLengthLimited   = 0x100 + 20, // �ı����ȳ�������
        ServerRejectCode_TicketOrUrlLimited  = 0x100 + 21, // �ɻ�Ʊ��URL���� �����Ϊ29,30��
        ServerRejectCode_AdminOneChatLimited = 0x100 + 22, // �ǹ���Ա���½�ֹ����˽��
        ServerRejectCode_RenPinLimit         = 0x100 + 23, // ��Ʒֵ���� // 8.49�汾����Ʒϵͳ����
        ServerRejectCode_RenPinPlatformLimited          = 0x100 + 24, // ��Ʒϵͳ����ȫƽ̨����һ��ʱ�� // 8.49�汾����Ʒϵͳ����
        ServerRejectCode_PoliceLimited                  = 0x100 + 25, // 400������ײ��ܷ�����
        ServerRejectCode_VipExpressionTooMuchLimited    = 0x100 + 26, // VIP�������5������
        ServerRejectCode_TicketTooMuch                  = 0x100 + 27, // �ɻ�Ʊ����1
        ServerRejectCode_SpecificTicketTooMuch          = 0x100 + 28, // IMAGE yxbq yyvip ���ͷɻ�Ʊ����5
        ServerRejectCode_TickLimited                    = 0x100 + 29, //�ɻ�Ʊ����
        ServerRejectCode_UrlLimited                     = 0x100 + 30, //url����
        ServerRejectCode_NornalTextIntervalLimited      = 0x100 + 31, //�οͷ��Լ������
        ServerRejectCode_ManagerTextIntervalLimited     = 0x100 + 32, //����Ա���Լ������ 
        ServerRejectCode_RenpinUnifyRes                 = 0x100 + 34,// ��Ʒϵͳͳһ������,json��ʽ��������Ʒֵ���巵���� // 8.49�汾����Ʒϵͳ����
		ServerRejectCode_NotMainlandIpLimitied			= 0x100 + 35,// �Ǵ�½ip��������
	};

	enum ErrorLevel
	{
		None = 0, // ������.
		Information = 1, // ��Ϣ
		Prompt = 2, // ��ʾ
		Warning = 3, // ����
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
		KCustomNick, // ҵ���ƹ��������ǳ�
		KIsYYNumVisible, // ҵ���ƹ��������Ƿ��YY��
		KIsRoleVisible, // ҵ���ƹ��������Ƿ�����
		KIsComponentIconVisible, // ҵ���ƹ��������Ƿ��ҵ��ͼ�꣨ѫ�£���Ա������...etc��
		KIsMessageWithBkgColor,// ҵ���ƣ��������
		KMessageBkgBeginColor,// ҵ���ƣ����������ɫ,��ʼɫ��eg.0xfffff����KMessageBkgEndColor�γɽ���ɫ��
		KMessageBkgEndColor,// ҵ���ƣ����������ɫ���յ�ɫ��eg.0x00000
		KSubsidMessageFrom,// ��Ϣ���Ե���Ƶ��������ת����Ϣ��8.40��
	};

	enum ItemProp
	{
		KItemTextFont,
		KItemTextColor,
		KItemLinkKey,
		KItemLinkValue,
		KItemImageKey,
		KItemImagePath,
		KItemIsImageType, //100-200 �ڲ��ã���yycommonconst.h 
		KItemIsTicketType,
		KItemTooltip,
		KItemPriority,
		KUserProp = 10000,
        KUserPropRole = KUserProp + 1,
	};

	enum EchoDetailType
	{
		KUnknowEchoType = 0,
		RoleChangEchoType = 1 ,	//�û���ɫ�仯��ʾ
		RoleChangeByInviteEchoType = 2, //�������ɫ�仯
		StyleTypeChangeEchoType = 3 , // Ƶ��˵��ģʽ�ı�
		KickOffEchoType = 4, //��T��ʾ
		RingBusyEchoType = 5, //Ƶ��ֻ����5����ͬʱ˵��
		KeyActiveEchoType = 6, //���ð���ͨ��
		ChannelUserAllowedActionEchoType = 7, // ����/��ֹ��ʾ
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
		ChannelShortIdExpired = 39, // Ƶ���̺Ź���
		MgrRejectGuestPrivateChatEchonType = 40, //�������Ϲ���Ա�������ο�˽��
		TopMaiOccupiedEchoType=41, //����ռ��֪ͨ����
		OfficialRemindEchoType = 42, //�������ð�����Ƶ���ٷ���ʾ
		IpLimitedEchoType = 43, //���ľ���ip����
		BindPhoneEchoType = 44, //���İ��ֻ�����

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
		KCustomNick, // ҵ���ƹ������������ǳ�
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

	// 8.49�汾���߹���
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

	//΢��������
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





