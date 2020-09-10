/*
* 通过AsyncTask方式调用YY函数
*/
#ifdef UNIT_TEST

#ifndef __CALLYYFUNC_H__
#define __CALLYYFUNC_H__

#include "test_use_bind.h"

// 调用UI函数,如果需要保存对象，等对象在函数执行后删除的，使用该类 
// classT为类类型，returnT为函数返回值类型
template<typename returnT, typename classT = void>
class CallYYFuncTask : public IAsyncTaskBase
{
public:
	CallYYFuncTask(const tiny_function0<returnT>& callbackFunc, classT* callbackObj)
		:m_tFunc(callbackFunc)
		,m_callbackObj(NULL)
	{
		if (callbackObj != NULL)
		{
			m_callbackObj = new classT(*callbackObj);
		}
		
	}

	virtual void run()
	{
		m_tFunc();
	}

	virtual void finished()
	{
		if (m_callbackObj)
		{
			delete m_callbackObj;
			m_callbackObj = NULL;
		}
	}

protected:
	tiny_function0<returnT>		m_tFunc;
	classT*						m_callbackObj;
};

// 调用UI函数，不需要保存对象的使用该函数 returnT为函数返回类型
template<typename returnT>
class CallYYFuncTask<returnT> : public IAsyncTaskBase
{
public:
	CallYYFuncTask(const tiny_function0<returnT>& callbackFunc)
		:m_tFunc(callbackFunc)
	{
	}

	virtual void run()
	{
		m_tFunc();
	}

	virtual void finished()
	{
	}

protected:
	tiny_function0<returnT>		m_tFunc;
};

#endif //__CALLYYFUNC_H__

#endif // UNIT_TEST