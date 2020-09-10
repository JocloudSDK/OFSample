#pragma once


struct IAsynCallBack_gethostbyname
{
	virtual void finished(const char* /*name*/, struct hostent* /*host*/, int /*error*/, void* /*param*/) = 0;
};

namespace DwUtility
{
	namespace sock
	{
		bool DW_DWUTILITY_EXPORT asyn_gethostbyname(const char* name, IAsynCallBack_gethostbyname* cb, void* param = NULL);

	} // namespace sock

} // namespace DwUtility

