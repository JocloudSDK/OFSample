#include "stable.h"

#include "duifw/duiwidget.h"
#include "dwutility/dwxmldocument.h"
#include "duiresourceloader.h"

bool parseXmlFont(QObject *widget, const DWXmlElement * xml);

DUI_DEFINE_DYNAMIC_DATA(Font, parseXmlFont);

typedef bool(* fontPropertyFunc)(QFont *font, const std::wstring &val);

typedef DuiDynamicList<fontPropertyFunc, QFont, void> FontProperties;

#define DUI_DEFINE_FONT_PROPERTY(prop, func) \
		DuiTypeRegister<fontPropertyFunc, DuiWidget, void> data_register_##func( \
				new DuiDynamicCreateDataNode<fontPropertyFunc>(#prop, func));

typedef QHash<QString, QFont> FontFile;
typedef QCache<QString, FontFile> FontCacheType;
Q_GLOBAL_STATIC(FontCacheType, kFontCache)

struct setProperty
{
	setProperty(QObject *widget) :widget_(widget)
	{
	}
	bool operator() (const QString &name, const QFont &font)
	{
		return widget_->setProperty(name.toLatin1().constData(), font);
	}
	private:
	QObject *widget_;
};

template<typename Callback_>
bool parseFontsInto(const DWXmlElement * xml, Callback_ &callback)
{
	int count = xml->attributeCount();
	QFont font;
	for (int i = 0; i < count; ++i)
	{
		bool res = FontProperties::createData(xml->attributeName(i), &font, xml->attribute(i));
		if(!res)
		{
			DW_ASSERT(res && L"invalid font property");
			return res;
		}
	}
	QString name = QStringFromWCharArray((*xml)[L"name"].c_str());
	return callback(name, font);
}

bool parseXmlFont(QObject *widget, const DWXmlElement * xml)
{
	return parseFontsInto(xml, setProperty(widget));
}

struct putIntoHash
{
	putIntoHash(FontFile *file) : file_(file){}
	bool operator() (const QString &name, const QFont &font)
	{
		bool res = !file_->contains(name);
		DW_ASSERT(res && L"duplicate font define");
		file_->insert(name, font);
		return res;
	}
	private:
	FontFile *file_;
};

QFont qStringToFont(const QString& val)
{
	QStringList s = val.split("|");
	DWXmlDocument xml;
	if (s.count() == 2 )
	{
		FontFile *file = kFontCache()->object(s[0]);
		if(file)
		{
			return file->value(s[1]);
		}
		file = new FontFile;
		kFontCache()->insert(s[0], file);
		if(getResourceLoader()->loadXml(s[0], xml))
		{
			const DWXmlElement *child = xml.elementByPath(L"xmlui-resource");
			int count = child->childCount();
			for (int index = 0; index < count; ++index)
			{
				parseFontsInto(child->child(index), putIntoHash(file));
			}
			return file->value(s[1]);
		}
	}
	DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid xml file.").arg(s[0]).toLatin1().constData());
	return QFont();
}

static bool parseBool(const std::wstring &txt)
{
	if(L"true" == txt)
	{
		return true;
	}
	else if(L"false" == txt)
	{
		return false;
	}
	else
	{
		DW_ASSERT(false && "invalid bool value!");
		return false;	 //unreachable
	}
}

namespace
{

	bool ignore(QFont *, const std::wstring &)
	{
		return true;
	}

	bool setBold(QFont *font, const std::wstring &val)
	{
		font->setBold(parseBool(val));
		return true;
	}

	bool setFamily(QFont *font, const std::wstring &val)
	{
		font->setFamily(QStringFromWString(val));
		return true;
	}

	bool setFixedPitch(QFont *font, const std::wstring &val)
	{
		font->setFixedPitch(parseBool(val));
		return true;
	}

	bool setItalic(QFont *font, const std::wstring &val)
	{
		font->setItalic(parseBool(val));
		return true;
	}

	bool setKerning(QFont *font, const std::wstring &val)
	{
		font->setKerning(parseBool(val));
		return true;
	}

	bool setOverline(QFont *font, const std::wstring &val)
	{
		font->setOverline(parseBool(val));
		return true;
	}

	bool setPixelSize(QFont *font, const std::wstring &val)
	{
		font->setPixelSize(_wtoi(val.c_str()));
		return true;
	}

	bool setPointSize(QFont *font, const std::wstring &val)
	{
		font->setPointSize(_wtoi(val.c_str()));
		return true;
	}

	bool setPointSizeF(QFont *font, const std::wstring &val)
	{
		font->setPointSize(wcstod(val.c_str(), NULL));
		return true;
	}

	bool setRawMode(QFont *font, const std::wstring &val)
	{
		font->setRawMode(parseBool(val));
		return true;
	}

	bool setRawName(QFont *font, const std::wstring &val)
	{
		font->setRawName(QStringFromWString(val));
		return true;
	}

	bool setStretch(QFont *font, const std::wstring &val)
	{
		font->setStretch(_wtoi(val.c_str()));
		return true;
	}

	bool setStrikeOut(QFont *font, const std::wstring &val)
	{
		font->setStrikeOut(parseBool(val));
		return true;
	}

	bool setUnderline(QFont *font, const std::wstring &val)
	{
		font->setUnderline(parseBool(val));
		return true;
	}

	bool setWeight(QFont *font, const std::wstring &val)
	{
		font->setWeight(_wtoi(val.c_str()));
		return true;
	}

	bool setWordSpacing(QFont *font, const std::wstring &val)
	{
		font->setWordSpacing(wcstod(val.c_str(), NULL));
		return true;
	}

	Q_DECLARE_METATYPE(QFont::Capitalization)
	Q_DECLARE_METATYPE(QFont::SpacingType)
	Q_DECLARE_METATYPE(QFont::Style)
	Q_DECLARE_METATYPE(QFont::StyleHint)
	Q_DECLARE_METATYPE(QFont::StyleStrategy)

	bool setCapitalization(QFont *font, const std::wstring &val)
	{
		font->setCapitalization(QVariant(QStringFromWString(val)).value<QFont::Capitalization>());
		return true;
	}

	bool setLetterSpacing(QFont *font, const std::wstring &val)
	{
		font->setLetterSpacing(QVariant(QStringFromWString(val)).value<QFont::SpacingType>(), wcstod(val.c_str(), NULL));
		return true;
	}

	bool setStyle(QFont *font, const std::wstring &val)
	{
		font->setStyle(QVariant(QStringFromWString(val)).value<QFont::Style>());
		return true;
	}

	bool setStyleHint(QFont *font, const std::wstring &val)
	{
		font->setStyleHint(QVariant(QStringFromWString(val)).value<QFont::StyleHint>());
		//font->StyleStrategy(QVariant(QStringFromWString(val)).value<QFont::StyleStrategy>());
		return true;
	}

	bool setStyleStrategy(QFont *font, const std::wstring &val)
	{
		font->setStyleStrategy(QVariant(QStringFromWString(val)).value<QFont::StyleStrategy>());
		return true;
	}


	DUI_DEFINE_FONT_PROPERTY(bold, setBold)
	DUI_DEFINE_FONT_PROPERTY(capitalization, setCapitalization)
	DUI_DEFINE_FONT_PROPERTY(family, setFamily)
	DUI_DEFINE_FONT_PROPERTY(fixedPitch, setFixedPitch)
	DUI_DEFINE_FONT_PROPERTY(italic, setItalic)
	DUI_DEFINE_FONT_PROPERTY(kerning, setKerning)
	DUI_DEFINE_FONT_PROPERTY(letterSpacing, setLetterSpacing)
	DUI_DEFINE_FONT_PROPERTY(overline, setOverline)
	DUI_DEFINE_FONT_PROPERTY(pixelSize, setPixelSize)
	DUI_DEFINE_FONT_PROPERTY(pointSize, setPointSize)
	DUI_DEFINE_FONT_PROPERTY(pointSizeF, setPointSizeF)
	DUI_DEFINE_FONT_PROPERTY(rawMode, setRawMode)
	DUI_DEFINE_FONT_PROPERTY(rawName, setRawName)
	DUI_DEFINE_FONT_PROPERTY(stretch, setStretch)
	DUI_DEFINE_FONT_PROPERTY(strikeOut, setStrikeOut)
	DUI_DEFINE_FONT_PROPERTY(style, setStyle)
	DUI_DEFINE_FONT_PROPERTY(styleHint, setStyleHint)
	DUI_DEFINE_FONT_PROPERTY(styleStrategy, setStyleStrategy)
	DUI_DEFINE_FONT_PROPERTY(underline, setUnderline)
	DUI_DEFINE_FONT_PROPERTY(weight, setWeight)
	DUI_DEFINE_FONT_PROPERTY(wordSpacing, setWordSpacing)
	DUI_DEFINE_FONT_PROPERTY(name, ignore)
}
