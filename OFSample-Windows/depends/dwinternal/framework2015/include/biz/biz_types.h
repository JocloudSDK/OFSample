
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
	UINT32 imid; //����yy��
};

struct TextChatInner : public TEXT_FONT_INFO
{
	TextChatInner() : gid(0), imid(0), type(0)
	{}

	UINT32 gid;
	UINT32 imid;
	UINT32 type;	// 0Ϊһ��������Ϣ��1Ϊ�Զ��ظ���Ϣ
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

// ����״̬
enum IMSTATUS
{
	IMSTATUS_ONLINE = 0,
	IMSTATUS_HIDE = 1,
	IMSTATUS_OFFLINE = 2, 
	IMSTATUS_BUSY = 3,    //  ��protocol����ǰ���ü��ݣ���UI��˵ ��ͬ������
	IMSTATUS_LEAVE = 4,   //  �������æµ���뿪��״̬����չ
	IMSTATUS_GAMING = 5,    //  ��Ϸ��

	IMSTATUS_UNKNOWN = 255,    //δ֪״̬
};

// ��EClientTypeͬ��
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

// ��EMobileTypeͬ��
enum IMMOBILETYPE
{
    IM_MOBILE_UNKNOWN = 0,
    IM_MOBILE_IOS,    // ���ֻ������������ͱ�ʾiPhone, ƽ������������iPad
    IM_MOBILE_ANDROID,
    IM_MOBILE_S60,
    IM_MOBILE_WINPHONE,
};

// ��EAppTypeͬ��
enum IMAPPLICATIONTYPE
{
    IM_APP_NONE = 0,
    IM_APP_DUOYOU = 6,   // ����
    IM_APP_LOL = 11,     // LOL����
    IM_APP_WONDER = 12,   // ��������
    //IM_APP_XUNHUAN = 35  // Ѱ��
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
	BIZ_CLOSED_BY_URL,    // ���URL�ر�, �������"�鿴"��ť������URL
	BIZ_CLOSED_BY_BUTTON, // ������ر�, �������"ȷ��"��ť�ر�
	BIZ_CLOSED_BY_TIMER,   // �Զ���ʧ��
    BIZ_CLOSED_BY_FULLSCREEN, // ȫ�����ͻ�������
    BIZ_CLOSED_BY_IMSTATUS,   // ��Ϸ�л�æµ״̬���ͻ�������
    BIZ_CLOSED_BY_INVALIDDATA,  //���ݲ���ȷδ��ʾ
    BIZ_CLOSED_BY_OUTOFDATE,    //������Ϣδ��ʾ
	BIZ_CLOSED_BY_IN_CHANNEL    //��Ƶ����
    // !!��������Ҫ�ȸ�������Э�̣�����Ҫ���������
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
	std::wstring nickname;	// MEMBER_CHANGE_HINT_NAME  �ǳƻ�Ⱥ��Ƭ
	std::wstring gnick;		// 
	UINT32 role;	// GF_ROLE_TYPE	// MEMBER_CHANGE_HINT_ROLE
	IMSTATUS	nStatus;	// MEMBER_CHANGE_HINT_ONLINE
	bool		bIsStatusGet;	// �ж�Ⱥ��Ա״̬�Ƿ�����ȡ�����ġ�
	int			sex;		// MEMBER_CHANGE_HINT_SEX
	UINT32		ban;		// MEMBER_CHANGE_HINT_BANNED
	UINT32		imid;		// MEMBER_CHANGE_HINT_IMID
	UINT32      imageIndex;		// MEMBER_CHANGE_HINT_AVATAR
	bool        isBuddy;    // MEMBER_CHANGE_HINT_ISBUDDY �����Ƿ���ѹ�ϵ
	IMCLIENTTYPE         clientType; // MEMBER_CHANGE_HINT_CLIENT_TYPE �Է��ĵ�¼����
	int			mobileType;	// EMobileType �ֻ�����
    int         appType;    // EAppTypeӦ������
	std::wstring imageURL;
	std::wstring friendRemark; // MEMBER_FRIEND_REMARK ���ѱ�ע��
	int      vipGrade;        // MEMBER_CHANGE_HINT_IMVIP
	int		  vipType; //MEMBER_CHANGE_HINT_IMVIPTYPE
	UINT32    lastReplayTime; //MEMBER_CHANGE_HINT_REPLYTIME
};
/*
 *	֪ͨUI��������
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
	std::wstring		ContentName;	// ������������
	std::wstring		SavePath;
	std::wstring		m_wstrFileErrnoNoteName; //�ļ�IO�쳣��־.
	UINT64				TotalSize;		// �ļ�/Ŀ¼�Ĵ�С
	UINT64				TransmitSize;	// �Ѵ��͵Ĵ�С
	UINT64				TransmitSpeed;	// �����ٶ�
	bool				IsSend;
	bool				IsFile;			
	bool				Resume;			// ����
	bool				CanSave;		// �ռ乻��������ļ�
	bool 				m_isPeerDieaway;//Cancelʱ�ж϶Է��Ƿ�ʱ�����.
	bool 				m_isPeerSuppUnionChan;//�Է��Ƿ�֧��1��1ͳһ����ͨ��.
	int					State;			// ��ǰ����״̬

	UINT32				uid;			// ���±���ֻ�ڽ����ļ�ʱʹ�ã������ײ���Ϣ
	UINT16				op_code;
	UINT16				op_flag;
	int					m_nDetailTransMethod;//����Ĵ�������,����֪ͨUI.
	UINT64			    m_dwLastTransBytes;//����������ʱ, ������С(�ϴ��Ѵ���С);
	UINT16		        task_action;	// �������
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
	time_t	  dt; // ���ں�ʱ��
	std::wstring msg;
	std::wstring from; //�������ǳ�
	std::wstring to;
	std::wstring fromid;
	std::wstring toid;
	std::wstring guid;
	UINT32 gid;
	UINT32 msgType;	//��Ϣ���ͣ������ı����ļ���������Ϣ����
	TEXT_FONT_INFO font;
	UINT32 belongToTableUid;	//��Ϣ��¼�����ĸ��û��ı�
	UINT32	pageIndex;			//������ҳ
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
	UINT32					groupId;		//����Ⱥid
	UINT32					folderId;		//������id
	UINT32					uSenderID;		//������ID
	UINT32					uSendTime;		//����ʱ��
	UINT32					seqId;			//seqid
	UINT64					tTimeStamp;		//��Ϣ��GUID
	std::wstring			groupName;		//Ⱥ���������
	std::wstring			strMsgText;		//��Ϣ����
	std::wstring			strSenderName;	//�����ߵ�ʱʹ�õ��ǳƻ���Ⱥ��Ƭ
	TEXT_FONT_INFO			fontInfo;		//��Ϣ��������Ϣ
	
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
 *	�׵���Ϣ���ӵ���Ϣ���
 */
enum MessageKind
{
	KIND_NULL				= 0,		//û��Ϣʱ��״̬
	//ADD_FRIEND_OFFLINE,		// ���ߵ�ʱ�����˼��Լ�Ϊ���ѵ�֪ͨ��
	//ADD_FRIEND_ONLINE ,		// ����Ҫ���Լ�Ϊ����
	//ADD_FRIEND_RESULT ,		// ��Ӻ���ʱ���˷�������ȷ����Ϣ
	FRIEND_ADD				= 0x1,				//�Ӻ��ѵ���Ϣ
	FRIEND_CHATVIEW			= 0x2 ,				// ���ѶԻ���Ϣ
	FRIEND_SEND_ERROR		= 0x4 ,				// ���ѷ���ʧ����Ϣ
	FRIEND_INVITE_CHANNEL	= 0x8,				//���������Ƶ��
	FRIEND_INVITE_GROUP		= 0x10,				//3.0��2.0���ѷ������뵽Ⱥ��Ϣ
	SESSION_ADD_RESULT		= 0x20,				//������빫��Ľ��
	SESSION_COMPLAINT_RESULT = 0x40,			// Ƶ��Ͷ�ߵĽ��

	GROUP_TEMP_MESSAGE		= 0x80,				// Ⱥ��ʱ�Ự

	POP_FRAME_MESSAGE		= 0x90,				// ���ݵ�����Ϣ

	//Ⱥ��Ϣ����
	GROUP_TEAM_CHAT			= 0x100,			//Ⱥ�����������Ϣ
	GROUP_SYSTEM_MESSAGE	= 0x200,			//Ⱥ����֤��Ϣ

	FRIEND_SEND_FILE		= 0x400,			// �յ��ļ�
	FRIEND_VOICE_INVITE		= 0x800,			// �յ���������
	FRIEND_VIDEO_INVITE		= 0x1000,			// �յ���Ƶ����
	MUL_CHATVIEW			= 0x2000,			// ��������
	FRIEND_VERIFY			= 0x4000,           //������֤
	FRIEND_AUTOREPLAY		= 0X8000,			//�Զ��ظ�
	//ֵ���ڻ��ߵ���20��ʱ�򣬱�ʾ����Ϣ���ܱ�����(��Ϣ���ӵĺ���ȫ����
	FRIEND_MUL_TALK			= 0x100000,

	//���������Ϣ����
	WORLD_NEW_ANSWER			= 0x200000,	//Y����������»ش�
	WORLD_ADOPT                 = 0x400000,	//Y����Ļش����
	WORLD_QUESTION_CALL        = 0x800000,     //Y�������ŵ��ٻ�

    //����֪ͨ
    GUILD_NOTICE            = 0x1000000,

	//vip������Ϣ
	VIP_MSG					= 0x2000000,

	//����������Ϣ
	PERSONAL_CENTER_MSG = 0X4000000,

	//Ƶ����Ϣ
	CHANNELMSG_PRIVATECHAT = 0x6000000,
	//Ƶ����Ϣ,���½�֪ͨ��Ϣ, �������MessageBox
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
	BIZ_CTL = 1,	//����
	BIZ_CNC = 2,	//��ͨ
	BIZ_CNII = 4,	//��ͨ
	BIZ_EDU = 8,	//������
	BIZ_WBN	= 16,	//���ǿ��

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
	GUILD_INTERACTIONS = 5, //����.
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
	MULCHAT_MEMBER_LESS,							//�Ự����������
	MULCHAT_MEMBER_LIMIT,							//�����˹涨���������ƣ����ڴ����ͼ���ʱ��
	MULCHAT_INVITE_ONLINE_EMPTY						//����������û�Ϊ��
};

// ��Ӧ protocol::im::E_UI_LIST_CHANGED_OP
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

// ����Ϣʱ���ı�ǩ����ʾ��ͬ����Ϣ����
enum MSG_FLAG
{
	 MSG_NORMAL = 0				//��ͨ��Ϣ
	,MSG_SENDER					//��Ϣ��ʵ������
	,MSG_LEAVE_MULCHAT			//ĳ���뿪�˶��˻Ự
	,MSG_ALLLEAVE_MULCHAT		//�������뿪�˶��˻Ự
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
	AUDIOCODEC_VOICE_HIGHER_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 4,			//10,������ΪVOICE_HIGHER(16K)��������ΪMUSIC_MID
	AUDIOCODEC_VOICE_HIGHER_QUALITY1_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 5,	//11,������ΪVOICE_HIGHER_QUALITY1(24k)��������ΪMUSIC_MID
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

//ϵͳ��Ϣ��ʾ����
enum SYSMSG_DISPLAY_TYPE
{
	//10����Զ��ر�ϵͳ��Ϣ����
	SYSMSG_AUTO_CLOSE = 0,
	//�û��ֶ��ر�
	SYSMSG_USER_CLOSE = 1,

	SYSMSG_PRIVILEDGE = 2,
	//�����ܱ�
	SYSMSG_MAILCHANGE = 3,

	//Ƶ�������
	SYSMSG_ACTIVITY = 4,

	//����Ϣ��Ϣ����
	COMMON_SYSTEMMSG = 100,

	//��ȫ�ȼ�
	ENUM_SYSMSG_SAFEACCT = 103,

	//YYBanner��Ϣ
	ENUM_YYBANNER = 5,
    ENUM_YYBANNERV2 = 6,

	// �Ӵ�ϵͳ
	ENUM_RECEPTION = 105,

    //DIYϵͳ����
    SYSMSG_DIY = 106,

    //�¹�������
    NEW_ARRIVAL = 108,

	// ���ʽ�Ŀ��Ϣ
	SYSMSG_WONDER_PROG = 200,

	// ��˿����
	SYSMSG_FOLLOW = 201,

	// ����������
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

// ���ʽ�Ŀ��Ϣ���ݽṹ��
struct WONDER_PROGRAM_SYSTEM_MESSAGE
{
	std::wstring	id; // ��Ϣ��Ψһid����ʽ�ǡ�channelSid:channelSubSid:aUid����
	UINT32			activeTime; // ������Чʱ�䡣ʱ�������λ���롣
	UINT32			channelSid;
	UINT32			channelSubSid;
	UINT32			aUid; // ����UID
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

// ������Զ���ͷ��Ķ��壬��ԭ���� protocol::im::IImChanelSet::CUSTOM_LOGO_NUMBER
enum
{
	CUSTOM_LOGO_NUMBER	= 0,
	DEFAULT_LOGO_NUMBER	= -1,
};

//audio statistic
enum AUDIO_STATISTIC_TYPE
{
	AUDIO_STATISTIC_RESET_LOST_PACKET,
	AUDIO_STATISTIC_CALCULATE_PACKET,		// �յ����ٰ�
	AUDIO_STATISTIC_SHAKE_BUFFER_DELAY,		// ��������
	AUDIO_STATISTIC_LOST_BY_NET_DELAY,		// �����ӳ���ɵĶ���
	AUDIO_STATISTIC_LOST_BY_PLAY,			// ���Ŷ���
	AUDIO_STATISTIC_SUCCEED_DECODE_LOSS,	// 
	AUDIO_STATISTIC_JITTER_DELAY,			// ����
	AUDIO_STATISTIC_CLIP,					// ��Ϊ�����ӳٲ���ֹͣ
	AUDIO_STATISTIC_CHANNEL_AUDIO_TYPE,		// ��Ƶ���͡��ߵ�����
	AUDIO_STATISTIC_UPLOAD_PACKET,			// �ϴ���
	AUDIO_STATISTIC_UPLOAD_LOST_PACKET,		// �ϴ�����
	AUDIO_STATISTIC_LOST_PACKET,			// ����
	AUDIO_STATISTIC_VAD,					// VAD������
    AUDIO_STATISTIC_NO_VOICE,               // ����������
    AUDIO_STATISTIC_LOST_BY_PLAY_PER_CHANNEL_TYPE// ��Ƶ������ͳ�ƵĲ��Ŷ���
};

enum YYReputationTypes
{
	TypeAllReputation = 1,	/**< ��ȡ���������������� */
	TypeReputation,		/**< ��ȡ���������� */
	TypeGameReputation,	/**< ��ȡ��Ϸ�������� */
	TypeChannelReputation,	/**< ��ȡ��������������� */
	TypeMusicReputation	/**< ��ȡ������������ */
};

enum YYReputationGameDataCommitTypes
{
	kTypeRoutineGameData = 1/**< �ͻ��˶�ʱ�ύ����Ϸ���� */
};

enum YYImGroupAppTypes
{
	BANGDAI_APP    //���ϵͳ����
};

struct ColorRingInfoItem
{
	UINT32 uid;
	UINT32 type;
	UINT32 idx;
};

enum VipRingType
{
	VRT_Text = 1, //ͬprotocol::imvip::E_RING_LITERA
	VRT_Voice = 2, //ͬprotocol::imvip::E_RING_VOICE
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
// �����Ա��Ϣ
enum EPurplePrivi
{
    E_PURPLE_NICK = 0,       // ��ɫ�ǳ�
    E_PURPLE_SUPER,          // ��������
    E_PURPLE_EXVIP,          // �����Ա
    E_PURPLE_GAME,           // ��Ϸ����
	E_SUPER_VIP,             // ������Ա
	E_GAME_PRIVI,            // ��Ա��Ϸ��Ȩ
};

struct VipPurple
{
    UINT8                      m_grade;                // 0 �������Ա  1 ~ 7
    UINT8                      m_type;                 // 0 �·�  1 ���
    UINT32                     m_upgradeTime;          // ����ʱ��
    std::map<UINT32, UINT8>    m_privi;                // ��Ȩ�ֶ�
};

// ��Ƶ������ص�״̬
enum VIDEO_STATE_TYPE
{
	VIDEO_STATE_NONE = 0x0,         // �ޣ�������
	VIDEO_STATE_INVITE = 0x1,       // ����
	VIDEO_STATE_INVITED = 0x2,      // ������
	VIDEO_STATE_ING = 0x4,          // ��Ƶ��
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
	CHANNELERR_LOCKED_WITH_TIME, // Ƶ������(������ʱ��)
	CHANNELERR_USER_NUMBER_LIMIT, // ����Ƶ���û�����
	CHANNELERR_USER_NUMBER_INFLUX_RETRY_TIME, //��ӵ����ʱ����ʾ�ȴ�ʱ��
	CHANNELERR_SERVER_BUSY, //��������æ
    CHANNELERR_COOKIE_ERROR, // cookieУ��ʧ��
    CHANNELERR_TICKET_EMPTY, // Ʊ��Ϊ��
    CHANNELERR_TICKET_OUTDATED, // Ʊ�ݹ���
    CHANNELERR_OTHER_TICKER_ERROR, // ����Ʊ��ʧ��
	CHANNELERR_JOIN_TOO_OFTEN, // ��Ƶ��̫Ƶ��
    CHANNELERR_CLIENT_ERROR, // Ƶ���������ͻ��˴�����Ŀǰ��δ��ͨ
    CHANNELERR_OTHER_CLIENT_KICK, // �����ͻ��˻���
	CHANNELERR_NEED_PASSWORD, // Ƶ�����������룬Ҫ����������
	CHANNELERR_PASSWORD_ERROR, // �����������
	CHANNELERR_DUOWAN_LIMIT, // ����duowanָ��Ƶ������
	CHANNELERR_SUBSID_FULL, // ��Ƶ�����������ޣ��Ӵ�Ƶ�������ޣ�����Ա������
	CHANNELERR_SUBSID_LIMIT, // ����Ƶ�������������ˡ���Ƶ��ֻ�Ի�Ա�ͼα����š����Ӵ�Ƶ�������ޣ�����Ա������
	CHANNELERR_GUEST_LIMIT, // ��Ƶ�������������ˡ���ֹ�οͽ���Ƶ����������Ƶ�������ޣ�����Ա������
	CHANNELERR_VIP_LIMIT, // VIP��Ƶ����ֹ����
	CHANNELERR_CHARGE_LIMIT, // ������Ƶ����ֹ����
	CHNNNELERR_APP_TOKEN_FAIL,//app��token��֤ʧ��
	CHANNELERR_WHITE_LIST_LIMIT, //Ƶ�������˰����������������в��Ҳ��ǹ���Ա
	CHANNELERR_REAL_NAME_REGISTER_LIMIT, //�����ߣ����ù���Աʱ����������û��ʵ����֤
	CHANNELERR_REAL_NAME_REGISTER_LIMIT2, //�������ߣ����ù���Աʱ����������û��ʵ����֤, ������ʾ
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

    // ������˽��Э�飬û����PROTOCOL������ͨ��sigEchoStopSession�ź�͸�����ͻ���
    CHANNEL_ANNOUNCE_RECEPTION = 50 // �Ӵ��ɹ���������û��볡�㲥
};

enum CHANNEL_OP_ERR_CODE
{
    CHANNEL_ERR_CREATE_SUBCHANNEL,
    CHANNEL_ERR_DISSMISS_SUBCHANNEL,
    CHANNEL_ERR_CHANGE_FOLDER,//��Ƶ��ʧ�ܣ����RES_EAUTHΪ��Ƶ���������
    CHANNEL_ERR_UPDATE_SINFO,
    CHANNEL_ERR_UPDATE_MEMBERROLER,
    CHANNEL_ERR_CHANGE_STYLE,
    CHANNEL_ERR_KICKOFF,
    CHANNEL_ERR_JOIN_SYNC,
    CHANNEL_ERR_SETKEYACTIVE,
    CHANNEL_ERR_USER_OVERLOAD,//Ƶ����������
    CHANNEL_ERR_VICE_OWNER_OVERLOAD, // �ܹܸ�����������
    CHANNEL_ERR_SUBCHANNEL_COUNT_EXCEEDED, // ��������Ƶ����Ŀ��������
    CHANNEL_ERR_TUOREN, // ����
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
    FT_Platform,            // �����Դ�����
    FT_Custom,              // �Զ�����
    FT_Vip,                 // vip����
};

// ��������
struct FACE_ATTR
{
    std::wstring strFileorg;        // ���ػ����ļ���
    std::wstring strTips;           // tips
    std::wstring strShortcut;       // ���
    std::wstring strUrl;            // �������ϵı���·��
    FACE_TYPE    type;              // ��������
    UINT32		  groupId;			//��������ȺID
    FACE_ATTR() : type(FT_Custom) { }
};

struct RECENT_FACE_ATTR : public FACE_ATTR
{
    std::wstring strGroup;          // ��������

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
    BIZ_CHAT_PROPERTY_EDUCATION = 1 // YY����
};

enum BIZ_SYSMSG_CLS
{
    BIZ_SYSMSG_SYSTEM = 1,   // ϵͳ�Զ��������Ϣ
    BIZ_SYSMSG_SUBSCRIPTION, // ��������Ϣ
    BIZ_SYSMSG_PROMOTION     // ҵ���ƹ���
};


enum BIZ_BUDDY_CHANNEL_INFO_TYPE
{
    BIZ_BUDDY_CHANNEL_INFO_CH,      // Ƶ��/�
    BIZ_BUDDY_CHANNEL_INFO_WW,      // ��������
	BIZ_BUDDY_COMPONENT_INFO = 2,         // �������
	BIZ_BUDDY_CHANNEL_INFO_UNKNOWN = 100, // ������ʾ�������Ƶ��
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

//�μ�protocol::login::DYNAMIC_TOKEN_TYPE 
enum LoginDynamicTokenType
{
	VFY_NONE        = 0x0,                                       // �޶�̬����
	VFY_MOBTOKEN    = 0x02,                                      // �ֻ�����
	VFY_HWTOKEN     = 0x04,                                      // Ӳ������
	VFY_SMSCODE     = 0x08,                                      // ������֤��
	VFY_SEC_QESTION = 0x10,                                       // �ܱ�����
	VFY_SMSCODE_UP = 0x12										// ����������֤��
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

//�������й��ˣ���onAnalyzerMsgResult
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