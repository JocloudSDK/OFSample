#pragma once

#include <dwbase/IData.h>

/*
 * 对Data::IDatabase封装，便于聚合到Service
 */
DWDEFINE_INTERFACE(IYYDatabase) : IUnk
{
	/**
	  * @brief 获得Data::IDatabasePtr指针用来做奇怪的操作，一般来说此封装提供的函数已经足够
	  * @return Data::IDatabasePtr 
	  */
	virtual Data::IDatabasePtr database() = 0;

	/**
	  * @brief 获得表
	  * @param name 表名
	  * @return Data::ITablePtr 
	  */
	virtual Data::ITablePtr getTable(const NAME_ID &name) = 0;

	/**
	  * @brief 获得可编辑的表
	  * @param name 表名
	  * @return Data::ITableEditPtr 
	  */
	virtual Data::ITableEditPtr getTableEdit(const NAME_ID &name) = 0;
	
	/**
	  * @brief 获得数据集
	  * @param name 数据集名
	  * @return Data::IFieldSetPtr 
	  */
	virtual Data::IFieldSetPtr getFieldSet(const NAME_ID &name) = 0;

	/**
	  * @brief 分配一张新表，一般是作为二级表使用
	  * @param name 表的模板名
	  * @return Data::ITableEditPtr 
	  */
	virtual Data::ITableEditPtr allocTable(const NAME_ID &name) = 0;
};
