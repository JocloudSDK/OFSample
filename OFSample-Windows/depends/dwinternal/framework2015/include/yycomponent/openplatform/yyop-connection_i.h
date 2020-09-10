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

	// sendPackage׼������ ʹ��sendTransferData
	virtual bool sendPackage(const DwMarshallable &msg) = 0;
	virtual void start() = 0;
	virtual void close() = 0;
    virtual bool isWorking() = 0;

	// yy 5.0 add
	/*
	appType	ȡֵ�μ�EDCFTerminalMonitorType
	apiVer	ȡֵ�μ�YY_ADDON_API_VER*��
	*/
	virtual QString buildCmd(int appType, const QString &strFileName, const QString &strLocalWorkPath, int apiVer, bool isTemplate);
	/*
	strVipPrefix	Ӧ���Լ���VIPǰ׺����Щ������룬���硰ֲ�
	uid				�û�UID����ʱ�Ǳ��룬������0
	ver				addon�İ汾����ʱ�Ǳ��룬������0
	cmd				���ɵ�cmd����ʱ�Ǳ��룬������մ�
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
   * @brief . ͨ��mac��ַ��ѯaddon id
   * @param . mac
   * @return . addon id
   */
    virtual quint32 getAddonIdByMac(UINT64 mac) = 0;

/**
   * @brief . ͨ��addon id ��ѯmac
   * @param . addon id
   * @return . mac
   */
    virtual UINT64 getMacByAddonId(quint32 addonId) = 0;
};