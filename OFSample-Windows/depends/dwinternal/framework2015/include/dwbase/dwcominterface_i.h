#pragma once
#include "dwcomex.h"

//这里不能依赖qt
DWDEFINE_INTERFACE(IDWComInfo): public IUnk
{
	virtual bool initDWComInfo(const std::wstring &path) = 0;
	virtual int type() const = 0;
	virtual std::wstring name() const = 0;
	virtual std::wstring title() const = 0;
	virtual std::wstring description() const= 0;
	virtual std::wstring buttonText() const = 0;
	virtual std::wstring iconPath() const = 0;	//path是相对于组件跟目录的路径
};

DWDEFINE_INTERFACE(IDWComLauncher): public IUnk
{
	virtual bool start(IDWComRoot *root) = 0;
	virtual bool init(IDWComRoot *root) = 0;
	virtual bool stop(IDWComRoot *root) = 0;
};

DWDEFINE_INTERFACE(IDWComponentEntry): public IUnk
{
	virtual bool onLoad(IDWComRoot *root, unsigned int key) = 0;
	virtual bool onUnLoad(unsigned int key) = 0;
};

DWDEFINE_INTERFACE(IDWComponentEntryEx) : public IDWComponentEntry
{
	// 仅在YY进程外的组件会调用该接口
	// 生存于YY进程内的组件请继续使用onUnLoad
	virtual bool onUnloadEx(IDWComRoot *root, unsigned int key) = 0;
};


DWDEFINE_INTERFACE(IDWComponentMiscInfo): public IUnk
{
	/*如果是内置组件，componentRootPath是YY的路径*/
	virtual std::wstring componentRootPath() const = 0;
	virtual std::wstring yypath() const = 0;
	virtual std::wstring componentDataPath() const = 0;	//返回%appdata%下的位置
	virtual int version() const = 0;	//返回组件自己的版本
    virtual void set(const std::string& key, const std::string& value) = 0;
    virtual std::string value(const std::string& key) const = 0;
};

DWDEFINE_INTERFACE(IDWSdkVersion): public IUnk
{
	virtual unsigned int componentSdkVersion() const = 0;
	virtual std::wstring componentSdkVersionString() const = 0;
	virtual std::wstring componentSdkVersionStringHex() const = 0;
	virtual unsigned int majorVersion() const = 0;
	virtual unsigned int minorVersion() const = 0;
	virtual unsigned int patchVersion() const = 0;
};

DWDEFINE_INTERFACE(IDWComponentLevel) : public IUnk
{
	virtual void setComponentId(int id) = 0;
	virtual int componentId() const = 0;
	virtual void setComponentName(const std::string &name) = 0;
	virtual std::string componentName() const = 0;
	virtual IDWComponentLevelPtr clone() = 0;
};

DWDEFINE_INTERFACE(IDWLanguageInfo) : public IUnk
{
	virtual std::wstring languageId() const = 0;
};

class DWComponentLevelImpl : public IDWComponentLevel
{
public:
	DWComponentLevelImpl():m_id(-1) {}

protected:
	virtual void setComponentId(int id)
	{
		m_id = id;
	};
	virtual int componentId() const
	{
		return m_id;
	};
	virtual void setComponentName(const std::string &name)
	{
		m_componentName = name;
	}
	virtual std::string componentName() const
	{
		return m_componentName;
	}
private:
	int m_id;
	std::string m_componentName;
};

DWDEFINE_INTERFACE(IDWCmdLine): public IUnk
{
	virtual bool cmdLine(const std::wstring &key, std::wstring &value) const = 0;
	virtual bool cmdLine(std::map<std::wstring, std::wstring> &out) const = 0;
};

DWDEFINE_INTERFACE(IDWCmdLineWritter): public IUnk
{
	virtual bool addCmdLine(const std::wstring &key, const std::wstring &value) = 0;
	virtual bool removeCmdLine(const std::wstring &key) = 0;
};