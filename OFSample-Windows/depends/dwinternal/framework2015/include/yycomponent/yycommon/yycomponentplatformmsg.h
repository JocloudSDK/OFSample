#pragma once

#include "core/packet.h"//sox::Marshallable的定义

const unsigned int KInvalidSeqId = 0xffffffff;
const unsigned int KPlatformReservedUriStart = 0xffffff00;
const unsigned int KPlatformReservedUriEnd = 0xffffffff;

const unsigned int KYYLoginMsgUri = KPlatformReservedUriStart;	//YY登陆
const unsigned int KComponentTrayiconMsgUri = 0xffffff01;	//托盘消息
const unsigned int KComponentTailLightMsgReqUri = 0xffffff02;	//尾灯获取
const unsigned int KComponentTailLightMsgResUri = 0xffffff03;	//尾灯回应
const unsigned int KComponentNoteMsgReqUri	= 0xffffff04;	//小黄条获取
const unsigned int KComponentNoteMsgResUri	= 0xffffff05;	//小黄条回应

/*
以下协议使用与组件平台与组件业务服务器之间
*/

//YY登陆，某些组件需要这个信号，便于业务服务器推送离线内容。
struct PCS_YYLogin2ComponentServer: public sox::Marshallable
{
	enum {uri = KYYLoginMsgUri};
	
	PCS_YYLogin2ComponentServer():uid(0),languageId(0) {}

	virtual void marshal(sox::Pack &p) const
	{
		p << uid << languageId << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uid >> languageId >> expand;
	}

	unsigned int uid;
	unsigned int languageId;
	std::string expand;
};


//获取尾灯信息
struct PCS_GetTailLightDataReq: public sox::Marshallable
{
	enum {uri = KComponentTailLightMsgReqUri};
	PCS_GetTailLightDataReq():pos(0), seq(0), languageId(0) {};
	;
	virtual void marshal(sox::Pack &p) const
	{
		p << pos << uids << seq << languageId << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> pos >> uids >> seq >> languageId >> expand;
	}

	unsigned int pos;
	std::vector<unsigned int> uids;
	unsigned int seq;
	unsigned int languageId;
	std::string expand;
};

//一个尾灯的信息
struct OneTailLightData: public sox::Marshallable
{
	OneTailLightData():pos(0), uid(0), priority(0), actionMask(0) {};

	virtual void marshal(sox::Pack &p) const
	{
		p << pos << uid << priority << actionMask << url << tooltip << text << data << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> pos >> uid >> priority >> actionMask >> url >> tooltip >> text >> data >> expand;
	}

	unsigned int pos;
	unsigned int uid;
	unsigned int priority;
	unsigned int actionMask;	//对哪些动作响应,1：mousePresee，2：mouseHover
	std::string url;
	std::string tooltip;	//服务器请使用utf8格式
	std::string text;		//服务器请使用utf8格式
	std::string data;	//组件平台不关心但组件本身关心的数据，
	std::string expand;
};


//组件业务服务器回应给客户端的尾灯信息
struct PSC_GetTailLightDataRes: public sox::Marshallable
{
	enum {uri = KComponentTailLightMsgResUri};

	PSC_GetTailLightDataRes():pos(0), seq(0) {};
	
	virtual void marshal(sox::Pack &p) const 
	{
		p << pos << seq << data << expand;
	};

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> pos >> seq >> data >> expand;
	}

	unsigned int pos;
	unsigned int seq;
	std::vector<OneTailLightData> data;
	std::string expand;
};

//托盘消息
struct PSC_TrayiconDataRes: public sox::Marshallable
{
	enum {uri = KComponentTrayiconMsgUri};
	virtual void marshal(sox::Pack &p) const
	{
		p << data << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> data >> expand;
	}

	std::string data;
	std::string expand;
};

/*
以下协议是YY组件平台与组件之间的协议，通过飞机票发送
*/

//尾灯启动组件
struct P2C_StartTailLight: public sox::Marshallable
{
	enum 
	{
		MouseClick = 1,
		MouseHover = 2,
		MouseClickJumpURL = 4, //点击并且跳转url
	};
	enum {uri = 1};

	P2C_StartTailLight():action(MouseClick) {};

	virtual void marshal(sox::Pack &p) const
	{
		p << pos << uid << action << data << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p>> pos >> uid >> action >> data >> expand;
	}

	unsigned int pos;	//那个位置的尾灯被点击了
	unsigned int uid;	//是谁
	unsigned int action;	//mouse hover,mouse click
	std::string data;//组件平台不关心但组件本身关心的数据
	std::string expand;
};

//托盘消息
struct P2C_TrayiconData: public PSC_TrayiconDataRes
{
	enum {uri = 2};
};

struct P2C_GameWordMouseEvent: public sox::Marshallable
{
	enum {uri = 3};
	enum 
	{
		MouseClick = 1,
		MouseHover = 2,
		MouseLeave = 3,
	};

	P2C_GameWordMouseEvent()
		:action(MouseClick) 
		,xPos(0)
		,yPos(0)
		,gameId(0)
		,winId(0)
	{
	};

	virtual void marshal(sox::Pack &p) const
	{
		p << action << xPos << yPos << gameId << winId << gameword << expand;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> action >> xPos >> yPos >> gameId >> winId >> gameword >> expand;
	}

	unsigned int action;		  //mouse hover,mouse click
	unsigned int xPos;			  //tip显示位置x屏幕坐标点
	unsigned int yPos;			  //tip显示位置y屏幕坐标点
	unsigned int gameId;		  //游戏id
	unsigned int winId;			  //当前IM窗口的聊天句柄
	std::string gameword; //游戏关键字
	std::string expand;
};