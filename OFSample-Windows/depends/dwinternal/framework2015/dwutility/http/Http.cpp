#include "stable.h"

#include "uploaddata.h"
#include "downloaddata.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			::DisableThreadLibraryCalls((HMODULE)hModule);
		}
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

namespace DwUtility
{
	namespace http
	{
		IUploadHttpData* CreateUploadHttpData()
		{
			return new UploadData();
		}

		IDownloadHttpData* CreateDownloadHttpData()
		{
			return new DownloadData();
		}
	}
}
