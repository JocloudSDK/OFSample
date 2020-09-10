#pragma once

define_interface(IChannel);
DWDEFINE_INTERFACE(IYYChannelComInfoFetcherInner) : public IUnk
{
    virtual void setChannel(IChannelPtrCR channelPtr) = 0;
    virtual void setJoinStyle(int style) = 0;
    virtual void channelJoined() = 0;

	/*
	 * ���ⲿ�û�ɾ��/����Ƶ���������Ľӿڣ���������ģ���ֱ���Ƴ���Ϸ�׼�����ʹcominfo�����global component idû��
	 * ͬ���Ƴ������ӣ���ͬ���������������л���Ƶ��ʱ�п��ܲ�������unload��load��Щ������
	*/
	virtual void bulletinComponentRemove(quint32 id) = 0;
	virtual void bulletinComponentAdd(quint32 id, quint32 type) = 0;
	virtual void setJoinFrom(const std::string &) = 0;
};