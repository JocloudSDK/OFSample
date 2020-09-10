#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief ������Ϣ�ӿ�, use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYPersonalProfile): public IUnk
{
	typedef std::map<int,std::wstring>	IndexMap;

	/**
	* @brief �õ�����ʡ�ݵ���Ϣ.
	* @param [output] indexMap ʡ����Ϣ��.
	* @return.
	*/
	virtual void getProvince(IndexMap& indexMap) const = 0 ;

	/**
	* @brief �õ�ָ��ʡ�ݵ�����.
	* @param index ʡ��index.
	* @return ����ʡ������.
	*/
	virtual std::wstring getProvince( int index ) const = 0;

	/**
	* @brief ͨ��ʡ�ݲ��ҵ����еĳ�����Ϣ.
	* @param provinceName ʡ������.
	* @param [output] citys ���г�����Ϣ.
	* @return.
	*/
	virtual void getCitys( const std::wstring& provinceName, IndexMap& citys ) = 0;

	/**
	* @brief �õ�ָ��ʡ����ָ���ĳ�����Ϣ.
	* @param provinceName ʡ������.
	* @param index ����index.
	* @return ָ����������.
	*/
	virtual std::wstring getCity( const std::wstring& provinceName, int index ) = 0 ;

	/**
	* @brief ��ȡ��������.
	* @param [output] constellation ����������Ϣ.
	* @return. 
	*/
	virtual void getConstellation( IndexMap& constellation ) = 0;

	/**
	* @brief ��ȡ���ҵ�����.
	* @param index ����index.
	* @return ���ع�������.
	*/
	virtual std::wstring getCountry(int index)const = 0;
};