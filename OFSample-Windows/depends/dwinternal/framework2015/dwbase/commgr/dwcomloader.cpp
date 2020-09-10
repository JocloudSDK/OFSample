#include "stable.h"
#include "dwcomloader.h"
#include "dwbase/yycomclsid.h"
#include "dwutility/environmentpath.h"
#include "dwutility/usystem.h"
#include "log/log.h"

HRESULT DwComLoader::coCreateComInstance(const char *name, IUnk* pOutUnk, comptr<IUnk>& pv)
{
	if (!m_comStore)
	{
		createComStore();
		if (!m_comStore)
		{
			return E_FAIL;
		}
	}

	if (strcmp(name, KSERVICEYYComStore) == 0)
	{
		if (m_comStore->queryInterface(pv))
		{
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}

	//直接找comstore创建
	if (m_comStore->createInstance(name, pOutUnk, pv))
	{
		return S_OK;
	}

	return E_FAIL;
}

void DwComLoader::createComStore()
{
	if (m_comStore)
	{
		return ;
	}

	std::wstring strFilePath(DwUtility::app::appRootPath());
	strFilePath += L"yycomstore.dll"; 
	HMODULE h = DwUtility::system::loadLibraryEx(strFilePath, 0);
	if (h == NULL)
	{
		LogFinal(LOG::KComponent) << L"Load library failed: yycomstore.dll" << ::GetLastError();
		return ;
	}

	typedef HRESULT (__cdecl *comInstanceCreator)(const char *, IUnk *, IUnk **);
	comInstanceCreator fun = reinterpret_cast<comInstanceCreator>(::GetProcAddress(h, "createComInstance"));
	if (fun != NULL)
	{
		IUnkPtr pUnk;
		HRESULT hRes = fun(KSERVICEYYComStore, NULL, pUnk._ppv());
		if (hRes != S_OK)
		{
			return ;
		}

		pUnk->queryInterface(m_comStore);
	}
};

IDWComStorePtr DwComLoader::comStore()
{
	createComStore();
	return m_comStore;
};

DwComLoader *g_loader;

 DwComLoader* comLoader()
{
	if (g_loader == NULL)
	{
		g_loader = new DwComLoader;
	}

	return g_loader;
}

 __declspec( dllexport ) bool coCreateComInstance(const char *name, IUnk* pOutUnk, IUnkPtr& pv)
 {
	return comLoader()->coCreateComInstance(name, pOutUnk, pv) == S_OK;
 }