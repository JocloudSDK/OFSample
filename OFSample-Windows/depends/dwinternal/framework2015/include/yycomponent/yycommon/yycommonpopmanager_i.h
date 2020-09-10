#pragma once

#include "yycomponent/yycomclsid.h"
#include "dwbase/dwcomex.h"

/**
* @brief ͨ��root->getServiceInterface<IYYCommonPopManager>(KSERVICECommon)����ȡ.
*/
DWDEFINE_INTERFACE(IYYCommonPopManager) : public IDWQComponent
{
	/**
	* @brief . ͨ������������
	* @param .
	* @return .
	*/
	virtual void clear() = 0;
	/**
	* @brief . �õ�������ʾ������
	* @param .
	* @return. 
	*/
	virtual QRect workArea() = 0;
	/**
	* @brief . ��������ϱ�ϵͳ�ƴ������Ϣ���汾��4.15����Ҫ��4.15�汾�ռ���ʹ�ô˽ӿ�
	* @param .
	* @return.
	*/
	virtual void sendUrlClickNotify(const UINT32 msgid, const UINT32 uid) = 0; 
	/**
	* @brief .��������ϱ�UIͳ����Ϣ����Ҫ���õ�ʱ��һ���ڴ�����ʾ��ʱ����ã��ӿڼ���汾��5.0
	* @param .
	* @return.
	*/
	virtual void markMsgSuccess(const UINT32 msgid, const UINT32 uid) = 0; 
	/*
	signals:
	void sigPopMsgArrived( const std::wstring& clsId, const std::wstring& xml );
	*/
};