#pragma once

#include "duifw/duitextobject.h"

namespace TextObjectType
{
	enum
	{
		// ����ΪƵ��/��ɻ�Ʊ��״̬����
		TicketLoading = 0x0,
		TicketNormal = 0x1,
		ChannelTicketLoading = TicketLoading,       // ���ڼ���Ƶ���ɻ�Ʊ
		ChannelTicketNormal = TicketNormal,         // ����Ƶ���ɻ�Ʊ�ɹ�
		ChannelTicketFailed = 0x3,                  // ����Ƶ���ɻ�Ʊʧ��
		ActivityTicketLoading = TicketLoading,      // ���ڼ��ػ�ɻ�Ʊ
		ActivityTicketNormal = TicketNormal,        // ���ػ�ɻ�Ʊ�ɹ�
		ActivityTicketFailed = 0x4,     // ��ȡƵ���ɻ�Ʊʧ��
		TicketLoadingFailed = 0x5,    //�ɻ�Ʊ����ʧ��

		// TextObject������
		ObjectType = DuiTextObject::objectType + 0x100,
		ActivityObjectType = ObjectType + 0x1,              // ��ɻ�Ʊ������ // �����ߣ�yy_buildfast_fb
		ChannelObjectType = ObjectType + 0x2,               // Ƶ���ɻ�Ʊ������
		UrlCheckObjectType = ObjectType + 0x3,				//url��������
		ImageGlitzObjectType = ObjectType + 0x4,			//����
		VoiceMsgObjectType = ObjectType + 0x5,			//��������
		TicketObjectType = ObjectType + 0x6,					//ͳһ�ɻ�Ʊ
		HistoryRecMsgsType = ObjectType + 0x7,				//��Ϣ�������Ƽ���Ϣ
		SubscriptionCenterMsgsType = ObjectType + 0x8,				//Y�ķɻ�Ʊ��Ϣ
		GameCommunityMsgType = ObjectType + 0x9, // ��Ϸ������ȡ�����Ϣ
		SuperSmileMsgType = ObjectType + 0xA,  // ��������
		AtTickObjectType = ObjectType + 0xb,    // @
		BaseHDObjectType = ObjectType + 0xc,    // ������������ɻ�Ʊ
		WonderTabObjectType = ObjectType + 0xd,    // ��������tab�ɻ�Ʊ
		GroupActivityObjectType = ObjectType + 0xe,   // Ⱥ�--��ɧ�ŷɻ�Ʊ��8.45�޸�Ϊ�����ţ�
		ComponentObjectType = DuiTextObject::objectType + 0x200, //�������Edit��ObjectType����СֵΪComponentObjectType + 1

		// Ƶ��������������ͼ��״̬
		LiangHaoIconObjectType = ObjectType + 0x202,  //��������ͼ������
		LiangHaoIconLevel = ObjectType + 0x203,  //��������ͼ��ȼ�
		LiangHaoUserId = ObjectType + 0x204,  //���������û�id

		// ����ΪƵ��/��ɻ�Ʊʹ�õ�����������
		ObjectPropertyIndex = QTextFormat::UserProperty + 0x100,
		ObjectValueIndex = ObjectPropertyIndex + 0x0,       // �����Ա���ɻ�Ʊ����
		ObjectStateIndex = ObjectPropertyIndex + 0x1,       // �����Ա���ɻ�Ʊ״̬����
		ObjectUidIndex = ObjectPropertyIndex + 0x2,         // ����uid
		ObjectIdIndex = ObjectPropertyIndex + 0x3,          // ����Ƶ��id��id
			
		// ����Ϊurl���ʹ�õ�����������
		OjbectUrlValue = QTextFormat::UserProperty + 0x200,		//	����url��ֵ
		ObjectNormalLink = OjbectUrlValue + 0x1,		//	��ͨ��������
		ObjectTooltip = ObjectNormalLink + 0x1,		    //	��ͨ��������
		
		// �����������������
		ObjectHDType = ObjectTooltip + 0x1, // �������

		// ����Ϊ�Ų�������������ͳһ�����ﶨ�壬��ֹ�ظ����壩
		KGlitzText = QTextFormat::UserProperty + 0x300,
		KGlitzFormatId = QTextFormat::UserProperty + 0x301,
		// Ⱥ���Ϣ����
		ObjectGrpActivityMsgType = QTextFormat::UserProperty + 0x380,
		// IMֱ���Ƽ�
		ObjectImliveKeywordData = QTextFormat::UserProperty + 0x390,
		ObjectImliveHitTxt = ObjectImliveKeywordData + 0x1,
	};
}