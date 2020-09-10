#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYVideoSDKInfo): public IUnk
{
	struct VersionData
	{
		VersionData():version(0), downloaded(false) {}
		std::wstring path;
		unsigned int version;
		bool downloaded;
	};

	/**
	* @brief ����1�����ʹ��������Ƶsdk�ĵ���������Ƶ���У��ұ����������İ汾���ͷ��������İ汾·�� 2�����ʹ��������Ƶsdk�ĵ���������Ƶ���У�������û�������İ汾�����ذ�װ·���е�sdk·��	3�����ʹ���߲�����Ƶsdk�ĵ���������Ƶ���У����ذ�װ·���е�sdk·��.
	* @return .
	*/
	virtual std::wstring recommentedVersionPath() const = 0;

	
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void allVersion(std::vector<VersionData> &v) const = 0;
};