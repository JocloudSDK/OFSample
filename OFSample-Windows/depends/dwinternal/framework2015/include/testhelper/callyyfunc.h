/*
* ͨ��AsyncTask��ʽ����YY����
*/
#ifdef UNIT_TEST

#ifndef __CALLYYFUNC_H__
#define __CALLYYFUNC_H__

#include "test_use_bind.h"

// ����UI����,�����Ҫ������󣬵ȶ����ں���ִ�к�ɾ���ģ�ʹ�ø��� 
// classTΪ�����ͣ�returnTΪ��������ֵ����
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

// ����UI����������Ҫ��������ʹ�øú��� returnTΪ������������
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