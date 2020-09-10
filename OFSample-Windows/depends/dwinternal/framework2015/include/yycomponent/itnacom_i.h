#pragma	once
#include "dwbase/dwcominterface_i.h"

typedef void (* PFN_ANTI_CALLBACK)(const char* data, int size, void* pContext);
DWDEFINE_INTERFACE(IDwAntiCom) : public IDWComponentEntry
{
	virtual bool exec(const char* data, int size, void* pContext, PFN_ANTI_CALLBACK cb) = 0;
};

DWDEFINE_INTERFACE(IDwInnerAntiCom) : public IUnk
{
	virtual bool exec(const char* data, int size, void* pContext, PFN_ANTI_CALLBACK cb) = 0;

//for uauth
	virtual void uauth_anti_hiidoreport(std::string& buffer) = 0;
	virtual bool uauth_anti_receive_anonymous_session() = 0;
	virtual bool uauth_anti_getaptoken(std::string& appid, bool encoded, std::string& ap_token) = 0;

};