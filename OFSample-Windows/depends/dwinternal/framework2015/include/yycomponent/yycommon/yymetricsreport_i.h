#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


DWDEFINE_INTERFACE( IYYMetricsReport ) : public IDWQComponent
{
	/*
	* 功能： 上报统计接口
	* 返回值：是否成功上报当前数据
	* 参数scode: 后端申请的指标ID，需要先申请才能使用（PCYY：50111）
	* 参数uri: 指标名称，用户自定义，无需申请，不能为空，否则失败
	* 参数timeConsumption: 耗时，单位：毫秒
	*参数code: 返回码(自定义)，不能为空，否则失败
	*/
	virtual bool reportMetricsReturnCode(int scode, const char* uri, long timeConsumption, const char* code) = 0;


	/*
	*功能：上报统计接口
	*返回值：是否成功上报当前数据
	*参数scode: 后端申请的指标ID，需要先申请才能使用（PCYY：50111）
	*参数uri: 指标名称，用户自定义，无需申请，不能为空，否则失败
	*参数countName: 统计名称，不能为空，否则失败
	*参数count：累加值（就是样本值的总和）
	*参数times：累加次数（就是统计时间窗口内的样本数量）
	*/
	virtual bool reportMettricsCount(int scode, const char* uri, const char* countName, long count, int times) = 0;

	/*
	*功能：上报原始数据接口（用户自定义计算类型）
	*返回值：是否成功上报当前数据
	*参数scode：后端申请的指标ID，需要先申请才能使用（PCYY：50111）
	*参数uri:：指标名称，用户自定义，无需申请，不能为空，否则失败
	*参数topic： 指标子名称，不能为空，否则失败
	*参数val：指标值
	*参数extraJson：用户附加参数，不参与统计，若为空可填写NULL，若不为空，则为json格式的字符串，具体如下
	{"key1":"val1", "key2":"val2",..., "keyN":"valN"}
	*/
	virtual bool reportMetricsSrcData(int scode, const char* uri, const char* topic, long val, const char* extraJson) = 0;
};
