#pragma once

#include "dwbase/dwcomex.h"

DWBASE_EXPORTS bool coCreateComInstance(const char *name, IUnk* pOutUnk, IUnkPtr& pv);

template<typename Interface>
bool coCreateComInstance(const char *name, IUnk *pOutUnk, comptr<Interface>& pv)
{
	IUnkPtr pUnk;
	bool b = coCreateComInstance(name, pOutUnk, pUnk);
	if (!b || !pUnk)
	{
		return false;
	}

	if (!pUnk->queryInterface(pv))
	{
		return false;
	}

	return true;
}

template<typename Interface>
bool coCreateComInstance(const char *name, comptr<Interface>& pv)
{
	return coCreateComInstance(name, NULL, pv);
}