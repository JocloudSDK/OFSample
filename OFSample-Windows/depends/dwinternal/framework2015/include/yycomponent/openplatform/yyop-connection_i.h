#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

//#include "extinternal/dcf-terminal-def.h"		// for apiVer
//#include "extinternal/dcf-platform-srv-def.h"	// for appType

class QString;
class DwMarshallable;

/*
    use coCreateComInstance function
*/
DWDEFINE_INTERFACE(IDWOpConnection): public IDWQComponent
{
	virtual bool initlize(unsigned int id) = 0;
	virtual void setPeerPath(const QString &path) = 0;
	virtual void setVirtualIp(const QString &virtualIp) = 0;
	virtual void setCmdLine(const QString &cmdLine) = 0;

	// sendPackage准备废弃 使用sendTransferData
	virtual bool sendPackage(const DwMarshallable &msg) = 0;
	virtual void start() = 0;
	virtual void close() = 0;
    virtual bool isWorking() = 0;

	// yy 5.0 add
	/*
	appType	取值参见EDCFTerminalMonitorType
	apiVer	取值参见YY_ADDON_API_VER*等
	*/
	virtual QString buildCmd(int appType, const QString &strFileName, const QString &strLocalWorkPath, int apiVer, bool isTemplate);
	/*
	strVipPrefix	应用自己的VIP前缀，有些插件必须，比如“植物”
	uid				用户UID，暂时非必须，可以填0
	ver				addon的版本，暂时非必须，可以填0
	cmd				生成的cmd，暂时非必须，可以填空串
	*/
	virtual QString buildVip(const QString &strVipPrefix, quint32 uid, quint32 ver, const QString &cmd);
/*
signals:
    void connected();
    void dataArrived(quint32 uri, const QByteArray &package); //deprecated, please use transferDataArrived
    void disconnected();
    void peerQuited();  
    void peerQuitedEx(qint32 exitCode);
	void connectFailed(qint32);
	void transferDataArrived(quint32, const QByteArray &);
*/
	virtual bool sendTransferData(UINT32 dataType, const BYTE *data, size_t length) = 0;
};

DWDEFINE_INTERFACE(IDWOpConnectionMgr) : public IUnk
{
/**
   * @brief . 通过mac地址查询addon id
   * @param . mac
   * @return . addon id
   */
    virtual quint32 getAddonIdByMac(UINT64 mac) = 0;

/**
   * @brief . 通过addon id 查询mac
   * @param . addon id
   * @return . mac
   */
    virtual UINT64 getMacByAddonId(quint32 addonId) = 0;
};