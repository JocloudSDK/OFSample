#include "stable.h"

#include "duifw.h"
#include "duiwidget.h"
#include "duifw/duiframewindow.h"
#include "duixmlui.h"
#include "dwutility/dwxmldocument.h"
#include "dwbase/log.h"
#include "dwutility/ustring.h"
#include "duiresourceloader.h"
#include "dwbase/perflog.h"
#include "core/duifwstyle.h"

extern QFont qStringToFont(const QString& val);

bool KDuiIsEditorMode = false;
DuiCreateMode KDuiCurrentEditorMode = DuiNormalMode;
#define DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(type, creator, parser) \
	DUI_DEFINE_PROPERTY_METATYPE(type, ObjectPropertyMarshal(creator, parser))

//利用__FUNCTION__的静态的char指针，做一个map，缓存住wNameID，使用静态Log，提高性能
//duixml只能主线程使用，本函数非多线程安全
WORD AddToPerfStrTable(const char* szName)
{
	static std::map<const char*, WORD> s_mapStrTable;

	std::map<const char*, WORD>::const_iterator it = s_mapStrTable.find(szName);
	if (it != s_mapStrTable.end())
	{
		return it->second;
	}

	WORD wNameID = Perf::AddToStrTable(szName);
	s_mapStrTable.insert(std::make_pair(szName, wNameID));

	return wNameID;
}

CPerfLogProxy::CPerfLogProxy(const char *szName)
{
	if (1 > Perf::GetPerfLogLevel())
	{
		m_wNameID = 0;
	}
	else
	{
		static WORD s_wFilterID = Perf::AddToStrTable(F_DUI);
		m_wNameID = AddToPerfStrTable(szName);
		Perf::Record(Perf::TYPE_E_BEGIN, 1, m_wNameID, s_wFilterID);
	}
}
CPerfLogProxy::~CPerfLogProxy()
{
	if (m_wNameID > 0)
	{
		static WORD s_wFilterID = Perf::AddToStrTable(F_DUI);
		Perf::Record(Perf::TYPE_E_END, 1, m_wNameID, s_wFilterID);
	}
}

namespace
{
	QStack<QPointer<QTranslator>> g_translatorStack;

	QVariant byPassConverter(const QString &value)
	{
		return value;
	}

	QVariant unSupportConverter(const QString &value)
	{
		DW_ASSERT(0);
		return byPassConverter(value);
	}

	QString byPassParser(const QVariant &val)
	{
		return val.value<QString>();
	}	
	
	QString unSupportParser(const QVariant &val)
	{
		Q_UNUSED(val);
		DW_ASSERT(0);
		QString s = byPassParser(val);
		return s;
	}

	void setProperties(const DWXmlElement *child, QObject* item, DuiCreateMode mode/* = DuiNormalMode*/)
	{
		// if under editor mode, set xml node as dynamic property
		if (DuiEditorMode == mode ||
			DuiPreviewMode == mode)
		{
			item->setProperty(KAttributeXmlNode, (unsigned int)child);
		}
		int count = child->attributeCount();
		for (int index = 0; index < count; ++index)
		{
			const std::wstring& wstrAttrName = child->attributeName(index);
			if (wstrAttrName.empty())
			{
				DW_ASSERT_X(false, __FUNCTION__, "attr name is null");
				continue;
			}
			QByteArray attrName = QStringFromWString(wstrAttrName).toLatin1();
			QString attr = QStringFromWString(child->attribute(index));
			int idx = item->metaObject()->indexOfProperty(attrName.constData());
			if(-1 != idx)
			{
				QMetaProperty prop = item->metaObject()->property(idx);
				int type_id = prop.type();
				if(type_id == QVariant::UserType)
				{
					type_id = prop.userType();
				}
				
				ObjectPropertyMarshal marshal = DuiWidgetPropertyDynamicList::findFunc(type_id);
				if (!marshal.valid())
				{
					ObjectPropertyMarshal tmp(byPassConverter, byPassParser);
					marshal = tmp;
				}

				if (!item->setProperty(attrName, marshal(attr)))
				{
					DW_ASSERT_X(false, __FUNCTION__, QString("setProperty %1 failed.").arg(QString(attrName)).toLatin1().constData());
					LogFinal(LOG::KDuiFw) << QString("setProperty %1 failed.").arg(attrName.constData());
				}
			}else //if (idx == -1)
			{
                // treat as dynamic property
                bool b = item->setProperty(attrName, attr);
                DW_ASSERT_X(!b, __FUNCTION__, QString("set dynamic property fail").toLatin1().constData());
			}
		}
	}

	/*
	use for connect by name
	*/
	struct connectByNameDataItem
	{
		connectByNameDataItem():connected(false)
		{
		}
		QString signalName;
		QMetaMethod method;
		bool connected;
	};
	void connectFromTo(QObject *from, QObject *to, QList<connectByNameDataItem> &signalNames);

	void setDuiWidgetCursorDeep(DuiWidget *widget, const QCursor &cursor)
	{
		if (!widget) return ;
		widget->setCursor(cursor);

		QList<QGraphicsItem *> items = widget->childItems();
		foreach(QGraphicsItem *item, items)
		{
			item->setCursor(cursor);
		}
	}
} 


DuiXmlUI::AutoTranslatorSetting::AutoTranslatorSetting(QTranslator *t)
{
	g_translatorStack.push(t);
}

DuiXmlUI::AutoTranslatorSetting::~AutoTranslatorSetting()
{
	DW_ASSERT(!g_translatorStack.empty());
	g_translatorStack.pop();
}

bool DuiXmlUI::isDynamicDataTag(const QString &xmlTag)
{
	return DuiWidgetDataDynamicList::hasClassName(xmlTag);
}

bool DuiXmlUI::isMarshalSupport(int type)
{
	ObjectPropertyMarshal marshal = DuiWidgetPropertyDynamicList::findFunc(type);
	return marshal.valid();
}

QVariant DuiXmlUI::propertyMarshal(int type, const QString &value)
{
	ObjectPropertyMarshal marshal = DuiWidgetPropertyDynamicList::findFunc(type);
	if (!marshal.valid())
	{
		DW_ASSERT(0);
		ObjectPropertyMarshal tmp(unSupportConverter, byPassParser);
		marshal = tmp;
	}
	return marshal(value);
}

QString  DuiXmlUI::propertyMarshal(int type, const QVariant &value)
{
	ObjectPropertyMarshal marshal = DuiWidgetPropertyDynamicList::findFunc(type);
	if (!marshal.valid())
	{
		ObjectPropertyMarshal tmp(byPassConverter, unSupportParser);
		marshal = tmp;
	}
	return marshal(value);
}

QString DuiXmlUI::property2string(const QMetaProperty &prop, const QObject *obj)
{
	DW_ASSERT(prop.isReadable() && obj);
	int type_id = prop.type();
	if (QVariant::UserType == type_id)
	{
		type_id = prop.userType();
	}

	return propertyMarshal(type_id, prop.read(obj));
}

class DEEnumPropertyType { };
class FlagsPropertyType { };

Q_DECLARE_METATYPE(DEEnumPropertyType)
Q_DECLARE_METATYPE(FlagsPropertyType);
Q_DECLARE_METATYPE(AnchorInfo);
Q_DECLARE_METATYPE(AnchorInfo2);

int DuiXmlUI::flagsTypeId()
{
	return qMetaTypeId<FlagsPropertyType>();
}

int DuiXmlUI::enumTypeId()
{
	return qMetaTypeId<DEEnumPropertyType>();
}

Q_DECLARE_METATYPE(QPainterPath);

int DuiXmlUI::painterPathTypeId()
{
	return qMetaTypeId<QPainterPath>();
}

void DuiXmlUI::initDuiEditorEnv()
{
	KDuiIsEditorMode = true;
}

void DuiXmlUI::setCurrentEditorMode(DuiCreateMode mode)
{
	KDuiCurrentEditorMode = mode;
}

QString DuiXmlUI::tr(const QString &value)
{
    const char separator[] = "\\0";
    DW_ASSERT(value.count(separator) <= 1);
    
	QString translation;
	QByteArray source;
	QByteArray disambiguation;

	int pos = value.indexOf(QLatin1String(separator));
	if (-1 == pos)
    {
		source = value.toUtf8();
    }
    else
    {
        source = value.left(pos).toUtf8();
        disambiguation = value.mid(pos + _countof(separator) - 1).toUtf8();
        DW_ASSERT(disambiguation.size());
    }

	QPointer<QTranslator> translator = g_translatorStack.empty() ? NULL : g_translatorStack.top();
	if (translator)
	{
		QApplication::installTranslator(translator);
	}
	translation = QObject::tr(source, disambiguation.isEmpty() ? NULL : disambiguation);
	if (translator)
	{
		QApplication::removeTranslator(translator);
	}

    return translation;
}

DuiFrameWindow* DuiXmlUI::CreateFromXml(const DWXmlDocument& xml, QWidget *parent, DuiCreateMode mode/* = DuiNormalMode*/)
{
    return CreateFromXml(xml, "", parent, mode);
}

DuiFrameWindow* DuiXmlUI::CreateFromXml( const DWXmlDocument& xml, const QString& rootPath, QWidget *parent, DuiCreateMode mode /*= DuiNormalMode*/ )
{
	return CreateFromXml(xml, rootPath, parent, mode, NULL);
}

void DuiXmlUI::constructWidget(DuiWidget *w)
{
	if (w)
	{
		w->doConstruct();
	}
};

DuiFrameWindow* DuiXmlUI::CreateFromXml( const DWXmlDocument& xml, const QString& rootPath, QWidget *parent, DuiCreateMode mode, QTranslator *t)
{
    PerfLogF(F_DUI);
	AutoTranslatorSetting setting(t);
    const DWXmlElement *child = xml.elementByPath(L"xmlui");

    if (NULL == child)
    {
        DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "child is null.");
        LogFinal(LOG::KDuiFw) << "child is null";
        return NULL;
    }

    child = child->child(0);

    if (NULL == child)
    {
        DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "child is null.");
        LogFinal(LOG::KDuiFw) << "child is null";
        return NULL;
    }

    std::wstring className = child->tag();

    if (className.empty())
    {
        DW_ASSERT_X(false, __FUNCTION__, "className is empty.");
        LogFinal(LOG::KDuiFw) << "className is empty.";
        return NULL;
    }

    //1、创建Frame
    //std::wstring className = xml.GetTagName();
    QString qclassName = QStringFromWString(className);
    DuiFrameWindow* frame = DuiFrameDynamicList::createObject(qclassName, parent);
    if (!rootPath.isEmpty())
    {
        frame->rootWidget()->setProperty(KRootPathProperty, rootPath);
    }
    // treat unknown frame class as "DuiFrameWindow" as default in editor mode
    if ((DuiEditorMode == mode || DuiPreviewMode == mode) && !frame)
    {
        qclassName = KClassNameDuiFrameWindow;
        frame = DuiFrameDynamicList::createObject(qclassName, parent);
    }
    if (frame == NULL)
    {
        DW_ASSERT_X(false, __FUNCTION__, "frame is null.");
        LogFinal(LOG::KDuiFw) << "can't create object of class." << className;
        return NULL;
    }
    //2、设置Frame属性
    setProperties(child, frame, mode);
    if (DuiEditorMode == mode || DuiPreviewMode == mode) {
        DuiFrameWindow::FrameStyles style = frame->frameStyle();
        if (DuiEditorMode == mode)
        {
            style |= DuiFrameWindow::AppWindow;
            style &= ~DuiFrameWindow::FrameLess;
            style &= ~DuiFrameWindow::ToolTip;
        }
        else if (DuiPreviewMode == mode)
        {
            style |= DuiFrameWindow::FrameLess;
        }
        frame->setFrameStyle(style);
    }

    //3、创建widget
    int count = child->childCount();
    for (int index = 0; index < count; ++index)
    {
        createChild(child->child(index), frame->rootWidget(), mode);
    }

    //二次构造
    frame->doConstruct();
    connectSignalSlotByName(frame, frame);
    return frame;
}

DuiFrameWindow* DuiXmlUI::createFromMemory(const std::string& data, QWidget *parent)
{
	LogDev(LOG::KDuiFw) << L"createFromMemory(QWidget)" << data.size() << L", parent:" << (parent ? parent->objectName() : QString("Null"));

	DWXmlDocument xml;
	if(!xml.loadFromString(data.c_str(), data.size()))
	{
		DW_ASSERT_X(false, __FUNCTION__, "xml data error!");
		LogFinal(LOG::KDuiFw) << "xml data error!";
		return false;
	}

	return CreateFromXml(xml, parent);
}


bool DuiXmlUI::createFromMemory(const std::wstring& data, DuiWidget *parent)
{
    PerfLogF(F_DUI);
    LogDev(LOG::KDuiFw) << "createFromMemory(DuiWidget)" << data.size() << ", parent:" << (parent ? parent->objectName() : QString("Null"));

    DWXmlDocument xml;
    if(!xml.loadFromString(data))
    {
        DW_ASSERT_X(false, __FUNCTION__, "xml data error!");
        LogFinal(LOG::KDuiFw) << "xml data error!";
        return false;
    }

    return CreateFromXml(xml, parent);

}

DuiFrameWindow* DuiXmlUI::createFromMemory(const std::wstring& data, QWidget *parent /*= NULL*/)
{
    LogDev(LOG::KDuiFw) << L"createFromMemory(QWidget)" << data.size() << L", parent:" << (parent ? parent->objectName() : QString("Null"));

    DWXmlDocument xml;
    if(!xml.loadFromString(data))
    {
        DW_ASSERT_X(false, __FUNCTION__, "xml data error!");
        LogFinal(LOG::KDuiFw) << "xml data error!";
        return false;
    }

    return CreateFromXml(xml, parent);
}

DuiFrameWindow* DuiXmlUI::createFromFile(const QString& fileName, QWidget *parent)
{
    return createFromFile(fileName, "", parent);
}

DuiFrameWindow* DuiXmlUI::createFromFile( const QString& fileName, const QString &rootPath, QWidget *parent /*= NULL*/ )
{
	return createFromFile(fileName, rootPath, parent, NULL);
}

DuiFrameWindow* DuiXmlUI::createFromFile( const QString& fileName, const QString &rootPath, QWidget *parent, QTranslator *t)
{
    PerfLogFP(F_DUI, fileName.toStdString().c_str());

    QString filePath = fileName;
    if (!rootPath.isEmpty())
    {
        filePath = rootPath + fileName;
    }

    LogDev(LOG::KDuiFw) << L"createFromFile(QWidget)" << filePath << L", parent:" << (parent ? parent->objectName() : QString("Null"));

    DWXmlDocument xml;
    if (!getResourceLoader()->loadXml(filePath, xml))
    {
        DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid xml file.").arg(filePath).toLatin1().constData());
        LogFinal(LOG::KDuiFw) << QString("%1 is not an valid xml file.").arg(filePath);
		QFile xmlFile(filePath);
		if (xmlFile.open(QFile::ReadOnly))
		{
			QByteArray data = xmlFile.readAll();
			xmlFile.close();
			LogFinal(LOG::KMainframe) << L"Error:filePath=" << filePath << L",data=" << data.constData();
		}
		else
		{
			DWORD dwLastError = ::GetLastError();
			qint32 nError = xmlFile.error();
			LogFinal(LOG::KMainframe) << L"Error:Failed to open filePath=" << filePath << L",nError=" << nError << L",LastError=" << dwLastError;
		}
        return NULL;
    }

    return CreateFromXml(xml, rootPath, parent, KDuiCurrentEditorMode, t);
}


bool DuiXmlUI::createFromMemory(const std::string& data, DuiWidget *parent)
{
	PerfLogF(F_DUI);
	LogDev(LOG::KDuiFw) << L"createFromMemory(DuiWidget)" << data.size() << L", parent:" << (parent ? parent->objectName() : QString("Null"));

	DWXmlDocument xml;
	if(!xml.loadFromString(data.c_str(), data.size()))
	{
		DW_ASSERT_X(false, __FUNCTION__, "xml data error!");
		LogFinal(LOG::KDuiFw) << "xml data error!";
		return false;
	}

	return CreateFromXml(xml, parent);
}


bool DuiXmlUI::createFromFile(const QString &fileName, DuiWidget *parent)
{
	return createFromFile(fileName, QString(), parent);
}

bool DuiXmlUI::createFromFile(const QString &fileName, const QString &rootPath, DuiWidget *parent)
{
	return createFromFile(fileName, rootPath, parent, NULL);
}

bool DuiXmlUI::createFromFile(const QString &fileName, const QString &rootPath, DuiWidget *parent, QTranslator *t)
{
	PerfLogF(F_DUI);

    QString filePath = fileName;
    if (!rootPath.isEmpty())
    {
        filePath = rootPath + fileName;
        parent->setProperty(KRootPathProperty, rootPath);
    }

    LogDev(LOG::KDuiFw) << L"createFromFile(DuiWidget)" << filePath << L", parent:" << (parent ? parent->objectName() : QString("Null"));

	DWXmlDocument xml;
	if (!getResourceLoader()->loadXml(filePath, xml))
	{
		DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid xml file.").arg(filePath).toLatin1().constData());
		LogFinal(LOG::KDuiFw) << QString("%1 is not an valid xml file.").arg(filePath);
		return false;
	}

	return CreateFromXml(xml, parent, KDuiCurrentEditorMode, t);
}

bool DuiXmlUI::CreateFromXml(const DWXmlDocument& xml, DuiWidget *parent, DuiCreateMode mode/* = DuiNormalMode*/)
{
	return CreateFromXml(xml, parent, mode, NULL);
}

bool DuiXmlUI::CreateFromXml(const DWXmlDocument& xml, DuiWidget *parent, DuiCreateMode mode, QTranslator *t)
{
    PerfLogF(F_DUI);
	AutoTranslatorSetting setting(t);
    const DWXmlElement *child = xml.elementByPath(L"xmlui");

    if (NULL == child)
    {
        DW_ASSERT_X(false, __FUNCTION__, "child is null.");
        LogFinal(LOG::KDuiFw) << "child is null.";
        return NULL;
    }

    int count = child->childCount();
    for (int index = 0; index < count; ++index)
    {
        createChild(child->child(index), parent, mode);
    }

    connectSignalSlotByName(parent, parent);

    return true;
}

DuiWidget *DuiXmlUI::createWidget(const QString &className, QGraphicsItem *parent)
{
	DuiWidget *widget = DuiWidgetDynamicList::createObject(className.toStdString().c_str(), parent);
	return widget;
}

bool DuiXmlUI::createChild(const DWXmlElement *child, DuiWidget *parent, DuiCreateMode mode/* = DuiNormalMode*/)
{
	PerfLogF(F_DUI);
	const std::wstring& className = child->tag();
	QString qclassName = QStringFromWString(className);
	DuiWidget *xmlParent = parent->getXmlNode();
	DuiWidget *layoutObj = DuiWidgetDynamicList::createObject(qclassName.toStdString().c_str(), xmlParent);
	if ((DuiEditorMode == mode || DuiPreviewMode == mode) && !layoutObj)
	{
		// if not dynamic data tag, default treat as DuiWidget
		if (!isDynamicDataTag(qclassName))
		{
			qclassName = KClassNameDuiWidget;
			layoutObj = DuiWidgetDynamicList::createObject(qclassName.toStdString().c_str(), xmlParent);
		}
	}
	if (layoutObj == NULL)
	{
		bool b = DuiWidgetDataDynamicList::createData(className, xmlParent, child);
		if (!b)
		{
			DW_ASSERT_X(false, __FUNCTION__, (qclassName + " is not valid tag name").toLatin1().constData());
			LogFinal(LOG::KDuiFw) << qclassName << " is not valid tag name";
		}
		return b;
	}

	QVariant v = parent->property(KRootPathProperty);
	if (v.isValid())
	{
		layoutObj->setProperty(KRootPathProperty, v);
	}
	if (DuiEditorMode == mode)
	{
		setDuiWidgetCursorDeep(layoutObj, Qt::ArrowCursor);
	}
	layoutObj->setParent(xmlParent);
	xmlParent->addItem(layoutObj);

	setProperties(child, layoutObj, mode);

	int count = child->childCount();
    int normalTagCount = count;
	for (int index = 0; index < count; ++index)
	{
        const DWXmlElement* childElement = child->child(index);
		createChild(childElement, layoutObj, mode);
        if (isDynamicDataTag(QStringFromWString(childElement->tag())))
        {
            --normalTagCount;
        }
	}

	if (normalTagCount > 0)
	{
		connectSignalSlotByName(layoutObj, layoutObj);
	}

	layoutObj->doConstruct();
	return true;
}

void DuiXmlUI::connectSignalSlotByName(QObject *from, QObject *to, bool includeFrom /*= true*/)
{
	PerfLogF(F_DUI);
	QMap<QString, QList<connectByNameDataItem>> name2Signal;
	QList<QObject*> children = from->findChildren<QObject*>();
	if (includeFrom)
	{
		children.push_back(from);
	}

	const QMetaObject *mo = to->metaObject();

	//1、找到所有命名规范的slots
	int nCount = mo->methodCount();
	for (int i = 0; i < nCount; ++i)
	{
		QMetaMethod method = mo->method(i);
		if (method.methodType() != QMetaMethod::Slot)
		{
			continue;
		}
		const char *cslot = method.signature();
		DW_ASSERT(cslot);
		QString slot(QString::fromLatin1(cslot));
		QStringList list = QString(slot).split('(');
		if (list.count() < 2)
		{
			DW_ASSERT(false);
			continue;
		}

		QString methodName = list[0];
		list = methodName.split('_');
		if (list.count() != 3 || list[0] != "on")
		{
			continue;
		}

		int index = slot.lastIndexOf('_');
		connectByNameDataItem item;
		item.signalName = slot.right(slot.count() - index-1);
		item.method = method;
		name2Signal[list[1]].push_back(item);
	}

	int iChildCount = children.count();
	for (int i = 0; i < iChildCount; ++i)
	{
		QMap<QString, QList<connectByNameDataItem>>::iterator It = name2Signal.find(children[i]->objectName());
		if (It != name2Signal.end())
		{
			connectFromTo(children[i], to, *It);
		}
	}

#ifdef _DEBUG
	QMap<QString, QList<connectByNameDataItem>>::const_iterator It = name2Signal.begin();
	for (; It != name2Signal.end(); ++It)
	{
		const QList<connectByNameDataItem>& list = *It;
		for (int i = 0; i < list.count(); ++i)
		{
			if (list[i].connected == false)	//slot没有连接
			{
				if (list[i].signalName.contains("triggered"))
				{
					continue;
				}
				//处理DuiDialog没有名字为buttonAccept和ButtonReject的button而引起的信号自动链接时的waring
				//使用者没有按设计者的设计来使用Dialog
				QString slotName = list[i].method.signature();
				if(slotName == "on_buttonAccept_clicked()" || slotName == "on_buttonReject_clicked()" )
				{
					continue;
				}
				qWarning("DuiXmlUI::connectSignalSlotByName: from class %s and its children, No matching signal for %s::%s", from->metaObject()->className(), 
					to->metaObject()->className(), list[i].method.signature());
			}
		}
	}
#endif
}

void DuiXmlUI::initStyleInfo( const QString &styleFile )
{
    static bool initStyleManager = false;
    if (!initStyleManager)
    {
        initStyleManager = true;
        duiGetStyleManager()->loadStyleInfo(styleFile);
    }
}

namespace
{
	//widget property
	QString sizeF2string(const QVariant& v)
	{
		QSizeF siz = v.value<QSizeF>();
		return QString("%1,%2").arg(siz.width()).arg(siz.height());
	}

	QVariant string2SizeF(const QString& value)
	{
		QStringList s = value.split(',');
		QSizeF siz;
		if (s.size() >= 2)
		{
			siz.setWidth(s.at(0).toFloat());
			siz.setHeight(s.at(1).toFloat());
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, "string can't convert to size");
			LogFinal(LOG::KDuiFw) << value << " can't convert to size.";
		}

		return siz;
	}

	QString size2string(const QVariant &v)
	{
		QSize siz = v.value<QSize>();
		return QString("%1,%2").arg(siz.width()).arg(siz.height());
	}

	QVariant string2Size(const QString& value)
	{
		return string2SizeF(value).toSize();
	}

	QString pointF2string(const QVariant &v)
	{
		QPointF pointF = v.value<QPointF>();
		return QString("%1,%2").arg(pointF.x()).arg(pointF.y());
	}

	QVariant string2PointF(const QString& value)
	{
		QStringList s = value.split(',');
		QPointF pointF;
		if (s.size() >= 2)
		{
			pointF.setX(s.at(0).toFloat());
			pointF.setY(s.at(1).toFloat());
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, "string can't convert to point");
			LogFinal(LOG::KDuiFw) << value << " can't convert to point.";
		}

		return pointF;
	}

	QVariant string2Point(const QString &value)
	{
		return string2PointF(value).toPoint();
	}

	QVariant stringToFont(const QString& value)
	{
		return qStringToFont(value);
	}

	QVariant string2RectF(const QString& value)
	{
		QStringList s = value.split(',');
		QRectF rt;
		if (s.size() >= 4)
		{
			rt.setX(s.at(0).toFloat());
			rt.setY(s.at(1).toFloat());
			rt.setWidth(s.at(2).toFloat());
			rt.setHeight(s.at(3).toFloat());
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, "string can't convert to rect");
			LogFinal(LOG::KDuiFw) << value << " can't convert to rect.";
		}

		return rt;
	}

	QString rectF2string(const QVariant &v)
	{
		QRectF rc = v.value<QRectF>();
		return QString("%1,%2,%3,%4").arg(rc.x()).arg(rc.y()).arg(rc.width()).arg(rc.height());
	}

	QString rect2string(const QVariant &v)
	{
		QRect rc = v.value<QRect>();
		QString tmp = QString("%1,%2,%3,%4").arg(rc.x()).arg(rc.y()).arg(rc.width()).arg(rc.height());
		return tmp;
	}

	QVariant string2Rect(const QString &value)
	{
		QRectF rt = string2RectF(value).value<QRectF>();
		return rt.toRect();
	}

	QString point2string(const QVariant &v)
	{
		QPoint p = v.value<QPoint>();
		return QString("%1,%2").arg(p.x()).arg(p.y());
	}

	QString margin2string(const QVariant &v)
	{
		QMargins r = v.value<QMargins>();
		return QString("%1,%2,%3,%4").arg(r.left()).arg(r.top()).arg(r.right()).arg(r.bottom());
	}

	QVariant stringToMargin(const QString& value)
	{
		QStringList s = value.split(',');
		if (s.size() >= 4)
		{
			QMargins r(s.at(0).toFloat(), s.at(1).toFloat(), s.at(2).toFloat(), s.at(3).toFloat());
			return qVariantFromValue(r);
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not valid margin value.").arg(value).toLatin1().constData());
			LogFinal(LOG::KDuiFw) << value << " can't convert to QMargins.";
			return qVariantFromValue(QMargins()); //unreachable
		}
	}

	QVariant translateString(const QString& value)
    {
        return qVariantFromValue(I18nString(DuiXmlUI::tr(value)));
	}

	QString i18nString2string(const QVariant& value)
	{
		I18nString s = value.value<I18nString>();
		return s;
	}

	QString color2string(const QVariant &v)
	{
		QColor clr = v.value<QColor>();
		return QString("%1,%2,%3,%4").arg(clr.red()).arg(clr.green()).arg(clr.blue()).arg(clr.alpha());
	}

	QVariant string2Color(const QString& value)
	{
		QColor color;
		QStringList s = value.split(',');
		if (s.size() >= 3)
		{
			int red = s[0].toInt();
			int green = s[1].toInt();
			int blue = s[2].toInt();
			int alpha = 255;
			if (s.size() >= 4)
			{
				alpha = s[3].toInt();
			}
			color.setRgb(red, green, blue, alpha);
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not valid color value.").arg(value).toLatin1().constData());
			LogFinal(LOG::KDuiFw) << value << " can't convert to color.";
		}
		return color;
	}

	QString anchorInfo2string(const QVariant &v)
	{
		AnchorInfo info = v.value<AnchorInfo>();
		int idx = DuiWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
		QMetaEnum enu = DuiWidget::staticMetaObject.enumerator(idx);
		QString res;
		for (int i = 0; i < 4; i ++)
		{
			if (i > 0)
			{
				res += "|";
			}
			res += QString("%1:%2").arg(enu.valueToKey(info.anchorPoint[i])).arg(info.distance[i]);
		}
		return res;
	}
	/*
		value的格式
		anchor:distance
		anchor的取值Left，Right，Top，Bottom，VCenter，HCenter
	*/
	QVariant string2AnchorInfo(const QString &value)
	{
		int index = DuiWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
		QMetaEnum enu = DuiWidget::staticMetaObject.enumerator(index);
		QStringList list = value.split('|');
		if (list.count() != 4)
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not valide anchor info").arg(value).toLatin1().constData());
			LogFinal(LOG::KDuiFw) << value << " is not valide anchor info";
		}
		AnchorInfo info;
		for (int i = 0; i < list.count() && i < 4; ++i)
		{
			QStringList innerList = list[i].split(':');
			if (innerList.count() != 2)
			{
				DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not valide anchor info").arg(value).toLatin1().constData());
				LogFinal(LOG::KDuiFw) << value << " is not valide anchor info";
			}
			if (innerList.count() >= 2)
			{
				int value = enu.keyToValue(innerList[0].toLatin1().constData());
				if (value == -1)
				{
					DW_ASSERT_X(false, __FUNCTION__, (innerList[0] + "is not a valid enum value").toLatin1().constData());
					LogFinal(LOG::KDuiFw) << innerList[0] << "is not a valid enum value";
				}
				if (value == -1)
				{
					value = 0;
				}
				info.anchorPoint[i] = (Qt::AnchorPoint)value;
				info.distance[i] = innerList[1].toInt();
			}
		}
		return qVariantFromValue(info);
	}

	QString anchorInfo2string2(const QVariant &v)
	{
		AnchorInfo2 info = v.value<AnchorInfo2>();
		int idx = DuiWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
		QMetaEnum enu = DuiWidget::staticMetaObject.enumerator(idx);
		QString res;
		for (int i = 0; i < info.anchorNum; i ++)
        {
			if (i > 0)
			{
				res += "|";
			}
            res += QString("%1:%2.%3:%4")
                .arg(enu.valueToKey(info.point[i]))
                .arg(info.objectName[i])
                .arg(enu.valueToKey(info.anchorPoint[i]))
                .arg(info.distance[i]);
		}
		return res;
	}
	/*
		value的格式
		anchor:distance
		anchor的取值Left，Right，Top，Bottom，VCenter，HCenter
	*/
    // compatibleMode : 'Left:4|Right:0|Top:0|Top:20'
    // newMode        : 'Left:parent.Left:4|Right:xx.Right:0|...'
	QVariant string2AnchorInfo2(const QString &value)
    {
        bool compatibleMode = false;
        if (value.count('|') == 3 && value.count(':') == 4)
        {
            compatibleMode = true;
        }
        int index = DuiWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
        QMetaEnum enu = DuiWidget::staticMetaObject.enumerator(index);
		QStringList list = value.trimmed().split('|');
        DW_ASSERT(list.count() <= 4);
		AnchorInfo2 info;
		for (int i = 0; i < list.count() && i < 4; ++i)
		{
            DW_ASSERT(!list[i].isEmpty());
			QStringList innerList = list[i].split(':');
		
            QString anchorObjectName;
            int point = -1;
            int anchorPoint = -1;
            int distance = 0;
            if (compatibleMode)
            {
                const char *dummy[] = {"Left", "Right", "Top", "Bottom"};
                point = enu.keyToValue(dummy[i]);
                anchorObjectName = KAnchorParent;
                anchorPoint = enu.keyToValue(innerList[0].toLatin1().constData());
                distance = innerList[1].toInt();
            }
            else
            {
                if (innerList.count() != 3)
                {
                    DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not valide anchor info").arg(point).toLatin1().constData());
                    LogFinal(LOG::KDuiFw) << point << " is not valide anchor info";
                }
                point = enu.keyToValue(innerList[0].toLatin1().constData());
				QStringList anchorObjectlist = innerList[1].split('.');
				if (anchorObjectlist.count() != 2)
				{
					DW_ASSERT_X(false, 
						__FUNCTION__,
						QString("%1 is not valide  anchorObject info").arg(innerList[1]).toLatin1().data());
				}
                anchorObjectName = anchorObjectlist[0];
                anchorPoint = enu.keyToValue(anchorObjectlist[1].toLatin1().constData());
                distance = innerList[2].toInt();
            }
            if (point == -1 || anchorPoint == -1)
            {
                DW_ASSERT_X(false, __FUNCTION__, (innerList[0] + "is not a valid enum value").toLatin1().constData());
                LogFinal(LOG::KDuiFw) <<(innerList[0] + " is not a valid enum value");
            }
            if (point == -1) point = 0;
            if (anchorPoint == -1) anchorPoint = 0;

            info.point[i] = (Qt::AnchorPoint)point;
            info.anchorPoint[i] = (Qt::AnchorPoint)anchorPoint;
            info.objectName[i] = anchorObjectName;
            info.distance[i] = distance;
            info.anchorNum++;
		}
		return qVariantFromValue(info);
	}

	Q_DECLARE_METATYPE(QMargins);
	//Q_DECLARE_METATYPE(AnchorInfo);

	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QSizeF, string2SizeF, sizeF2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QSize, string2Size, size2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QFont, stringToFont, unSupportParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QMargins, stringToMargin, margin2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(I18nString, translateString, i18nString2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QColor, string2Color, color2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(AnchorInfo, string2AnchorInfo, anchorInfo2string);
    DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(AnchorInfo2, string2AnchorInfo2, anchorInfo2string2);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QPointF, string2PointF, pointF2string);
	// for property save
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QString, byPassConverter, byPassParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(bool, byPassConverter, byPassParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(int, byPassConverter, byPassParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QRect, string2Rect, rect2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QRectF, string2RectF, rectF2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(double, byPassConverter, byPassParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(uint, byPassConverter, byPassParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(QPoint, string2Point, point2string);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(DEEnumPropertyType, byPassConverter, unSupportParser);
	DEFINE_DYNAMIC_CREATE_PROPERTY_TYPE(FlagsPropertyType, byPassConverter, unSupportParser);

	bool setDefaultDataProperty(QObject *widget, const DWXmlElement * child)
	{
		int count = child->childCount();
		QList<QVariant> mapList;
		for (int i = 0; i < count; ++i)
		{
			int attrCount = child->child(i)->attributeCount();
			QMap<QString, QVariant> m;
			const DWXmlElement *elem = child->child(i);
			for (int j = 0; j < attrCount; j++)
			{
				std::wstring &attr = elem->attribute(j);
				m.insert(QStringFromWString(elem->attributeName(j)),
                    DuiXmlUI::tr(DwUtility::text::toNarrowString(attr.c_str(), attr.length(), CP_UTF8).c_str()));
			}
			mapList.push_back(m);
		}
		return widget->setProperty("data", mapList);
	}

	bool getTreeWidgetDataChildren(QList<QVariant>& list, const DWXmlElement* xml)
	{
		int count = xml->childCount();
		if (count == 0)
		{
			return false;
		}

		for (int i = 0; i < count; ++i)
		{
			int attrCount = xml->child(i)->attributeCount();
			QMap<QString, QVariant> m;
			const DWXmlElement *elem = xml->child(i);
			for (int j = 0; j < attrCount; j++)
			{
				std::wstring &attr = elem->attribute(j);
				m.insert(QStringFromWString(elem->attributeName(j)), 
                    DuiXmlUI::tr(DwUtility::text::toNarrowString(attr.c_str(), attr.length(), CP_UTF8).c_str()));
			}

			QList<QVariant> child;
			if (getTreeWidgetDataChildren(child, elem))
			{
				m.insert("children", child);
			}

			list.append(m);
		}

		return true;
	}

	bool setTreeWidgetDataProperty(QObject *widget, const DWXmlElement * child)
	{
		QList<QVariant> mapList;
		getTreeWidgetDataChildren(mapList, child);

		bool b = widget->setProperty("data", mapList);
		return b;
	}

	void connectFromTo(QObject *from, QObject *to, QList<connectByNameDataItem> &signalNames)
	{
		const QMetaObject *mo = from->metaObject();
		std::string sig;
		std::string slo;
		int methodCountCount = mo->methodCount();
		for (int i = 0; i < methodCountCount; ++i)
		{
			QMetaMethod method = mo->method(i);
			if (method.methodType() == QMetaMethod::Signal)
			{
				int signalNamesCount = signalNames.count();
				for (int j = 0; j < signalNamesCount; ++j)
				{
					const QString& signalName = signalNames[j].signalName;
					if (signalName == QLatin1String(method.signature()))	//match
					{
						sig.clear();
						sig.append("2");
						sig.append(method.signature());

						slo.clear();
						slo.append("1");
						slo.append(signalNames[j].method.signature());
						bool b = QObject::connect(from, sig.c_str(), to, slo.c_str(), Qt::UniqueConnection);
#ifdef _DEBUG
						if (!b)
						{
							DW_ASSERT_X(b, __FUNCTION__, QString("connect object(type %1) %2 signal:%3 \nto object(type %4) %5 slot:%6 more than once!")
									.arg(from->metaObject()->className()).arg(from->objectName()).arg(method.signature()).arg(to->metaObject()->className())
									.arg(to->objectName()).arg(signalNames[j].method.signature()).toLatin1().constData());
						}
#endif
						if (b)
						{
							signalNames[j].connected = true;
						}
					}
				}
			}
		}
	}
}

DUI_DEFINE_DYNAMIC_DATA(TabData, setDefaultDataProperty);
DUI_DEFINE_DYNAMIC_DATA(ComboData, setDefaultDataProperty);
DUI_DEFINE_DYNAMIC_DATA(TreeData, setTreeWidgetDataProperty);
