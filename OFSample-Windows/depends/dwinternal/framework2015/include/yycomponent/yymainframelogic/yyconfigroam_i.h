
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <string>
#include <dwbase/dwcomex.h>

DWDEFINE_INTERFACE(IYYConfigRoam): public IDWQComponent
{
	virtual void getConfigFromSrv() = 0;
	virtual void setConfigToSrv(bool quit = false) = 0;

	virtual bool isDataReady() = 0;

	virtual bool getConfigData(const std::string &tag, std::wstring &data) = 0;
	virtual bool setConfigData(const std::string &tag, const std::wstring &data) = 0;
	virtual bool delConfigData(const std::string &tag) = 0;
	virtual int  getArrayConfigDataCount(const std::string &tag) = 0;
	virtual bool getArrayConfigData(const std::string &tag, const int &index, std::wstring &data, const std::string &objname) = 0;
	virtual bool setArrayConfigData(const std::string &tag, const std::wstring &value, const int &index, const std::string &objname) = 0;
	virtual bool delArrayConfigData(const std::string &tag, const int &index) = 0;

	virtual bool connectSignal(const char *signal, const QObject *receiver, const char *member) = 0;
	virtual bool disConnectSignal(const char *signal, const QObject *receiver, const char *member) = 0;
};