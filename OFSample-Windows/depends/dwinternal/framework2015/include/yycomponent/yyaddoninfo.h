#pragma once

#include "dwbase/dwcomex.h"

//老版本插件信息
struct VersionInfo
{
	quint32 version;
	quint32 vMin;
	quint32 vMax;
	QString md5;
	QString	url;
};  //VersionInfo

/**
   * @brief addon infomaion.
   * @param .
   * @return .
   */
struct AddonInfo
{
	quint32 addonId;
	quint32 recommendedVersion; //推荐使用的插件版本
	bool opByOw; // if true, a message need to be displayed.
	QString addonName;	//music
	QString addonDescription;	//音乐应用
	QList<VersionInfo> addonVersions;
}; //AddonInfo

/// @brief 新版本插件信息
struct VersionItem
{
	VersionItem()
		:version(0),yymax(0), yymin(0), downloadCount(0)
	{}

	quint32 version;
	QString url;
	QString md5;
	QString runCheck;
	quint32 yymax;
	quint32 yymin;
	QString expand;
	quint32 downloadCount;
};

struct AppInfoPack
{
	AppInfoPack()
		:appid(0),executeType(0),uiType(0),delayflag(0), opbyow(0), suitable_version(0) {}
	quint32 appid;
	QString name;
	QString fileName;
	QString appDesc;
	quint32 recommand_version;
	quint32 suitable_version;	//客户端经过计算得出的实际用来运行的版本
	quint16 delayflag;			//是否延时下载
	quint16 executeType;
	quint16 uiType; // 目前0表示盒子应用，1表示三栏应用，2表示公告应用
	quint16 opbyow;
	QString expand;
	QList<VersionItem> versions;
};

typedef AppInfoPack AddonInfoEx;
typedef QList<AddonInfoEx> AddonInfoExList;

struct PGetAppsReq 
{
	PGetAppsReq()
		:uid(0),sid(0),yysdkver(0),yyver(0) {}
	quint32 uid;
	quint32 sid;
	quint32 yysdkver;
	quint32 yyver;
	QString expand;
};

struct Sid2Apps
{
	Sid2Apps()
		:subsid(0),frontAppid(0) {}
	quint32 subsid;
	QList<quint32> appids;
	quint32 frontAppid;	//三栏插件中谁显示在最前面
};

struct PAddAppReq
{
	PAddAppReq(){}
	QMap<quint32, AppInfoPack> appInfos;
	QMap<quint32, Sid2Apps> sidapps;
	QList<quint32> globalapps;
	QString expand;
};

typedef PAddAppReq AddonInfoMap;

struct PGetAppsRes:public PAddAppReq
{
	PGetAppsRes()
		:uid(0),sid(0) {}
	quint32 uid;
	quint32 sid;
};

struct PDelAppReq
{
	QList<Sid2Apps> sidapps;
	QList<quint32> globalapps;
	QString expand;
};