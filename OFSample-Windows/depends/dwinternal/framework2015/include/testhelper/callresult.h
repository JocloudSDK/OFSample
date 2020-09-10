/*
* ͨ��AsyncTask��ʽ����YY��������������
*/
#ifdef UNIT_TEST

#ifndef __CALLRESULT_H__
#define __CALLRESULT_H__

#pragma warning(disable : 4530)

#include <map>

#include "testhelperdef.h"
#include "test_use_bind.h"
#include "duifw/duiapplication.h"

// ��ѯ���صĽ������
typedef std::map<UINT, void*>	T_MAP_RESULT;
extern T_MAP_RESULT YY_TESTHELPER_EXPORT g_sResult;


// ���������ID�����ڲ��Ҷ�Ӧ��ִ�к���
class YY_TESTHELPER_EXPORT IDAlloctor
{
public:
	static UINT NewId();

protected:
	static UINT m_uCurMaxId;
};


// ��Ҫ��������ں���ִ����ɺ�ɾ���ģ�ʹ�ø���
template<typename returnT, typename classT = void>
class QueryResultTask : public IAsyncTaskBase
{
public:
	QueryResultTask(const tiny_function0<returnT>& callbackFunc, classT* callbackObj)
		:m_tFunc(callbackFunc)
		,m_callbackObj(NULL)
	{
		m_uId = IDAlloctor::NewId();
		if (callbackObj != NULL)
		{
			m_callbackObj = new classT(*callbackObj);
		}
	}

	QueryResultTask(const tiny_function0<returnT>& callbackFunc, UINT uId, classT* callbackObj)
		:m_tFunc(callbackFunc)
		,m_uId(uId)
		,m_callbackObj(NULL)
	{
		if (callbackObj != NULL)
		{
			m_callbackObj = new classT(*callbackObj);
		}
	}

	QueryResultTask(QueryResultTask& sRet)
		:m_tFunc(sRet.m_tFunc)
		,m_uId(sRet.m_uId)
		,m_callbackObj(NULL)
	{
		if (sRet.m_callbackObj != NULL)
		{
			m_callbackObj = new classT(sRet.m_callbackObj);
		}
	}

	void SetTaskId(UINT uId)
	{
		m_uId = uId;
	}

	UINT GetTaskId()
	{
		return m_uId;
	}

	virtual void run()
	{
		returnT sRet = m_tFunc();
		returnT* psRet = new returnT(sRet);
		g_sResult[m_uId] = (void*)psRet;
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
	tiny_function0<returnT>	m_tFunc;
	UINT						m_uId;
	classT*						m_callbackObj;
};

// ��ͨ����ʹ�ø���
template<typename returnT>
class QueryResultTask<returnT> : public IAsyncTaskBase
{
public:
	QueryResultTask(const tiny_function0<returnT>& callbackFunc, UINT uId)
		:m_tFunc(callbackFunc)
		,m_uId(uId)
	{
	}

	QueryResultTask(const tiny_function0<returnT>& callbackFunc)
		:m_tFunc(callbackFunc)
	{
		m_uId = IDAlloctor::NewId();
	}

	QueryResultTask(QueryResultTask& sRet)
		:m_tFunc(sRet.m_tFunc)
		,m_uId(sRet.m_uId)
	{
	}
	
	UINT GetTaskId()
	{
		return m_uId;
	}

	void SetTaskId(UINT uId)
	{
		m_uId = uId;
	}

	virtual void run()
	{
		returnT sRet = m_tFunc();
		returnT* psRet = new returnT(sRet);
		g_sResult[m_uId] = (void*)psRet;
	}

	virtual void finished()
	{
	}

protected:
	tiny_function0<returnT>	m_tFunc;
	UINT					m_uId;
};

#endif //__CALLRESULT_H__

#endif // UNIT_TEST