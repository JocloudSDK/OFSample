#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duiframewindow.h"

/*
IYYHistoryMsgComponentPtr prop = root->getServiceInterface<IYYHistoryMsgComponent>(KSERVICEYYHistorymsgComponent);
*/
DWDEFINE_INTERFACE(IYYHistoryMsgComponent) : public IDWQComponent
{
	struct LogicTreeFolderInfo 
	{
		QString folderName;
		QString folderGuid;
	};

	struct LogicTreeItemInfo 
	{
		quint32	id;
		QString displayText;
		QString iconPath;
		QPixmap pixmap;
	};

	struct HistoryMsgEditorRowInfo
	{
		QString imagePath;
		QString title;
		QString message;
		quint32	dateTime;
	};

/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual bool appendHistoryMsgTreeItem( const LogicTreeFolderInfo& folderInfo, const QList<LogicTreeItemInfo*>& itemLists )	= 0; // ��Tree�������Item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool updateHistoryMsgTreeItem( const LogicTreeFolderInfo& folderInfo, const LogicTreeItemInfo& itemInfo ) = 0; // ����Tree���Item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setCurrentSelItem( const LogicTreeFolderInfo& folderInfo, const LogicTreeItemInfo& itemInfo ) = 0; // ��λtree�����item,����������Ŀ¼
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getCurItem( LogicTreeFolderInfo& folderInfo, LogicTreeItemInfo& itemInfo ) = 0; // ��ѯtree��ǰ��item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setPageInfo( int nCurPage, int totalPage ) = 0; // ����ҳ����Ϣ
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setTipInfo( QString tip ) = 0; // ������ʾ��Ϣ
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool showHistoryMsgEditorInfo( const QList<HistoryMsgEditorRowInfo*>& list ) = 0; // ��edit���������
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isHistoryMsgFrameOpened() = 0; // ��ѯ��Ϣ����������ָ���Ƿ����
	virtual DuiFrameWindow* getHistoryMsgFramePtr() = 0; // ��ȡ��Ϣ����������ָ��

//signals:
	//	void historyFrameOpened();
	//	void treeItemSelectChanged( IYYHistoryMsgComponent::LogicTreeFolderInfo& folderInfo, IYYHistoryMsgComponent::LogicTreeItemInfo& itemInfo );
	//	void treeFolderSelectChanged( IYYHistoryMsgComponent::LogicTreeFolderInfo& floderInfo );
	//	// ��ҳ���� actionCode = 1Ϊ��ҳ��ť��actionCode = 2Ϊǰһҳ��ť��
	//	// actionCode = 3 Ϊ��һҳ��ť��actionCode = 4 Ϊ���һҳ��ť��actionCode = 5 Ϊ����return
	//	void pageAction( int actionCode, int nCurPage, int totalPage );
	//	void deleteBtnClicked( bool isFolder, IYYHistoryMsgComponent::LogicTreeFolderInfo& folderInfo, IYYHistoryMsgComponent::LogicTreeItemInfo& itemInfo );
};