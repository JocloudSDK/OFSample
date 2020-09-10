#pragma once

#include <dwbase/IData.h>

/*
 * ��Data::IDatabase��װ�����ھۺϵ�Service
 */
DWDEFINE_INTERFACE(IYYDatabase) : IUnk
{
	/**
	  * @brief ���Data::IDatabasePtrָ����������ֵĲ�����һ����˵�˷�װ�ṩ�ĺ����Ѿ��㹻
	  * @return Data::IDatabasePtr 
	  */
	virtual Data::IDatabasePtr database() = 0;

	/**
	  * @brief ��ñ�
	  * @param name ����
	  * @return Data::ITablePtr 
	  */
	virtual Data::ITablePtr getTable(const NAME_ID &name) = 0;

	/**
	  * @brief ��ÿɱ༭�ı�
	  * @param name ����
	  * @return Data::ITableEditPtr 
	  */
	virtual Data::ITableEditPtr getTableEdit(const NAME_ID &name) = 0;
	
	/**
	  * @brief ������ݼ�
	  * @param name ���ݼ���
	  * @return Data::IFieldSetPtr 
	  */
	virtual Data::IFieldSetPtr getFieldSet(const NAME_ID &name) = 0;

	/**
	  * @brief ����һ���±�һ������Ϊ������ʹ��
	  * @param name ���ģ����
	  * @return Data::ITableEditPtr 
	  */
	virtual Data::ITableEditPtr allocTable(const NAME_ID &name) = 0;
};
