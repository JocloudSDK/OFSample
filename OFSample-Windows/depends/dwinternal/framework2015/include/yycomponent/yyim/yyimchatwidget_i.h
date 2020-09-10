#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/yynoticetipwidget_i.h"
#include "yycomponent/yyim/yyimframetipinfomodel_i.h"
#include "duifw/duitextedit.h"

class DuiFrameWindow;
class DuiWidget;
class DuiTextAbstractIntellisenseInput;

DWDEFINE_INTERFACE(IYYImChatWidget) : public IDWQComponent
{
	//���촰�ڵ�λ��
	enum CHATFRAMEPOS
	{
		POS_BEFORE_NICKNAME,
		POS_BEHIND_NICKNAME
	};

    enum SOUND_ALERT_TYPE
    {
        SAT_TEXT = 0x1, //������Ϣ
        SAT_VOICE = 0x2, //������Ϣ
        SAT_OTHER = 0x4, //��������Ϣ��ʾ�����Զ��ظ����ļ�������ɵȵ�
        SAT_1vs1 = 0x8, //1��1�ĻỰ
        SAT_1vsX = 0x10, //1�Զ�ĻỰ
        SAT_VIDEO = 0x20,    // ��Ƶ��Ϣ
    };
	/**
	* @brief . IM�Ի�������������Է���uid
	* @param . ��
	* @return .�Է���uid
	*/
	virtual quint32 peerId() = 0;

	/**
	* @brief . ����IM����Ի������ʵ�Ĵ����ࡣ
	* @param . ��
	* @return. ����Ի������ʵ�����ࡣ
	*/
	virtual DuiWidget* realWidget()	= 0;

	/**
	* @brief . �õ�������ڵ�noticetip widget  �ѷ���
	* @return. noticetipwidget
	*/
	virtual IYYNoticeTipWidgetPtr getNoticeTipWidgetWrap() = 0;
	
	/**
	* @brief . ��IM��������һ��tip.
	* @return. ������tip����ָ��
	*/
	virtual IYYImFrameTipInfoModelPtr appendNoticeTipModel() = 0;

	/**
	* @brief . ͨ��TipInfoModel ��objectname���� ���ҵ���Ӧ��Model����, 
			   Ҳ�ɴ���modulename��ѯ����������β�Ϊmodulenameʱ�����ص��Ǵ�modelname��Ӧ�����ȼ���ߵ�Model����
	* @return. ����tipInfoModel����ָ��
	*/
	virtual IYYImFrameTipInfoModelPtr getNoticeTipModel( const std::wstring& objectname ) = 0;

	/**
	* @brief �û��Լ������Ķ���
	* @brief ����λ��
	*/
	virtual bool insertWidget(DuiWidget* pWidget, CHATFRAMEPOS pos) = 0;
    
    /**
	* @brief �ô�������������˸
	* @brief ��˸���� SOUND_ALERT_TYPE
	*/
    virtual void flashWindow(int msgAlertType) = 0;

    /**
	* @brief ����im���ڵ�Ƥ��
	* @brief Ƥ����·��������ǿձ�ʾʹ�õ�ǰim���õ�Ƥ��
	*/
    virtual void setDecorationImage(const QString& imagePath) = 0;

	/**
	* @brief ����ı�������
	* @brief 
	*/
	virtual void addIntellisenseInput(DuiTextAbstractIntellisenseInput *) = 0;
	virtual void insertIntellisenseInput( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input ) = 0;

	/*
	* @ brief ��ȡ��Ϣ��ʾeditor
	*/
	virtual DuiTextEdit* displayEditor() = 0;

	/**
	* @brief �Ѵ���im p2p video��֪ͨ����imchat����UI
	* @brief
	*/
	virtual void onlyOneVideoInstantError() = 0;

    virtual void removeNoticeTipModel(IYYImFrameTipInfoModelPtr spTipInfo) = 0;
};