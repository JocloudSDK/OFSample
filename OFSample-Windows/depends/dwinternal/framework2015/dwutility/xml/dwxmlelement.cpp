#include "stable.h"

#include "dwxmlelement.h"
#include "dwutility.h"

#include "pugixml/pugixml.hpp"
using namespace pugi;
#pragma warning(disable:4355)//关闭在构造函数是使用this的警告

//////////////////////////////////////////////////////////////////////////

class DWXmlElementPrivate
{
public:
	DWXmlElementPrivate(xml_node _xmlNode):xmlNode(_xmlNode),bManagered(true){}
	DWXmlElementPrivate(const std::wstring &strName,DWXmlElement *pEle):bManagered(false)
	{
		xmlNode=xmlDoc.append_child(strName.c_str());
		DWXmlElement *pEle2=(DWXmlElement *)xmlNode.get_user_data();
		delete pEle2;
		xmlNode.set_user_data(pEle);
	}

	~DWXmlElementPrivate(){}

	xml_node	xmlNode;
	xml_document xmlDoc;
	std::vector<DWXmlElement *> m_childs;	//子节点指针列表
	std::vector<DWXmlAttribute> m_attrs;	//节点属性列表,用于原有接口提供属性的随机索引
	bool		bManagered;					//结点是由调用都创建的。
};

class DWXmlAttributePrivate
{
public:
	DWXmlAttributePrivate(xml_attribute attr):xmlAttr(attr)
	{

	}

	DWXmlAttributePrivate(){}

	xml_attribute xmlAttr;
};

class CXmlNodeHook : public xml_node_hook
{
public:
	CXmlNodeHook()
	{
	}

	void onNodeCreate(xml_node node)
	{
		DWXmlElement *pEle=new DWXmlElement(node);
		node.set_user_data(pEle);
		xml_node parent=node.parent();
		if(parent && (parent.type() == node_element))
		{
			DWXmlElement *pParent=(DWXmlElement*)parent.get_user_data();
			pParent->d_ptr->m_childs.push_back(pEle);
		}
	}

	void onNodeDestory(xml_node node)
	{
		DWXmlElement *pEle = (DWXmlElement *)node.get_user_data();
		xml_node parent=node.parent();
		if(parent && (parent.type() == node_element))
		{
			DWXmlElement *pParent=(DWXmlElement*)parent.get_user_data();
			std::vector<DWXmlElement*>::iterator it=pParent->d_ptr->m_childs.begin();
			while(it!=pParent->d_ptr->m_childs.end())
			{
				if(*it == pEle)
				{
					pParent->d_ptr->m_childs.erase(it);
					break;
				}
				it++;
			}
		}
		if(pEle->d_ptr->bManagered) 
			delete pEle;//内部分配的节点才能自己删除
	}

	virtual void onFreeUserData(xml_node node)	//document destroy释放时调用
	{
		DWXmlElement *pEle = (DWXmlElement *)node.get_user_data();
		if(pEle && pEle->d_ptr->bManagered) delete pEle;
	}

	void onAttrCreate(xml_node node,xml_attribute attr)
	{
		DWXmlElement *pEle = (DWXmlElement *)node.get_user_data();
		pEle->d_ptr->m_attrs.push_back(DWXmlAttribute(attr));
	}

	void onAttrDestroy(xml_node node,xml_attribute attr)
	{
		DWXmlElement *pEle = (DWXmlElement *)node.get_user_data();
		std::vector<DWXmlAttribute>::iterator it=pEle->d_ptr->m_attrs.begin();

		while(it!=pEle->d_ptr->m_attrs.end())
		{
			if(it->d_ptr->xmlAttr == attr)
			{
				pEle->d_ptr->m_attrs.erase(it);
				break;
			}
			it++;
		}
	}

}xmlHook;

namespace pugi
{
	xml_node_hook * g_pNodeHook=&xmlHook;
}

//////////////////////////////////////////////////////////////////////////
DWXmlElement::DWXmlElement(const pugi::xml_node &xmlNode ): d_ptr(new DWXmlElementPrivate(xmlNode))
{
}

DWXmlElement::DWXmlElement( const std::wstring &name ) : d_ptr(new DWXmlElementPrivate(name,this))
{

}

DWXmlElement::~DWXmlElement()
{
	DW_SAFE_DELETE(d_ptr);
}

std::wstring DWXmlElement::tag() const
{
	Q_D(DWXmlElement);
	return d->xmlNode.name();
}

std::wstring DWXmlElement::text() const
{
	Q_D(DWXmlElement);
	return d->xmlNode.text().get();
}

void DWXmlElement::setText( const std::wstring& text )
{
	Q_D(DWXmlElement);
	d->xmlNode.text().set(text.c_str());
}

int DWXmlElement::childCount() const
{
	Q_D(DWXmlElement);
	return d->m_childs.size();
}

const DWXmlElement* DWXmlElement::child( int index ) const
{
	Q_D(DWXmlElement);
	if(index<0 || index>=(int)d->m_childs.size())
		return NULL;
	return d->m_childs[index];
}

DWXmlElement * DWXmlElement::addChild(const std::wstring & str)
{
	Q_D(DWXmlElement);
	xml_node added=d->xmlNode.append_child(str.c_str());
	if(added)
	{
		return (DWXmlElement *)added.get_user_data();
	}else
	{
		return NULL;
	}
}

//
void DWXmlElement::addChild( DWXmlElement *child )
{
	Q_D(DWXmlElement);
	DW_ASSERT(child != NULL);
	xml_node source = child->d_ptr->xmlNode;
	xml_node dest=d->xmlNode.append_child(source.name());
	DW_ASSERT(!!dest);

	DWXmlElement * pXmlDest = (DWXmlElement *)dest.get_user_data();
	d->m_childs.pop_back();
	d->m_childs.push_back(child);

	//copy value
	dest.text().set(source.text().get());

	//copy attributes
	for (xml_attribute a = source.first_attribute(); a; a = a.next_attribute())
		dest.append_attribute(a.name()).set_value(a.value());

	dest.set_user_data(child);
	child->d_ptr->xmlNode = dest;
	child->d_ptr->m_attrs = pXmlDest->d_ptr->m_attrs;
	child->d_ptr->bManagered = true;//将结点的属性设置为接受doc管理

	delete pXmlDest;
	source.set_user_data(NULL);

	//深度优先递归插入新child树上的结点
	std::vector<DWXmlElement*> childs=child->d_ptr->m_childs;
	child->d_ptr->m_childs.clear();
	for(UINT i=0;i<childs.size();i++)
	{
		DWXmlElement *  p=childs[i];
		DW_ASSERT(p!=NULL);
		child->addChild(p);
	}
	child->d_ptr->xmlDoc.reset();
}

const wchar_t* DWXmlElement::attribute2(const wchar_t* attrName) const
{
	Q_D(const DWXmlElement);
	return d->xmlNode.attribute(attrName).value();
}

std::wstring DWXmlElement::attribute( const std::wstring &attrName ) const
{
	Q_D(const DWXmlElement);
	return d->xmlNode.attribute(attrName.c_str()).value();
}

std::wstring DWXmlElement::attribute( int index ) const
{
	
	Q_D(const DWXmlElement);
	if(index<0 || index>= (int)d->m_attrs.size()) return L"";
	return d->m_attrs[index].value();
}

std::wstring DWXmlElement::attributeName( int index ) const
{
	Q_D(const DWXmlElement);
	if(index<0 || index>= (int)d->m_attrs.size()) return L"";
	return d->m_attrs[index].name();
}

void DWXmlElement::setAttribute( const std::wstring &attrName, const std::wstring &attrValue )
{
	Q_D(DWXmlElement);
	xml_attribute attr=d->xmlNode.attribute(attrName.c_str());
	if(!attr) attr=d->xmlNode.append_attribute(attrName.c_str());
	attr.set_value(attrValue.c_str());
}

void DWXmlElement::removeAttribute(const std::wstring &attrName)
{
	
	Q_D(DWXmlElement);
	d->xmlNode.remove_attribute(attrName.c_str());
}

DWXmlElement * DWXmlElement::elementByName( const std::wstring &name ) const
{
	
	Q_D(const DWXmlElement);
	return (DWXmlElement*)(d->xmlNode.child(name.c_str()).get_user_data());
}

std::wstring DWXmlElement::operator[]( const std::wstring &attrName ) const
{
	return attribute(attrName);
}

const DWXmlElement* DWXmlElement::operator[]( int index ) const
{
	return child(index);
}

int DWXmlElement::attributeCount() const
{
	
	Q_D(DWXmlElement);
	return d->m_attrs.size();
}

void DWXmlElement::deleteChild(const DWXmlElement *child)
{
	
	Q_D(DWXmlElement);
	if(!child) return;
	xml_node node=child->d_ptr->xmlNode;
	d->xmlNode.remove_child(node);
}

bool DWXmlElement::isEmpty() const
{
	Q_D(DWXmlElement);
	return d->xmlNode?false:true;
}


class DWXmlNodeWalker: public xml_tree_walker
{
public:
	DWXmlNodeWalker(DWXmlElement::PCB_XMLTRAVERSE _fnCB,void * _pCtx):fnCb(_fnCB),pCtx(_pCtx)
	{
	}
protected:
	virtual bool for_each(xml_node& node){
		if(!fnCb) return false;
		return fnCb((DWXmlElement *)(node.get_user_data()),pCtx);
	}

	DWXmlElement::PCB_XMLTRAVERSE fnCb;
	void	*pCtx;
};


void DWXmlElement::traverse( PCB_XMLTRAVERSE fnTraverse,void *pCtx )
{
	Q_D(DWXmlElement);
	DWXmlNodeWalker walker(fnTraverse,pCtx);
	d->xmlNode.traverse(walker);
}

bool DWXmlElement::saveToStream( std::wstring &xmlStream ) const
{
	Q_D(DWXmlElement);
	xmlStream.clear();
	xml_writer_string write(xmlStream);
	xmlStream=L"<?xml version=\"1.0\"?>\n";	//强制加入XML声明，保持与老版本兼容
	d->xmlNode.print(write,L"\t",format_default|format_no_declaration,encoding_utf16);
	return true;
}

DWXmlAttribute DWXmlElement::attr( const std::wstring &attrName ) const
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.attribute(attrName.c_str()));	
}

DWXmlAttribute DWXmlElement::firstAttr() const
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.first_attribute());
}

DWXmlAttribute DWXmlElement::lastAttr() const
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.last_attribute());
}

DWXmlAttribute DWXmlElement::appendAttr( const std::wstring & name )
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.append_attribute(name.c_str()));
}

DWXmlAttribute DWXmlElement::prependAttr( const std::wstring & name )
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.prepend_attribute(name.c_str()));
}

DWXmlAttribute DWXmlElement::insertAttrAfter( const std::wstring & name, const DWXmlAttribute& attr )
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.insert_attribute_after(name.c_str(),attr.d_ptr->xmlAttr));
}

DWXmlAttribute DWXmlElement::insertAttrBefore( const std::wstring & name, const DWXmlAttribute& attr )
{
	Q_D(DWXmlElement);
	return DWXmlAttribute(d->xmlNode.insert_attribute_before(name.c_str(),attr.d_ptr->xmlAttr));
}

//////////////////////////////////////////////////////////////////////////

DWXmlAttribute::DWXmlAttribute():d_ptr(new DWXmlAttributePrivate)
{

}

DWXmlAttribute::DWXmlAttribute( pugi::xml_attribute attr ):d_ptr(new DWXmlAttributePrivate(attr))
{

}

DWXmlAttribute::DWXmlAttribute( const DWXmlAttribute & src ):d_ptr(new DWXmlAttributePrivate(src.d_ptr->xmlAttr))
{
	
}

DWXmlAttribute::~DWXmlAttribute()
{
	delete d_ptr;
}

bool DWXmlAttribute::isEmpty() const
{
	Q_D(DWXmlAttribute);
	return d->xmlAttr.empty();
}

const std::wstring DWXmlAttribute::name() const
{
	Q_D(DWXmlAttribute);
	return d->xmlAttr.name();
}

const std::wstring DWXmlAttribute::value() const
{
	Q_D(DWXmlAttribute);
	return d->xmlAttr.value();
}

DWXmlAttribute DWXmlAttribute::nextAttribute() const
{
	Q_D(DWXmlAttribute);
	return 	DWXmlAttribute(d->xmlAttr.next_attribute());
}

DWXmlAttribute DWXmlAttribute::prevAttribute() const
{
	Q_D(DWXmlAttribute);
	return 	DWXmlAttribute(d->xmlAttr.previous_attribute());
}

void DWXmlAttribute::set( std::wstring value )
{
	Q_D(DWXmlAttribute);
	d->xmlAttr.set_value(value.c_str());
}

DWXmlAttribute& DWXmlAttribute::operator=( const DWXmlAttribute & src )
{
	d_ptr->xmlAttr=src.d_ptr->xmlAttr;
	return *this;
}
