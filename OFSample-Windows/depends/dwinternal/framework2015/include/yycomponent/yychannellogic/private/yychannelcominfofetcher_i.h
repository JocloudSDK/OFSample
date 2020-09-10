#pragma once

define_interface(IChannel);
DWDEFINE_INTERFACE(IYYChannelComInfoFetcherInner) : public IUnk
{
    virtual void setChannel(IChannelPtrCR channelPtr) = 0;
    virtual void setJoinStyle(int style) = 0;
    virtual void channelJoined() = 0;

	/*
	 * 供外部用户删除/增加频道公告插件的接口（例如娱乐模板会直接移除游戏套件），使cominfo里面的global component id没有
	 * 同步移除或增加，不同步引起的问题就是切换子频道时有可能不再正常unload或load这些插件组件
	*/
	virtual void bulletinComponentRemove(quint32 id) = 0;
	virtual void bulletinComponentAdd(quint32 id, quint32 type) = 0;
	virtual void setJoinFrom(const std::string &) = 0;
};