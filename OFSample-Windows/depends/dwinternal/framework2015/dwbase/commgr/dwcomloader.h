#pragma once
#include "dwbase/dwcomex.h"
#include "dwbase/dwcomstore_i.h"

class DwComLoader
{
public:
	virtual HRESULT coCreateComInstance(const char *name, IUnk* pOutUnk, comptr<IUnk>& pv);
	IDWComStorePtr comStore();

private:;
	void createComStore();

private:
	IDWComStorePtr m_comStore;
}; 
DwComLoader* comLoader();