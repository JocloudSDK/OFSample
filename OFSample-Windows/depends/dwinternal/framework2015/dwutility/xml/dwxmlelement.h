
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <string>
#include <vector>
#include <utility>

#include "dwglobal.h"

class DWXmlElementPrivate;

namespace pugi
{
	class xml_node;
	class xml_attribute;
}

class DW_DWUTILITY_EXPORT DWXmlAttribute
{
	friend class DWXmlAttributePrivate;
	friend class DWXmlElement;
	friend class CXmlNodeHook;
public:
	explicit DWXmlAttribute();
	DWXmlAttribute(const DWXmlAttribute & src);
	~DWXmlAttribute();

	// Check if attribute is empty
	bool isEmpty() const;

	// Get attribute name/value, or "" if attribute is empty
	const std::wstring name() const;
	const std::wstring value() const;
	void set(std::wstring value);

	DWXmlAttribute nextAttribute() const;
	DWXmlAttribute prevAttribute() const;

	DWXmlAttribute& operator=(const DWXmlAttribute & src);
private:
	DWXmlAttribute(pugi::xml_attribute attr);
	DWXmlAttributePrivate	*d_ptr;
};

class DW_DWUTILITY_EXPORT DWXmlElement
{
	friend class DWXmlElementPrivate;
	friend class DWXmlDocument;
	friend class CXmlNodeHook;
public:
	explicit DWXmlElement(const std::wstring &name);
	~DWXmlElement();

	std::wstring tag() const;
	std::wstring text() const;
	void setText(const std::wstring& text);

	int childCount() const;
	const DWXmlElement *child(int index) const;
	void addChild(DWXmlElement *child);
	int attributeCount() const;

	
	std::wstring attribute( const std::wstring &attrName ) const;
	std::wstring attribute( int index ) const;
	std::wstring attributeName( int index ) const;
	void setAttribute(const std::wstring &attrName, const std::wstring &attrValue);
	void removeAttribute(const std::wstring &attrName);

	const DWXmlElement *operator[](int index) const;
	std::wstring operator[](const std::wstring &attrName) const;
	

	void deleteChild(const DWXmlElement *child);

	DWXmlElement *elementByName(const std::wstring &name) const;

	const wchar_t* attribute2(const wchar_t* attrName) const;
public:
	bool isEmpty() const;
	
	typedef bool (__stdcall *PCB_XMLTRAVERSE)(const DWXmlElement *node, void* pContext);
	void traverse( PCB_XMLTRAVERSE fnTraverse,void *pCtx =NULL);
	bool saveToStream( std::wstring &xmlStream ) const;

	DWXmlElement * addChild(const std::wstring & str);
	DWXmlAttribute attr(const std::wstring &attrName) const;
	DWXmlAttribute firstAttr() const;
	DWXmlAttribute lastAttr() const;
	DWXmlAttribute appendAttr(const std::wstring & name);
	DWXmlAttribute prependAttr(const std::wstring & name);
	DWXmlAttribute insertAttrAfter(const std::wstring & name, const DWXmlAttribute& attr);
	DWXmlAttribute insertAttrBefore(const std::wstring & name, const DWXmlAttribute& attr);

protected:

	DWXmlElement(const pugi::xml_node & xmlNode);
	
	DWXmlElementPrivate	*d_ptr;
};
