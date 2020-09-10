#pragma once

#include "dwbase/dwcomex.h"
#include "dwbase/dwcominterface_i.h"
#include "dwbase/yycomclsid.h"
#include "dwutility/dwlocker.h"

DWDEFINE_INTERFACE(IRootSink): public IUnk
{
	virtual void onUnRegister(const char *name) = 0;
};

class DWComRoot: public IDWComRoot
				,public IDWComponentMiscInfo
				,public IDWSdkVersion
				,public DWComponentLevelImpl
				,public IRootSink
				,public IDWLanguageInfo
				,public IServiceRegister
				,public IDWCmdLine
				,public IDWCmdLineWritter
{
	DWDECLARE_NOT_AGGREGATABLE(DWComRoot);
	DWBEGIN_COM_MAP(DWComRoot, IDWComRoot)
		DWCOM_INTERFACE_ENTRY(IDWComRoot)
		DWCOM_INTERFACE_ENTRY(IDWComponentMiscInfo)
		DWCOM_INTERFACE_ENTRY(IDWSdkVersion)
		DWCOM_INTERFACE_ENTRY(IDWComponentLevel)
		DWCOM_INTERFACE_ENTRY(IRootSink)
		DWCOM_INTERFACE_ENTRY(IDWLanguageInfo)
		DWCOM_INTERFACE_ENTRY(IServiceRegister)
		DWCOM_INTERFACE_ENTRY(IDWCmdLine)
		DWCOM_INTERFACE_ENTRY(IDWCmdLineWritter)
	DWEND_COM_MAP()

private:	//IDWComRoot
	virtual HRESULT coCreateComService(const char *name, IUnk* pOutUnk, comptr<IUnk>& pv);
	virtual IUnkPtr getService(const char *name) const;

private:	//IDWComponentMiscInfo
	virtual std::wstring componentRootPath() const;
	virtual std::wstring yypath() const;
	virtual std::wstring componentDataPath() const;
	virtual int version() const;	
    virtual void set(const std::string& key, const std::string& value);
    virtual std::string value(const std::string& key) const;

private:	//IDWSdkVersion
	virtual unsigned int componentSdkVersion() const;
	virtual std::wstring componentSdkVersionString() const;
	virtual std::wstring componentSdkVersionStringHex() const;
	virtual unsigned int majorVersion() const;
	virtual unsigned int minorVersion() const;
	virtual unsigned int patchVersion() const;

private:	//IRootSink
	virtual void onUnRegister(const char *name);

private:	//IServiceRegister
	virtual bool registerService(const char *name, IUnkPtr p);
	virtual bool unregisterService(const char *name);

private:
	virtual IDWComponentLevelPtr clone();

private:	//IDWLanguageInfo
	virtual std::wstring languageId() const;

private:	//IDWCmdLine
	virtual bool cmdLine(const std::wstring &key, std::wstring &value) const;
	virtual bool cmdLine(std::map<std::wstring, std::wstring> &out) const;

private:	//IDWCmdLineWritter
	virtual bool addCmdLine(const std::wstring &key, const std::wstring &value);
	virtual bool removeCmdLine(const std::wstring &key);

private:
	std::map<std::string, IUnkPtr> m_objMap;
	DwUtility::DwCSLock m_cs;
	std::map<std::wstring, std::wstring> m_cmdLine;
    std::map<std::string, std::string> m_data;
};
DWCLASS_MAP_ENTRY(KSERVICEComRoot, DWComRoot);