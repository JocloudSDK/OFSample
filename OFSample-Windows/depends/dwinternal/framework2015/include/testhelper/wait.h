/*
*	wait 提供两类操作等待：
*		1 等待YY执行某函数后的返回值，直到返回和期望值相符的关系或者超时
*		2 等待直接执行某函数后的返回值，直到返回和期望值相符的关系或者超时
*/

#ifdef UNIT_TEST

#ifndef __WAIT_H__
#define __WAIT_H__

#include <QDateTime>

#include "testhelperdef.h"
#include "testhelper.h"
#include "callresult.h"

// 等待一个查询任务返回期望的值 taskT任务类型 returnT返回值类型
template<typename returnT = int, typename taskT = void>
class Wait
{
protected:
	typedef bool (*T_COMPARE_FUNC)(const returnT& valueA, const returnT& valueB);
	typedef bool (*T_COMPARE_FUNC_PTR)(const returnT* valueA, const returnT* valueB);

public:
	static bool WaitEQ_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec);
	static bool WaitNE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec);
	static bool WaitLE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec);
	static bool WaitGE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec);

	static returnT* WaitEQ_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec);
	static returnT* WaitNE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec);
	static returnT* WaitLE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec);
	static returnT* WaitGE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec);


	static returnT WaitResultTillTimeout(IAsyncTaskBase* psTask, int nMSec);

protected:
	static bool EQ(const returnT& valueA, const returnT& valueB);
	static bool EQ(const returnT* valueA, const returnT* valueB);

	static bool NE(const returnT& valueA, const returnT& valueB);
	static bool NE(const returnT* valueA, const returnT* valueB);

	static bool LE(const returnT& valueA, const returnT& valueB);
	static bool LE(const returnT* valueA, const returnT* valueB);

	static bool GE(const returnT& valueA, const returnT& valueB);
	static bool GE(const returnT* valueA, const returnT* valueB);

	static bool		WaitTillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec, T_COMPARE_FUNC sFunc);
	static returnT* WaitTillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec, T_COMPARE_FUNC_PTR sFunc);
	
	static bool CheckResult(returnT expectedValue, T_COMPARE_FUNC sFunc);

protected:
	static UINT m_uCurTaskId;
	static UINT m_uTaskMaxId;
};

template<typename returnT, typename taskT>
UINT Wait<returnT, taskT>::m_uCurTaskId = 0;

template<typename returnT, typename taskT>
UINT Wait<returnT, taskT>::m_uTaskMaxId = 0;



template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::WaitEQ_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec)
{
	return WaitTillTimeout(psTask, expectedValue, nMSec, EQ);
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::WaitNE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec)
{
	return WaitTillTimeout(psTask, expectedValue, nMSec, NE);
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::WaitLE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec)
{
	return WaitTillTimeout(psTask, expectedValue, nMSec, LE);
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::WaitGE_TillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec)
{
	return WaitTillTimeout(psTask, expectedValue, nMSec, GE);
}

template<typename returnT, typename taskT>
returnT* Wait<returnT, taskT>::WaitEQ_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec)
{
	return WaitTillTimeout(callbackFunc, expectedValue, nMSec, EQ);
}

template<typename returnT, typename taskT>
returnT* Wait<returnT, taskT>::WaitNE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec)
{
	return WaitTillTimeout(callbackFunc, (returnT*)expectedValue, nMSec, NE);
}

template<typename returnT, typename taskT>
returnT* Wait<returnT, taskT>::WaitLE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec)
{
	return WaitTillTimeout(callbackFunc, expectedValue, nMSec, LE);
}

template<typename returnT, typename taskT>
returnT* Wait<returnT, taskT>::WaitGE_TillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec)
{
	return WaitTillTimeout(callbackFunc, expectedValue, nMSec, GE);
}

template<typename returnT, typename taskT>
returnT Wait<returnT, taskT>::WaitResultTillTimeout(IAsyncTaskBase* psTask, int nMSec)
{
	const int QUERY_RESULT_SLEEP_TIME = 20; // ms

	qint64 nStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	m_uCurTaskId = ((taskT*)psTask)->GetTaskId();
	((DuiApplication*)qApp)->appendTask(psTask);

	while (true)
	{
		if (QDateTime::currentDateTime().toMSecsSinceEpoch() - nStartTime > nMSec)
		{
			break;
		}

		if (g_sResult.find(m_uCurTaskId) != g_sResult.end())
		{
			returnT sRet = *(returnT*)g_sResult[m_uCurTaskId];
			delete g_sResult[m_uCurTaskId];
			g_sResult.clear();
			return sRet;
		}
		Sleep(QUERY_RESULT_SLEEP_TIME);
	}

	return returnT();
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::EQ(const returnT& valueA, const returnT& valueB)
{
	return abs((double)valueA - (double)valueB) < 0.00001;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::EQ(const returnT* valueA, const returnT* valueB)
{
	return valueA == valueB;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::NE(const returnT& valueA, const returnT& valueB)
{
	return !EQ(valueA, valueB);
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::NE(const returnT* valueA, const returnT* valueB)
{
	return !EQ(valueA, valueB);
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::LE(const returnT& valueA, const returnT& valueB)
{
	return valueA - valueB <= 0;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::LE(const returnT* valueA, const returnT* valueB)
{
	return valueA - valueB <= 0;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::GE(const returnT& valueA, const returnT& valueB)
{
	return valueA - valueB >= 0;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::GE(const returnT* valueA, const returnT* valueB)
{
	return valueA - valueB >= 0;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::WaitTillTimeout(IAsyncTaskBase* psTask, returnT expectedValue, int nMSec, T_COMPARE_FUNC sFunc)
{
	const int QUERY_RESULT_SLEEP_TIME = 20; // ms

	qint64 nStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	while (true)
	{
		if (m_uCurTaskId == 0)
		{
			UINT nTaskId = IDAlloctor::NewId();
			taskT* psTempTask = new taskT(*(taskT*)psTask);
			psTempTask->SetTaskId(nTaskId);
			m_uCurTaskId = nTaskId;
			((DuiApplication*)qApp)->appendTask(psTempTask);
		}
		else
		{
			bool bRet = CheckResult(expectedValue, sFunc);
			if (bRet)
			{
				delete psTask;
				return true;
			}
		}
		Sleep(QUERY_RESULT_SLEEP_TIME);

		if (QDateTime::currentDateTime().toMSecsSinceEpoch() - nStartTime > nMSec)
		{
			break;
		}
	}
	delete psTask;

	return false;
}

template<typename returnT, typename taskT>
returnT* Wait<returnT, taskT>::WaitTillTimeout(const tiny_function0<returnT*>& callbackFunc, returnT* expectedValue, int nMSec, T_COMPARE_FUNC_PTR sFunc)
{
	const int QUERY_RESULT_SLEEP_TIME = 20; // ms

	qint64 nStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	while (true)
	{
		returnT* returnValue = callbackFunc();
		bool bRet = sFunc(returnValue, expectedValue);
		if (bRet)
		{
			return returnValue;
		}

		Sleep(QUERY_RESULT_SLEEP_TIME);

		if (QDateTime::currentDateTime().toMSecsSinceEpoch() - nStartTime > nMSec)
		{
			break;
		}
	}

	return NULL;
}

template<typename returnT, typename taskT>
bool Wait<returnT, taskT>::CheckResult(returnT expectedValue, T_COMPARE_FUNC sFunc)
{
	if (g_sResult.find(m_uCurTaskId) == g_sResult.end())
	{
		return false;
	}

	returnT& returnValue = *(returnT*)(g_sResult[m_uCurTaskId]);
	bool bRet = sFunc(returnValue, expectedValue);
	delete g_sResult[m_uCurTaskId];
	g_sResult.erase(m_uCurTaskId);
	m_uCurTaskId = 0;

	return bRet;
}

#endif // __WAIT_H__

#endif // UNIT_TEST