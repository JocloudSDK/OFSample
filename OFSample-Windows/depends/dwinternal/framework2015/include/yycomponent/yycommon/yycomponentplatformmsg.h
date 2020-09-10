#pragma once

#include "core/packet.h"//sox::Marshallable�Ķ���

const unsigned int KInvalidSeqId = 0xffffffff;
const unsigned int KPlatformReservedUriStart = 0xffffff00;
const unsigned int KPlatformReservedUriEnd = 0xffffffff;

const unsigned int KYYLoginMsgUri = KPlatformReservedUriStart;	//YY��½
const unsigned int KComponentTrayiconMsgUri = 0xffffff01;	//������Ϣ
const unsigned int KComponentTailLightMsgReqUri = 0xffffff02;	//β�ƻ�ȡ
const unsigned int KComponentTailLightMsgResUri = 0xffffff03;	//β�ƻ�Ӧ
const unsigned int KComponentNoteMsgReqUri	= 0xffffff04;	//С������ȡ
const unsigned int KComponentNoteMsgResUri	= 0xffffff05;	//С������Ӧ

/*
����Э��ʹ�������ƽ̨�����ҵ�������֮��
*/

//YY��½��ĳЩ�����Ҫ����źţ�����ҵ������������������ݡ�
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


//��ȡβ����Ϣ
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

//һ��β�Ƶ���Ϣ
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
	unsigned int actionMask;	//����Щ������Ӧ,1��mousePresee��2��mouseHover
	std::string url;
	std::string tooltip;	//��������ʹ��utf8��ʽ
	std::string text;		//��������ʹ��utf8��ʽ
	std::string data;	//���ƽ̨�����ĵ����������ĵ����ݣ�
	std::string expand;
};


//���ҵ���������Ӧ���ͻ��˵�β����Ϣ
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

//������Ϣ
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
����Э����YY���ƽ̨�����֮���Э�飬ͨ���ɻ�Ʊ����
*/

//β���������
struct P2C_StartTailLight: public sox::Marshallable
{
	enum 
	{
		MouseClick = 1,
		MouseHover = 2,
		MouseClickJumpURL = 4, //���������תurl
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

	unsigned int pos;	//�Ǹ�λ�õ�β�Ʊ������
	unsigned int uid;	//��˭
	unsigned int action;	//mouse hover,mouse click
	std::string data;//���ƽ̨�����ĵ����������ĵ�����
	std::string expand;
};

//������Ϣ
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
	unsigned int xPos;			  //tip��ʾλ��x��Ļ�����
	unsigned int yPos;			  //tip��ʾλ��y��Ļ�����
	unsigned int gameId;		  //��Ϸid
	unsigned int winId;			  //��ǰIM���ڵ�������
	std::string gameword; //��Ϸ�ؼ���
	std::string expand;
};