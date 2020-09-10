#pragma once

#include <dwutility/dwxmlelement.h>
#include <dwutility/utf8to.h>
#include "yycomponent/yychannel/yychannelconstdefine.h"

namespace
{
	enum
	{
		MessageTailLight = 100,
		ActivityTailLight = 101,
		ForwordNickName = 102,
        BehindMessage = 103,
	};

	class YYChannelMessageExtraInfoHelper
	{
	public:
		explicit YYChannelMessageExtraInfoHelper(DWXmlElement *e, const std::wstring &componentName);
		~YYChannelMessageExtraInfoHelper();

		void addMessageTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip = std::wstring(), const bool isCache = false);
		void addActivityTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip = std::wstring(), const bool isCache = false);
		void addIconForwordNickName(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip = std::wstring(), const bool isCache = false);
        void addIconBeindMessage(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip = std::wstring(), const bool isCache = false);

	private:
		DWXmlElement *addTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip, const bool isCache);

	private:
		DWXmlElement *m_elem;
		const std::wstring m_componentName;
		std::list<DWXmlElement *> m_extras;
	};

	YYChannelMessageExtraInfoHelper::YYChannelMessageExtraInfoHelper(DWXmlElement *e, const std::wstring &componentName)
		: m_elem(e)
		, m_componentName(componentName)
	{
	}

	YYChannelMessageExtraInfoHelper::~YYChannelMessageExtraInfoHelper()
	{
		if (m_elem && !m_extras.empty())
		{
			DWXmlElement *elemItem = new DWXmlElement(KTagExtra);
			elemItem->setAttribute(KAttrID, m_componentName);

			for (std::list<DWXmlElement *>::const_iterator itor = m_extras.begin(); itor != m_extras.end(); ++itor)
			{
				elemItem->addChild(*itor);
			}

			m_elem->addChild(elemItem);
		}
	}

	void YYChannelMessageExtraInfoHelper::addMessageTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip /*= std::wstring()*/, const bool isCache /*= false*/)
	{
		DWXmlElement *iEle = addTailLight(priority, key, url, tooltip, isCache);
		iEle->setAttribute(KAttrPos, itow10(MessageTailLight));
	}

	void YYChannelMessageExtraInfoHelper::addActivityTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip /*= std::wstring()*/, const bool isCache /*= false*/)
	{
		DWXmlElement *iEle = addTailLight(priority, key, url, tooltip, isCache);
		iEle->setAttribute(KAttrPos, itow10(ActivityTailLight));
	}

	DWXmlElement * YYChannelMessageExtraInfoHelper::addTailLight(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip, const bool isCache)
	{
		DWXmlElement *iEle = new DWXmlElement(KTagImage);
		iEle->setAttribute(KAttrData, key);
		iEle->setAttribute(KAttrUrl, url);
		iEle->setAttribute(KAttrPriority, itow10(priority));
		iEle->setAttribute(KAttrTooltip, tooltip);
		iEle->setAttribute(KAttrIsCache, isCache ? L"1" : L"0");
		m_extras.push_back(iEle);

		return iEle;
	}

	void YYChannelMessageExtraInfoHelper::addIconForwordNickName(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip /*= std::wstring()*/, const bool isCache /*= false*/)
	{
		DWXmlElement *iEle = addTailLight(priority, key, url, tooltip, isCache);
		iEle->setAttribute(KAttrPos, itow10(ForwordNickName));
	}

    void YYChannelMessageExtraInfoHelper::addIconBeindMessage(const int priority, const std::wstring &key, const std::wstring &url, const std::wstring &tooltip /*= std::wstring()*/, const bool isCache /*= false*/)
    {
        DWXmlElement *iEle = addTailLight(priority, key, url, tooltip, isCache);
        iEle->setAttribute(KAttrPos, L"3"); // ExtraInfoPos::MessageTail
    }

	class YYChannelMessageSenderInfoHelper
	{
	public:
		explicit YYChannelMessageSenderInfoHelper(DWXmlElement *e);
		~YYChannelMessageSenderInfoHelper();

		void setSenderNickColor(QRgb color);

	private:
		DWXmlElement *m_elem;
		DWXmlElement *m_sender;
	};

	YYChannelMessageSenderInfoHelper::YYChannelMessageSenderInfoHelper(DWXmlElement *e)
		: m_elem(e)
		, m_sender(NULL)
	{
	}

	YYChannelMessageSenderInfoHelper::~YYChannelMessageSenderInfoHelper()
	{
		if (!m_sender)
		{
			// WTF to DWXmlElement::child return a const DWXmlElement!!!!!
			m_elem->addChild(m_sender);
		}
	}

	void YYChannelMessageSenderInfoHelper::setSenderNickColor(QRgb color)
	{
		if (!m_sender)
		{
			m_sender = new DWXmlElement(KTagSender);
		}

		m_sender->setAttribute(KAttrNickColor, QString::number(color, 16).toStdWString());
	}

}
