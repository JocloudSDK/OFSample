#pragma once
#include "dwcominterface_i.h"
#include "dwutility/dwxmldocument.h"

struct DWCOM_NO_VTABLE DWComInfoImpl: public IDWComInfo
{
	typedef struct _DWCOMINFO
	{
		int type;
		std::wstring name;
		std::wstring title;
		std::wstring desc;
		std::wstring buttonText;
		std::wstring iconPath;
		_DWCOMINFO() : type(0) { }
	} DWCOMINFO;

public:
	explicit DWComInfoImpl() { }
	virtual ~DWComInfoImpl() { }

private:
	virtual bool initDWComInfo(const std::wstring& path)
	{
		std::wstring file = path + L"/desc.xml";

		DWXmlDocument doc;
		if (!doc.loadFromFile(file))
		{
			return false;
		}

		const DWXmlElement* root = doc.rootElement();
		if (!root)
		{
			return false;
		}

		const DWXmlElement* child = root->child(0);
		if (!child)
		{
			return false;
		}

		m_info.type      = ::_ttol(child->attribute(std::wstring(L"type")).c_str());
		m_info.name       = child->attribute(std::wstring(L"name"));
		m_info.title      = child->attribute(std::wstring(L"title"));
		m_info.desc       = child->attribute(std::wstring(L"description"));
		m_info.buttonText = child->attribute(std::wstring(L"buttonText"));
		m_info.iconPath   = child->attribute(std::wstring(L"iconPath"));

		return true;
	}

	virtual int type() const { return m_info.type; }
	virtual std::wstring name() const{ return m_info.name; }
	virtual std::wstring title() const{ return m_info.title; }
	virtual std::wstring description() const{ return m_info.desc; }
	virtual std::wstring iconPath() const{ return m_info.iconPath; }
	virtual std::wstring buttonText() const{ return m_info.buttonText; }

private:
	DWCOMINFO m_info;
};

/*DWDEFINE_INTERFACE(IDWComDownloadTask): public IDWQComponent
{
	enum {Silent, DefaultUI, CustomUI, UseNotify = 4, NoUseNotify = 8};
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void setForce(bool b) = 0;
	virtual void setUIMode(int mode) = 0;	
};

DWDEFINE_INTERFACE(IYYComStore): public IDWQComponent
{
	virtual bool init(...) = 0;
	virtual IDWComDownloadTaskPtr install(const QString &name) = 0;
	virtual QString find(const QString &name) = 0;
};*/

