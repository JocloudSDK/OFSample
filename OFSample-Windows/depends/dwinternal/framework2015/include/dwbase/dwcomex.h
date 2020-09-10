#pragma once

#include "sysbase/sysbase.h"

#pragma push_macro("emit")
#undef emit
#include <sysbase/tool/xsignals.h>
#pragma pop_macro("emit")

#define DWDECLARE_SIGNAL(Name, Signature)	\
	typedef struct : xsignals::signal<Signature> {} Name

#define DWDEFINE_SIGNAL(Signal)	\
	DWBIND_INTERFACE(Signal, DWMAKE_INTERFACE_UUID_NAME(Signal))	\

#define DWCOM_SIGNAL_ENTRY(Member)	\
	else if (iid == __dwuuidof2(&pThis->Member))	\
	{	\
		*ppv = &pThis->Member;	\
		return true;	\
	}	\

DWDEFINE_INTERFACE(IDWComponent): public IUnk
{
	virtual bool queryXSignal(const char *name, void **ppS) = 0;

	template<typename T>
	bool queryXSignal(const char *name, T **ppS);
};

class QObject;
DWDEFINE_INTERFACE(IDWQComponent): public IUnk
{
	enum ConnectType {AutoConnection, DirectConnection, QueuedConnection, UniqueConnection = 0x80 };
	virtual bool connectSignal(const char *sigName, const QObject *recv, const char *slotName, const ConnectType type) = 0;

	virtual bool disconnectSignal(const char *sigName, const QObject *recv, const char *slotName) = 0;

private:
	virtual bool _callMethod(const char *method, void **args, int num) = 0;

public:
	bool connectSignal(const char *sigName, const QObject *recv, const char *slotName)
	{
		return connectSignal(sigName, recv, slotName, UniqueConnection);
	}

	bool callMethod(const char *method)
	{
		return _callMethod(method, NULL, 0);
	}

	template<typename T>
	bool callMethod(const char *method, T arg);

	template<typename T1, typename T2>
	bool callMethod(const char *method, T1 _1, T2 _2);

	template<typename T1, typename T2, typename T3>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3);

	template<typename T1, typename T2, typename T3, typename T4>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4);

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5);

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6);

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6, T7 _7);

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	bool callMethod(const char *method, T1 _1, T2 _2, T3 _3, T4 _4, T5 _5, T6 _6, T7 _7, T8 _8);
};

#include "./detail/dwcomeximpl.h"

DWDEFINE_INTERFACE(IDWComRoot): public IUnk
{
private:
	virtual HRESULT coCreateComService(const char *name, IUnk* pOutUnk, comptr<IUnk>& pv) = 0;

public:
	virtual IUnkPtr getService(const char *name) const = 0;

	template<typename Interface>
	bool coCreateComService(const char *name, IUnk *pOutUnk, comptr<Interface>& pv)
	{
		IUnkPtr pUnk;
		HRESULT res = coCreateComService(name, pOutUnk, pUnk);
		if (res != S_OK || !pUnk)
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
	comptr<Interface> getServiceInterface(const char *name) const
	{
		IUnkPtr unk = getService(name);
		if (!unk)
		{
			return NULL;
		}

		comptr<Interface> inter;
		unk->queryInterface(inter);

		return inter;
	}	
};

DWDEFINE_INTERFACE(IServiceRegister): public IUnk
{
	virtual bool registerService(const char *name, IUnkPtr p) = 0;
	virtual bool unregisterService(const char *name) = 0;
};