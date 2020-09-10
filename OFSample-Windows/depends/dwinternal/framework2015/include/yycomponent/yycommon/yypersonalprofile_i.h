#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief 个人信息接口, use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYPersonalProfile): public IUnk
{
	typedef std::map<int,std::wstring>	IndexMap;

	/**
	* @brief 得到所有省份的信息.
	* @param [output] indexMap 省份信息表.
	* @return.
	*/
	virtual void getProvince(IndexMap& indexMap) const = 0 ;

	/**
	* @brief 得到指定省份的名字.
	* @param index 省份index.
	* @return 返回省份名字.
	*/
	virtual std::wstring getProvince( int index ) const = 0;

	/**
	* @brief 通过省份查找到所有的城市信息.
	* @param provinceName 省份名字.
	* @param [output] citys 所有城市信息.
	* @return.
	*/
	virtual void getCitys( const std::wstring& provinceName, IndexMap& citys ) = 0;

	/**
	* @brief 得到指定省份里指定的城市信息.
	* @param provinceName 省份名字.
	* @param index 城市index.
	* @return 指定城市名字.
	*/
	virtual std::wstring getCity( const std::wstring& provinceName, int index ) = 0 ;

	/**
	* @brief 获取所有星座.
	* @param [output] constellation 所有星座信息.
	* @return. 
	*/
	virtual void getConstellation( IndexMap& constellation ) = 0;

	/**
	* @brief 获取国家的名字.
	* @param index 国家index.
	* @return 返回国家名字.
	*/
	virtual std::wstring getCountry(int index)const = 0;
};