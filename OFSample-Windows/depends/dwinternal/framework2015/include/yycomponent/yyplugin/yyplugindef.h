#pragma once

#include <QString>
#include <QMetaType>
#include <QStandardItem>

/**
   * @brief 合作游戏信息.
   */
struct CooperationGameInfo
{
	int      id;
	QString  gameName;
	QString  processName;
	QString  windowTitle;
};

/**
   * @brief 最近频道列表信息.
   */
struct RecentSessLstInfo
{
	quint32 asid;
	QString name;
};

/**
* @brief 最近频道列表信息 版本2：增加频道长ID.
*/
struct RecentSessLstInfo2 : RecentSessLstInfo
{
	quint32 sid;
};

/**
* @brief 频道信息.
*/
struct PChannelInfo
{
	quint32  sid;
	bool     haspwd;
	quint32  online;
	quint32  sfoldernum;
	QString  name;
};

struct PChannelInfo2 : public PChannelInfo
{
	quint32 asid;
	quint32 logoIndex;
	QString logoUrl;
	bool isLimit;
	quint32 joinType;

	QVariantList  activities;
};

/**
* @brief 频道用户信息.
*/
struct PChannelUserInfo
{
	quint32 uid;
	QString name;
};

/**
* @brief 合作游戏信息（游戏大厅）.
*/
struct PCooperationGameInfo
{
	int     id;
	QString name;
	QString runName;	//启动游戏大厅中的游戏
	bool    playing;
	QString qufu; // 所玩游戏所在区服, s1,s2,或k1,k2,k开头指快快区服
};

/**
* @brief 进程下载信息.
*/
struct ProcessDownloadInfo
{
	int      id;					//游戏ID
	QString  name;					//名字。
	QString  processName;			//进程名
	QString  windowTitle;			//窗口名字
	QString  exceptWindow;			//用来排除相似度的软件。
	bool	 noMoreOpen;			//判断此项是否需要多开统计
	bool	 isHideProcess;			//该统计项是统计进程被隐藏的情况。

	ProcessDownloadInfo()
	{
		id = 0;
		isHideProcess = false;
		noMoreOpen = false ;
	}
};

/**
* @brief 基础频道信息.
*/
struct ChannelBasicInfo 
{
	quint32 id;
	quint32 userCount;
};


/**
* @brief 频道插件信息.
*/
struct ChannelAddonInfo
{
	quint32 id;
	quint32 version;
};

/**
* @brief 我的频道列表信息.
*/
struct MyGuildLstInfo
{
	quint32 sid;
	quint32 asid;
	QString name;
};

/**
* @brief 我的群列表信息.
*/
struct MyGroupFolderListInfo
{
	MyGroupFolderListInfo()
	{
		fid = 0;
		bJoined = false;
	}
	UINT32       fid;          // 组uid，如果fid等于上面的群gid，则是全体成员组
	std::wstring strFName;     // 组名，如果是全体成员组，则此参数为空
	bool         bJoined;      // 我是否 加入了该组；如果是全体成员组，则此值必定为true
};
struct MyGroupListInfo
{
	MyGroupListInfo()
	{
		gid = 0;
		aliasGid = 0;
	}
	UINT32 gid;
	UINT32 aliasGid;             // 群号码
	std::wstring strGAliasName;  // 群别名
	std::wstring strGName;       // 群名称
	std::vector<MyGroupFolderListInfo> vFolder;  // 组列表
};

typedef QList<CooperationGameInfo>  CooperationGameInfoList;
typedef QList<RecentSessLstInfo>    RecentSessLstInfoList;
typedef QList<RecentSessLstInfo2>   RecentSessLstInfo2List;
typedef QList<PChannelInfo>         PChannelInfoList;
typedef QList<PChannelUserInfo>     PChannelUserInfoList;
typedef QList<ProcessDownloadInfo>	ProcessDownloadInfoList;
typedef QList<quint32>					PUserIdList;
typedef QList<ChannelAddonInfo>    ChannelAddonInfoList;
typedef QList<MyGuildLstInfo>       MyGuildLstInfoList;
typedef QList<QStandardItem*>       MyGuildItemList;
typedef QList<PChannelInfo2>        RecentListInfo2;
typedef QList<MyGroupListInfo>      MyGroupListInfoList;

Q_DECLARE_METATYPE(CooperationGameInfoList)
Q_DECLARE_METATYPE(RecentSessLstInfoList)
Q_DECLARE_METATYPE(RecentSessLstInfo2List)
Q_DECLARE_METATYPE(PChannelInfoList)
Q_DECLARE_METATYPE(PChannelUserInfoList)
Q_DECLARE_METATYPE(PCooperationGameInfo)
Q_DECLARE_METATYPE(ProcessDownloadInfoList)
Q_DECLARE_METATYPE(PUserIdList)
Q_DECLARE_METATYPE(ChannelBasicInfo)
Q_DECLARE_METATYPE(ChannelAddonInfoList)
Q_DECLARE_METATYPE(MyGuildLstInfoList)
Q_DECLARE_METATYPE(MyGuildItemList)
Q_DECLARE_METATYPE(RecentListInfo2)
Q_DECLARE_METATYPE(MyGroupListInfoList)
