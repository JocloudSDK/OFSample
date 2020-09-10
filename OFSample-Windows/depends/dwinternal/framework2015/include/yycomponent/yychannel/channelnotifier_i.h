#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IChannelNotifier): public IUnk
{
	enum UIPosition
	{
		MiniCard = 1,
		IMUserList = 2,
		IMFrame = 4,
	};

	/*
	@bref. text		显示到UI界面的字符串
		   iconUrl	显示在字符串前面的小图标
		   priority 字符串在 pos 位置上显示的优先级, (游戏组队  TIP_PRIORITY_GAMEPARTY  = 100 )
		   pos      需要显示在哪些位置上 .是UIPosition的一个并集
		   data     透传给组件的数据
    */
	virtual void enterChannel( const std::string& text, const std::string& iconUrl,  int priority, unsigned int pos, const std::string& data ) = 0;
	virtual void leaveChannel() = 0;

	//同enterChannel一样，因为编码问题，所以加了接口
	virtual void enterChannel2( const std::wstring& text, const std::wstring& iconUrl,  int priority, unsigned int pos, const std::string& data ) = 0;
};