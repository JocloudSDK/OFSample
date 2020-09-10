
/****************************************************************************
Author: 贺灏
Email : hehao@chinaduo.com
Mobile: 15018457467
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

class DWXmlDocument;
class DuiWidget;
class DuiFrameWindow;
class DWXmlElement;
class QTranslator;

class QGraphicsItem;

//性能Log代理类，为了使用此头文件的不依赖perflog
struct DW_DUIFW_EXPORT CPerfLogProxy
{
	CPerfLogProxy(const char *szName);
	~CPerfLogProxy();
	unsigned short m_wNameID;
};

typedef enum
{
	DuiNormalMode = 0,
	DuiEditorMode = 1,
	DuiPreviewMode = 2,
}DuiCreateMode;

extern bool          KDuiIsEditorMode;
extern DuiCreateMode KDuiCurrentEditorMode;

//////////////////////////////////////////////////////////////////////////
class DuiXmlUI
{
public:
#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
    static DW_DUIFW_EXPORT void initStyleInfo(const QString &styleFile);
	static DW_DUIFW_EXPORT bool createFromFile(const QString &fileName, DuiWidget *parent);
	static DW_DUIFW_EXPORT bool createFromFile(const QString &fileName, const QString &rootPath, DuiWidget *parent);
	static DW_DUIFW_EXPORT bool createFromFile(const QString &fileName, const QString &rootPath, DuiWidget *parent, QTranslator *t);
	static DW_DUIFW_EXPORT bool createFromMemory(const std::string& data, DuiWidget *parent);
    static DW_DUIFW_EXPORT bool createFromMemory(const std::wstring& data, DuiWidget *parent);
    static DW_DUIFW_EXPORT DuiFrameWindow* createFromFile(const QString& fileName, QWidget *parent = NULL);
    static DW_DUIFW_EXPORT DuiFrameWindow* createFromFile(const QString& fileName, const QString &rootPath, QWidget *parent = NULL);
    static DW_DUIFW_EXPORT DuiFrameWindow* createFromFile(const QString& fileName, const QString &rootPath, QWidget *parent, QTranslator *t);
	static DW_DUIFW_EXPORT DuiFrameWindow* createFromMemory(const std::string& data, QWidget *parent = NULL);
    static DW_DUIFW_EXPORT DuiFrameWindow* createFromMemory(const std::wstring& data, QWidget *parent = NULL);
	static DW_DUIFW_EXPORT void connectSignalSlotByName(QObject *from, QObject *to, bool includeFrom = true);	//includeFrom=true表示from中signal也需要连接

    static DW_DUIFW_EXPORT bool CreateFromXml(const DWXmlDocument& xml, DuiWidget *parent, DuiCreateMode mode = DuiNormalMode);
    static DW_DUIFW_EXPORT bool CreateFromXml(const DWXmlDocument& xml, DuiWidget *parent, DuiCreateMode mode, QTranslator *t);
	static DW_DUIFW_EXPORT DuiFrameWindow* CreateFromXml(const DWXmlDocument& xml, QWidget *parent, DuiCreateMode mode = DuiNormalMode);
    static DW_DUIFW_EXPORT DuiFrameWindow* CreateFromXml(const DWXmlDocument& xml, const QString& rootPath, QWidget *parent, DuiCreateMode mode = DuiNormalMode);
    static DW_DUIFW_EXPORT DuiFrameWindow* CreateFromXml(const DWXmlDocument& xml, const QString& rootPath, QWidget *parent, DuiCreateMode mode, QTranslator *t);
	static DW_DUIFW_EXPORT void constructWidget(DuiWidget *w);
#endif

	// for duieditor
	static DW_DUIFW_EXPORT DuiWidget *createWidget(const QString &className, QGraphicsItem *parent);

	static DW_DUIFW_EXPORT bool     isDynamicDataTag(const QString &xmlTag);
	static DW_DUIFW_EXPORT bool     isMarshalSupport(int type);
	static DW_DUIFW_EXPORT QVariant propertyMarshal(int type, const QString &value);
	static DW_DUIFW_EXPORT QString  propertyMarshal(int type, const QVariant &value);
	static DW_DUIFW_EXPORT QString  property2string(const QMetaProperty &prop, const QObject *obj);
	static DW_DUIFW_EXPORT int      flagsTypeId();
	static DW_DUIFW_EXPORT int      enumTypeId();
	static DW_DUIFW_EXPORT int      painterPathTypeId();
	static DW_DUIFW_EXPORT void     initDuiEditorEnv();
	static DW_DUIFW_EXPORT void     setCurrentEditorMode(DuiCreateMode mode);

    // i18n support
    static DW_DUIFW_EXPORT QString tr(const QString &value);

public:
	class AutoTranslatorSetting
	{
	public:
		AutoTranslatorSetting(QTranslator *t);
		~AutoTranslatorSetting();
	};

private:
	static bool createChild(const DWXmlElement *child, DuiWidget *root, DuiCreateMode mode = DuiNormalMode);

	static bool getColor(const QString& value, QColor& clr);
};

//辅助的Singlton
//////////////////////////////////////////////////////////////////////////
template<typename TYPE>
class DuiHelperStaticGlobalHandle
{
public:
	typedef TYPE	VALUE_TYPE;

	static TYPE* instance()
	{
		static QGlobalStatic<TYPE >  instance = { Q_BASIC_ATOMIC_INITIALIZER(0), false };
		if (!instance.pointer && !instance.destroyed) 
		{          
			TYPE *x = new TYPE;                                        
			if (!instance.pointer.testAndSetOrdered(0, x))          
				delete x;                                              

			static QGlobalStaticDeleter<TYPE > cleanup(instance); 
		}                                                          
		return instance.pointer; 
	}
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//用于widget系列的动态创建
typedef DuiWidget* (* pfnWidgetCreateFuncPointer)(QGraphicsItem *parent);
//用于frame系列的动态创建
typedef DuiFrameWindow* (* pfnFrameCreateFuncPointer)(QWidget *parent);
//用于处理一类名字叫data的列表属性 ，如果Combox的下拉列表
typedef bool (* pfnObjectDataCreator)(QObject *parent, const DWXmlElement *xml);
//用于处理Widget的属性扩展，从一个string 解析到各种属性类型
typedef QVariant (* pfnObjectPropertyCreator)(const QString &value);
//用于处理Widget的属性序列化 从 属性容器中解析处字符串
typedef QString (* pfnObjectPropertyParser)(const QVariant &value);

//////////////////////////////////////////////////////////////////////////
// use functor to extend "objectPropertyCreator"
// append parser, use to convert property to string
class ObjectPropertyMarshal
{
public:
	//ObjectPropertyMarshal() : _creator(0), _parser(0) { ; }
	ObjectPropertyMarshal(pfnObjectPropertyCreator creator=NULL, pfnObjectPropertyParser parser=NULL) 
		: _creator(creator), _parser(parser) { ; }
	QVariant operator()(const QString  &val) { return _creator(val); }
	QString  operator()(const QVariant &val) { return _parser(val); }
	bool operator==(pfnObjectPropertyCreator create) {return create == _creator;}
	bool operator!=(pfnObjectPropertyCreator create) { return !(operator==(create));}
	bool valid() { return _creator && _parser; }
protected:
private:
	pfnObjectPropertyParser _parser;
	pfnObjectPropertyCreator _creator;
};

//元信息节点类型：一个类型名字，函数地址
//////////////////////////////////////////////////////////////////////////
template <typename FUNCTYPE, typename KEYTYPE=QString>
class DuiDynamicCreateDataNode
{
public:
	typedef FUNCTYPE FUNC_TYPE;
    typedef KEYTYPE KEY_TYPE;

    DuiDynamicCreateDataNode(KEYTYPE name = KEYTYPE(), FUNCTYPE pfnFunc = NULL)
        :className(name), createFunc(pfnFunc){}

	KEYTYPE className;
	FUNCTYPE createFunc;
};

//获取FUNCTYPE对应的元信息列表的头节点
//////////////////////////////////////////////////////////////////////////
template <typename FUNCTYPE, typename KEYTYPE=QString>
class MetaFuncWrapper
{
public:
	typedef FUNCTYPE	FUNC_TYPE;
    typedef KEYTYPE     KEY_TYPE;
	typedef DuiDynamicCreateDataNode<FUNCTYPE, KEY_TYPE> NODE_TYPE;
    typedef QHash<typename NODE_TYPE::KEY_TYPE, NODE_TYPE*> NODE_HASH_MAP;
	
    static NODE_HASH_MAP* Handlers()
	{
		return DuiHelperStaticGlobalHandle<NODE_HASH_MAP>::instance();
	}
};

//定义元信息列表,
//TODO: 应该把 CreateData....CreateObject.... 这类东西分解出来，需要对功能解耦
//////////////////////////////////////////////////////////////////////////
template <typename FUNCTYPE, typename PARENTTYPE, typename OBJECTTYPE, typename KEYTYPE=QString>
class DuiDynamicList
{
public:
	//元信息类型
	typedef FUNCTYPE FUNC_TYPE;
    typedef KEYTYPE KEY_TYPE;
	typedef PARENTTYPE PARENT_TYPE;
	typedef OBJECTTYPE OBJECT_TYPE;
	typedef MetaFuncWrapper<FUNC_TYPE, KEY_TYPE> META_TYPE;
	typedef typename META_TYPE::NODE_TYPE NODE_TYPE;

	static void addToDynamicList(NODE_TYPE *data)
	{
#ifdef _DEBUG
		FUNCTYPE pfnFunc = findFunc(data->className);
		DW_ASSERT_X(pfnFunc==NULL, __FUNCTION__, QString("duplicated class name %1.").arg(data->className).toLatin1().constData());
#endif

        META_TYPE::NODE_HASH_MAP* hashHandlers = META_TYPE::Handlers();
        hashHandlers->insert(data->className, data);
	}

	static void removeFromDynamicList(NODE_TYPE *data)
	{
        META_TYPE::NODE_HASH_MAP* hashHandlers = META_TYPE::Handlers();
        hashHandlers->remove(data->className);
	}

    static FUNC_TYPE findFunc(const typename META_TYPE::KEY_TYPE& findClassName)
	{
        META_TYPE::NODE_HASH_MAP* hashHandlers = META_TYPE::Handlers();
        META_TYPE::NODE_HASH_MAP::const_iterator nodeIte = hashHandlers->find(findClassName);
        if (nodeIte == hashHandlers->constEnd())
        {
            return FUNC_TYPE(NULL);
        }
        return (*nodeIte)->createFunc;
	}

	static bool hasClassName(const typename META_TYPE::KEY_TYPE& findClassName)
	{
		META_TYPE::NODE_HASH_MAP* hashHandlers = META_TYPE::Handlers();
		META_TYPE::NODE_HASH_MAP::const_iterator nodeIte = hashHandlers->find(findClassName);
		if (nodeIte == hashHandlers->constEnd())
		{
			return false;
		}
		return true;
	}

	static OBJECT_TYPE* createObject(const typename META_TYPE::KEY_TYPE& findClassName, PARENTTYPE *parent)
	{
		CPerfLogProxy perflogProxy(__FUNCTION__);
		OBJECT_TYPE *obj = NULL;
		NODE_TYPE::FUNC_TYPE pfnCreate = findFunc(findClassName);

		if (pfnCreate)
		{
			obj = (*pfnCreate)(parent);
		}

		return obj;
	}

	//SFINAE(Substitution Failure Is Not An Error) do the trick! 这里会由一个转换，从 createFunc.valtype -- val
	template<typename VALUETYPE>
	static bool createData(const std::wstring &dataType, PARENTTYPE *parent, const VALUETYPE& val)
	{
		NODE_TYPE::FUNC_TYPE pfnCreate = findFunc(QStringFromWString(dataType));
		
		if (pfnCreate)
		{
			return (*pfnCreate)(parent, val);
		}
		
		return false;
	}
};

//辅助类型，用来注册节点信息到元信息列表
//////////////////////////////////////////////////////////////////////////
template <typename FUNCTYPE, typename PARENTTYPE, typename OBJECTTYPE, typename KEYTYPE=QString>
class DuiTypeRegister
{
public:
	typedef	FUNCTYPE FUNC_TYPE;
    typedef KEYTYPE KEY_TYPE;
	typedef PARENTTYPE	PARENT_TYPE;
	typedef	OBJECTTYPE	OBJECT_TYPE;
	typedef DuiDynamicCreateDataNode<FUNC_TYPE, KEY_TYPE> NODE_TYPE;
	typedef DuiDynamicList<FUNC_TYPE, PARENT_TYPE, OBJECT_TYPE, KEY_TYPE> NODE_LIST_TYPE;
	
	DuiTypeRegister(NODE_TYPE *data)
	{
		NODE_LIST_TYPE::addToDynamicList(data);
		data_ = data;
	}

	~DuiTypeRegister() {
		NODE_LIST_TYPE::removeFromDynamicList(data_);
		delete data_;
	}
private:
	NODE_TYPE *data_;
};

//下面的宏，用来填充控件的原信息
//////////////////////////////////////////////////////////////////////////
#define DUI_DECLARE_DYNAMIC_CREATE_SELF(classname,itemtype,parenttype,functype)	\
public:\
	static itemtype* dynamicCreateObject##classname(parenttype* parent)	\
	{	\
		CPerfLogProxy perflogProxy(__FUNCTION__);	\
		classname* p = new classname(parent);	\
		return static_cast<itemtype*>(p);\
	}	\
private:\
	static DuiTypeRegister<functype,parenttype,itemtype> s_register##classname;

#define DUI_DECLARE_DYNAMIC_CREATE_SELF2(classname,itemtype,parenttype,functype)	\
public:\
    static itemtype* dynamicCreateObject2##classname(parenttype* parent)	\
{	\
    CPerfLogProxy perflogProxy(__FUNCTION__);	\
    classname* p = new classname(parent);	\
    return static_cast<itemtype*>(p);\
}	\
private:\
    static DuiTypeRegister<functype,parenttype,itemtype> s_register2##classname;

#define DUI_IMPLEMENT_DYNAMIC_CREATE_SELF(classname,itemtype,parenttype,functype)	\
	DuiTypeRegister<functype, parenttype, itemtype> classname::s_register##classname( \
			new DuiDynamicCreateDataNode<functype>(#classname,classname::dynamicCreateObject##classname));


//支持widget系列的动态创建宏
#define DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(classname) \
	DUI_DECLARE_DYNAMIC_CREATE_SELF(classname, DuiWidget, QGraphicsItem, pfnWidgetCreateFuncPointer)
	
#define DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(classname)	\
    DWCHECKSIZEOF(classname, 28)	\
    DUI_IMPLEMENT_DYNAMIC_CREATE_SELF(classname, DuiWidget, QGraphicsItem, pfnWidgetCreateFuncPointer)


//支持XML标签和类名分开定义
#define DUI_IMPLEMENT_DYNAMIC_CREATE_SELF2(classname,classKey,itemtype,parenttype,functype)	\
    DuiTypeRegister<functype, parenttype, itemtype> classname::s_register2##classname( \
    new DuiDynamicCreateDataNode<functype>(#classKey,classname::dynamicCreateObject2##classname));

#define DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF2(classname) \
    DUI_DECLARE_DYNAMIC_CREATE_SELF2(classname, DuiWidget, QGraphicsItem, pfnWidgetCreateFuncPointer)

#define DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF2(classname,classKey)	\
	DWCHECKSIZEOF(classname, 28)	\
	DUI_IMPLEMENT_DYNAMIC_CREATE_SELF2(classname,classKey, DuiWidget, QGraphicsItem, pfnWidgetCreateFuncPointer)

//支持frame系列的动态创建宏
#define DUI_DECLARE_DYNAMIC_FRAME_CREATE_SELF(classname) \
	DUI_DECLARE_DYNAMIC_CREATE_SELF(classname, DuiFrameWindow, QWidget, pfnFrameCreateFuncPointer)

#define DUI_IMPLEMENT_DYNAMIC_FRAME_CREATE_SELF(classname)	\
	DWCHECKSIZEOF(classname, 24)	\
	DUI_IMPLEMENT_DYNAMIC_CREATE_SELF(classname, DuiFrameWindow, QWidget, pfnFrameCreateFuncPointer)

//下面的宏用来扩展属性类型
//////////////////////////////////////////////////////////////////////////
//属性系统：支持复杂属性配置，在xml中，作为子节点出现
#define DUI_DEFINE_DYNAMIC_DATA(dataname, creator)	\
	namespace { \
	DuiTypeRegister<pfnObjectDataCreator, QObject, void> data_register_##dataname( \
	new DuiDynamicCreateDataNode<pfnObjectDataCreator>(#dataname, creator)); }

//注册属性解析接口 : Property类型的属性，调用 pfnFunc 指定的接口进行解析
#define DUI_DEFINE_PROPERTY_METATYPE(PropertyType, pfnFunc) \
    namespace { \
    DuiTypeRegister<ObjectPropertyMarshal, QObject, void, int> data_register_##PropertyType( \
    new DuiDynamicCreateDataNode<ObjectPropertyMarshal, int>(qRegisterMetaType<PropertyType>(#PropertyType), pfnFunc)); }

//扩展的属性类型
//////////////////////////////////////////////////////////////////////////
//锚点类型
struct AnchorInfo
{
    Qt::AnchorPoint anchorPoint[4];
    int distance[4];

    AnchorInfo() 
    {
        Qt::AnchorPoint dummy[] = {
            Qt::AnchorLeft, Qt::AnchorRight, 
            Qt::AnchorTop, Qt::AnchorBottom};
            for (int i = 0; i < DW_ARRAY_SIZE(distance); ++i)
            {
                distance[i] = 0;
                anchorPoint[i] = dummy[i];
            }
    }
};

struct AnchorInfo2
{
	Qt::AnchorPoint point[4];
	Qt::AnchorPoint anchorPoint[4];
	QString objectName[4];
	int distance[4];
	int anchorNum;

	AnchorInfo2()
	{
		anchorNum = 0;
	}

    AnchorInfo2(const AnchorInfo &data)
    {
        anchorNum = 4;
        point[0] = Qt::AnchorLeft;
        point[1] = Qt::AnchorRight;
        point[2] = Qt::AnchorTop;
        point[3] = Qt::AnchorBottom;
        for (int i = 0; i < 4; ++ i)
        {
            anchorPoint[i] = data.anchorPoint[i];
            distance[i] = data.distance[i];
        }
    }
};

//用来翻译xml中配置的字符串, 如果xml配置的字段需要翻译，那么字段对应的属性类型必须是 I18nString
class I18nString : public QString
{
	//alias via inheritance, keep this empty FOREVER
public:
	I18nString() {}
	I18nString(const QString &rhs)
		:QString(rhs)
	{
	}
};
Q_DECLARE_METATYPE(I18nString);

//导出实例化的目标类
//因为DuiDynamicList 使用了SFINAE, 所以不能直接实例化列表类
//////////////////////////////////////////////////////////////////////////
template class DW_DUIFW_EXPORT MetaFuncWrapper<pfnWidgetCreateFuncPointer>;
template class DW_DUIFW_EXPORT MetaFuncWrapper<pfnFrameCreateFuncPointer>;
template class DW_DUIFW_EXPORT MetaFuncWrapper<pfnObjectDataCreator>;
template class DW_DUIFW_EXPORT MetaFuncWrapper<ObjectPropertyMarshal, int>;

//声明三个列表
//////////////////////////////////////////////////////////////////////////
typedef DuiDynamicList<pfnWidgetCreateFuncPointer, QGraphicsItem, DuiWidget> DuiWidgetDynamicList;	//保存Widget系列的元信息
typedef DuiDynamicList<pfnFrameCreateFuncPointer, QWidget, DuiFrameWindow> DuiFrameDynamicList;		//保存Frame系列的元信息
typedef DuiDynamicList<pfnObjectDataCreator, QObject , void> DuiWidgetDataDynamicList;				//保存创建子属性的元信息
typedef DuiDynamicList<ObjectPropertyMarshal, QObject, void, int> DuiWidgetPropertyDynamicList;  //保存创建属性的元信息
