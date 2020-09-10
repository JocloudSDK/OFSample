#pragma once

#include "dwbase/dwcomex.h"

class DuiFrameWindow;

DWDEFINE_INTERFACE(IYYGroupMemberListViewWrap) : public IDWQComponent
{
	// 群组的管理角色
	enum GF_ROLE_TYPE
	{
		ROLE_GRP_OW, //Group owner
		ROLE_GRP_ADM, //Group admin
		ROLE_FLD_ADM, //Folder admin
		ROLE_UNKNOWN, //normal value
	};

	/**
	* @brief . 设置是否需要显示右键菜单
	* @param . b 是否需要显示右键菜单，如果设置为false，右键菜单通知后不会再显示菜单出来
	* @return.
	*/
	virtual void setShowRightClickMemuMenu( bool b ) = 0;
};