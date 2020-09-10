#pragma once

#include "ITable.h"
#include "IFieldSet.h"

//bobdeng：数据中心当前实现属于临时方案, 只支持PostMessage到UI线程，未来需要切换到统一事件模型
namespace Data
{
	class CEventDispatch
	{
	public:
		static CEventDispatch * GetInstance();
		
		static void CALLBACK doPollSig(void * pPollParms);

		inline void Emit(xsignals::signal<void (IFieldSetDataPtr)> *pSigObj, IFieldSetData * pFieldSetData) { (*pSigObj)(pFieldSetData); }
		inline void Emit(xsignals::signal<void (IRowArrayPtr)> *pSigObj, IRowArray * pRowArray) { (*pSigObj)(pRowArray); }
		inline void Emit(xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> *pSigObj, IRowArray * pRowArray1, IRowArray * pRowArray2) { (*pSigObj)(pRowArray1, pRowArray2); }
	
		inline void QueuedEmit(xsignals::signal<void (IFieldSetDataPtr)> *pSigObj, IFieldSetData * pFieldSetData) { QueuedEmitHelper(E_FIELDSET_DATA, pSigObj, pFieldSetData, NULL); }
		inline void QueuedEmit(xsignals::signal<void (IRowArrayPtr)> *pSigObj, IRowArray * pRowArray) { QueuedEmitHelper(E_ONE_ROWARRAY, pSigObj, pRowArray, NULL); }
		inline void QueuedEmit(xsignals::signal<void (IRowArrayPtr, IRowArrayPtr)> *pSigObj, IRowArray * pRowArray1, IRowArray * pRowArray2) { QueuedEmitHelper(E_TWO_ROWARRAY, pSigObj, pRowArray1, pRowArray2); }
	
	private:
		enum E_PARAMS_TYPE
		{
			E_FIELDSET_DATA,
			E_ONE_ROWARRAY,
			E_TWO_ROWARRAY,
		};

		struct PARAMS_INFO
		{
			DWORD			dwVerifyFlag;
			E_PARAMS_TYPE	eType;
			IUnk *			pUnkArg1;
			IUnk *			pUnkArg2;
			void *			pSigObj;
		};

		CEventDispatch();
		~CEventDispatch();

		void QueuedEmitHelper(E_PARAMS_TYPE eType, void *pSigObj, IUnk *pUnkArg1, IUnk *pUnkArg2);
	};
};