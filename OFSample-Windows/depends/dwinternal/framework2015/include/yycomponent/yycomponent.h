#pragma once

#include "dwbase/dwcomex.h"


struct InvokeParam
{
	int size;
};


namespace InvokeCode
{
	enum
	{
		InvokeOK,						//调用成功.
		InvalidateInParam,			//输入参数非法.
		InvalidateOutParam,		//输入参数非法.
		InvokeNotSupport,			//不支持该函数调用，一般情况下该函数不存在.
		InvokeFail,						// 其它原因调用失败.
	};
}

// PL => praram list.

template< typename T>
struct PL_1 : public InvokeParam
{
	T	p1;
};

template< typename T1, typename T2 >
struct PL_2 : public InvokeParam
{
	T1	p1;
	T2 p2;
};

template< typename T1, typename T2, typename T3>
struct PL_3 : public InvokeParam
{
	T1 p1;
	T2 p2;
	T3 p3;
};

template<typename T1, typename T2, typename T3, typename T4>
struct PL_4 : public InvokeParam
{
	T1 p1;
	T2 p2;
	T3 p3;
	T4	p4;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct PL_5: public InvokeParam
{
	T1 p1;
	T2	p2;
	T3 p3;
	T4 p4;
	T5 p5;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct PL_6: public InvokeParam
{
	T1 p1;
	T2 p2;
	T3 p3;
	T4	p4;
	T5 p5;
	T6	p6;
};