
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//struct
#include <deque>

#include "sysbase/tool/string_helper.h"
#include <vector>

#pragma pack(push, 8)

typedef std::deque<UINT32>	_XIDS;
typedef _XIDS	UIDS;
typedef _XIDS	SIDS;
typedef _XIDS	GIDS;
typedef _XIDS	FIDS;


struct TEXT_FONT_INFO
{
	TEXT_FONT_INFO() : height(0), bold(false), italic(false), underline(false), strikeout(false), color(0), bubbleId(0)
	{
	}

	INT32 height;
	bool bold;
	bool italic;
	bool underline;
	bool strikeout;
	UINT32 color;
	UINT32 bubbleId;
	std::wstring fontName;
	bool operator !=( const TEXT_FONT_INFO &other )
	{
		return ( height != other.height 
			|| bold != other.bold
			|| italic != other.italic
			|| underline != other.underline
			|| strikeout != other.strikeout
			|| color != other.color
			|| fontName != other.fontName 
			|| bubbleId != other.bubbleId
			);
	}
};

struct TEXTCHAT_INFO : public TEXT_FONT_INFO
{
	TEXTCHAT_INFO() : sd(0)
	{
	}

	UINT32 sd;		// send time
	std::wstring msg;
};

struct TEXTCHAT4GRP_INFO : public TEXT_FONT_INFO
{
	TEXTCHAT4GRP_INFO() : gid(0), sd(0), imid(0)
	{
	}

	UINT32 gid;
	std::wstring msg;
	UINT32 sd;		// send time
	UINT32 imid; //增加yy号
};

struct TextChatInner : public TEXT_FONT_INFO
{
	TextChatInner() : gid(0), imid(0), type(0)
	{}

	UINT32 gid;
	UINT32 imid;
	UINT32 type;	// 0为一般聊天信息，1为自动回复信息
	std::wstring msg;
};

struct UserItem_t
{
	UserItem_t() : m_uUid(0)
	{
	}

	UINT32 m_uUid;
	std::wstring m_wstrNick;
};

struct UserItemOfflineMsg_t : public UserItem_t
{
	UserItemOfflineMsg_t() : uOfflineMsgCount(0)
	{
	}

	UINT32 uOfflineMsgCount;
};

// 在线状态
enum IMSTATUS
{
	IMSTATUS_ONLINE = 0,
	IMSTATUS_HIDE = 1,
	IMSTATUS_OFFLINE = 2, 
	IMSTATUS_BUSY = 3,    //  在protocol中提前做好兼容，对UI来说 等同于在线
	IMSTATUS_LEAVE = 4,   //  方便后续忙碌、离开等状态的扩展
	IMSTATUS_GAMING = 5,    //  游戏中

	IMSTATUS_UNKNOWN = 255,    //未知状态
};

// 与EClientType同步
enum IMCLIENTTYPE
{
	IMCLIENTTYPE_PC = 0,
	IMCLIENTTYPE_MOBILE,
	IMCLIENTTYPE_WEBYY,
    IMCLIENTTYPE_XMAN,
    IMCLIENTTYPE_PAD,
	IMCLIENTTYPE_HD,
	IMCLIENTTYPE_UNKNOW = 255
};

// 与EMobileType同步
enum IMMOBILETYPE
{
    IM_MOBILE_UNKNOWN = 0,
    IM_MOBILE_IOS,    // 对手机来讲，该类型表示iPhone, 平板来讲，就是iPad
    IM_MOBILE_ANDROID,
    IM_MOBILE_S60,
    IM_MOBILE_WINPHONE,
};

// 与EAppType同步
enum IMAPPLICATIONTYPE
{
    IM_APP_NONE = 0,
    IM_APP_DUOYOU = 6,   // 多友
    IM_APP_LOL = 11,     // LOL盒子
    IM_APP_WONDER = 12,   // 精彩世界
    //IM_APP_XUNHUAN = 35  // 寻欢
};

struct tagUserStats
{
    UINT32 uId;
    IMSTATUS status;
    IMCLIENTTYPE client;
	IMMOBILETYPE moblietype;
    IMAPPLICATIONTYPE appType;

    tagUserStats()
        : uId(0), status(IMSTATUS_ONLINE), client(IMCLIENTTYPE_PC), moblietype(IM_MOBILE_UNKNOWN), appType(IM_APP_NONE)
    { }
};

enum BizSysMsgClosedType
{
	BIZ_CLOSED_BY_URL,    // 点击URL关闭, 包括点击"查看"按钮触发的URL
	BIZ_CLOSED_BY_BUTTON, // 点击×关闭, 包括点击"确定"按钮关闭
	BIZ_CLOSED_BY_TIMER,   // 自动消失的
    BIZ_CLOSED_BY_FULLSCREEN, // 全屏被客户端屏蔽
    BIZ_CLOSED_BY_IMSTATUS,   // 游戏中或忙碌状态被客户端屏蔽
    BIZ_CLOSED_BY_INVALIDDATA,  //数据不正确未显示
    BIZ_CLOSED_BY_OUTOFDATE,    //过期消息未显示
	BIZ_CLOSED_BY_IN_CHANNEL    //在频道中
    // !!加新类型要先跟服务器协商，而且要加在最后面
};

struct GroupUIInfo
{
	GroupUIInfo()
	{
		clear();
	}
	void clear()
	{
		vipGrade = uid = role = imid = imageIndex = ban = 0; 
		isBuddy = false;
		nStatus = IMSTATUS_UNKNOWN; 
		sex = 0;
		nickname.clear();
		gnick.clear();
		imageURL.clear();
		clientType = IMCLIENTTYPE_PC;
		mobileType = 0;
		bIsStatusGet = false;
        mobileType = IM_APP_NONE;
		friendRemark.clear();
	}
	UINT32 uid;
	std::wstring nickname;	// MEMBER_CHANGE_HINT_NAME  昵称或群名片
	std::wstring gnick;		// 
	UINT32 role;	// GF_ROLE_TYPE	// MEMBER_CHANGE_HINT_ROLE
	IMSTATUS	nStatus;	// MEMBER_CHANGE_HINT_ONLINE
	bool		bIsStatusGet;	// 判断群成员状态是否是拉取回来的。
	int			sex;		// MEMBER_CHANGE_HINT_SEX
	UINT32		ban;		// MEMBER_CHANGE_HINT_BANNED
	UINT32		imid;		// MEMBER_CHANGE_HINT_IMID
	UINT32      imageIndex;		// MEMBER_CHANGE_HINT_AVATAR
	bool        isBuddy;    // MEMBER_CHANGE_HINT_ISBUDDY 与我是否好友关系
	IMCLIENTTYPE         clientType; // MEMBER_CHANGE_HINT_CLIENT_TYPE 对方的登录类型
	int			mobileType;	// EMobileType 手机类型
    int         appType;    // EAppType应用类型
	std::wstring imageURL;
	std::wstring friendRemark; // MEMBER_FRIEND_REMARK 好友备注名
	int      vipGrade;        // MEMBER_CHANGE_HINT_IMVIP
	int		  vipType; //MEMBER_CHANGE_HINT_IMVIPTYPE
	UINT32    lastReplayTime; //MEMBER_CHANGE_HINT_REPLYTIME
};
/*
 *	通知UI接受数据
 */
struct P2PTransmitUIData
{
	P2PTransmitUIData()
	{
		TotalSize		= 0;
		TransmitSize	= 0;
		TransmitSpeed	= 0;
		IsSend			= false;
		IsFile			= false;			
		Resume			= false;
		CanSave			= false;
		State			= 0;

		uid				= -1;
		op_code			= 0;
		op_flag			= 0;
        m_nDetailTransMethod = 0;
		m_wstrFileErrnoNoteName = _T("");
		m_isPeerDieaway = false;
		m_isPeerSuppUnionChan = true;
		task_action    = 0;
	}

	P2PTransmitUIData& operator=(const P2PTransmitUIData& input)
	{
		if (this == &input)
		{
			return *this;
		}

		TaskID			= input.TaskID;
		ContentName		= input.ContentName;
		SavePath		= input.SavePath;
		TotalSize		= input.TotalSize;
		TransmitSize	= input.TransmitSize;
		TransmitSpeed	= input.TransmitSpeed;
		IsSend			= input.IsSend;
		IsFile			= input.IsFile;
		Resume			= input.Resume;
		CanSave			= input.CanSave;
		State			= input.State;

		uid				= input.uid;
		op_code			= input.op_code;
		op_flag			= input.op_flag;
		m_nDetailTransMethod = 2; //E_P2P_TRAVERSE_WAN
		m_dwLastTransBytes = 0;
		m_wstrFileErrnoNoteName = input.m_wstrFileErrnoNoteName;
		m_isPeerDieaway = input.m_isPeerDieaway;
		m_isPeerSuppUnionChan = input.m_isPeerSuppUnionChan;
		task_action        =  input.task_action;

		return *this;
	}
	bool IsCancel() const
	{
		return op_flag < 200 && op_flag == 106;
	}

	std::string			TaskID;
	std::wstring		ContentName;	// 传送内容名字
	std::wstring		SavePath;
	std::wstring		m_wstrFileErrnoNoteName; //文件IO异常日志.
	UINT64				TotalSize;		// 文件/目录的大小
	UINT64				TransmitSize;	// 已传送的大小
	UINT64				TransmitSpeed;	// 传送速度
	bool				IsSend;
	bool				IsFile;			
	bool				Resume;			// 续传
	bool				CanSave;		// 空间够不够存放文件
	bool 				m_isPeerDieaway;//Cancel时判断对方是否长时间掉线.
	bool 				m_isPeerSuppUnionChan;//对方是否支持1对1统一数据通道.
	int					State;			// 当前任务状态

	UINT32				uid;			// 以下变量只在接受文件时使用，保留底层信息
	UINT16				op_code;
	UINT16				op_flag;
	int					m_nDetailTransMethod;//具体的传输类型,用于通知UI.
	UINT64			    m_dwLastTransBytes;//采用续传的时, 续传大小(上次已传大小);
	UINT16		        task_action;	// 任务操作
};

struct ImHistroyMsgRecord
{
	ImHistroyMsgRecord()
		: dt(0)
		, gid(0)
		, msgType(0)
		, belongToTableUid(0)
		, pageIndex(0)
	{}
	time_t	  dt; // 日期和时间
	std::wstring msg;
	std::wstring from; //发送者昵称
	std::wstring to;
	std::wstring fromid;
	std::wstring toid;
	std::wstring guid;
	UINT32 gid;
	UINT32 msgType;	//消息类型，包括文本、文件与语音消息类型
	TEXT_FONT_INFO font;
	UINT32 belongToTableUid;	//消息记录属于哪个用户的表
	UINT32	pageIndex;			//属于哪页
};

struct	ImCacheMsgRecord : ImHistroyMsgRecord
{
	ImCacheMsgRecord()
		:isinvite(false)
		,isauto(false)
		,msgid(0)
		,clr(0)
		,height(0)
	{

	}
	BOOL isinvite;
	std::wstring channelid;
	std::wstring channelname;
    std::wstring data1;
	std::wstring groupid;
	UINT32 msgid;
	BOOL isauto;
	UINT32 clr;
	UINT32 height;
};

struct GroupHistroyMsgRecord
{
	UINT32					groupId;		//所在群id
	UINT32					folderId;		//所在组id
	UINT32					uSenderID;		//发送者ID
	UINT32					uSendTime;		//发送时间
	UINT32					seqId;			//seqid
	UINT64					tTimeStamp;		//消息的GUID
	std::wstring			groupName;		//群或组的名称
	std::wstring			strMsgText;		//消息正文
	std::wstring			strSenderName;	//发送者当时使用的昵称或者群名片
	TEXT_FONT_INFO			fontInfo;		//消息的字体信息
	
	GroupHistroyMsgRecord()
		: uSenderID(0)
		, uSendTime(0)
		, tTimeStamp(0)
		, groupId(0)
		, folderId(0)
	{}
};

#pragma pack(pop)

/*
 *	抛到消息盒子的消息类别
 */
enum MessageKind
{
	KIND_NULL				= 0,		//没消息时的状态
	//ADD_FRIEND_OFFLINE,		// 离线的时候，有人加自己为好友的通知。
	//ADD_FRIEND_ONLINE ,		// 有人要加自己为好友
	//ADD_FRIEND_RESULT ,		// 添加好友时别人发过来的确认信息
	FRIEND_ADD				= 0x1,				//加好友的消息
	FRIEND_CHATVIEW			= 0x2 ,				// 好友对话信息
	FRIEND_SEND_ERROR		= 0x4 ,				// 好友发送失败信息
	FRIEND_INVITE_CHANNEL	= 0x8,				//好友邀请进频道
	FRIEND_INVITE_GROUP		= 0x10,				//3.0向2.0好友发送邀请到群消息
	SESSION_ADD_RESULT		= 0x20,				//申请加入公会的结果
	SESSION_COMPLAINT_RESULT = 0x40,			// 频道投诉的结果

	GROUP_TEMP_MESSAGE		= 0x80,				// 群临时会话

	POP_FRAME_MESSAGE		= 0x90,				// 泡泡弹窗消息

	//群消息类型
	GROUP_TEAM_CHAT			= 0x100,			//群或都组的聊天消息
	GROUP_SYSTEM_MESSAGE	= 0x200,			//群的验证消息

	FRIEND_SEND_FILE		= 0x400,			// 收到文件
	FRIEND_VOICE_INVITE		= 0x800,			// 收到语音请求
	FRIEND_VIDEO_INVITE		= 0x1000,			// 收到视频请求
	MUL_CHATVIEW			= 0x2000,			// 多人聊天
	FRIEND_VERIFY			= 0x4000,           //好友验证
	FRIEND_AUTOREPLAY		= 0X8000,			//自动回复
	//值大于或者等于20的时候，表示此消息不能被忽略(消息盒子的忽略全部）
	FRIEND_MUL_TALK			= 0x100000,

	//多玩多问消息类型
	WORLD_NEW_ANSWER			= 0x200000,	//Y世界的问题新回答
	WORLD_ADOPT                 = 0x400000,	//Y世界的回答采纳
	WORLD_QUESTION_CALL        = 0x800000,     //Y世界新闻的召唤

    //公会通知
    GUILD_NOTICE            = 0x1000000,

	//vip弹窗消息
	VIP_MSG					= 0x2000000,

	//个人中心消息
	PERSONAL_CENTER_MSG = 0X4000000,

	//频道消息
	CHANNELMSG_PRIVATECHAT = 0x6000000,
	//频道信息,右下角通知消息, 点击弹出MessageBox
	CHANNELMSG_MSGBOX = 0x8000000


};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//enum, and their stringizing/stringparser


//////////////////////////////////////////////////////////////////////////
//common enum

enum ISP_TYPE //same as ISPType
{
	ISP_AUTO = 0, 
	ISP_CTC = 1, 
	ISP_CNC = 2, 
	ISP_CTCxCNC = ISP_CTC | ISP_CNC, 
};

enum BIZ_ISPType{
	BIZ_AUTO_DETECT = 0,
	BIZ_CTL = 1,	//电信
	BIZ_CNC = 2,	//网通
	BIZ_CNII = 4,	//铁通
	BIZ_EDU = 8,	//教育网
	BIZ_WBN	= 16,	//长城宽带

	BIZ_GENERAL_DEPLOY = 3,	// general session, main deploy type
	BIZ_SIFU_DEPLOY = 45,		
	BIZ_PIP_DEPLOY = 46,
	BIZ_IM_DEPLOY = 47,
	BIZ_MAFIA_DEPLOY = 48,
};

namespace stringizing {
	template <> inline std::wstring ToString(const ISP_TYPE& v) {
		return ToString((UINT32)v);
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, ISP_TYPE* pv) {
		UINT32 n;
		if (! Parse(sz, &n))
			return false;
		*pv = (ISP_TYPE)n;
		return true;
	}
}

enum FREEZEN_TYPE
{
	FREEZEN_KICKOUT_CHANNEL,
	FREEZEN_LOGIN
};
namespace stringizing {
	template <> inline std::wstring ToString(const FREEZEN_TYPE& v) {
		return ToString((UINT32)v);
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, FREEZEN_TYPE* pv) {
		UINT32 n;
		if (! Parse(sz, &n))
			return false;
		*pv = (FREEZEN_TYPE)n;
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
//channel enum

enum CHANNEL_STYLE	//same as protocol::session::ChannelStyle
{
	CHANNEL_STYLE_FREE = 0, 
	CHANNEL_STYLE_CHAIR = 1, 
	CHANNEL_STYLE_MAIXU = 2, 
	CHANNEL_STYLE_DEFAULT = CHANNEL_STYLE_CHAIR
};
namespace stringizing {
	template <> inline std::wstring ToString(const CHANNEL_STYLE& v) {
		switch (v) {
			case CHANNEL_STYLE_CHAIR:	return L"chair";
			case CHANNEL_STYLE_MAIXU:	return L"maixu";
			case CHANNEL_STYLE_FREE:	return L"free";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, CHANNEL_STYLE* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			return false;
		else if (string_helper::is_string_begin_with(sz, L"chair", false))
			*pv = CHANNEL_STYLE_CHAIR;
		else if (string_helper::is_string_begin_with(sz, L"maixu", false))
			*pv = CHANNEL_STYLE_MAIXU;
		else if (string_helper::is_string_begin_with(sz, L"free", false))
			*pv = CHANNEL_STYLE_FREE;
		else
			return false;
		return true;
	}
}


enum VOICE_QUALITY //same as protocol::session::VoiceQC
{
	VOICE_QUALITY_HIGH = 0, 
	VOICE_QUALITY_REALTIME = 1, 
	VOICE_QUALITY_DEFAULT = VOICE_QUALITY_REALTIME 
};
namespace stringizing {
	template <> inline std::wstring ToString(const VOICE_QUALITY& v) {
		switch (v) {
			case VOICE_QUALITY_HIGH:	return L"high";
			case VOICE_QUALITY_REALTIME:return L"realtime";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, VOICE_QUALITY* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			return false;
		else if (string_helper::is_string_begin_with(sz, L"high", false))
			*pv = VOICE_QUALITY_HIGH;
		else if (string_helper::is_string_begin_with(sz, L"realtime", false))
			*pv = VOICE_QUALITY_REALTIME;
		else
			return false;
		return true;
	}
}

enum GUILD_TYPE //same as protocol::session::GuildType
{
	GUILD_GAME = 0, 
	GUILD_FUN,
	GUILD_OTHER,
	GUILD_EDUCATION,
	GUILD_FINANCIAL,
	GUILD_DEFAULT = GUILD_OTHER,
	GUILD_INTERACTIONS = 5, //互动.
    GUILD_YUEZHAN
};
namespace stringizing {
	template <> inline std::wstring ToString(const GUILD_TYPE& v) {
		switch (v) {
			case GUILD_GAME:	return L"game";
			case GUILD_FUN:		return L"fun";
			case GUILD_OTHER:	return L"other";
			case GUILD_EDUCATION: return L"education";
			case GUILD_FINANCIAL: return L"financial";
			case GUILD_INTERACTIONS: return L"interactions";
            case GUILD_YUEZHAN: return L"yuezhan";
			default: ASSERT(false); return L"other";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, GUILD_TYPE* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			return false;
		else if (string_helper::is_string_begin_with(sz, L"game", false))
			*pv = GUILD_GAME;
		else if (string_helper::is_string_begin_with(sz, L"fun", false))
			*pv = GUILD_FUN;
		else if (string_helper::is_string_begin_with(sz, L"other", false))
			*pv = GUILD_OTHER;
		else if (string_helper::is_string_begin_with(sz, L"education", false))
			*pv = GUILD_EDUCATION;
		else if (string_helper::is_string_begin_with(sz, L"financial", false))
			*pv = GUILD_FINANCIAL;
		else if (string_helper::is_string_begin_with(sz, L"interactions", false))
			*pv = GUILD_INTERACTIONS;
        else if (string_helper::is_string_begin_with(sz, L"yuezhan", false))
            *pv = GUILD_YUEZHAN;
		else
			*pv = GUILD_OTHER;
		return true;
	}
}




//////////////////////////////////////////////////////////////////////////
//user enum

enum GENDER	//same as protocol::uinfo::SEX
{
	GENDER_UNKNOWN = 2, //=unknown
	GENDER_FEMALE = 0, 
	GENDER_MALE = 1, 
};
namespace stringizing {
	template <> inline std::wstring ToString(const GENDER& v) {
		switch (v) {
			case GENDER_UNKNOWN:	return L"";
			case GENDER_FEMALE:	return L"female";
			case GENDER_MALE:		return L"male";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, GENDER* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			*pv = GENDER_UNKNOWN;
		else if (string_helper::is_string_begin_with(sz, L"female", false))
			*pv = GENDER_FEMALE;
		else if (string_helper::is_string_begin_with(sz, L"male", false))
			*pv = GENDER_MALE;
		else
			return false;
		return true;
	}
}


enum IM_STATUS	//same as protocol::E_IMSTATUS
{
	IM_STATUS_OFFLINE = 2, //=IMOFFLINE
	IM_STATUS_NORMAL = 0, 
	IM_STATUS_HIDE = 1, 
	IM_STATUS_BUSY = 3,
	IM_STATUS_LEAVE = 4,
	IM_STATUS_GAMING = 5 
};

enum MULCHAT_ERRCODE
{
	MULCHAT_SUCCESS = 0,
	MULCHAT_MEMBER_LESS,							//会话的人数不够
	MULCHAT_MEMBER_LIMIT,							//超出了规定的人数限制（限于创建和加人时）
	MULCHAT_INVITE_ONLINE_EMPTY						//邀请的在线用户为空
};

// 对应 protocol::im::E_UI_LIST_CHANGED_OP
enum LIST_CHANGED_OP
{
	LIST_CHANGED_BUDDY_REMARK,
	LIST_CHANGED_FOLDER_REMARK,
	LIST_CHANGED_BUDDY_INFO,
	LIST_CHANGED_BUDDY_STATUS,		
	LIST_CHANGED_BUDDY_HEADPHOTO,
	LIST_CHANGED_BUDDY_GAME,
	LIST_CHANGED_BUDDY_CHANEL,
	LIST_CHANGED_BUDDY_ALL,
	LIST_CHANGED_BUDDY_IMID,
	LIST_CHANGED_BUDDY_CUSTOMHDHEADPHOTO,
	LIST_CHANGED_BUDDY_SIMPLE_USERINFO,
	//LIST_CHANGED_VIPMEMBER_INFO
};

enum BUDDY_HDPHOTO_TYPE
{
	HEADPHOTO_60_60 = 0 ,
	HEADPHOTO_100_100,
	HEADPHOTO_144_144,
	HEADPHOTO_640_640
};

// 抛消息时带的标签，表示不同的消息类型
enum MSG_FLAG
{
	 MSG_NORMAL = 0				//普通消息
	,MSG_SENDER					//消息真实发送者
	,MSG_LEAVE_MULCHAT			//某人离开了多人会话
	,MSG_ALLLEAVE_MULCHAT		//所有人离开了多人会话
};

namespace stringizing {
	template <> inline std::wstring ToString(const IM_STATUS& v) {
		switch (v) {
			case IM_STATUS_OFFLINE:	return L"";
			case IM_STATUS_NORMAL:	return L"normal";
			case IM_STATUS_HIDE:	return L"hide";
			case IM_STATUS_BUSY:	return L"busy";
			case IM_STATUS_LEAVE:	return L"leave";
			case IM_STATUS_GAMING:	return L"gaming";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, IM_STATUS* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			*pv = IM_STATUS_OFFLINE;
		else if (string_helper::is_string_begin_with(sz, L"normal", false))
			*pv = IM_STATUS_NORMAL;
		else if (string_helper::is_string_begin_with(sz, L"hide", false))
			*pv = IM_STATUS_HIDE;
		else if (string_helper::is_string_begin_with(sz, L"busy", false))
			*pv = IM_STATUS_BUSY;
		else if (string_helper::is_string_begin_with(sz, L"leave", false))
			*pv = IM_STATUS_LEAVE;
		else if (string_helper::is_string_begin_with(sz, L"gaming", false))
			*pv = IM_STATUS_GAMING;
		else
			return false;
		return true;
	}
}


enum ROLE_IN_CHANNEL	//same as protocol::session::ChannelRoler
{
	ROLE_IN_CHANNEL_NONE = 0, 
	ROLE_IN_CHANNEL_VISITOR = 20, 
	ROLE_IN_CHANNEL_NORMAL = 25, 
	ROLE_IN_CHANNEL_DELETED = 50, //unused
	ROLE_IN_CHANNEL_TMPVIP = 66, 
	ROLE_IN_CHANNEL_VIP = 88, 
	ROLE_IN_CHANNEL_MEMBER = 100, 
	ROLE_IN_CHANNEL_CMANAGER = 150, 
	ROLE_IN_CHANNEL_PMANAGER = 175, 
	ROLE_IN_CHANNEL_MANAGER = 200, 
	ROLE_IN_CHANNEL_VICE_OWNER = 230, 
	ROLE_IN_CHANNEL_OWNER = 255, 
	ROLE_IN_CHANNEL_KEFU = 300, 
	ROLE_IN_CHANNEL_POLICE = 400, 
	ROLE_IN_CHANNEL_SA = 1000, 
};
namespace stringizing {
	template <> inline std::wstring ToString(const ROLE_IN_CHANNEL& v) {
		switch (v) {
			case ROLE_IN_CHANNEL_NONE:		return L"";
			case ROLE_IN_CHANNEL_VISITOR:	return L"visitor";
			case ROLE_IN_CHANNEL_NORMAL:	return L"normal";
			case ROLE_IN_CHANNEL_DELETED:	return L"deleted";
			case ROLE_IN_CHANNEL_TMPVIP:	return L"tmpvip";
			case ROLE_IN_CHANNEL_VIP:		return L"vip";
			case ROLE_IN_CHANNEL_MEMBER:	return L"member";
			case ROLE_IN_CHANNEL_CMANAGER:	return L"cmanager";
			case ROLE_IN_CHANNEL_PMANAGER:	return L"pmanager";
			case ROLE_IN_CHANNEL_MANAGER:	return L"manager";
			case ROLE_IN_CHANNEL_VICE_OWNER:return L"viceOwner";
			case ROLE_IN_CHANNEL_OWNER:		return L"owner";
			case ROLE_IN_CHANNEL_KEFU:		return L"kefu";
			case ROLE_IN_CHANNEL_POLICE:	return L"police";
			case ROLE_IN_CHANNEL_SA:		return L"sa";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, ROLE_IN_CHANNEL* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			*pv = ROLE_IN_CHANNEL_NONE;
		else if (string_helper::is_string_begin_with(sz, L"visitor", false))
			*pv = ROLE_IN_CHANNEL_VISITOR;
		else if (string_helper::is_string_begin_with(sz, L"normal", false))
			*pv = ROLE_IN_CHANNEL_NORMAL;
		else if (string_helper::is_string_begin_with(sz, L"deleted", false))
			*pv = ROLE_IN_CHANNEL_DELETED;
		else if (string_helper::is_string_begin_with(sz, L"tmpvip", false))
			*pv = ROLE_IN_CHANNEL_TMPVIP;
		else if (string_helper::is_string_begin_with(sz, L"vip", false))
			*pv = ROLE_IN_CHANNEL_VIP;
		else if (string_helper::is_string_begin_with(sz, L"member", false))
			*pv = ROLE_IN_CHANNEL_MEMBER;
		else if (string_helper::is_string_begin_with(sz, L"cmanager", false))
			*pv = ROLE_IN_CHANNEL_CMANAGER;
		else if (string_helper::is_string_begin_with(sz, L"pmanager", false))
			*pv = ROLE_IN_CHANNEL_PMANAGER;
		else if (string_helper::is_string_begin_with(sz, L"manager", false))
			*pv = ROLE_IN_CHANNEL_MANAGER;
		else if (string_helper::is_string_begin_with(sz, L"viceOwner", false))
			*pv = ROLE_IN_CHANNEL_VICE_OWNER;
		else if (string_helper::is_string_begin_with(sz, L"owner", false))
			*pv = ROLE_IN_CHANNEL_OWNER;
		else if (string_helper::is_string_begin_with(sz, L"kefu", false))
			*pv = ROLE_IN_CHANNEL_KEFU;
		else if (string_helper::is_string_begin_with(sz, L"police", false))
			*pv = ROLE_IN_CHANNEL_POLICE;
		else if (string_helper::is_string_begin_with(sz, L"sa", false))
			*pv = ROLE_IN_CHANNEL_SA;
		else
			return false;
		return true;
	}
}




//////////////////////////////////////////////////////////////////////////
//folder enum

enum FOLDER_TYPE
{
	FOLDER_NORMAL, 
	FOLDER_FAVOR, 
	FOLDER_BLACK, 
};
namespace stringizing {
	template <> inline std::wstring ToString(const FOLDER_TYPE& v) {
		switch (v) {
			case FOLDER_NORMAL:		return L"";
			case FOLDER_FAVOR:		return L"favor";
			case FOLDER_BLACK:		return L"black";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, FOLDER_TYPE* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			*pv = FOLDER_NORMAL;
		else if (string_helper::is_string_begin_with(sz, L"favor", false))
			*pv = FOLDER_FAVOR;
		else if (string_helper::is_string_begin_with(sz, L"black", false))
			*pv = FOLDER_BLACK;
		else
			return false;
		return true;
	}
}




//////////////////////////////////////////////////////////////////////////
//audioDev enum

enum AUDIODEV_TYPE 
{
	AUDIODEV_INVALID, 
	AUDIODEV_INPUT, 
	AUDIODEV_OUTPUT, 
};
namespace stringizing {
	template <> inline std::wstring ToString(const AUDIODEV_TYPE& v) {
		switch (v) {
			case AUDIODEV_INVALID:	return L"";
			case AUDIODEV_INPUT:	return L"input";
			case AUDIODEV_OUTPUT:	return L"output";
			default: ASSERT(false); return L"";
		}
	}
}
namespace stringparser {
	template <> inline bool Parse(LPCWSTR sz, AUDIODEV_TYPE* pv) {
		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (string_helper::is_string_empty(sz))
			*pv = AUDIODEV_INVALID;
		else if (string_helper::is_string_begin_with(sz, L"input", false))
			*pv = AUDIODEV_INPUT;
		else if (string_helper::is_string_begin_with(sz, L"output", false))
			*pv = AUDIODEV_OUTPUT;
		else
			return false;
		return true;
	}
}


//////////////////////////////////////////////////////////////////////////
//audioCodecType enum

const unsigned long dwASSEMBLE_CODEC_TYPE_BASE = 6;
const unsigned long dwCUSTOM_CODEC_TYPE_BASE = 20;
enum AUDIOCODEC_TYPE 
{
	AUDIOCODEC_INVALID = -1, 
	AUDIOCODEC_VOICE_MID = 0, 
	AUDIOCODEC_MUSIC_MID = 1, 
	AUDIOCODEC_VOICE_HIGHER = 2,
	AUDIOCODEC_VOICE_HIGH = 3,
	AUDIOCODEC_VOICE_HIGHER_JITTER = 4,
	AUDIOCODEC_MUSIC_MID_JITTER = 5,
	AUDIOCODEC_VOICE_MID_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 1,
	AUDIOCODEC_VOICE_HIGH_AND_MUSIC_MID =  dwASSEMBLE_CODEC_TYPE_BASE + 2,
	AUDIOCODEC_VOICE_HIGHER_AND_MUSIC_MID_JITTER = dwASSEMBLE_CODEC_TYPE_BASE + 3,
	AUDIOCODEC_VOICE_HIGHER_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 4,			//10,低音质为VOICE_HIGHER(16K)，高音质为MUSIC_MID
	AUDIOCODEC_VOICE_HIGHER_QUALITY1_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 5,	//11,低音质为VOICE_HIGHER_QUALITY1(24k)，高音质为MUSIC_MID
	AUDIOCODEC_VOICE_LOW_FOR_MOBILE = dwCUSTOM_CODEC_TYPE_BASE + 1,
	AUDIOCODEC_VOICE_HIGHER_QUALITY1 = dwCUSTOM_CODEC_TYPE_BASE + 2,
	AUDIOCODEC_VOICE_LOW_FOR_MOBILE_QUALITY2 = dwCUSTOM_CODEC_TYPE_BASE + 3,
	AUDIOCODEC_MUSIC_MID_1_32KBR = dwCUSTOM_CODEC_TYPE_BASE + 4,
	AUDIOCODEC_MUSIC_MID_1_40KBR = dwCUSTOM_CODEC_TYPE_BASE + 5,
	AUDIOCODEC_MUSIC_MID_1_48KBR = dwCUSTOM_CODEC_TYPE_BASE + 6,
	AUDIOCODEC_MUSIC_MID_2_24KBR = dwCUSTOM_CODEC_TYPE_BASE + 7,
	AUDIOCODEC_MUSIC_MID_2_32KBR = dwCUSTOM_CODEC_TYPE_BASE + 8,
	AUDIOCODEC_MUSIC_MID_2_40KBR = dwCUSTOM_CODEC_TYPE_BASE + 9,
	AUDIOCODEC_MUSIC_MID_2_48KBR = dwCUSTOM_CODEC_TYPE_BASE + 10,
	AUDIOCODEC_MUSIC_MID_2_24KBR_VBR = dwCUSTOM_CODEC_TYPE_BASE + 11,
	AUDIOCODEC_MUSIC_MID_2_32KBR_VBR = dwCUSTOM_CODEC_TYPE_BASE + 12,
	AUDIOCODEC_MUSIC_MID_2_40KBR_VBR = dwCUSTOM_CODEC_TYPE_BASE + 13,
	AUDIOCODEC_MUSIC_MID_2_48KBR_VBR = dwCUSTOM_CODEC_TYPE_BASE + 14,
	AUDIOCODEC_MUSIC_AAC_44100HZ_STEREO_128KBPS = dwCUSTOM_CODEC_TYPE_BASE + 15,
	AUDIOCODEC_MUSIC_AAC_44100HZ_STEREO_160KBPS = dwCUSTOM_CODEC_TYPE_BASE + 16,
	AUDIOCODEC_MUSIC_AAC_44100HZ_STEREO_192KBPS = dwCUSTOM_CODEC_TYPE_BASE + 17
};

//////////////////////////////////////////////////////////////////////////
//Channel Voice Speak Style

enum AUDIO_SPEAK_STYLE_TYPE
{
	AUDIO_SPEAK_STYLE_FREE = 0,
	AUDIO_SPEAK_STYLE_PRESS_KEY
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//internal used

template <class Signature>
struct _def_boost_signals2_signal_type
{
	typedef xsignals::signal<Signature> type;
};

#define _SUPPORTING_SIGS_BATCH()		\
	typedef xsignals::signal<void ()>	_SIG_BEGIN_SIGS_BATCH;	\
	_SIG_BEGIN_SIGS_BATCH sigBeginSigsBatch;					\
	typedef xsignals::signal<void ()>	_SIG_END_SIGS_BATCH;	\
	_SIG_END_SIGS_BATCH sigEndSigsBatch; 


enum BROADCAST_RECIEVER_MASK
{
	BROADCAST_TO_ALL     = 0x01,
	BROADCAST_TO_MANAGER = 0x02,
	BROADCAST_TO_MEMBER  = 0x04,
	BROADCAST_TO_VIP     = 0x08,
	BROADCAST_TO_GUEST   = 0x10
};

//系统消息显示类型
enum SYSMSG_DISPLAY_TYPE
{
	//10秒后自动关闭系统消息窗口
	SYSMSG_AUTO_CLOSE = 0,
	//用户手动关闭
	SYSMSG_USER_CLOSE = 1,

	SYSMSG_PRIVILEDGE = 2,
	//邮箱密保
	SYSMSG_MAILCHANGE = 3,

	//频道活动提醒
	SYSMSG_ACTIVITY = 4,

	//新消息消息类型
	COMMON_SYSTEMMSG = 100,

	//安全等级
	ENUM_SYSMSG_SAFEACCT = 103,

	//YYBanner消息
	ENUM_YYBANNER = 5,
    ENUM_YYBANNERV2 = 6,

	// 接待系统
	ENUM_RECEPTION = 105,

    //DIY系统弹窗
    SYSMSG_DIY = 106,

    //新功能提醒
    NEW_ARRIVAL = 108,

	// 精彩节目消息
	SYSMSG_WONDER_PROG = 200,

	// 粉丝弹窗
	SYSMSG_FOLLOW = 201,

	// 保留的类型
	ENUM_RESERVE_MIN = 500,
	ENUM_RESERVE_MAX = 599
};

struct NORMAL_SYSTEM_MESSAGE
{
	SYSMSG_DISPLAY_TYPE		type;
	std::wstring			catalog;
	std::wstring			title;
	std::wstring			message;
	std::wstring			url;
	UINT32					mid;
};

struct COMPLAIN_SYSTEM_MESSAGE
{
	std::wstring	guid;
	UINT32			seqId;
	UINT32			channelSid;
	std::wstring	result;
	std::wstring	action;
	UINT32			channelAsid;
};

struct APPLY_GUILD_SYSTEM_MESSAGE
{
	UINT32			channelSid;
	UINT32			channelAsid;
	std::wstring	channelName;
	UINT32			managerUid;
	std::wstring	managerNick;
	ROLE_IN_CHANNEL	role;
	bool			isPass;
};

// 精彩节目消息数据结构体
struct WONDER_PROGRAM_SYSTEM_MESSAGE
{
	std::wstring	id; // 消息的唯一id。格式是“channelSid:channelSubSid:aUid”。
	UINT32			activeTime; // 最晚有效时间。时间戳，单位是秒。
	UINT32			channelSid;
	UINT32			channelSubSid;
	UINT32			aUid; // 主播UID
	std::wstring	title;
};

enum BUDDYSEARCH_CONDITION
{
	CONDITION_BYJIFENRES,
	CONDITION_QUESTIONRES,
	CONDITION_BYOPRES
};

enum AUDIO_RESULT
{
	AUDIO_OK,
	AUDIO_ERROR,
	AUDIO_NO_PERMISSIONS,
};

// 这个是自定义头像的定义，其原身在 protocol::im::IImChanelSet::CUSTOM_LOGO_NUMBER
enum
{
	CUSTOM_LOGO_NUMBER	= 0,
	DEFAULT_LOGO_NUMBER	= -1,
};

//audio statistic
enum AUDIO_STATISTIC_TYPE
{
	AUDIO_STATISTIC_RESET_LOST_PACKET,
	AUDIO_STATISTIC_CALCULATE_PACKET,		// 收到多少包
	AUDIO_STATISTIC_SHAKE_BUFFER_DELAY,		// 抖动缓冲
	AUDIO_STATISTIC_LOST_BY_NET_DELAY,		// 网络延迟造成的丢包
	AUDIO_STATISTIC_LOST_BY_PLAY,			// 播放丢包
	AUDIO_STATISTIC_SUCCEED_DECODE_LOSS,	// 
	AUDIO_STATISTIC_JITTER_DELAY,			// 抖动
	AUDIO_STATISTIC_CLIP,					// 因为网络延迟播放停止
	AUDIO_STATISTIC_CHANNEL_AUDIO_TYPE,		// 音频类型。高低音质
	AUDIO_STATISTIC_UPLOAD_PACKET,			// 上传包
	AUDIO_STATISTIC_UPLOAD_LOST_PACKET,		// 上传丢包
	AUDIO_STATISTIC_LOST_PACKET,			// 丢包
	AUDIO_STATISTIC_VAD,					// VAD灵敏度
    AUDIO_STATISTIC_NO_VOICE,               // 听不到声音
    AUDIO_STATISTIC_LOST_BY_PLAY_PER_CHANNEL_TYPE// 分频道类型统计的播放丢包
};

enum YYReputationTypes
{
	TypeAllReputation = 1,	/**< 获取所有类型声望数据 */
	TypeReputation,		/**< 获取总声望数据 */
	TypeGameReputation,	/**< 获取游戏声望数据 */
	TypeChannelReputation,	/**< 获取公会管理声望数据 */
	TypeMusicReputation	/**< 获取音乐声望数据 */
};

enum YYReputationGameDataCommitTypes
{
	kTypeRoutineGameData = 1/**< 客户端定时提交的游戏数据 */
};

enum YYImGroupAppTypes
{
	BANGDAI_APP    //帮带系统类型
};

struct ColorRingInfoItem
{
	UINT32 uid;
	UINT32 type;
	UINT32 idx;
};

enum VipRingType
{
	VRT_Text = 1, //同protocol::imvip::E_RING_LITERA
	VRT_Voice = 2, //同protocol::imvip::E_RING_VOICE
};


struct VIP_TYPE_GRADE
{
	VIP_TYPE_GRADE()
	:vipType(0)
	,grade(0)
	{
	}
	UINT32 vipType;
	UINT32 grade;
};
// 紫钻会员信息
enum EPurplePrivi
{
    E_PURPLE_NICK = 0,       // 紫色昵称
    E_PURPLE_SUPER,          // 超级紫钻
    E_PURPLE_EXVIP,          // 至尊会员
    E_PURPLE_GAME,           // 游戏紫钻
	E_SUPER_VIP,             // 超凡会员
	E_GAME_PRIVI,            // 会员游戏特权
};

struct VipPurple
{
    UINT8                      m_grade;                // 0 非紫钻会员  1 ~ 7
    UINT8                      m_type;                 // 0 月费  1 年费
    UINT32                     m_upgradeTime;          // 升级时间
    std::map<UINT32, UINT8>    m_privi;                // 特权字段
};

// 视频操作相关的状态
enum VIDEO_STATE_TYPE
{
	VIDEO_STATE_NONE = 0x0,         // 无，空闲中
	VIDEO_STATE_INVITE = 0x1,       // 邀请
	VIDEO_STATE_INVITED = 0x2,      // 被邀请
	VIDEO_STATE_ING = 0x4,          // 视频中
};

enum CHANNEL_ERROR_CODE
{
	CHANNELERR_UNKNOWN_ERROR, 
	CHANNELERR_INITIATIVE_LEAVE, 
	CHANNELERR_NOT_EXIST,
	CHANNELERR_NETWORK_ERROR,
	CHANNELERR_NETWORK_ERROR_WITH_PARAM,
	CHANNELERR_CONNECT_BROKEN,
	CHANNELERR_BAN_ID_OR_IP,
	CHANNELERR_BAN_ID,
	CHANNELERR_BAN_IP,
	CHANNELERR_RECOVERED,
	CHANNELERR_FROZEN,
	CHANNELERR_FROZEN_WITH_TIME,
	CHANNELERR_BUSY,
	CHANNELERR_FULL_EX,
	CHANNELERR_KICK_OFF,
	CHANNELERR_MODULE_DOWNLOAD_ERROR,
	CHANNELERR_LOCKED,
	CHANNELERR_LOCKED_WITH_TIME, // 频道被锁(带解锁时间)
	CHANNELERR_USER_NUMBER_LIMIT, // 超过频道用户人数
	CHANNELERR_USER_NUMBER_INFLUX_RETRY_TIME, //蜂拥人数时候，提示等待时间
	CHANNELERR_SERVER_BUSY, //服务器繁忙
    CHANNELERR_COOKIE_ERROR, // cookie校验失败
    CHANNELERR_TICKET_EMPTY, // 票据为空
    CHANNELERR_TICKET_OUTDATED, // 票据过期
    CHANNELERR_OTHER_TICKER_ERROR, // 其他票据失败
	CHANNELERR_JOIN_TOO_OFTEN, // 进频道太频繁
    CHANNELERR_CLIENT_ERROR, // 频道由其他客户端创建，目前还未互通
    CHANNELERR_OTHER_CLIENT_KICK, // 其他客户端互踢
	CHANNELERR_NEED_PASSWORD, // 频道设置了密码，要求输入密码
	CHANNELERR_PASSWORD_ERROR, // 输入密码错误
	CHANNELERR_DUOWAN_LIMIT, // 进入duowan指定频道受限
	CHANNELERR_SUBSID_FULL, // 子频道人数达上限，接待频道不受限，管理员不受限
	CHANNELERR_SUBSID_LIMIT, // 顶级频道属性中设置了“子频道只对会员和嘉宾开放”，接待频道不受限，管理员不受限
	CHANNELERR_GUEST_LIMIT, // 子频道属性中设置了“禁止游客进入频道”，顶级频道不受限，管理员不受限
	CHANNELERR_VIP_LIMIT, // VIP子频道禁止进入
	CHANNELERR_CHARGE_LIMIT, // 付费子频道禁止进入
	CHNNNELERR_APP_TOKEN_FAIL,//app或token验证失败
	CHANNELERR_WHITE_LIST_LIMIT, //频道开启了白名单，不在名单中并且不是管理员
	CHANNELERR_REAL_NAME_REGISTER_LIMIT, //设置者：设置管理员时，被设置者没有实名认证
	CHANNELERR_REAL_NAME_REGISTER_LIMIT2, //被设置者：设置管理员时，被设置者没有实名认证, 弹窗提示
};

enum CHANNEL_KICK_OUT_REASON //same as protocol::session::KickOffType
{
    CHANNEL_KICK_OUT, 
    CHANNEL_BAN_ID, 
    CHANNEL_BAN_IP, 
};

enum CHANNEL_ANNOUNCE_TYPE	//same as protocol::session::ISessionWatcher::AnnounceType
{
    CHANNEL_ANNOUNCE_EMERGENCY,
    CHANNEL_ANNOUNCE_ANNOUNCEMENT,
    CHANNEL_ANNOUNCE_AD,
    CHANNEL_ANNOUNCE_NOTICE,
    CHANNEL_ANNOUNCE_WARNING,
    CHANNEL_ANNOUNCE_ERR,
    CHANNEL_ANNOUNCE_ANSWER_QUESTION, 

    // 以下是私有协议，没有在PROTOCOL备案，通过sigEchoStopSession信号透传到客户端
    CHANNEL_ANNOUNCE_RECEPTION = 50 // 接待成功邀请的新用户入场广播
};

enum CHANNEL_OP_ERR_CODE
{
    CHANNEL_ERR_CREATE_SUBCHANNEL,
    CHANNEL_ERR_DISSMISS_SUBCHANNEL,
    CHANNEL_ERR_CHANGE_FOLDER,//进频道失败，结合RES_EAUTH为进频道密码错误
    CHANNEL_ERR_UPDATE_SINFO,
    CHANNEL_ERR_UPDATE_MEMBERROLER,
    CHANNEL_ERR_CHANGE_STYLE,
    CHANNEL_ERR_KICKOFF,
    CHANNEL_ERR_JOIN_SYNC,
    CHANNEL_ERR_SETKEYACTIVE,
    CHANNEL_ERR_USER_OVERLOAD,//频道人数超载
    CHANNEL_ERR_VICE_OWNER_OVERLOAD, // 总管个数超出限制
    CHANNEL_ERR_SUBCHANNEL_COUNT_EXCEEDED, // 创建的子频道数目超出上限
    CHANNEL_ERR_TUOREN, // 调度
};

enum CHANNEL_ROLE_CHANGED_BY_REASON //same as protocol::session::SyncRoleReason
{
    CHANNEL_REASON_INVITE_CODE = 1, 
};

enum CHANNEL_USER_ALLOWED_ACTION
{
    CHANNEL_USER_ACTION_TEXT,
    CHANNEL_USER_ACTION_VOICE,
};

enum SUB_CHANNEL_OP
{
    SUB_CHANNEL_CREATED,
    SUB_CHANNEL_REMOVED,
	SUB_CHANNEL_REMOVED_BY_SYSTEM,
};

struct CHANNEL_MAIXU_CHANGED
{
    enum 
    {
        CHANNEL_MAIXU_TUOREN,
        CHANNEL_MAIXU_KICKOFF,
        CHANNEL_MAIXU_CLEAR,
        CHANNEL_MAIXU_MOVE,
        CHANNEL_MAIXU_MOVE2TOP,
        CHANNEL_MAIXU_MUTIMAI,
    } changedType;

    union 
    {
        UINT32 reserved;
        struct  
        {
            bool down;
        } moveExtendData;
        struct 
        {
            UINT32 first;
            bool add;
        }mutimaiExtendData;
    };
};

namespace yyplugin
{
    namespace appMgr
    {
        struct PluginInfoAdd;
        struct PluginInfoDel;
    }
}

struct CHANNEL_PLATFORM_APP_VERSION //same as yyplugin::appMgr::VersionInfo
{
    UINT32 version;
    UINT32 yyMin;
    UINT32 yyMax;
    std::string md5;
    std::wstring url;
};
struct CHANNEL_PLATFORM_APP_INFO //same as yyplugin::appMgr::PluginInfo
{
    UINT32 appId;
    std::wstring appFileName;
    std::wstring appDescription;
    UINT32 niceAppVersion;
    std::deque<CHANNEL_PLATFORM_APP_VERSION> appVersions;
    bool opByOw;
};

const UINT32 INVALID_TRANSPSVC_CONNID = 0xFFFFFFFF;
typedef UINT16 TRANSPSVC_T;//service transparent tunnel type, defined by server and corresponding to appid.
enum TRANSPSVC_STATUS
{
    TRANSPSVC_CONNECTED,
    TRANSPSVC_CLOSE,
    TRANSPSVC_INIT,
    TRANSPSVC_CONNECTING,
};

struct AppGroupId //for app (multicast)
{
    UINT32 top32;
    UINT64 low64;
};

enum FACE_TYPE
{
    FT_Platform,            // 程序自带表情
    FT_Custom,              // 自定表情
    FT_Vip,                 // vip表情
};

// 表情属性
struct FACE_ATTR
{
    std::wstring strFileorg;        // 本地缓存文件名
    std::wstring strTips;           // tips
    std::wstring strShortcut;       // 快捷
    std::wstring strUrl;            // 服务器上的表情路径
    FACE_TYPE    type;              // 表情类型
    UINT32		  groupId;			//表情所在群ID
    FACE_ATTR() : type(FT_Custom) { }
};

struct RECENT_FACE_ATTR : public FACE_ATTR
{
    std::wstring strGroup;          // 表情组名

    RECENT_FACE_ATTR() { }
    RECENT_FACE_ATTR(const FACE_ATTR& _Right)
    {
        strFileorg = _Right.strFileorg;
        strTips = _Right.strTips;
        strShortcut = _Right.strShortcut;
        strUrl = _Right.strUrl;
        type = _Right.type;
    }
    RECENT_FACE_ATTR(const RECENT_FACE_ATTR& _Right) : FACE_ATTR(_Right)
    {
        strGroup = _Right.strGroup;
    }

    bool operator == (const RECENT_FACE_ATTR& _Right)
    {
        return (strGroup == _Right.strGroup && strFileorg == _Right.strFileorg);
    }
};


//login type

enum LOGINTYPE{
	LOGINTYPE_NORMAL,
	LOGINTYPE_ANONYMOUS,
	LOGINTYPE_FACEBOOK,
	LOGINTYPE_BEANFUN
};


enum P2PChatFrom
{
    From_Default = 0x0,

    From_RecentDefault = 0x1,
    From_RecentListStranger = 0x2,
    From_RecentMask = 0x7,

    From_GroupDefault = 0x8,
    From_GroupListStranger = 0x10,
    From_GroupMask = 0x38,

    From_Other = 0x40,
    From_OtherPlusin = 0x80,
    From_OtherPlatfrom = 0xc0,
    From_OtherMask = 0x1c0,

    From_History = 0x200,
    From_HistoryStranger = 0x400,
    From_HistoryMask = 0xE00,

    From_MultiChat = 0x1000,
    From_MultiStranger = 0x2000,
    From_MultiMask = 0x7000,

	From_NotificationBoxListClick = 0x300,
	From_NotificationBoxViewAll = 0x301,
};

enum BIZ_CHAT_PROPERTY
{
    BIZ_CHAT_PROPERTY_NONE = 0,
    BIZ_CHAT_PROPERTY_EDUCATION = 1 // YY教育
};

enum BIZ_SYSMSG_CLS
{
    BIZ_SYSMSG_SYSTEM = 1,   // 系统自动发起的消息
    BIZ_SYSMSG_SUBSCRIPTION, // 订阅类消息
    BIZ_SYSMSG_PROMOTION     // 业务推广类
};


enum BIZ_BUDDY_CHANNEL_INFO_TYPE
{
    BIZ_BUDDY_CHANNEL_INFO_CH,      // 频道/活动
    BIZ_BUDDY_CHANNEL_INFO_WW,      // 精彩世界
	BIZ_BUDDY_COMPONENT_INFO = 2,         // 组件类型
	BIZ_BUDDY_CHANNEL_INFO_UNKNOWN = 100, // 不想显示到分享的频道
};


enum DATA_NOTIFY_TYPE
{
	DATA_INS,
	DATA_DEL,
	DATA_CHANGED,
	DATA_RESET,
};


struct BCUser{
	UINT32 sid;
	UINT32 scrollbar_uid;
	UINT32 scollbar_pos;
	UINT32 capacity_cnt;
	bool      firstExpand;
	bool      locateUser;
	std::vector<UINT32>	uids;
};
typedef std::vector<BCUser> BCUsers;


typedef enum{
	emPT_Channel_Top = 0, 
	emPT_Channel_Red_Name = 1, 
	emPT_Channel_Vip_Face = 2,
    emPT_Channel_Purple_Name = 3,
}EPrivilegeType;

enum USERROW_LOC_TYPE
{
	USERROW_LOC_NOMAL,
	USERROW_LOC_MGR,
	USERROW_LOC_MICRO,
	USERROW_LOC_ECHO,
};

struct ETEntry
{
	UINT32 sid;
	UINT32 scrollBarUid;
	UINT32 scrollBarPos;
};

struct TimePart
{
	TimePart()
		: begin( 0 )
		, end( 0 )
		, line( 0xffffffff )
	{
	}

	float begin;
	float end;
	int line;
};

#pragma pack(push, 8)
enum NET_PROXY_TYPE //same as protocol::ProxyType
{
	NET_PROXY_NONE, 
	NET_PROXY_SOCKS, 
	NET_PROXY_HTTP, 
	NET_PROXY_IE,
};
struct NET_PROXY_INFO //same as protocol::ProxyInfo
{
	NET_PROXY_TYPE type;
	std::string ip;
	UINT32 port;
	std::string username;
	std::string password;
	std::string domain;
};
#pragma pack(pop)

enum AUDIO_SOURCE_TYPE
{
	AUDIO_SOURCE_MIC = 0, //mic
	AUDIO_SOURCE_OUTER_PCM,  //outer-pcm
	AUDIO_SOURCE_MIX  //mic & outer-pcm
};

//参见protocol::login::DYNAMIC_TOKEN_TYPE 
enum LoginDynamicTokenType
{
	VFY_NONE        = 0x0,                                       // 无动态策略
	VFY_MOBTOKEN    = 0x02,                                      // 手机令牌
	VFY_HWTOKEN     = 0x04,                                      // 硬件令牌
	VFY_SMSCODE     = 0x08,                                      // 短信验证码
	VFY_SEC_QESTION = 0x10,                                       // 密保问题
	VFY_SMSCODE_UP = 0x12										// 短信上行验证码
};

struct SESS_MIC_INFO
{
	UINT32	sid;
	UINT32	pid;
	bool		disable;
	bool		mute;
	UINT32	top_queue;	
	UINT32	time;
	std::vector<UINT32>	mic_list;
	std::vector<UINT32>	muti_mic;
};

struct SESS_SUB_CHANNEL_USER_CHANGED 
{
	UINT32 uid;
	std::string nick;
	ROLE_IN_CHANNEL role;
	UINT32 termnimal;
};

//秩序敏感过滤，见onAnalyzerMsgResult
struct  IMSession_AnalyzerMsgResult
{
    UINT32 fromuid;
    UINT32 touid;
    UINT32 uSeqid;
    UINT32 uSendTime;
    std::string strAlertWords;

	IMSession_AnalyzerMsgResult::IMSession_AnalyzerMsgResult()
	{
		fromuid = 0;
		touid = 0;
		uSeqid = 0;
		uSendTime = 0;
		strAlertWords = "";
	}
};