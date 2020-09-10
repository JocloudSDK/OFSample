#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yyim/yyimchatwidget_i.h"

/*
 * ��ͨ�� getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup)
*/
DWDEFINE_INTERFACE(IYYImChatFrameManager) : public IDWQComponent
{
/**
   * @brief . ����ϵ��uid������������������ӿ�
   * @param . uid �û�uid
   * @return . IYYImChatWidgetPtr �û���������wrap����ӿ�
   */
	virtual IYYImChatWidgetPtr findImWidget( quint32 uid ) = 0;
/**
   * @brief . �ö��������
   * @param . ��
   * @return. IYYImChatWidgetPtr �û���������wrap����ӿ�
   */
	virtual IYYImChatWidgetPtr foregroundWnd( void ) = 0;
/**
   * @brief . �Ƿ��������ϵ���д������
   * @param . uid �û�uid
   * @return. true ��ʾ�Ѿ�������򣬷���û�д�
   */
	virtual bool isOpenChatWnd( quint32 uid ) =	0;
/**
   * @brief . �����˶��ٸ������
   * @param . ��
   * @return. ���ش�����������
   */ 
	virtual int  chatWndCount() = 0;

    /*
    * ����IM���ڣ�����Ѿ����ڣ�ֱ�ӷ���
    */
    virtual IYYImChatWidgetPtr createImWidget(quint32 uid) = 0;

	/*
	* ����IM���ڣ�����Ѿ����ڣ�ֱ�ӷ���
	*/
	virtual QList<IYYImChatWidgetPtr> allImChatFrames() = 0;

    virtual QList<IYYImChatWidgetPtr> foregrounds() const = 0;

    /** 
	signals:
		void imWidgetCreated(quint32 user_id);
		void imWidgetDestoryed(quint32 user_id);
    */
};
