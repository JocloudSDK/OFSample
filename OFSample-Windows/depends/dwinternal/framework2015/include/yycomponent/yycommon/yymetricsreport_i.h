#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


DWDEFINE_INTERFACE( IYYMetricsReport ) : public IDWQComponent
{
	/*
	* ���ܣ� �ϱ�ͳ�ƽӿ�
	* ����ֵ���Ƿ�ɹ��ϱ���ǰ����
	* ����scode: ��������ָ��ID����Ҫ���������ʹ�ã�PCYY��50111��
	* ����uri: ָ�����ƣ��û��Զ��壬�������룬����Ϊ�գ�����ʧ��
	* ����timeConsumption: ��ʱ����λ������
	*����code: ������(�Զ���)������Ϊ�գ�����ʧ��
	*/
	virtual bool reportMetricsReturnCode(int scode, const char* uri, long timeConsumption, const char* code) = 0;


	/*
	*���ܣ��ϱ�ͳ�ƽӿ�
	*����ֵ���Ƿ�ɹ��ϱ���ǰ����
	*����scode: ��������ָ��ID����Ҫ���������ʹ�ã�PCYY��50111��
	*����uri: ָ�����ƣ��û��Զ��壬�������룬����Ϊ�գ�����ʧ��
	*����countName: ͳ�����ƣ�����Ϊ�գ�����ʧ��
	*����count���ۼ�ֵ����������ֵ���ܺͣ�
	*����times���ۼӴ���������ͳ��ʱ�䴰���ڵ�����������
	*/
	virtual bool reportMettricsCount(int scode, const char* uri, const char* countName, long count, int times) = 0;

	/*
	*���ܣ��ϱ�ԭʼ���ݽӿڣ��û��Զ���������ͣ�
	*����ֵ���Ƿ�ɹ��ϱ���ǰ����
	*����scode����������ָ��ID����Ҫ���������ʹ�ã�PCYY��50111��
	*����uri:��ָ�����ƣ��û��Զ��壬�������룬����Ϊ�գ�����ʧ��
	*����topic�� ָ�������ƣ�����Ϊ�գ�����ʧ��
	*����val��ָ��ֵ
	*����extraJson���û����Ӳ�����������ͳ�ƣ���Ϊ�տ���дNULL������Ϊ�գ���Ϊjson��ʽ���ַ�������������
	{"key1":"val1", "key2":"val2",..., "keyN":"valN"}
	*/
	virtual bool reportMetricsSrcData(int scode, const char* uri, const char* topic, long val, const char* extraJson) = 0;
};
