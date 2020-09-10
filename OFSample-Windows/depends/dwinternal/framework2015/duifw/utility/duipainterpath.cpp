#include "stable.h"

#include "duifw/duiwidget.h"
#include "dwutility/dwxmldocument.h"

typedef bool(* anchorFunction)(QPainterPath *path, const DWXmlElement *xml);
typedef DuiDynamicList<anchorFunction, QPainterPath, void> AnchorTypeList;

#define DUI_DEFINE_PATH_ANCHOR_TYPE(anchor_type) \
	DuiTypeRegister<anchorFunction, DuiWidget, void> data_register_##anchor_type( \
	new DuiDynamicCreateDataNode<anchorFunction>(#anchor_type, anchor_type));

Q_DECLARE_METATYPE(QPainterPath);

static void parseAttribute(QPainterPath *path, const std::wstring &name, const std::wstring &value)
{
	if(name == L"fillRule")
	{
		if(value == L"OddEvenFill")
		{
			path->setFillRule(Qt::OddEvenFill);
		}
		else if(value == L"WindingFill")
		{
			path->setFillRule(Qt::WindingFill);
		}
		else
		{
			DW_ASSERT(false && L"invalid fillRule");
		}
	}
}

bool parseXmlPainterPath(QObject *widget, const DWXmlElement * xml)
{
	DW_ASSERT(widget);

	QPainterPath path;
	QByteArray propertyName = QStringFromWCharArray((*xml)[L"name"].c_str()).toLatin1();
	DW_ASSERT(!propertyName.isEmpty());

	int count = xml->attributeCount();
	for(int index = 0; index < count; ++index)
	{
		std::wstring name = xml->attributeName(index);
		if(name.empty())
		{
			break;
		}
		std::wstring value = xml->attribute(index);
		parseAttribute(&path, name, value);
	}

	count = xml->childCount();
	for(int index = 0; index < count; ++index)
	{
		const DWXmlElement *child = xml->child(index);
		AnchorTypeList::createData(child->tag(), &path, child);
	}

	QVariant v = qVariantFromValue(path);
	bool b = widget->setProperty(propertyName.constData(), v);
	DW_ASSERT_X(b, __FUNCTION__, QString("setProperty %1 failed.").arg(QString(propertyName)).toLatin1().constData());
	return true;
}
DUI_DEFINE_DYNAMIC_DATA(PainterPath, parseXmlPainterPath);

//////////////////////////////////////////////////////////////////////////
bool addEllipse(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	qreal width(wcstod(xml->attribute2(L"width"), NULL));
	qreal height(wcstod(xml->attribute2(L"height"), NULL));

	path->addEllipse(x, y, width, height);
	return true;
}

bool addRect(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	qreal width(wcstod(xml->attribute2(L"width"), NULL));
	qreal height(wcstod(xml->attribute2(L"height"), NULL));

	path->addRect(x, y, width, height);
	return true;
}

bool addRoundedRect(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	qreal w(wcstod(xml->attribute2(L"w"), NULL));
	qreal h(wcstod(xml->attribute2(L"h"), NULL));
	qreal xRadius(wcstod(xml->attribute2(L"xRadius"), NULL));
	qreal yRadius(wcstod(xml->attribute2(L"yRadius"), NULL));
	Qt::SizeMode mode = Qt::AbsoluteSize;

	path->addRoundedRect(x, y, w, h, xRadius, yRadius, mode);
	return true;
}

bool addText(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	const QFont font;
	const QString text;

	path->addText(x, y, font, text);
	return true;
}

bool arcMoveTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	qreal width(wcstod(xml->attribute2(L"width"), NULL));
	qreal height(wcstod(xml->attribute2(L"height"), NULL));
	qreal angle(wcstod(xml->attribute2(L"angle"), NULL));

	path->arcMoveTo(x, y, width, height, angle);
	return true;
}

bool arcTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));
	qreal width(wcstod(xml->attribute2(L"width"), NULL));
	qreal height(wcstod(xml->attribute2(L"height"), NULL));
	qreal startAngle(wcstod(xml->attribute2(L"startAngle"), NULL));
	qreal sweepLength(wcstod(xml->attribute2(L"sweepLength"), NULL));

	path->arcTo(x, y, width, height, startAngle, sweepLength);
	return true;
}

bool closeSubpath(QPainterPath *path, const DWXmlElement *xml)
{
	Q_UNUSED(xml);
	path->closeSubpath();
	return true;
}

bool cubicTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal c1X(wcstod(xml->attribute2(L"c1X"), NULL));
	qreal c1Y(wcstod(xml->attribute2(L"c1Y"), NULL));
	qreal c2X(wcstod(xml->attribute2(L"c2X"), NULL));
	qreal c2Y(wcstod(xml->attribute2(L"c2Y"), NULL));
	qreal endPointX(wcstod(xml->attribute2(L"endPointX"), NULL));
	qreal endPointY(wcstod(xml->attribute2(L"endPointY"), NULL));

	path->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
	return true;
}

bool lineTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));

	path->lineTo(x, y);
	return true;
}

bool moveTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal x(wcstod(xml->attribute2(L"x"), NULL));
	qreal y(wcstod(xml->attribute2(L"y"), NULL));

	path->moveTo(x, y);
	return true;
}

bool quadTo(QPainterPath *path, const DWXmlElement *xml)
{
	qreal cx(wcstod(xml->attribute2(L"cx"), NULL));
	qreal cy(wcstod(xml->attribute2(L"cy"), NULL));
	qreal endPointX(wcstod(xml->attribute2(L"endPointX"), NULL));
	qreal endPointY(wcstod(xml->attribute2(L"endPointY"), NULL));

	path->quadTo(cx, cy, endPointX, endPointY);
	return true;
}

DUI_DEFINE_PATH_ANCHOR_TYPE(addEllipse)
DUI_DEFINE_PATH_ANCHOR_TYPE(addRect)
DUI_DEFINE_PATH_ANCHOR_TYPE(addRoundedRect)
DUI_DEFINE_PATH_ANCHOR_TYPE(addText)
DUI_DEFINE_PATH_ANCHOR_TYPE(arcMoveTo)
DUI_DEFINE_PATH_ANCHOR_TYPE(arcTo)
DUI_DEFINE_PATH_ANCHOR_TYPE(closeSubpath)
DUI_DEFINE_PATH_ANCHOR_TYPE(cubicTo)
DUI_DEFINE_PATH_ANCHOR_TYPE(lineTo)
DUI_DEFINE_PATH_ANCHOR_TYPE(moveTo)
DUI_DEFINE_PATH_ANCHOR_TYPE(quadTo)
