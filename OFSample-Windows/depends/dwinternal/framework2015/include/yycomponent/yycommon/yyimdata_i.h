#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

class QString;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ע�⣺��������ֵ����Ҫ������YYImBuddyTreeWidget::_cloneImBuddyItem��������Ӧ����޸�
#define KBuddyTreeUIDRole Qt::UserRole + 2				//����UID
#define KBuddyTreeSign KBuddyTreeUIDRole+1				//����ǩ��
#define KBuddyTreeImage KBuddyTreeSign+1				//Ĭ��ͼƬ·��
#define KBudyyIsSysImage KBuddyTreeImage+1				//�Ƿ�ϵͳͼƬ
#define KBuddyImStatus KBudyyIsSysImage+1				//����״̬
#define KBuddyCustomImage KBuddyImStatus+1				//�Զ���ͼƬURL
#define KBuddyTreeFolderId KBuddyCustomImage+1			//���� UID
#define KBuddyTreeRemark KBuddyTreeFolderId+1			//��ע
#define KBuddyIsInRecentList KBuddyTreeRemark+1			//�Ƿ��������ϵ������
#define KFolderGuid KBuddyIsInRecentList+1				//����Guid,��ʶ������֧�����
#define KFolderName KFolderGuid+1					//��������
#define KFolderChild KFolderName+1						//��������
#define KFolderOnLineChild KFolderChild+1				//������������
#define KBuddyTreeChannelID KFolderOnLineChild+1		//��������Ƶ��ID
#define KDisplayNickName KBuddyTreeChannelID+1			//������ʾ����(��ע���ǳ�)
#define KItemIsFolder KDisplayNickName+1				//�ж��Ƿ����
#define KBuddyTreeImageIndex KItemIsFolder+1			//Ĭ��ͼƬ�±�
#define KBuddyYYNumber KBuddyTreeImageIndex + 1			//YY��
#define KBuddyShowInfoIndex KBuddyYYNumber+1			//���û�ͬʱ����Ϸ������Ƶ����ʱ��������ʾ
#define KBuddyIsPlayGame KBuddyShowInfoIndex+1			//�û��Ƿ�����Ϸ
#define KBuddyIsBlock	KBuddyIsPlayGame+1				//�û��Ƿ��Ѿ�������
#define KFolderChildFlashCount KBuddyIsBlock+1			//���������˸����
#define KBuddyFlashImageIndex KFolderChildFlashCount+1		//����ͷ����˸λ���±�
#define KFolderFlashTextIndex	KBuddyFlashImageIndex+1     //����������˸λ���±�
#define KFolderIsGroup			KFolderFlashTextIndex+1     //����ITEM�Ƿ�Ⱥfolder
#define KRecentGroupName		KFolderIsGroup+1			//�����ϵ��Ⱥ��
#define KRecentGroupsFolderName	KRecentGroupName+1          //�����ϵ��Ⱥfolder��
#define KTalkPopFrameMode	KRecentGroupsFolderName+1   //������������ģʽ
#define KRecentListIndex		KTalkPopFrameMode+1    //�����ϵ�������±�
#define KDisableTextColor		KRecentListIndex+1          //��ʾ�����б��л���
#define KInviteMemberToGroupFolder		KDisableTextColor+1          //������Ⱥ��Ա���顢��������Ѽ���Ⱥ����
#define KRecentGroupRecvMode		    KInviteMemberToGroupFolder+1 //�����ϵ��Ⱥ����Ϣ����ģʽ���Ƿ�ֻ��ʾ��Ϣ��Ŀ����ʾ��Ϣ����
#define KForceUseQStandardItemCopy	    KRecentGroupRecvMode+1  //ǿ��ʹ��QStandardItem ������ YYImBuddyTreeWidget::_cloneImBuddyItem����ʹ��
#define KInVerifyFriends				KForceUseQStandardItemCopy+1 //�ж�������֤���ѵ�ʱ��ʹ��
#define KInHistroyMessage				KInVerifyFriends+1           //�ж��Ƿ�����ʷ��¼��ʹ��
#define KFolderIndex					KInHistroyMessage+1   //���ѷ�������
#define KHideUserRealInfo		KFolderIndex+1  //�Ƿ������û���ʵ��Ϣ��΢�������Ӻ����õ�.
#define KUserOtherNickName  KHideUserRealInfo+1 //�û������ǳƣ�����΢�������õ��ǳ�.

#define KTemp	KRecentGroupsFolderName+100            //�����ĵط���ʱʹ�õ��ֶ���

const int KCommonViewItem_BaseSection	=	Qt::UserRole + 1000;
const int KCommonViewItem_DisplayText	=	KCommonViewItem_BaseSection;
const int KCommonViewItem_HeaderIcon	=	KCommonViewItem_BaseSection + 1;	

const int KMyAttentionFolderIndex		=	0xffffffe;
const int KKefuFolderIndex              =   0xfffffff;

#define FRIENDS_LIST_FOLDERID	1					//�ҵĺ��ѷ���ID	
#define BLACK_LIST_FOLDERID		-1					//����������ID
#define RECENT_LIST_FOLDERID	-2					//�����ϵ�˷���ID
#define STRANGER_LIST_FOLDERID	-3					//İ���˷���ID
#define MY_ATTENTION_FOLDERID	-4					//�ҵĹ�ע����ID
#define MY_DEVICE_FOLDER        -5                  //�ҵ��ն������б�
#define SIMPLE_WIDGET_FOLDERID  -6					
#define MY_CONTACTS_FOLDER      -7                  //�ҵ���ϵ�˷��飨��Ϣ�������У�
// ����id <= -8 && >= -8ʱ��������ȫ���Ƽ�folderName�������� folderName(0/0)��ʽ
#define Full_customization_foldername_start -8
#define MY_KEFU_FOLDER          -8                  //�ҵ�С�������
#define Full_customization_foldername_end -8
#define YYFRIEND_LIST_FOLDERID  0x7FFFFFFE			//��Ե����

#define KGroupWithCustomImageID         999
#define KRunningGroupLogoIndex          998         //��Y����ɧ��logoindex
#define MAKE_UINT64(h,l) (((UINT64)h << 32) | (UINT64)l)
#define LO_UINT64(v) ((UINT64)v & 0x00000000ffffffff)
#define HI_UINT64(v) (((UINT64)v & 0xffffffff00000000) >> 32)

#define KGroupIDRole					(Qt::UserRole + 1)	// ȺID
#define KFolderIDRole					(Qt::UserRole + 2)	// ��ID

#define KToolTipsRole					(Qt::UserRole + 3)	// tooltips

#define KGroupLogoIndexRole				(Qt::UserRole + 5)	// Ⱥͼ������
#define KGroupLogoURLRole				(Qt::UserRole + 6)	// Ⱥͼ��·������ʱûʵ�֣�

#define KMemberIDRole					(Qt::UserRole + 10)	// ��ԱID
#define KMemberNameRole					(Qt::UserRole + 11)	// ��Ա����

#define KMessageState					(Qt::UserRole + 12)	// ��Ϣ���ã�1���Զ�������2��������ʾ��3������ʾ��Ϣֻ��ʾ��Ŀ

#define KUnReadMsgRole					(Qt::UserRole + 13)	// δ����Ϣ��

#define	KAuthMode						(Qt::UserRole + 14)	// ��֤ģʽ

#define KGroupAlias						(Qt::UserRole + 15)	// Ⱥ��ע
#define KMemberIMID						(Qt::UserRole + 16)	// Ⱥ��ԱYY��
#define KGroupName						(Qt::UserRole + 17) // Ⱥ�ǳ�
#define KGroupAliasNumber				(Qt::UserRole + 18)	// Ⱥ�ţ���ʾ��
#define KGroupCreatedTime               (Qt::UserRole + 19) // Ⱥ����ʱ��uint
#define KGroupOwnerName                 (Qt::UserRole + 20) // Ⱥ����������
#define KGroupMsgId                     (Qt::UserRole + 21) // Ⱥ����Ϣid 

#define KDeviceItemRoleType             (Qt::UserRole + 0x30)   // �豸/�ն����ͣ�EClientType
#define KDeviceItemRoleOS               (Qt::UserRole + 0x31)   // �豸ϵͳ. ios/android ==, EMobileType
#define KDeviceItemRoleStatus           (Qt::UserRole + 0x32)   // IM_STATUS
#define KDeviceItemRoleTime             (Qt::UserRole + 0x33)   // �����¼ʱ��
#define KDeviceItemRoleIP               (Qt::UserRole + 0x34)   // ip
#define KDeviceItemRolePosition         (Qt::UserRole + 0x35)   // ����λ��
#define KDeviceItemRoleNick             (Qt::UserRole + 0x36)   // �ǳ�
#define KDeviceItemRoleImage            (Qt::UserRole + 0x37)   // ͷ��index
#define KDeviceItemRoleImageUrl         (Qt::UserRole + 0x38)   // ͷ��url
#define KDeviceItemRoleImageExists      (Qt::UserRole + 0x39)   // ͷ������ڱ���
#define KDeviceItemRoleMyUid            (Qt::UserRole + 0x3a)   // �ҵ�uid
#define KDeviceItemRoleDesc             (Qt::UserRole + 0x3b)   // ��������YY for iPhone
#define KDeviceItemRoleComType          (Qt::UserRole + 0x3c)   // KDeviceItemRoleType��KDeviceItemRoleOS��ϵ�ֵ
#define KDeviceItemRoleAPP              (Qt::UserRole + 0x3d)   // ĳOS�ϵ�ĳӦ��
#define KDeviceItemRoleClientString     (Qt::UserRole + 0x3e)   // �ն˱�ʶ�ַ���������"yy"��"yymand"��

#define KGroupItemType_Group			(QStandardItem::UserType + 50)	// ��
#define KGroupItemType_Folder			(QStandardItem::UserType + 51)	// Ⱥ
#define KGroupItemType_UnJoinedGroup	(QStandardItem::UserType + 52)	// δ�������ĸ�
#define KGroupItemType_UnJoinedFolder	(QStandardItem::UserType + 53)	// δ�������
#define KRecentItemType					(QStandardItem::UserType + 54)  //�����ϵ������
#define KCommon_ItemType				(QStandardItem::UserType + 55)  //ͨ��viewitem��
#define KDeviceItemType                 (QStandardItem::UserType + 56)  // �ն��б�
#define KGroupItemType_Discuss          (QStandardItem::UserType + 57)  // ������
#define KKefuItemType                   (QStandardItem::UserType + 58)  // �ͷ�item
#define KDiscussCollectedStatus         (QStandardItem::UserType + 59)  //�������ղ�״̬
#define KDiscussCanBeCollected          (QStandardItem::UserType + 60)  //�ղ�������
#define KCollectedDiscussIndex          (QStandardItem::UserType + 61)  //���ղ�������index


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
* @brief . �����б��к�����Ϣ����
* @param . yyNumber yy��
* @param . nickName �ǳ�
* @param . sign ����ǩ��
* @param . remark ����˵��
* @param . uid �˺�uid
* @param . sysImageLogo ϵͳͷ��index
* @param . imStatus ����״̬
* @param . imageURL �Զ���ͷ����߶�̬ͷ���url
* @param . uFolderID ���ڵ�Ŀ¼id
* @param . buddyFlashImageIndex ͷ������offset,�ֱ��-1,0,1
* @param . channelID ����Ƶ��ID 0��ʾû����
* @param . playGame ���������Ϸ �ձ�ʾû����
* @param . showGameChannelInfoIndex ���û�ͬʱ����Ϸ������Ƶ����ʱ��������ʾ 0��ʶ����Ƶ����1��ʶ������Ϸ
* @param . nIndex ������Ⱥ�˳�����������������Ⱥ��������
* @param . bIsBlock �Ƿ����δ�����Ϣ
* @param . bIsInBlackList �Ƿ��ں�������Ŀ¼
* @param . bStanger �ж��Ƿ���İ���ˣ�ֻ�����������ϵ��
* @param . bMobile �ж��Ƿ��ֻ��û�
* @param . mobileType �ֻ����ͣ������뿴EMobileType
* @param . bIsVipMember �ж��Ƿ���YY��Ա
* @param . uVipGrade  ��Ա�ȼ�
* @param . uVipType   ��Ա��ֵ����
* @param . gender �Ա𣬷ֱ�Ϊmale,female
* @param . bIsOnlineAlert �Ƿ��ں������ߺ�����
* @param . uOnlineRing ����������������
* @param . nOnlinePrivilegeStatus �����뿴OnlinePrivilegeStatus
* @param . vipColorRings vip����
* @param . bHDPhoto �ж��Ƿ����ͷ���־
* @param . DeviceType �ͻ������еĻ������ͣ������뿴ClientType
* @param . appType �豸Ӧ�ã����忴EAppType
* @param . svipGrade ������Ա�ȼ�
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
	* @brief . �Ƿ���ϵͳͼƬ
	* @param .
	* @return .
	*/
	bool IsSystemImage()
	{
		return (CUSTOM_LOGO_NUMBER != sysImageLogo);
	}
	/**
	* @brief . �Ƿ�����
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
	* @brief . �Ƿ��������߶�������
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
	* @brief . �Ƿ������������ɼ�
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
	* @brief . ���������״̬
	* @param . bSet �Ƿ�����
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
	* @brief . ȥ������״̬
	* @param . bSet �Ƿ�����
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
* @brief . �����б�Ŀ¼������Ϣ
* @param . folderID Ŀ¼id
* @param . folderName Ŀ¼����
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
	* @brief . ���Һ����б��ָ���û���Ϣ
	* @param . uid �û�uid
	* @return .
	*/
	virtual YYImTreeItemData *findData(UINT32 uid) const;

	/**
	* @brief . ���Һ����б��ж���Ŀ¼�û���Ϣ
	* @param . uid �û�uid
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
	* @brief . ���Һ����б��Ŀ¼
	* @param . uid Ŀ¼uid
	* @return .
	*/
	virtual YYImTreeFolderData *findData(UINT32 uid) const;
	/**
	* @brief . ���Һ����б��ж���Ŀ¼
	* @param . uid Ŀ¼uid
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
	* @brief . ����ָ��Ŀ¼�������û�uid
	* @param . folderid Ŀ¼uid
	* @return. 
	*/
	virtual QSet<UINT32> *buddyList(UINT32 folderid);

	/**
	* @brief . �õ������б�������
	* @param . 
	* @return. 
	*/
	virtual QMap< UINT32, QSet< UINT32 > >& treeData(void);

	/**
	* @brief . ����ָ������Ŀ¼�������û�
	* @param . folderid Ŀ¼uid
	* @return. 
	*/
	virtual QSet<UINT32> *buddyListByIndependent(UINT32 folderid);

	/**
	* @brief . �õ������б������ж���Ŀ¼����
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
	* @brief ע��װ��Ʒ��ȡ��
	* @param method ע��ĺ���������ֵ�μ�namespace ImDecorator::Method
	* @param fetcher��ȡ��ָ�룬Ҫ����ȡ����invoke�������Ե��õ���ע���method������
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool registerDecorationFetcher(ImDecorator::Method method, IDWQComponentPtr fetcher) = 0;

	/**
	* @brief ��ע��װ����ȡ��
	* @param method ��ע��ĺ���������ֵ�μ�namespace ImDecorator::Method
	* @param fetcher װ����ȡ��ָ��
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool unregisterDecorationFetcher(ImDecorator::Method method, IDWQComponentPtr fetcher) = 0;

	/**
	* @brief ������ȡ����
	* @param method ��ȡ����������ֵ�μ�namespace ImDecorator::Method
	* @param args �����б�˳����ݺ���ȷ��
	* @return �Ƿ���óɹ�
	*/
	virtual bool invokeDecorationFetcher(ImDecorator::Method method, QVariantList &args) = 0;
};

/*
IYYImTreeDataPtr im_tree_data = root->getServiceInterface<IYYImTreeData>(KSERVICECommon);
*/