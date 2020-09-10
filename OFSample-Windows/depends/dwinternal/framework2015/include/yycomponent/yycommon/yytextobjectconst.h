#pragma once

#include "duifw/duitextobject.h"

namespace TextObjectType
{
	enum
	{
		// 以下为频道/活动飞机票的状态类型
		TicketLoading = 0x0,
		TicketNormal = 0x1,
		ChannelTicketLoading = TicketLoading,       // 正在加载频道飞机票
		ChannelTicketNormal = TicketNormal,         // 加载频道飞机票成功
		ChannelTicketFailed = 0x3,                  // 加载频道飞机票失败
		ActivityTicketLoading = TicketLoading,      // 正在加载活动飞机票
		ActivityTicketNormal = TicketNormal,        // 加载活动飞机票成功
		ActivityTicketFailed = 0x4,     // 获取频道飞机票失败
		TicketLoadingFailed = 0x5,    //飞机票加载失败

		// TextObject的类型
		ObjectType = DuiTextObject::objectType + 0x100,
		ActivityObjectType = ObjectType + 0x1,              // 活动飞机票的类型 // 已下线，yy_buildfast_fb
		ChannelObjectType = ObjectType + 0x2,               // 频道飞机票的类型
		UrlCheckObjectType = ObjectType + 0x3,				//url检测的类型
		ImageGlitzObjectType = ObjectType + 0x4,			//炫字
		VoiceMsgObjectType = ObjectType + 0x5,			//语音留言
		TicketObjectType = ObjectType + 0x6,					//统一飞机票
		HistoryRecMsgsType = ObjectType + 0x7,				//消息管理器推荐消息
		SubscriptionCenterMsgsType = ObjectType + 0x8,				//Y阅飞机票消息
		GameCommunityMsgType = ObjectType + 0x9, // 游戏公社领取礼包消息
		SuperSmileMsgType = ObjectType + 0xA,  // 超级表情
		AtTickObjectType = ObjectType + 0xb,    // @
		BaseHDObjectType = ObjectType + 0xc,    // 基础活动及荣誉飞机票
		WonderTabObjectType = ObjectType + 0xd,    // 精彩世界tab飞机票
		GroupActivityObjectType = ObjectType + 0xe,   // 群活动--跑骚团飞机票（8.45修改为富豪团）
		ComponentObjectType = DuiTextObject::objectType + 0x200, //组件插入Edit的ObjectType的最小值为ComponentObjectType + 1

		// 频道公屏皇族靓号图标状态
		LiangHaoIconObjectType = ObjectType + 0x202,  //皇族靓号图标类型
		LiangHaoIconLevel = ObjectType + 0x203,  //皇族靓号图标等级
		LiangHaoUserId = ObjectType + 0x204,  //皇族靓号用户id

		// 以下为频道/活动飞机票使用到的属性索引
		ObjectPropertyIndex = QTextFormat::UserProperty + 0x100,
		ObjectValueIndex = ObjectPropertyIndex + 0x0,       // 该属性保存飞机票链接
		ObjectStateIndex = ObjectPropertyIndex + 0x1,       // 该属性保存飞机票状态类型
		ObjectUidIndex = ObjectPropertyIndex + 0x2,         // 保存uid
		ObjectIdIndex = ObjectPropertyIndex + 0x3,          // 保存频道id或活动id
			
		// 以下为url检测使用到的属性索引
		OjbectUrlValue = QTextFormat::UserProperty + 0x200,		//	保存url的值
		ObjectNormalLink = OjbectUrlValue + 0x1,		//	普通链接属性
		ObjectTooltip = ObjectNormalLink + 0x1,		    //	普通链接属性
		
		// 基础活动类型属性索引
		ObjectHDType = ObjectTooltip + 0x1, // 活动或荣誉

		// 以下为炫彩字属性索引（统一到这里定义，防止重复定义）
		KGlitzText = QTextFormat::UserProperty + 0x300,
		KGlitzFormatId = QTextFormat::UserProperty + 0x301,
		// 群活动消息类型
		ObjectGrpActivityMsgType = QTextFormat::UserProperty + 0x380,
		// IM直播推荐
		ObjectImliveKeywordData = QTextFormat::UserProperty + 0x390,
		ObjectImliveHitTxt = ObjectImliveKeywordData + 0x1,
	};
}