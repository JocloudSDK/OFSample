#include "stable.h"

#include "dwglobal.h"
#include "duifwstyle.h"

#include "dwutility/dwxmldocument.h"
#include "dwbase/log.h"
#include "dwutility/ustring.h"

#include "core/duiresourceloader.h"
#include "xmlui/duixmlui.h"

//------------------------------------------------------------------------------
DuifwStyleManager::StyleInfo::StyleInfo()
{
	m_index = InvalidIndex;
	m_parentIndex = InvalidIndex;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::setId(const QString& id)
{
	m_id = id;
}

//------------------------------------------------------------------------------
const QString& DuifwStyleManager::StyleInfo::id()const
{
	return m_id;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::setParent(const QString& parent)
{
	m_parent = parent;
}

//------------------------------------------------------------------------------
const QString& DuifwStyleManager::StyleInfo::parent()const
{
	return m_parent;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::addAttr(const QString& attrName, qint32 attrIndex)
{
	m_attrs[attrName] = attrIndex;
}

//------------------------------------------------------------------------------
const DuifwStyleManager::AttrsMap& DuifwStyleManager::StyleInfo::attrContainer()const
{
	return m_attrs;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::setIndex(qint32 index)
{
	m_index = index;
}

//------------------------------------------------------------------------------
qint32 DuifwStyleManager::StyleInfo::index()const
{
	return m_index;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::setParentIndex(qint32 parentIndex)
{
	m_parentIndex = parentIndex;
}

//------------------------------------------------------------------------------
qint32 DuifwStyleManager::StyleInfo::parentIndex()const
{
	return m_parentIndex;
}

//------------------------------------------------------------------------------
void DuifwStyleManager::StyleInfo::validateIndex(const DuifwStyleManager* manager, qint32 index)
{
	DW_ASSERT(manager);
	DW_ASSERT(index != InvalidIndex);
	DW_ASSERT(!this->m_id.isEmpty());

	setIndex( index );

	if (!this->m_parent.isEmpty())
	{
		setParentIndex( manager->findIndex(m_parent) );
	}
}

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
DuifwStyleManager::DuifwStyleManager()
{
	//empty
}

//------------------------------------------------------------------------------
DuifwStyleManager::~DuifwStyleManager()
{
	//empty
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::loadStyleInfo(const QString& stylesFile)
{  
	DWXmlDocument xml;
	if (!getResourceLoader()->loadXml(stylesFile, xml))
	{
		DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid xml file.").arg(stylesFile).toLatin1().constData());
		LogFinal(LOG::KDuiFw) << stylesFile << " is not an valid xml file.";
		return NULL;
	}

	const DWXmlElement *root = xml.elementByPath(L"styleInfo");
	if (NULL == root)
	{
		return false;
	}

	return loadStyleInfo(root);
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::loadStyleInfo(const DWXmlElement* styleRoot)
{
	DW_ASSERT(styleRoot->tag() == L"styleData" || styleRoot->tag() == L"styleInfo");

	bool success = true;
	int count = styleRoot->childCount();
	for (int index = 0; index < count; ++index)
	{
		const DWXmlElement *styleElement = styleRoot->child(index);
		success = success && appendStyleInfo(styleElement);
	}

	return success;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::appendStyleInfo(const DWXmlElement* styleElement)
{
	DW_ASSERT(styleElement);
	DW_ASSERT(styleElement->tag() == L"style");

	StyleInfo info;
	if (praseStyleInfo(styleElement, info))
	{
		insertStyleInfo(info);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::praseStyleInfo(const DWXmlElement* styleElement, StyleInfo& info)
{
	int attrCount = styleElement->attributeCount();
	DW_ASSERT(attrCount >= 1); // must have the id

	for (int attrIndex = 0; attrIndex < attrCount; ++attrIndex)
	{
		QString wstrAttrName =QStringFromWString(styleElement->attributeName(attrIndex));
		QString wstrAttrValue = QStringFromWString(styleElement->attribute(attrIndex));

		// do not include "styleSheet" in styleInfo
		DW_ASSERT(wstrAttrName != QLatin1String("styleSheet"));
		if (wstrAttrName == QLatin1String("id"))
		{
			info.setId(wstrAttrValue);
		}
		else if (wstrAttrName == QLatin1String("parent"))
		{
			info.setParent(wstrAttrValue);
		}
		else
		{
			QVariant val(wstrAttrValue);
			qint32 attrIndex = appendAttrValue(val);
			info.addAttr(wstrAttrName, attrIndex);
		}
	}
	DW_ASSERT(!info.id().isNull() && !info.id().isEmpty());

	int childCount = styleElement->childCount();
	for (int childIndex=0; childIndex<childCount; ++childIndex)
	{
		const DWXmlElement* styleChildElement = styleElement->child(childIndex);

		std::wstring xmlStream;
		if( saveToWString(styleChildElement, xmlStream) )
		{
			QVariant val(QStringFromWString(xmlStream) );
			qint32 attrIndex = appendAttrValue(val);
			info.addAttr(QStringFromWString(styleChildElement->tag()), attrIndex);
		}
	}

	return !info.id().isEmpty();
}

//------------------------------------------------------------------------------
void DuifwStyleManager::insertStyleInfo(const StyleInfo& info)
{
	DW_ASSERT(!info.id().isEmpty());
	DW_ASSERT(m_styles.count() < Max_StyleSheets);

	qint32 insertIndex = InvalidIndex;

	if(m_id2index.contains(info.id()))
	{
		insertIndex = m_id2index.value(info.id());
		DW_ASSERT(insertIndex >=0 && insertIndex<m_styles.count());
		m_styles[insertIndex] = info;
	}
	else
	{
		insertIndex = m_styles.count();
		m_id2index.insert(info.id(), insertIndex);
		m_styles.append(info);
	}

	m_styles[insertIndex].validateIndex(this, insertIndex);
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::hasStyleInfo(const QString& id)const
{
	return m_id2index.contains(id);
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::hasStyleInfo(qint32 index)const
{
	return index >=0 && index < m_styles.count();
}

//------------------------------------------------------------------------------
const DuifwStyleManager::StyleInfo& DuifwStyleManager::findById(const QString& id)const
{
	DW_ASSERT(hasStyleInfo(id));

	qint32 index = findIndex(id);
	return findByIndex(index);
}

//------------------------------------------------------------------------------
const DuifwStyleManager::StyleInfo& DuifwStyleManager::findByIndex(qint32 index)const
{
	DW_ASSERT(index>=0 && index<m_styles.count());
	return m_styles[index];
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::applyStyleInfo(QObject* target, const QString& id)const
{
	DW_ASSERT(target);

	DuifwStyleManager::AttrsMap attrs;
	fillAttrContainer(id, attrs);

	return applyProperty(target, attrs);
}

//------------------------------------------------------------------------------
static QVariant _default_attr_converter(const QString &value)
{
	return value;
}

static QString byPassParser(const QVariant &val)
{
	return val.value<QString>();
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::applyProperty(QObject* target, const QString& attrName, qint32 attrIndex)const
{
	DW_ASSERT(target);

	bool success = true;

	if (!isComplexProperty(attrIndex))
	{
		success = applySimpleProperty(target, attrName, attrIndex);
	}
	else
	{
		success = applyComplexProperty(target, attrName, attrIndex);
	}

	if (!success)
	{
		DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, (attrName + " is not a property of " + target->objectName()).toLatin1().constData());
		LogFinal(LOG::KDuiFw) << (attrName + " is not a property of " + target->objectName()).toLatin1().constData();
	}

	return success;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::applyProperty(QObject* target, const AttrsMap& attrs)const
{
	DW_ASSERT(target);
	if (attrs.count() > 0)
	{
		MapIte attrIte(attrs);
		while (attrIte.hasNext()) {
			attrIte.next();
			applyProperty(target, attrIte.key(), attrIte.value());
		}
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::fillAttrContainer(const QString& id, AttrsMap& attrs)const
{
	qint32 index = findIndex(id);

	return fillAttrContainer(index, attrs);
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::fillAttrContainer(qint32 index, AttrsMap& attrs)const
{
	if (hasStyleInfo(index))
	{
		const StyleInfo& styleInfo = findByIndex(index);

		fillAttrContainer(styleInfo.parentIndex(), attrs);

		const AttrsMap& styleAttrs = styleInfo.attrContainer();
		if (styleAttrs.count() > 0)
		{
			MapIte attrIte(styleAttrs);
			while (attrIte.hasNext()) {
				attrIte.next();
				attrs.insert(attrIte.key(), attrIte.value());
			}
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
qint32 DuifwStyleManager::findIndex(const QString& id)const
{
	if (m_id2index.contains(id))
	{
		return m_id2index[id];
	}
	return InvalidIndex;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::isComplexProperty(qint32 attrIndex)const
{
	const QVariant& attrValue = featchAttrValue(attrIndex);

	if (attrValue.type() == QVariant::String)
	{
		return  attrValue.toString().startsWith("<?xml version=\"1.0\"?>");
	}
	return false;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::applySimpleProperty(QObject* target, const QString& attrName,qint32 attrIndex)const
{
	qint32 idx = target->metaObject()->indexOfProperty(attrName.toLatin1().constData());
	if(-1 != idx)
	{
		QVariant attrValue = featchAttrValue(attrIndex);

		if (attrValue.type() == QVariant::String)
		{
			// only prase once
			QMetaProperty prop = target->metaObject()->property(idx);
			qint32 type_id = prop.type();
			ObjectPropertyMarshal creator;
			if(type_id == QVariant::UserType)
			{
				type_id = prop.userType();
			}
			creator = DuiWidgetPropertyDynamicList::findFunc(prop.userType()); 
			if (!creator.valid())
			{
				ObjectPropertyMarshal tmp(_default_attr_converter, byPassParser);
				creator = tmp;
			}

			attrValue = creator(attrValue.toString());
		}
		// simple property
		if (!target->setProperty(attrName.toLatin1().constData(), attrValue))
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("applyProperty %1 failed.").arg(attrName).toLatin1().constData());
			LogFinal(LOG::KDuiFw) << QString("setProperty %1 failed.").arg(attrName);
			return false;
		}

		return true;  
	}
	return false;
}

//------------------------------------------------------------------------------
bool DuifwStyleManager::applyComplexProperty(QObject* target, const QString& attrName, qint32 attrIndex)const
{
	Q_UNUSED(attrName);

	DWXmlDocument complexDoc;
	complexDoc.loadFromString( featchAttrValue(attrIndex).toString().toStdWString() );
	const DWXmlElement *rootElement = complexDoc.rootElement();
	if (rootElement)
	{
		bool b = DuiWidgetDataDynamicList::createData(rootElement->tag(), target, rootElement);
		if (!b)
		{
			QString qclassName = QStringFromWString(rootElement->tag());

			DW_ASSERT_X(false, __FUNCTION__, (qclassName + " is not valid tag name").toLatin1().constData());
			LogFinal(LOG::KDuiFw) << qclassName << " is not valid tag name";
		}
		return b;
	}

	return false;
}

//------------------------------------------------------------------------------
const qint32 DuifwStyleManager::appendAttrValue(const QVariant& attrValue)
{
	DW_ASSERT(m_attrValues.count() < Max_AttrValues);
	m_attrValues.append(attrValue);
	return (m_attrValues.count() - 1);
}

//------------------------------------------------------------------------------
const bool DuifwStyleManager::insertAttrAtIndex(qint32 index, const QVariant& attrValue)
{
	DW_ASSERT(index>=0 && index<m_attrValues.count());
	m_attrValues[index] = attrValue;
	return true;
}

//------------------------------------------------------------------------------
const QVariant&     DuifwStyleManager::featchAttrValue(qint32 index)const
{
	DW_ASSERT(index>=0 && index<m_attrValues.count());
	return m_attrValues[index];
}

//////////////////////////////////////////////////////////////////////////
Q_GLOBAL_STATIC(DuifwStyleManager, _dui_Get_DuifwStyleManager);

DuifwStyleManager* duiGetStyleManager()
{
	return _dui_Get_DuifwStyleManager();
}

//////////////////////////////////////////////////////////////////////////
bool addStyleData(QObject *parent, const DWXmlElement *xml)
{
	Q_UNUSED(parent);
	return duiGetStyleManager()->loadStyleInfo(xml);
}
DUI_DEFINE_DYNAMIC_DATA(styleData, addStyleData)