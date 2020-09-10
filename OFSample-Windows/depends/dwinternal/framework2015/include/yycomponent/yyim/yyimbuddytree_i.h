#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"
#include "yyimbuddytreeiteminfoareahandler_i.h"

//���õ�handler��Ϣ
const UINT32 KSignHandlerLevel				= 0xffff;
const UINT32 KAreaSharedHandlerLevel		= 0xfffe;
const UINT32 KPersonalCenterHandlerLevel	= 0xfffd;
const UINT32 KChannelInfoLevel				= 0x01;
const UINT32 KGameInfoLevel					= 0x01;
const UINT32 KComponentInfoLevel = 0x02;

const char KSignHandlerName[]				= "signhandler";
const char KChannelInfoHandlerName[]		= "channelindohandler";
const char KGameInfoHandlerName[]			= "gameinfohandler";
const char KAreaSharedHandlerName[]			= "areasharedhandler"; // ��handler��δ�ṩ
const char KPersonalCenterHandlerName[]		= "personalcenterhandler";
//

/*
IYYImBuddyTreePtr pBuddyTree= root->getServiceInterface<IYYImBuddyTree>(KSERVICEImGroup);

*/

DWDEFINE_INTERFACE(IYYImBuddyTree) : public IDWQComponent
{
	/**
	* @brief . �����б��Ƿ��ʼ�������ڷ���
	* @param . ��
	* @return. ����true����ʾ�Ѿ����ڣ����򲻴���
	*/
	virtual bool isImBuddyTreeCreated() = 0;

	/**
	* @brief . �ѷ���
	* @param .
	* @return .
	*/
	virtual bool registerInfoAreaHandle(IYYImBuddyTreeItemInfoAreaHandler *handler) = 0;
	/**
	* @brief . �ѷ���
	* @param .
	* @return.
	*/
	virtual bool unregisterInfoAreaHandle(IYYImBuddyTreeItemInfoAreaHandler *handler) = 0;	// ɾ��ָ��uid��handler
	/**
	* @brief .�ѷ���
	* @param .
	* @return.
	*/
	virtual bool unregisterUserInfoAreaHandler( UINT32 uid ) = 0; 
	/**
	* @brief .�ѷ���
	* @param .
	* @return.
	*/
	virtual QMap<UINT32, QList<IYYImBuddyTreeItemInfoAreaHandler*>>* userInfoAreaHandlers( UINT32 uid ) = 0;	// ָ��uid������handler

	/**
	* @brief .�ѷ���
	* @param .
	* @return.
	*/
	virtual QMap<UINT32,  QMap<UINT32,QList<IYYImBuddyTreeItemInfoAreaHandler*>>>* allUserInfoAreaHandlers() = 0;	// �����û���handler

	/**
	* @brief . ���ù��
	* @param . cursor ��ʾ���Ķ��󣬾����QCursor�ĵ�
	* @return. ��
	*/
	virtual void setCursor( QCursor cursor ) = 0; 

	/**
	* @brief . �����б��޸����ݺ���Ҫˢ�²��ܿ������µ��޸����ݡ�
	* @param . ��
	* @return. ��
	*/
	virtual void updateTree() = 0; 

	/**
	* @brief . ����Ŀ¼
	* @param . folderId Ŀ¼Id
	* @param . folderId Ŀ¼����
	* @param . folderPositionIndex Ŀ¼λ��
	* @return. ��
	*/
	virtual void insertFolder(const UINT32 folderId, const QString title, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . ����ָ��Ŀ¼����ϵ��
	* @param . folderId Ŀ¼Id
	* @param . userId �û�uid
	* @return. ��
	*/
	virtual void appendBuddy(const UINT32 folderId, const UINT32 userId )	=	0;

	/**
	* @brief . ��ʾָ��Ŀ¼
	* @param . folderId Ŀ¼Id
	* @param . folderPositionIndex Ŀ¼λ��
	* @return. ��
	*/
	virtual void showFolder(const UINT32 folderId, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . ����Ŀ¼
	* @param . folderId Ŀ¼Id
	* @return. ��
	*/
	virtual void hideFolder(const UINT32 folderId)	=	0;

	/**
	* @brief . ע����ʾ�ں����б����ϵ��ǩ��������ʾmodel
	* @param . modelItem modelʵ��
	* @return. ��
	*/
	virtual	void registerBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . ��ע����ʾ�ں����б����ϵ��ǩ��������ʾmodel
	* @param . modelItem modelʵ��
	* @return. ��
	*/
	virtual void unregisterBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . ����ָ���û���ָ��Ŀ¼����ʾmodel������Ϣ����ָ����ǩ������ʾ������
	��������ʾ��Ҫ�����ȼ����Ƿ���ת��������
	* @param . folderId	Ŀ¼Id
	* @param . userId �û�uid
	* @param . modelName	model����
	* @return. ��
	*/
	virtual void appendBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . ɾ��ָ���û���ָ��Ŀ¼����ʾmodel��
	* @param . folderId	Ŀ¼Id
	* @param . userId �û�uid
	* @param . modelName model����
	* @return. ��
	*/
	virtual void removeBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . ɾ�������б���ָ��Ŀ¼����ϵ��
	* @param . folderId	Ŀ¼Id
	* @param . userId	�û�uid
	* @return. ��
	*/
	virtual void removeBuddy(const UINT32 folderId, const UINT32 userId) = 0;

	/**
	* @brief . ����ָһĿ¼��QStandardItem
	* @param . folderId	Ŀ¼Id
	* @return. QStandardItem��ָ��Ŀ¼��������Ϣ��
	*/
	virtual QStandardItem* findFolderStandardItem( const UINT32 folderId ) = 0;

	/**
	* @brief . �������Ͽ�����ʾ��Ƶ����Ϸ��Ϣ
	* @param . ���ĳ����ϢΪ��������յ�QString������Ƶ����channelID��ֵ��0
	* @param . folderId	Ŀ¼Id
	* @param . userId		�û�uid
	* @param . gameName	��Ϸ����
	* @param . channelName Ƶ������
	* @param . activityStr �����������activityStr://sid=400000&asid=2080&childsid=456&aid=1234&name=ʮһ���������� �����ַ���&imageurl=iowqruiowqur&
	* @return. ��
	*/
	virtual void setMinicardAreaShowInfo(const UINT32 userId, const UINT32 folderId, const QString& gameName, const QString& channelName, const UINT32 channelID, const QString& activityStr) = 0;

	/**
	* @brief . �������Ͽ���ȥ��ĳ��Ŀ¼����ϵ����ʾƵ����Ϸ��Ϣ
	* @param . folderId	Ŀ¼Id
	* @param . userId		�û�uid
	* @return. ��
	*/
	virtual void clearMinicardAreaShowInfo(const UINT32 userId, const UINT32 folderId) = 0;

	virtual void visibleUsers(std::vector<unsigned int> &uids) = 0;

	/**
	* @brief . ����Ϣ����֪ͨ��ָ������folderId�����ڵ�uid����
	* @param . folderId��Ŀ¼Id
	* @param . uid��	�û�uid
	* @return. ��
	*/
	virtual void addFlashUid(const UINT32 folderId, const UINT64 uid) = 0;

	/**
	* @brief . ����Ϣ���������ָ������folderId�����ڵ�uid���
	* @param . folderId��Ŀ¼Id
	* @param . uid��	�û�uid
	* @return. ��
	*/
	virtual void removeFlashUid(const UINT32 folderId, const UINT64 uid) = 0;

	/*
	* @brief . �����Լ��ں����б�im���촰�ڵķ����ֲ���Ϣ
	* @param . isTurn���Ƿ��ֲ�
	* @param . iconUrl��ͼ���ַ
	* @param . name����������
	* @param . jumpTicket����ת�ķɻ�Ʊ
	* @return. ��
	*/
	virtual void updateMyCurrentShareStatus(IPropBagWrapperPtr pbag) = 0;

	/*
	* @brief . ɾ���ҵ�����Ƶ����Ϣ�ֶ�
	* @param . keys��ɾ����propbag�ֶ�key��
	* @return. ��
	*/
	virtual void removeMyCurrentShareStatus(std::vector<std::wstring> &keys) = 0;

	virtual IPropBagWrapperPtr getBuddyChannelInfo(UINT32 buddyID) = 0;

	/*
signal:
	void buddiesInfoChanged(const std::vector<UINT32> & uid, UINT32 flags);
	*/
};

DWDEFINE_INTERFACE(IYYImBuddyFolderItemProp) : public IUnk
{
	/**
	* @brief . ָ��Ŀ¼�Ƿ���չ��
	* @param . folderId	Ŀ¼Id
	* @return. �Ƿ���չ��Ŀ¼
	*/
	virtual bool isExpanded(const UINT32 folderId) = 0;
};

DWDEFINE_INTERFACE(IYYImIndependentBuddyFolderItemProp) : public IUnk
{
	/**
	* @brief . ָ��Ŀ¼�Ƿ���չ��(����Ŀ¼)
	* @param . folderId	Ŀ¼Id
	* @return. �Ƿ���չ��Ŀ¼
	*/
	virtual bool isExpanded(const UINT32 folderId) = 0;

	/**
	* @brief . ����Ŀ¼����
	* @param . folderId Ŀ¼id
	* @param . folderName Ŀ¼����
	* @return. ��
	*/
	virtual void setFolderName( const UINT32 folderId, const QString folderName )	=	0;
};

DWDEFINE_INTERFACE(IYYImIndepentendBuddyTree) : public IDWQComponent
{
	/**
	* @brief . �����б��Ƿ��ʼ�������ڷ���
	* @param . ��
	* @return. ����true����ʾ�Ѿ����ڣ����򲻴���
	*/
	virtual bool isImBuddyTreeCreated();

	/**
	* @brief . ���ù��
	* @param . cursor ��ʾ���Ķ��󣬾����QCursor�ĵ�
	* @return. ��
	*/
	virtual void setCursor( QCursor cursor ) = 0; // ���ù��

	/**
	* @brief . �����б��޸����ݺ���Ҫˢ�²��ܿ������µ��޸����ݡ�
	* @param . ��
	* @return. ��
	*/
	virtual void updateTree() = 0;

	/**
	* @brief . ����Ŀ¼
	* @param . folderId Ŀ¼Id
	* @param . folderId Ŀ¼����
	* @param . folderPositionIndex Ŀ¼λ��
	* @return. ��
	*/
	virtual void insertFolder(const UINT32 folderId, const QString title, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . ��ʾָ��Ŀ¼
	* @param . folderId Ŀ¼Id
	* @param . folderPositionIndex Ŀ¼λ��
	* @return. ��
	*/
	virtual void showFolder(const UINT32 folderId, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . ����Ŀ¼
	* @param . folderId Ŀ¼Id
	* @return. ��
	*/
	virtual void hideFolder(const UINT32 folderId)	=	0;

	/**
	* @brief . ����ָ��Ŀ¼����ϵ��
	* @param . folderId Ŀ¼Id
	* @param . userId �û�uid
	* @return. ��
	*/
	virtual void appendBuddy(const UINT32 folderId, const UINT32 userId )	=	0;

	/**
	* @brief . ɾ�������б���ָ��Ŀ¼����ϵ��
	* @param . folderId	Ŀ¼Id
	* @param . userId �û�uid
	* @return. ��
	*/
	virtual void removeBuddy(const UINT32 folderId, const UINT32 userId) = 0;

	/**
	* @brief . ����ָһĿ¼��QStandardItem
	* @param . folderId	Ŀ¼Id
	* @return. QStandardItem��ָ��Ŀ¼��������Ϣ��
	*/
	virtual QStandardItem* findFolderStandardItem( const UINT32 folderId ) = 0;

	/**
	* @brief . ע����ʾ�ں����б����ϵ��ǩ��������ʾmodel
	* @param . modelItem modelʵ��
	* @return. ��
	*/
	virtual	void registerBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . ��ע����ʾ�ں����б����ϵ��ǩ��������ʾmodel
	* @param . modelItem modelʵ��
	* @return. ��
	*/
	virtual void unregisterBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . ����ָ���û���ָ��Ŀ¼����ʾmodel������Ϣ����ָ����ǩ������ʾ������
	��������ʾ��Ҫ�����ȼ����Ƿ���ת��������
	* @param . folderId	Ŀ¼Id
	* @param . userId �û�uid
	* @param . modelName model����
	* @return. ��
	*/
	virtual void appendBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . ɾ��ָ���û���ָ��Ŀ¼����ʾmodel��
	* @param . folderId	Ŀ¼Id
	* @param . userId �û�uid
	* @param . modelName model����
	* @return. ��
	*/
	virtual void removeBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . ָ��Ŀ¼������״̬����Ϣ���� 
	* @param . folderId	Ŀ¼Id
	* @return. ��
	*/
	virtual void sortTreeItemData( const UINT32 folderid ) = 0;

	/**
	* @brief . ���û�ѡ��ֻ��ʾ������ϵ��ʱ���������ָ����ϵ������״̬�������Ƿ���ʾ��
	* @param . userId	�û�uid
	* @return. ��
	*/
	virtual void updateOnlineOnlyStatus(  const UINT32 userId ) = 0;
	virtual void showOnlineMemberOnly() = 0;
};
