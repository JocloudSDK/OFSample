#include "stable.h"
#include <assert.h>
#include "EventDispatch.h"
#include "DataCenterPub.h"
#include "queuedfunc/dwqueuedfunc.h"
#include "perflog.h"

const DWORD c_dwVerifyFlag = 0x19810202;

namespace Data
{
	CEventDispatch::CEventDispatch()
	{
	}

	CEventDispatch::~CEventDispatch()
	{
	}

	CEventDispatch * CEventDispatch::GetInstance()
	{
		static CEventDispatch *pInst = NULL;
		if (!pInst)
		{
#ifdef _DEBUG
			int nOldFlag = _CrtSetDbgFlag(0);
#endif
			pInst = new CEventDispatch();
#ifdef _DEBUG
			_CrtSetDbgFlag(nOldFlag);
#endif
		}
		return pInst;
	}

	void CEventDispatch::doPollSig(void *pPollParms)
	{
#define F_DATACENTER "DataCenter"
		PerfLog(2, __FUNCTION__, F_DATACENTER);
		PARAMS_INFO *pParamsObj = (PARAMS_INFO *)pPollParms;
		if (!pParamsObj || pParamsObj->dwVerifyFlag != c_dwVerifyFlag)
		{
			Error::OutputError(_T("无效的参数pPollParms"));
			return;
		}

		switch (pParamsObj->eType)
		{
			case E_FIELDSET_DATA:
				(*((xsignals::signal<void (IFieldSetDataPtr)> *) pParamsObj->pSigObj))((IFieldSetData *)pParamsObj->pUnkArg1);	
				break;
			case E_ONE_ROWARRAY:
				(*((xsignals::signal<void (IRowArrayPtr)> *) pParamsObj->pSigObj))((IRowArray *)pParamsObj->pUnkArg1);
				break;
			case E_TWO_ROWARRAY:
				(*((xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> *) pParamsObj->pSigObj))((IRowArray *)pParamsObj->pUnkArg1, (IRowArray *)pParamsObj->pUnkArg2);
				break;
			default:
				assert(0);
				break;
		}

		pParamsObj->pUnkArg1->release();
		if (pParamsObj->pUnkArg2)
			pParamsObj->pUnkArg2->release();

		pParamsObj->dwVerifyFlag = 0xdddddddd;
		delete pParamsObj;
	}

	void CEventDispatch::QueuedEmitHelper(E_PARAMS_TYPE eType, void *pSigObj, IUnk *pUnkArg1, IUnk *pUnkArg2)
	{
		/* 去掉数据中心的异步信号支持
		PARAMS_INFO *pParamsObj = new PARAMS_INFO;
		pParamsObj->dwVerifyFlag = c_dwVerifyFlag;

		pParamsObj->eType = eType;
		pParamsObj->pSigObj = pSigObj;

		pUnkArg1->addRef();
		pParamsObj->pUnkArg1 = pUnkArg1;

		if (pUnkArg2)
			pUnkArg2->addRef();
		pParamsObj->pUnkArg2 = pUnkArg2;

		DwQueuedFunc::instance()->post(&CEventDispatch::doPollSig, pParamsObj);
		*/
	}
}