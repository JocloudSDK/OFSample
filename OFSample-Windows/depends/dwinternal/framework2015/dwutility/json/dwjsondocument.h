#pragma once

#include <string>

#include "dwglobal.h"
#include "json.h"

class json2_t;
class DW_DWUTILITY_EXPORT DWJsonObject
{
public:
	explicit DWJsonObject();
	~DWJsonObject();

	std::string name() const;
	std::string value() const;

	std::wstring get(const char *name) const;

	DWJsonObject getChildObject(const char *name) const;
	DWJsonObject getChildObject(int index) const;
	int getChildCount() const;
	bool isValid() const;
	json_value_type type() const;
	json_value_type valueType() const;
	std::string toString() const;
	
private:
	friend class DWJsonDocument;
	DWJsonObject(json2_t *jsonObject);
	json2_t *m_jsonObject;
};

class DW_DWUTILITY_EXPORT DWJsonDocument
{
public:
	explicit DWJsonDocument();
	~DWJsonDocument();

	bool loadFromFile(const std::wstring &fileName);
	bool loadFromData(const char *data);

	DWJsonObject rootObject() const;

	int objectCount() const;
	DWJsonObject getObject(int index) const;

private:
	json2_t *m_jsonDocument;
};

class DW_DWUTILITY_EXPORT DWJsonRW
{
public:
	explicit DWJsonRW();
	virtual ~DWJsonRW();

protected:
	json_t* m_json;

protected:
	static bool get(const json_t* object, std::wstring &ret, const char* objectlabel = NULL);

public:
	bool load(const char *filenameordata, bool file = true);
	bool save(const char *filename);
	bool save(std::string &data);
	void free();

	bool del(const char *label);
	bool delarray(const char *label, const int &index);

	bool get(const char* label, std::wstring &ret);
	bool getarray(const char* label, const int index, std::wstring &ret, const char* objectlabel = NULL);
	int  getarraycount(const char* label);

	bool set(const char *label, enum json_value_type type, const std::wstring &value);
	bool setarray(const char *label, enum json_value_type type, const std::wstring &value, const int &index, const char* objectlabel = NULL);
};

