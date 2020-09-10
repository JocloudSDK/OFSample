
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "dwxmlelement.h"
#include <set>

namespace pugi
{
	class xml_document;
}

class DW_DWUTILITY_EXPORT DWXmlDocument
{
public:
	enum DWXmlElementType 
	{
		DWXmlElement_tag,
		DWXmlElement_attr,
	};

	typedef bool (__stdcall *PFN_XMLFILTER)(DWXmlElementType type, LPCWSTR name, LPCWSTR value, void* filterContext);

public:
	explicit DWXmlDocument();
	~DWXmlDocument(void);

public:
	bool loadFromFile( const std::wstring &xmlFile);
	bool loadFromString( const std::wstring &xmlString );
	bool loadFromString( const char *content, int lenght );


	void setXmlFilter(PFN_XMLFILTER pFnFilter, void* filterContext);
	
	const DWXmlElement *elementByPath( const std::wstring &elementPath ) const;
	std::wstring query( const std::wstring &valuePath ) const;
	bool setValue( const std::wstring &valuePath, const std::wstring &value );

	const DWXmlElement *rootElement() const;

	bool saveToFile(const std::wstring &xmlFile);
	bool saveToStream(std::wstring &xmlStream);

	bool isValid() const;
	DWXmlElement *createElementByPath( const std::wstring &elementPath );
	
private:
	bool isUtf8( const char *content, int lenght );
	bool isUtf16( const char *content, int lenght );

	void doNodeTraverse();

private:
	pugi::xml_document *m_pXmlDoc;
	PFN_XMLFILTER m_pFnFilter;
	void* m_filterContext;
};

bool DW_DWUTILITY_EXPORT saveToWString(const DWXmlElement *element, std::wstring& xmlStream);
