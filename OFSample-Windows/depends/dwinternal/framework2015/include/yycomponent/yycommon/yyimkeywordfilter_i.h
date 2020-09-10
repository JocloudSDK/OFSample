#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYImKeywordFilter) : public IUnk
{
	enum FILTER_RESULT
	{
		FILTER_NO_INCLUDE		= 0,    // ��������
		FILTER_FORBID_SUBMIT	= 1, // ��������
		FILTER_REPLACE_SEND		= 2  // �����ͣ������еĹؼ����Ѿ����滻��
	};

/**
   * @brief �Ƿ��������.
   * @param message ������Ϣ����.
   * @param param ������Ϣ���ݲ���.
   * @return ��������.
   */
	virtual FILTER_RESULT filter( QString& message, const QString& param ) = 0;
};