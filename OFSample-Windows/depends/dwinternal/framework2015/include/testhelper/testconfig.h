/*
* ≤‚ ‘≈‰÷√Œƒº˛∂¡»°
*/

#ifdef UNIT_TEST

#ifndef __TESTCONFIG_H__
#define __TESTCONFIG_H__

#include "testhelperdef.h"
#include "dwutility/dwxmldocument.h"

class YY_TESTHELPER_EXPORT TestConfig
{
public:
	TestConfig(QString& strFileName);
	virtual ~TestConfig();

	static TestConfig* GetConfig();
public:
	QString	GetStringValue(QString& strKey);
	int		GetIntValue(QString& strKey);

protected:
	DWXmlDocument*		m_psXmlDoc;
};

#endif // __TESTCONFIG_H__

#endif // UNIT_TEST