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
	virtual bool appendHistoryMsgTreeItem( const LogicTreeFolderInfo& folderInfo, const QList<LogicTreeItemInfo*>& itemLists )	= 0; // 往Tree里面添加Item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool updateHistoryMsgTreeItem( const LogicTreeFolderInfo& folderInfo, const LogicTreeItemInfo& itemInfo ) = 0; // 更新Tree里的Item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setCurrentSelItem( const LogicTreeFolderInfo& folderInfo, const LogicTreeItemInfo& itemInfo ) = 0; // 定位tree里面的item,仅限于两级目录
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getCurItem( LogicTreeFolderInfo& folderInfo, LogicTreeItemInfo& itemInfo ) = 0; // 查询tree当前的item
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setPageInfo( int nCurPage, int totalPage ) = 0; // 设置页面信息
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setTipInfo( QString tip ) = 0; // 设置提示信息
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool showHistoryMsgEditorInfo( const QList<HistoryMsgEditorRowInfo*>& list ) = 0; // 往edit里插入内容
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isHistoryMsgFrameOpened() = 0; // 查询消息管理器窗口指针是否存在
	virtual DuiFrameWindow* getHistoryMsgFramePtr() = 0; // 获取消息管理器窗口指针

//signals:
	//	void historyFrameOpened();
	//	void treeItemSelectChanged( IYYHistoryMsgComponent::LogicTreeFolderInfo& folderInfo, IYYHistoryMsgComponent::LogicTreeItemInfo& itemInfo );
	//	void treeFolderSelectChanged( IYYHistoryMsgComponent::LogicTreeFolderInfo& floderInfo );
	//	// 翻页操作 actionCode = 1为首页按钮，actionCode = 2为前一页按钮，
	//	// actionCode = 3 为后一页按钮，actionCode = 4 为最后一页按钮，actionCode = 5 为按下return
	//	void pageAction( int actionCode, int nCurPage, int totalPage );
	//	void deleteBtnClicked( bool isFolder, IYYHistoryMsgComponent::LogicTreeFolderInfo& folderInfo, IYYHistoryMsgComponent::LogicTreeItemInfo& itemInfo );
};