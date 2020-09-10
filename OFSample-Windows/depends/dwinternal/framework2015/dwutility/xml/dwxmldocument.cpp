#include "stable.h"

#include "dwutility/ustring.h"

#include "dwxmldocument.h"
#include "dwxmlelement.h"
#include "pugixml/pugixml.hpp"
using namespace pugi;


class NodeFilterWalker : public xml_tree_walker
{
public:
	NodeFilterWalker(DWXmlDocument::PFN_XMLFILTER pFn,void *pCtx):m_pFnFilter(pFn),m_pCtx(pCtx)
	{

	}
	virtual bool for_each(xml_node& node)
	{
		DW_ASSERT(m_pFnFilter != NULL);
		if(m_pFnFilter(DWXmlDocument::DWXmlElement_tag,node.name(),NULL,m_pCtx)) return false;
		xml_attribute attr=node.first_attribute();
		while(attr)
		{
			if(m_pFnFilter(DWXmlDocument::DWXmlElement_attr,attr.name(),attr.value(),m_pCtx)) return false;
			attr=attr.next_attribute();
		}
		return true;
	}
private:
	DWXmlDocument::PFN_XMLFILTER m_pFnFilter;
	void	*     m_pCtx;
};


DWXmlDocument::DWXmlDocument()
: m_pXmlDoc(new xml_document),m_filterContext(0),m_pFnFilter(0)
{
}


DWXmlDocument::~DWXmlDocument(void)
{
	delete m_pXmlDoc;
}


void DWXmlDocument::doNodeTraverse()
{
	if(m_pFnFilter)
	{
		NodeFilterWalker walker(m_pFnFilter,m_filterContext);
		m_pXmlDoc->traverse(walker);
	}
}

bool DWXmlDocument::loadFromFile( const std::wstring &xmlFile )
{
	bool bret = m_pXmlDoc->load_file(xmlFile.c_str());
	if(!bret || !m_pXmlDoc->first_child()) return false;
	doNodeTraverse();
	return true;
}

bool DWXmlDocument::loadFromString( const std::wstring &xmlString )
{
	//bool bret =  m_pXmlDoc->load(xmlString.c_str());
	bool bret = m_pXmlDoc->load_buffer(xmlString.c_str(), xmlString.length() * sizeof(char_t), parse_default, encoding_wchar);
	if(!bret || !m_pXmlDoc->first_child()) return false;
	doNodeTraverse();
	return true;
}

bool DWXmlDocument::loadFromString( const char *content, int lenght )
{
	if (isUtf8(content, lenght))
	{
		std::wstring buf;
		DwUtility::text::toWideString(content + 3, lenght - 3, buf, CP_UTF8);
		return loadFromString(buf);
	}
	if (isUtf16(content, lenght))
	{
		if (1 == (lenght & 1))
		{
			_ASSERT(false);
			return false;
		}
		std::wstring stream;
		stream.assign((wchar_t*)(content + 2), (lenght - 2) / 2);
		return loadFromString(stream);
	}
	std::wstring buf;
	DwUtility::text::toWideString(content, lenght, buf, CP_UTF8);
	return loadFromString(buf);
}


const DWXmlElement* DWXmlDocument::elementByPath( const std::wstring &elementPath ) const
{
	xml_node xmlNode= m_pXmlDoc->first_element_by_path(elementPath.c_str());
	if(!xmlNode) return NULL;
	return (DWXmlElement *)xmlNode.get_user_data();
}

DWXmlElement * DWXmlDocument::createElementByPath( const std::wstring &elementPath )
{
	std::vector<std::wstring> vectorElement;
	DwUtility::text::splitWString(elementPath, L"/", vectorElement);

	if (vectorElement.size() < 1)
	{
		//Path must have root,exmaple: root/subPath/thirdPath
		DW_ASSERT(false);
		return NULL;
	}

	xml_node xmlNode=m_pXmlDoc->first_child();

	if (xmlNode.name() != vectorElement[0])
	{
		if(xmlNode)
		{
			//Xml only one root. Please use proper root.
			//you must do with this assert, It's very dangerous!!!!!
			DW_ASSERT(false);
			m_pXmlDoc->reset();
		}
		xml_node root=m_pXmlDoc->append_child(vectorElement[0].c_str());
	}
	vectorElement.erase(vectorElement.begin());

	xmlNode=m_pXmlDoc->first_child();

	if(xmlNode)
	{
		DWXmlElement *currentElement = (DWXmlElement *)xmlNode.get_user_data();

		for (std::vector<std::wstring>::iterator iter = vectorElement.begin();
			vectorElement.end() != iter;
			++iter)
		{
			DWXmlElement *tempElement = currentElement->elementByName(*iter);
			if (!tempElement)
			{
				tempElement = currentElement->addChild(*iter);
			}
			currentElement = tempElement;
		}
		return currentElement;
	}else
	{
		_ASSERT(false);
		return NULL;
	}
}

std::wstring DWXmlDocument::query( const std::wstring &valuePath ) const
{
	std::vector<std::wstring> vectorElement;
	DwUtility::text::splitWString(valuePath, L":", vectorElement);
	if (vectorElement.empty())
	{
		return std::wstring();
	}

	const DWXmlElement *currentElement = elementByPath(vectorElement[0]);
	if (!currentElement)
	{
		return std::wstring();
	}

	if (vectorElement.size() > 1)
	{
		return currentElement->attribute(vectorElement[1]);
	}
	else
	{
		return currentElement->text();
	}
}

bool DWXmlDocument::isUtf8( const char *content, int lenght )
{
	if (lenght > 3)
	{
		return 0xef == content[0] && 0xbb == content[1] && 0xbf == content[2];
	}
	return false;
}

bool DWXmlDocument::isUtf16( const char *content, int lenght )
{
	if (lenght > 2)
	{
		return 0xff == content[0] && 0xfe == content[1];
	}
	return false;
}

const DWXmlElement * DWXmlDocument::rootElement() const
{
	return (DWXmlElement*)(m_pXmlDoc->first_child().get_user_data());
}

bool DWXmlDocument::setValue( const std::wstring &valuePath, const std::wstring &value )
{
	std::vector<std::wstring> vectorElement;
	DwUtility::text::splitWString(valuePath, L":", vectorElement);

	if (vectorElement.empty())
	{
		return false;
	}

	DWXmlElement *currentElement = createElementByPath(vectorElement[0]);
	if (!currentElement)
	{
		return false;
	}

	if (vectorElement.size() > 1)
	{
		currentElement->setAttribute(vectorElement[1], value);
	}
	else
	{
		currentElement->setText( value );
	}

	return true;
}

bool DWXmlDocument::saveToFile( const std::wstring &xmlFile )
{
	return m_pXmlDoc->save_file(xmlFile.c_str(),L"\t",format_default|format_write_bom,encoding_utf8);
}

bool DWXmlDocument::saveToStream( std::wstring &xmlStream )
{
	xmlStream.clear();
	xml_writer_string write(xmlStream);

	xmlStream=L"<?xml version=\"1.0\"?>\n";	//强制加入XML声明，保持与老版本兼容
	m_pXmlDoc->print(write,L"\t",format_default|format_no_declaration,encoding_utf16);
	return true;
}

bool DWXmlDocument::isValid() const
{
	return m_pXmlDoc->root()?true:false;
}

void DWXmlDocument::setXmlFilter( PFN_XMLFILTER pFnFilter, void* filterContext )
{
	m_pFnFilter=pFnFilter;
	m_filterContext=filterContext;
}

bool saveToWString( const DWXmlElement *element, std::wstring& xmlStream )
{
	return element->saveToStream(xmlStream);
}
