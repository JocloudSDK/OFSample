#pragma once

#include <QString>
#include <QMetaType>
#include <QStandardItem>

/**
   * @brief ������Ϸ��Ϣ.
   */
struct CooperationGameInfo
{
	int      id;
	QString  gameName;
	QString  processName;
	QString  windowTitle;
};

/**
   * @brief ���Ƶ���б���Ϣ.
   */
struct RecentSessLstInfo
{
	quint32 asid;
	QString name;
};

/**
* @brief ���Ƶ���б���Ϣ �汾2������Ƶ����ID.
*/
struct RecentSessLstInfo2 : RecentSessLstInfo
{
	quint32 sid;
};

/**
* @brief Ƶ����Ϣ.
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
* @brief Ƶ���û���Ϣ.
*/
struct PChannelUserInfo
{
	quint32 uid;
	QString name;
};

/**
* @brief ������Ϸ��Ϣ����Ϸ������.
*/
struct PCooperationGameInfo
{
	int     id;
	QString name;
	QString runName;	//������Ϸ�����е���Ϸ
	bool    playing;
	QString qufu; // ������Ϸ��������, s1,s2,��k1,k2,k��ͷָ�������
};

/**
* @brief ����������Ϣ.
*/
struct ProcessDownloadInfo
{
	int      id;					//��ϷID
	QString  name;					//���֡�
	QString  processName;			//������
	QString  windowTitle;			//��������
	QString  exceptWindow;			//�����ų����ƶȵ������
	bool	 noMoreOpen;			//�жϴ����Ƿ���Ҫ�࿪ͳ��
	bool	 isHideProcess;			//��ͳ������ͳ�ƽ��̱����ص������

	ProcessDownloadInfo()
	{
		id = 0;
		isHideProcess = false;
		noMoreOpen = false ;
	}
};

/**
* @brief ����Ƶ����Ϣ.
*/
struct ChannelBasicInfo 
{
	quint32 id;
	quint32 userCount;
};


/**
* @brief Ƶ�������Ϣ.
*/
struct ChannelAddonInfo
{
	quint32 id;
	quint32 version;
};

/**
* @brief �ҵ�Ƶ���б���Ϣ.
*/
struct MyGuildLstInfo
{
	quint32 sid;
	quint32 asid;
	QString name;
};

/**
* @brief �ҵ�Ⱥ�б���Ϣ.
*/
struct MyGroupFolderListInfo
{
	MyGroupFolderListInfo()
	{
		fid = 0;
		bJoined = false;
	}
	UINT32       fid;          // ��uid�����fid���������Ⱥgid������ȫ���Ա��
	std::wstring strFName;     // �����������ȫ���Ա�飬��˲���Ϊ��
	bool         bJoined;      // ���Ƿ� �����˸��飻�����ȫ���Ա�飬���ֵ�ض�Ϊtrue
};
struct MyGroupListInfo
{
	MyGroupListInfo()
	{
		gid = 0;
		aliasGid = 0;
	}
	UINT32 gid;
	UINT32 aliasGid;             // Ⱥ����
	std::wstring strGAliasName;  // Ⱥ����
	std::wstring strGName;       // Ⱥ����
	std::vector<MyGroupFolderListInfo> vFolder;  // ���б�
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
