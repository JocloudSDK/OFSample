#include "stable.h"

#include "text/ustring.h"
#include "dwjsondocument.h"

#define BufferLen (1024)
class json2_t;
class Json2Bulder
{
public:
	static json2_t * buildJson2Tree(json_t *pNode);
	static void freeJson2Tree(json2_t *pRoot);
private:
	static void _freeJson2Tree(json2_t *pNode);
};

class json2_t
{
	friend class DWJsonObject;
	friend class DWJsonDocument;
	friend class Json2Bulder;
public:
	json2_t(json_t * pNode):m_pNode(pNode)
	{
	}

	const char * text(){return m_pNode->text;}
	enum json_value_type type(){return m_pNode->type;}


	size_t getChildCount(){return m_children.size();}
	json2_t * getChild(UINT iChild){
		if(iChild>=m_children.size()) return NULL;
		return m_children[iChild];
	}
protected:
	json_t * child(){return m_pNode->child;}
	json_t * next() {return m_pNode->next;}
	json_t * value(){return m_pNode->child;}

	json_t * m_pNode;
	std::vector<json2_t*> m_children;
};

json2_t * Json2Bulder::buildJson2Tree(json_t *pNode)
{
	json2_t *pNode2=new json2_t(pNode);
	json_t *pChild=pNode->child;
	while(pChild)
	{
		json2_t *pChild2=buildJson2Tree(pChild);
		pNode2->m_children.push_back(pChild2);
		pChild=pChild->next;
	}
	return pNode2;
}


void Json2Bulder::freeJson2Tree(json2_t *pRoot)
{
	json_free_value(&pRoot->m_pNode);
	_freeJson2Tree(pRoot);
}

void Json2Bulder::_freeJson2Tree(json2_t *pNode)
{
	for(size_t i=0;i<pNode->m_children.size();i++)
	{
		_freeJson2Tree(pNode->m_children[i]);
	}
	delete pNode;
}
//////////////////////////////////////////////////////////////////////////
DWJsonObject::DWJsonObject()
:m_jsonObject(NULL)
{
}

DWJsonObject::DWJsonObject(json2_t *jsonObject)
: m_jsonObject(jsonObject)
{

}

DWJsonObject::~DWJsonObject()
{
}

std::wstring DWJsonObject::get(const char *name) const
{
	PERFLOG;
	std::wstring ret;
	if (!isValid())
	{
		return ret;
	}

	json_t *item = m_jsonObject->child();
	while(item)
	{
		if (item->text && strcmp(name, item->text) == 0)
		{
			char *value = item->child->text;
			json_value_type type = item->child->type;

			switch (type)
			{
			case JSON_STRING:
				{
					int len = strlen(value) + 1;
					if (len > BufferLen)
					{
						char * unescape = (char*)malloc(len);
						if (unescape)
						{
							int len2 = json_unescape2(value, unescape);
							DwUtility::text::toWideString(unescape, len2, ret, CP_UTF8);
							free(unescape);
						}
					}
					else if (len > 1)
					{
						char unescape[BufferLen];
						int len2 = json_unescape2(value, unescape);
						DwUtility::text::toWideString(unescape, len2, ret, CP_UTF8);
					}
					break;
				}
			case JSON_NUMBER:
				{
					DwUtility::text::toWideString(value, -1, ret, CP_UTF8);
					break;
				}
			
			case JSON_FALSE:
				ret = L"false";
				break;

			case JSON_TRUE:
				ret = L"true";
				break;

			case JSON_NULL:
				break;
			default:
				break;
			}
			break;
		}

		item = item->next;
	}

	return ret;
}

DWJsonObject DWJsonObject::getChildObject(const char *name) const
{
	if (!isValid())
	{
		return DWJsonObject();
	}
	for(size_t i=0;i<m_jsonObject->getChildCount();i++)
	{
		json2_t *temp = m_jsonObject->getChild(i);
		json_t *value=temp->value();
		if(value && (value->type == JSON_ARRAY || value->type == JSON_OBJECT))
		{
			if(strcmp(name, temp->text()) == 0)
			{
				return DWJsonObject(temp->getChild(0));
			}
		}	
	}

	return DWJsonObject();
}

DWJsonObject DWJsonObject::getChildObject( int index ) const
{
	if (!isValid())
	{
		return DWJsonObject();
	}
	return DWJsonObject(m_jsonObject->getChild(index));

}

int DWJsonObject::getChildCount() const
{

	int count = 0;
	if (!isValid())
	{
		return count;
	}
	return m_jsonObject->getChildCount();
}

bool DWJsonObject::isValid() const
{
	return m_jsonObject != NULL;
}

json_value_type DWJsonObject::type() const
{
	if (!isValid())
	{
		return JSON_NULL;
	}

	return m_jsonObject->type();
}

json_value_type DWJsonObject::valueType() const
{
	if (!isValid())
	{
		return JSON_NULL;
	}

	if(m_jsonObject->child())
	{
		return m_jsonObject->child()->type;
	}

	return JSON_NULL;
}

std::string DWJsonObject::name() const
{
	std::string ret;
	if (isValid())
	{
		return m_jsonObject->text();
	}

	return ret;
}

std::string DWJsonObject::value() const
{
	std::string ret;
	if (isValid())
	{
		if (const char* value = (m_jsonObject->child() ? m_jsonObject->child()->text : NULL))
		{
			return value;
		}
	}
	return ret;
}

std::string DWJsonObject::toString() const
{
	std::string ret;
	if (m_jsonObject)
	{
		char* data = NULL;
		if (JSON_OK == ::json_tree_to_string(m_jsonObject->m_pNode, &data))
		{
			ret.append(data);
			::free(data);
			data = NULL;
		}
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////

DWJsonDocument::DWJsonDocument()
: m_jsonDocument(NULL)
{

}

DWJsonDocument::~DWJsonDocument()
{
	if(m_jsonDocument)
	{
		Json2Bulder::freeJson2Tree(m_jsonDocument);
		m_jsonDocument = NULL;
	}
}

bool DWJsonDocument::loadFromFile(const std::wstring &fileName)
{
	PERFLOG;

	FILE *fp = NULL;
	_wfopen_s(&fp, fileName.c_str(), L"r");

	bool ret = false;
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char *data = (char*)malloc(size + 1);
		memset(data, 0, size + 1);

		fread(data, size, 1, fp);
	
		ret = loadFromData(data);

		free(data);

		fclose(fp);
	}

	return ret;
}


bool DWJsonDocument::loadFromData(const char *data)
{
	json_t *root=NULL;
	if(json_parse_document(&root, (char*)data) != JSON_OK)
	{
		return false;
	}
	if(m_jsonDocument) Json2Bulder::freeJson2Tree(m_jsonDocument);
	m_jsonDocument = Json2Bulder::buildJson2Tree(root);
	return true;
}

DWJsonObject DWJsonDocument::rootObject() const
{
	return DWJsonObject(m_jsonDocument);
}

DWJsonObject DWJsonDocument::getObject( int index ) const
{
	DW_ASSERT(m_jsonDocument != NULL);
	if(index>=(int)m_jsonDocument->getChildCount()) return DWJsonObject();
	return m_jsonDocument->getChild(index);
}

int DWJsonDocument::objectCount() const
{
	return m_jsonDocument->getChildCount();
}


DWJsonRW::DWJsonRW() : m_json(NULL)
{
}

DWJsonRW::~DWJsonRW()
{
	DWJsonRW::free();
}

bool DWJsonRW::load(const char *filenameordata, bool file)
{
	PERFLOG;
	enum json_error error = JSON_UNKNOWN_PROBLEM;

	DWJsonRW::free();
	if (!filenameordata || '\0' == *filenameordata)
		return false;

	if (file)
	{
		FILE *fp = fopen(filenameordata, "r");
		if (!fp)
			return false;

		error = json_stream_parse(fp, &m_json);
		fclose(fp);
		fp = NULL;
	}
	else
		error = json_parse_document(&m_json, (char*)filenameordata);

	return (JSON_OK == error) ? true : false;
}

bool DWJsonRW::save(const char *filename)
{
	PERFLOG;
	if (!m_json || !filename || '\0' == *filename)
		return false;

	FILE *fp = fopen(filename, "w");
	if (!fp)
		return false;

	enum json_error error = json_stream_output(fp, m_json);
	fclose(fp);
	fp = NULL;

	return (JSON_OK == error) ? true : false;
}

bool DWJsonRW::save( std::string &stream )
{
	PERFLOG;
	if (m_json)
	{
		char* data = NULL;
		if (json_tree_to_string(m_json, &data) == JSON_OK)
		{
			stream.clear();
			stream.append(data);

			::free(data);
			return true;
		}
	}
	return false;
}

void DWJsonRW::free()
{
	if (m_json)
		json_free_value(&m_json);
	m_json = NULL;
}

bool DWJsonRW::del(const char *label)
{
	if (!m_json || !label || '\0' == *label)
		return false;

	return (JSON_OK == dw_json_tree_delete(m_json, label)) ? true : false;
}

bool DWJsonRW::delarray(const char *label, const int &index)
{
	if (!m_json || !label || '\0' == *label || index < 0)
		return false;

	return (JSON_OK == dw_json_tree_delete_array(m_json, label, index)) ? true : false;
}

bool DWJsonRW::get(const json_t* object, std::wstring &ret, const char* objectlabel)
{
	PERFLOG;
	if (!object)
		return false;
	ret.clear();

	json_t * cursor = (json_t*) object;
	if (objectlabel && '\0' != *objectlabel)
	{
		cursor = dw_json_tree_find_helper(object, objectlabel);
		if (!cursor || !cursor->child)
			return false;
		cursor = cursor->child;
	}

	bool fret = false;

	switch (cursor->type)
	{
	case JSON_STRING:
		{
			int len = strlen(cursor->text) + 1;
			if (len > BufferLen)
			{
				char * unescape = (char*)malloc(len);
				if (unescape)
				{
					int len2 = json_unescape2(cursor->text, unescape);
					DwUtility::text::toWideString(unescape, len2, ret, CP_UTF8);
					::free(unescape);
					unescape = NULL;
				}
			}
			else if (len > 1)
			{
				char unescape[BufferLen];
				int len2 = json_unescape2(cursor->text, unescape);
				DwUtility::text::toWideString(unescape, len2, ret, CP_UTF8);
			}
			fret = true;
			break;
		}
	case JSON_NUMBER:
		{
			DwUtility::text::toWideString(cursor->text, -1, ret, CP_UTF8);
			fret = true;
			break;
		}
	case JSON_TRUE:
		ret = L"true";
		fret = true;
		break;
	case JSON_FALSE:
		ret = L"false";
		fret = true;
		break;
	case JSON_NULL:
	case JSON_ARRAY:
	case JSON_OBJECT:
		break;
	default:
		break;
	}

	return fret;
}

bool DWJsonRW::get(const char* label, std::wstring &ret)
{
	if (!m_json || !label || '\0' == *label)
		return false;
	ret.clear();

	json_t * cursor = dw_json_tree_find(m_json, label);
	if (!cursor || !cursor->child)
		return false;
	return DWJsonRW::get(cursor->child, ret);
}

bool DWJsonRW::getarray(const char* label, const int index, std::wstring &ret, const char* objectlabel)
{
	PERFLOG;
	if (!m_json || !label || '\0' == *label || index < 0)
		return false;
	ret.clear();

	json_t * cursor = dw_json_tree_find_array(m_json, label, index);
	if (!cursor)
		return false;
	return DWJsonRW::get(cursor, ret, objectlabel);
}

int DWJsonRW::getarraycount(const char* label)
{
	if (!m_json || !label || '\0' == *label)
		return 0;
	return dw_json_tree_count_array(m_json, label);
}

bool DWJsonRW::set(const char *label, enum json_value_type type, const std::wstring &value)
{
	if (!label || '\0' == *label)
		return false;
	if (!m_json)
		m_json = json_new_object();
	if (!m_json)
		return false;

	std::string str;
	DwUtility::text::toNarrowString(value.c_str(), value.length(), str, CP_UTF8);
	char * p = NULL;
	if (!str.empty())
	{
		p = json_escape((char*)str.c_str());
	}
	else
	{
		p = (char*)malloc(1);
		*p = '\0';
	}

	json_t * ret = dw_json_tree_set(m_json, label, type, p);

	::free(p);
	p = NULL;

	return (ret != NULL) ? true : false;
}

bool DWJsonRW::setarray(const char *label, enum json_value_type type, const std::wstring &value, const int &index, const char* objectlabel)
{
	if (!label || '\0' == *label || index < 0)
		return false;
	if (!m_json)
		m_json = json_new_object();
	if (!m_json)
		return false;

	std::string str;
	DwUtility::text::toNarrowString(value.c_str(), value.length(), str, CP_UTF8);
	char* p = json_escape((char*)str.c_str());
	enum json_error error = JSON_UNKNOWN_PROBLEM;

	if (objectlabel && '\0' != *objectlabel)
	{
		json_t * cursor = dw_json_tree_set(m_json, label, JSON_ARRAY, NULL);
		if (!cursor)
			return false;

		json_t * jfind = dw_json_tree_find_array_helper(cursor, index);
		if (!jfind)
		{
			if (!cursor->child)
				return false;
			cursor = cursor->child;
			json_t * temp = json_new_object();
			if (!temp)
				return false;
			if (JSON_OK != json_insert_child(cursor, temp))
				return false;
			jfind = temp;
		}
		error = (dw_json_tree_set(jfind, objectlabel, type, p) != NULL) ? JSON_OK : JSON_UNKNOWN_PROBLEM;
	}
	else
		error = dw_json_tree_set_array(m_json, label, type, index, p);

	::free(p);
	p = NULL;

	return (JSON_OK == error) ? true : false;
}

