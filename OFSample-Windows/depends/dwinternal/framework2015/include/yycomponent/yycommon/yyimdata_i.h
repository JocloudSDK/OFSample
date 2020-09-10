#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

class QString;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// 注意：增加属性值后，需要考虑在YYImBuddyTreeWidget::_cloneImBuddyItem函数里响应添加修改
#define KBuddyTreeUIDRole Qt::UserRole + 2				//个人UID
#define KBuddyTreeSign KBuddyTreeUIDRole+1				//个人签名
#define KBuddyTreeImage KBuddyTreeSign+1				//默认图片路径
#define KBudyyIsSysImage KBuddyTreeImage+1				//是否系统图片
#define KBuddyImStatus KBudyyIsSysImage+1				//在线状态
#define KBuddyCustomImage KBuddyImStatus+1				//自定义图片URL
#define KBuddyTreeFolderId KBuddyCustomImage+1			//分组 UID
#define KBuddyTreeRemark KBuddyTreeFolderId+1			//备注
#define KBuddyIsInRecentList KBuddyTreeRemark+1			//是否在最近联系人里面
#define KFolderGuid KBuddyIsInRecentList+1				//分组Guid,标识分组以支持组件
#define KFolderName KFolderGuid+1					//分组名称
#define KFolderChild KFolderName+1						//分组人数
#define KFolderOnLineChild KFolderChild+1				//分组在线人数
#define KBuddyTreeChannelID KFolderOnLineChild+1		//好友所在频道ID
#define KDisplayNickName KBuddyTreeChannelID+1			//好友显示名称(备注，昵称)
#define KItemIsFolder KDisplayNickName+1				//判断是否分组
#define KBuddyTreeImageIndex KItemIsFolder+1			//默认图片下标
#define KBuddyYYNumber KBuddyTreeImageIndex + 1			//YY号
#define KBuddyShowInfoIndex KBuddyYYNumber+1			//当用户同时玩游戏，进入频道的时候，轮流显示
#define KBuddyIsPlayGame KBuddyShowInfoIndex+1			//用户是否玩游戏
#define KBuddyIsBlock	KBuddyIsPlayGame+1				//用户是否已经被屏蔽
#define KFolderChildFlashCount KBuddyIsBlock+1			//分组好友闪烁人数
#define KBuddyFlashImageIndex KFolderChildFlashCount+1		//好友头像闪烁位置下标
#define KFolderFlashTextIndex	KBuddyFlashImageIndex+1     //分组文字闪烁位置下标
#define KFolderIsGroup			KFolderFlashTextIndex+1     //区分ITEM是否群folder
#define KRecentGroupName		KFolderIsGroup+1			//最近联系人群名
#define KRecentGroupsFolderName	KRecentGroupName+1          //最近联系人群folder名
#define KTalkPopFrameMode	KRecentGroupsFolderName+1   //邀请语音泡泡模式
#define KRecentListIndex		KTalkPopFrameMode+1    //最近联系人排序下标
#define KDisableTextColor		KRecentListIndex+1          //显示在树列表中灰显
#define KInviteMemberToGroupFolder		KDisableTextColor+1          //是邀请群成员到组、或邀请好友加入群的树
#define KRecentGroupRecvMode		    KInviteMemberToGroupFolder+1 //最近联系人群的消息接收模式，是否只显示消息数目不提示消息到来
#define KForceUseQStandardItemCopy	    KRecentGroupRecvMode+1  //强制使用QStandardItem 拷贝在 YYImBuddyTreeWidget::_cloneImBuddyItem函数使用
#define KInVerifyFriends				KForceUseQStandardItemCopy+1 //判断正在验证好友的时候使用
#define KInHistroyMessage				KInVerifyFriends+1           //判断是否在历史记录中使用
#define KFolderIndex					KInHistroyMessage+1   //好友分组索引
#define KHideUserRealInfo		KFolderIndex+1  //是否隐藏用户真实信息，微房匿名加好友用到.
#define KUserOtherNickName  KHideUserRealInfo+1 //用户其他昵称，比如微房里设置的昵称.

#define KTemp	KRecentGroupsFolderName+100            //其他的地方临时使用的字段名

const int KCommonViewItem_BaseSection	=	Qt::UserRole + 1000;
const int KCommonViewItem_DisplayText	=	KCommonViewItem_BaseSection;
const int KCommonViewItem_HeaderIcon	=	KCommonViewItem_BaseSection + 1;	

const int KMyAttentionFolderIndex		=	0xffffffe;
const int KKefuFolderIndex              =   0xfffffff;

#define FRIENDS_LIST_FOLDERID	1					//我的好友分组ID	
#define BLACK_LIST_FOLDERID		-1					//黑名单分组ID
#define RECENT_LIST_FOLDERID	-2					//最近联系人分组ID
#define STRANGER_LIST_FOLDERID	-3					//陌生人分组ID
#define MY_ATTENTION_FOLDERID	-4					//我的关注分组ID
#define MY_DEVICE_FOLDER        -5                  //我的终端设置列表
#define SIMPLE_WIDGET_FOLDERID  -6					
#define MY_CONTACTS_FOLDER      -7                  //我的联系人分组（消息管理器中）
// 分组id <= -8 && >= -8时，分组名全定制即folderName，否则是 folderName(0/0)形式
#define Full_customization_foldername_start -8
#define MY_KEFU_FOLDER          -8                  //我的小秘书分组
#define Full_customization_foldername_end -8
#define YYFRIEND_LIST_FOLDERID  0x7FFFFFFE			//友缘分组

#define KGroupWithCustomImageID         999
#define KRunningGroupLogoIndex          998         //手Y端跑骚团logoindex
#define MAKE_UINT64(h,l) (((UINT64)h << 32) | (UINT64)l)
#define LO_UINT64(v) ((UINT64)v & 0x00000000ffffffff)
#define HI_UINT64(v) (((UINT64)v & 0xffffffff00000000) >> 32)

#define KGroupIDRole					(Qt::UserRole + 1)	// 群ID
#define KFolderIDRole					(Qt::UserRole + 2)	// 组ID

#define KToolTipsRole					(Qt::UserRole + 3)	// tooltips

#define KGroupLogoIndexRole				(Qt::UserRole + 5)	// 群图标索引
#define KGroupLogoURLRole				(Qt::UserRole + 6)	// 群图标路径（暂时没实现）

#define KMemberIDRole					(Qt::UserRole + 10)	// 成员ID
#define KMemberNameRole					(Qt::UserRole + 11)	// 成员名称

#define KMessageState					(Qt::UserRole + 12)	// 消息设置（1：自动弹出；2：闪动提示；3：不提示信息只显示数目

#define KUnReadMsgRole					(Qt::UserRole + 13)	// 未读消息数

#define	KAuthMode						(Qt::UserRole + 14)	// 验证模式

#define KGroupAlias						(Qt::UserRole + 15)	// 群备注
#define KMemberIMID						(Qt::UserRole + 16)	// 群成员YY号
#define KGroupName						(Qt::UserRole + 17) // 群昵称
#define KGroupAliasNumber				(Qt::UserRole + 18)	// 群号，显示用
#define KGroupCreatedTime               (Qt::UserRole + 19) // 群创建时间uint
#define KGroupOwnerName                 (Qt::UserRole + 20) // 群创建者名字
#define KGroupMsgId                     (Qt::UserRole + 21) // 群组消息id 

#define KDeviceItemRoleType             (Qt::UserRole + 0x30)   // 设备/终端类型，EClientType
#define KDeviceItemRoleOS               (Qt::UserRole + 0x31)   // 设备系统. ios/android ==, EMobileType
#define KDeviceItemRoleStatus           (Qt::UserRole + 0x32)   // IM_STATUS
#define KDeviceItemRoleTime             (Qt::UserRole + 0x33)   // 最近登录时间
#define KDeviceItemRoleIP               (Qt::UserRole + 0x34)   // ip
#define KDeviceItemRolePosition         (Qt::UserRole + 0x35)   // 地理位置
#define KDeviceItemRoleNick             (Qt::UserRole + 0x36)   // 昵称
#define KDeviceItemRoleImage            (Qt::UserRole + 0x37)   // 头像index
#define KDeviceItemRoleImageUrl         (Qt::UserRole + 0x38)   // 头像url
#define KDeviceItemRoleImageExists      (Qt::UserRole + 0x39)   // 头像存在于本地
#define KDeviceItemRoleMyUid            (Qt::UserRole + 0x3a)   // 我的uid
#define KDeviceItemRoleDesc             (Qt::UserRole + 0x3b)   // 描述，如YY for iPhone
#define KDeviceItemRoleComType          (Qt::UserRole + 0x3c)   // KDeviceItemRoleType与KDeviceItemRoleOS组合的值
#define KDeviceItemRoleAPP              (Qt::UserRole + 0x3d)   // 某OS上的某应用
#define KDeviceItemRoleClientString     (Qt::UserRole + 0x3e)   // 终端标识字符串（比如"yy"、"yymand"）

#define KGroupItemType_Group			(QStandardItem::UserType + 50)	// 组
#define KGroupItemType_Folder			(QStandardItem::UserType + 51)	// 群
#define KGroupItemType_UnJoinedGroup	(QStandardItem::UserType + 52)	// 未加入的组的根
#define KGroupItemType_UnJoinedFolder	(QStandardItem::UserType + 53)	// 未加入的组
#define KRecentItemType					(QStandardItem::UserType + 54)  //最近联系人类型
#define KCommon_ItemType				(QStandardItem::UserType + 55)  //通用viewitem类
#define KDeviceItemType                 (QStandardItem::UserType + 56)  // 终端列表
#define KGroupItemType_Discuss          (QStandardItem::UserType + 57)  // 讨论组
#define KKefuItemType                   (QStandardItem::UserType + 58)  // 客服item
#define KDiscussCollectedStatus         (QStandardItem::UserType + 59)  //讨论组收藏状态
#define KDiscussCanBeCollected          (QStandardItem::UserType + 60)  //收藏讨论组
#define KCollectedDiscussIndex          (QStandardItem::UserType + 61)  //已收藏讨论组index


#define	DEFAULT_GROUP_ONE		01
#define DEFAULT_GROUP_TWO		02
#define DEFAULT_GROUP_THREE	03

enum OnlinePrivilegeStatus
{
	UNSET = 0,
	SET_HIDE = 1,
	SET_UNHIDE = 2
};

enum ClientType
{
	CLIENT_PC = 0,
	CLIENT_MOBILE = 1,
	CLIENT_WEBYY = 2,
    CLIENT_XMAN = 3,
    CLIENT_PAD = 4,
	CLIENT_HD = 5,
};

/**
* @brief . 好友列表中好友信息描述
* @param . yyNumber yy号
* @param . nickName 昵称
* @param . sign 个人签名
* @param . remark 个人说明
* @param . uid 账号uid
* @param . sysImageLogo 系统头像index
* @param . imStatus 在线状态
* @param . imageURL 自定义头像或者动态头像的url
* @param . uFolderID 所在的目录id
* @param . buddyFlashImageIndex 头像闪动offset,分别的-1,0,1
* @param . channelID 好友频道ID 0表示没有玩
* @param . playGame 正在玩的游戏 空表示没有玩
* @param . showGameChannelInfoIndex 当用户同时玩游戏，进入频道的时候，轮流显示 0标识进入频道；1标识在玩游戏
* @param . nIndex 加入的先后顺序，用来给黑名单的先后加入排序
* @param . bIsBlock 是否屏蔽此人消息
* @param . bIsInBlackList 是否在黑名单的目录
* @param . bStanger 判断是否是陌生人，只出现在最近联系人
* @param . bMobile 判断是否手机用户
* @param . mobileType 手机类型，具体请看EMobileType
* @param . bIsVipMember 判断是否是YY会员
* @param . uVipGrade  会员等级
* @param . uVipType   会员充值类型
* @param . gender 性别，分别为male,female
* @param . bIsOnlineAlert 是否在好友上线后提醒
* @param . uOnlineRing 好友上线提醒铃声
* @param . nOnlinePrivilegeStatus 具体请看OnlinePrivilegeStatus
* @param . vipColorRings vip彩铃
* @param . bHDPhoto 判断是否高清头像标志
* @param . DeviceType 客户端运行的机器类型，具体请看ClientType
* @param . appType 设备应用，具体看EAppType
* @param . svipGrade 超凡会员等级
*/
struct YYImTreeItemData
{
	QString	yyNumber;
	QString nickName;
	QString sign;
	QString remark;
	UINT32  uid;				
	UINT32	sysImageLogo;		
	int		imStatus;		
	QString imageURL;
	UINT32  uFolderID;			
	int		buddyFlashImageIndex;

	UINT32	channelID;			
	QString	playGame;			
	int		showGameChannelInfoIndex; 

	int     nIndex;             
	bool	bIsBlock;
	bool	bIsInBlackList;		
	bool    bStanger;			
	bool    bMobile;			
	int		mobileType;	
	bool    bIsVipMember; 
	UINT32 uVipGrade;  
	UINT32 uVipType;
	QString  gender;     
	bool    bIsOnlineAlert;		
	UINT32  uOnlineRing;		
	int		nOnlinePrivilegeStatus;
	std::vector<ColorRingInfoItem> vipColorRings;
	bool bHDPhoto; 
	ClientType	DeviceType;
    QString gameQufu;
    int     appType;
	UINT32 uSVipGrade;

	YYImTreeItemData()
		: uid(0)
		, sysImageLogo(0)
		, nIndex(0)
		, mobileType(0)
        , appType(0)
		, channelID(0)
		, uVipGrade(0)
		, uVipType(0)
		, uOnlineRing(0)
		, nOnlinePrivilegeStatus(0)
		, showGameChannelInfoIndex(-1)
		, buddyFlashImageIndex(-1)
		, imStatus(2)
		, bIsBlock(false)
		, bIsInBlackList(false)
		, bStanger(false)
		, bMobile(false)
		, bIsVipMember(false)
		, bIsOnlineAlert(false)
		, bHDPhoto(false)
		, gender(QString::fromStdWString(stringizing::ToString(GENDER_UNKNOWN)))
		, DeviceType(CLIENT_PC)
		, uSVipGrade(0)
	{}

	/**
	* @brief . 是否是系统图片
	* @param .
	* @return .
	*/
	bool IsSystemImage()
	{
		return (CUSTOM_LOGO_NUMBER != sysImageLogo);
	}
	/**
	* @brief . 是否在线
	* @param .
	* @return .
	*/
	bool IsOnLine()
	{
		if(imStatus ==  IMSTATUS_OFFLINE  || imStatus == IMSTATUS_HIDE || imStatus == IMSTATUS_UNKNOWN)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	/**
	* @brief . 是否设置在线对其隐身
	* @param .
	* @return .
	*/
	bool isSetHide()
	{
		if ( nOnlinePrivilegeStatus & SET_HIDE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	* @brief . 是否设置隐身对其可见
	* @param .
	* @return .
	*/
	bool isSetUnHide()
	{
		if ( nOnlinePrivilegeStatus & SET_UNHIDE )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	* @brief . 设置隐身的状态
	* @param . bSet 是否设置
	* @return .
	*/
	void setHideStatus(bool bSet)
	{
		if ( bSet )
		{
			nOnlinePrivilegeStatus = nOnlinePrivilegeStatus | SET_HIDE;
		}
		else
		{
			nOnlinePrivilegeStatus = nOnlinePrivilegeStatus & ~SET_HIDE;
		}
	}
	/**
	* @brief . 去掉隐身状态
	* @param . bSet 是否设置
	* @return .
	*/
	void setUnHideStatus(bool bSet)
	{
		if ( bSet )
		{
			nOnlinePrivilegeStatus = nOnlinePrivilegeStatus | SET_UNHIDE;
		}
		else
		{
			nOnlinePrivilegeStatus = nOnlinePrivilegeStatus & ~SET_UNHIDE;
		}
	}
};

/**
* @brief . 好友列表目录描述信息
* @param . folderID 目录id
* @param . folderName 目录名称
*/
struct YYImTreeFolderData
{
	UINT32 folderID;
	QString folderName;
};

/*
use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYImBuddyData) : public IUnk
{
	/**
	* @brief . 查找好友列表的指定用户信息
	* @param . uid 用户uid
	* @return .
	*/
	virtual YYImTreeItemData *findData(UINT32 uid) const;

	/**
	* @brief . 查找好友列表中独立目录用户信息
	* @param . uid 用户uid
	* @return .
	*/
	virtual YYImTreeItemData *findDataByIndependent(UINT32 uid) const;
};

/*
use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYImFolderData) : public IUnk
{
	/**
	* @brief . 查找好友列表的目录
	* @param . uid 目录uid
	* @return .
	*/
	virtual YYImTreeFolderData *findData(UINT32 uid) const;
	/**
	* @brief . 查找好友列表中独立目录
	* @param . uid 目录uid
	* @return.
	*/
	virtual YYImTreeFolderData *findDataByIndependent(UINT32 uid) const;
};

/*
use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYImTreeData) : public IUnk
{
	/**
	* @brief . 查找指定目录的所有用户uid
	* @param . folderid 目录uid
	* @return. 
	*/
	virtual QSet<UINT32> *buddyList(UINT32 folderid);

	/**
	* @brief . 得到好友列表中内容
	* @param . 
	* @return. 
	*/
	virtual QMap< UINT32, QSet< UINT32 > >& treeData(void);

	/**
	* @brief . 查找指定独立目录的所有用户
	* @param . folderid 目录uid
	* @return. 
	*/
	virtual QSet<UINT32> *buddyListByIndependent(UINT32 folderid);

	/**
	* @brief . 得到好友列表中所有独立目录内容
	* @param . 
	* @return. 
	*/
	virtual QMap< UINT32, QSet< UINT32 > >& treeDataByIndependent(void);
};

namespace ImDecorator
{
	typedef enum
	{
		DecorationBegin = 0,
		SVipIcon = DecorationBegin,
		DecorationEnd,
	} Method;
}

/*
use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYImDecorationFetcher) : public IUnk
{
	/**
	* @brief 注册装饰品提取器
	* @param method 注册的函数，可用值参见namespace ImDecorator::Method
	* @param fetcher提取器指针，要求提取器的invoke函数可以调用到所注册的method函数。
	* @return 是否注册成功
	*/
	virtual bool registerDecorationFetcher(ImDecorator::Method method, IDWQComponentPtr fetcher) = 0;

	/**
	* @brief 反注册装饰提取器
	* @param method 反注册的函数，可用值参见namespace ImDecorator::Method
	* @param fetcher 装饰提取器指针
	* @return 是否反注册成功
	*/
	virtual bool unregisterDecorationFetcher(ImDecorator::Method method, IDWQComponentPtr fetcher) = 0;

	/**
	* @brief 调用提取函数
	* @param method 提取函数，可用值参见namespace ImDecorator::Method
	* @param args 参数列表，顺序根据函数确定
	* @return 是否调用成功
	*/
	virtual bool invokeDecorationFetcher(ImDecorator::Method method, QVariantList &args) = 0;
};

/*
IYYImTreeDataPtr im_tree_data = root->getServiceInterface<IYYImTreeData>(KSERVICECommon);
*/