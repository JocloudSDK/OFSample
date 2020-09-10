#pragma once
/****************************************************************************
 @class DuifwStyleManager
 
 DuifwStyle : main entry of target's styles, just like css
 
 format 1, like this : TODO
 id : parent { attr1="value1" attr2="value2" }
 
 format 2, like this
 <style id="channelClassCombo"  fixSize="120,38" alignment="AlignHCenter">
     <ComboData>
         <item text="Game"/>
         <item text="Entertainment"/>
         <item text="Education"/>
         <item text="Other"/>
     </ComboData>
 </style>

 TODO:
    done : 1. speed parent lookup , use index replace the string id
    done :2. speed property prase , beforehand the string attr, may use the QVariant
    3. extends the instance
    4. implement the simple style sheet as [ id : { attr = "value"} ]
    5. state extend, used to config target in dynamic
	6. save memery: share the name of attr between the all of styles as the attr name is a const var

  (C) 2011 Duowan.GZ
****************************************************************************/
#include "stable.h"


//------------------------------------------------------------------------------
class DWXmlElement;


//------------------------------------------------------------------------------
class DuifwStyleManager
{
public :
    class StyleInfo;

    typedef QMap<QString, qint32> Id2IndexMap;
    typedef QMap<QString, qint32> AttrsMap;
    typedef QVector<QVariant> AttrValueCon;
	typedef QVector<QString> ConstNameCon;

    typedef QMapIterator<DuifwStyleManager::AttrsMap::key_type, DuifwStyleManager::AttrsMap::mapped_type> MapIte;
    typedef QVector<qint32> IndexsCon;
    typedef QMap<QString, IndexsCon > Style2InstanceMap;
    
    static const qint32 InvalidIndex = -1;
    enum { Max_StyleSheets=10000, 
        Max_AttrValues=100000};

	// style's structure
	class StyleInfo
	{
	public:
        StyleInfo();

        void setId(const QString& id);
        const QString& id()const;
    
        void setParent(const QString& parent);
        const QString& parent()const;

        void            addAttr(const QString& attrName, qint32 atrrIndex);
        const AttrsMap& attrContainer() const;

	private:
        friend class DuifwStyleManager;

        void setIndex(qint32 index);
        qint32 index()const;

        void setParentIndex(qint32 parentIndex);
        qint32 parentIndex()const;

        void validateIndex(const DuifwStyleManager* manager, qint32 index);
      
        QString m_id;
		QString m_parent;
		AttrsMap m_attrs;

        qint32 m_parentIndex;
        qint32 m_index;
	};

    DuifwStyleManager();

    ~DuifwStyleManager();
	
	// load
	bool                loadStyleInfo(const QString& stylesFile);
    bool                loadStyleInfo(const DWXmlElement* styleRoot);
    bool                appendStyleInfo(const DWXmlElement* styleElement);
    bool                praseStyleInfo(const DWXmlElement* styleElement, StyleInfo& info);
    void                insertStyleInfo(const StyleInfo& info);
	
	// find
    bool                hasStyleInfo(const QString& id)const;
    bool                hasStyleInfo(qint32 index)const;
	const StyleInfo&    findById(const QString& id)const;
	const StyleInfo&    findByIndex(qint32 index)const;
	
	// apply
	bool                applyStyleInfo(QObject* target, const QString& id)const;
    bool                applyProperty(QObject* target, const QString& attrName, qint32 attrIndex)const;
    bool                applyProperty(QObject* target, const AttrsMap& attrs)const;

    // fill
    bool                fillAttrContainer(const QString& id, AttrsMap& attrs)const;
    bool                fillAttrContainer(qint32 index, AttrsMap& atttrs)const;
	
private:
    friend class StyleInfo;
    friend class StyleInstance;

    // helper
    qint32              findIndex(const QString& id)const;

    bool                isComplexProperty(qint32 attrIndex)const;
    bool                applySimpleProperty(QObject* target, const QString& attrName,  qint32 attrIndex)const;
    bool                applyComplexProperty(QObject* target, const QString& attrName, qint32 attrIndex)const;

    const qint32        appendAttrValue(const QVariant& attrValue);
    const bool          insertAttrAtIndex(qint32 index, const QVariant& attrValue);
    const QVariant&     featchAttrValue(qint32 index)const;
   
    // styles
	Id2IndexMap m_id2index;
	QVector<StyleInfo> m_styles;

    // attr values
    AttrValueCon m_attrValues;
};

//------------------------------------------------------------------------------
DW_DUIFW_EXPORT DuifwStyleManager* duiGetStyleManager();