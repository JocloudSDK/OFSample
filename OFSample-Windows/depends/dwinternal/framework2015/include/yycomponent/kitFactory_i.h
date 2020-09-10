#pragma once

#include "yycomponent/detail/yycomclsid.h"
#include "dwbase/dwcommgr_i.h"

define_interface(IPropBag);
define_interface(IMarshalHelper);


define_interface(IKitFactory) : IUnk
{
	virtual IPropBagPtr newPropBag() = 0;
	virtual IMarshalHelperPtr newMarshalHelper() = 0;
};

inline IKitFactoryPtr bizGetKitFactorySingleton()
{
	return static_cast<IKitFactory*>(
		(IUnk*)comMgr()->getService(KSERVICEYYBizKitFactory)
		);
}
