#pragma once

#include "dwbase/dwcomex.h"

class DuiFrameWindow;

DWDEFINE_INTERFACE(IYYGroupMemberListViewWrap) : public IDWQComponent
{
	// Ⱥ��Ĺ����ɫ
	enum GF_ROLE_TYPE
	{
		ROLE_GRP_OW, //Group owner
		ROLE_GRP_ADM, //Group admin
		ROLE_FLD_ADM, //Folder admin
		ROLE_UNKNOWN, //normal value
	};

	/**
	* @brief . �����Ƿ���Ҫ��ʾ�Ҽ��˵�
	* @param . b �Ƿ���Ҫ��ʾ�Ҽ��˵����������Ϊfalse���Ҽ��˵�֪ͨ�󲻻�����ʾ�˵�����
	* @return.
	*/
	virtual void setShowRightClickMemuMenu( bool b ) = 0;
};