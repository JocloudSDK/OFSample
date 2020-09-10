#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duiview.h"
/**
  * @brief �����״̬�ӿ�, ͨ��Root->getServiceInterface<IYYMainFrameState>(KSERVICEMainframe)��ȡ.
*/

DWDEFINE_INTERFACE(IYYMainFrameState) : public IDWQComponent
{
	virtual bool isMainFrameFocus() = 0;//�������Ƿ��Ǽ���״̬���Ƿ���ǰ��
	virtual bool isVisible() = 0;
	virtual bool isMinimized() = 0;
	virtual bool isMaximized() = 0;

	// �ӿ�������״̬������ʾ
	// isMouseLeaveTrigger - ����չ��������뿪��������Ƿ�ᴥ������
	// Ŀǰ�ÿ�ݼ���˫��������YYͼ���������ʱ��Ҫ����false
	virtual bool showFromEdge(bool isMouseLeaveTrigger) = 0;
	virtual bool hideToEdge() = 0; 

	virtual bool addView(DuiView* view) = 0;
	virtual bool isHideInEdge() = 0;		// �Ƿ�������״̬������������������ʾ���㣩
	virtual bool isCloseToEdge() = 0;	// �Ƿ���������ʾ״̬
	virtual bool isHideToEdge() = 0;		// �Ƿ�����������״̬

	virtual QRect realFrameGeometry() = 0;//���ش��ڴ�С

	virtual void setChildBlockDock(bool block) = 0;	// �Ƿ����Ӵ������״̬����ʱ����dockЧ��

	virtual bool isLargeModel() = 0; // �Ƿ������ģʽ

	virtual bool openMiniWonderWorldTab(const QString &joinInfo) = 0; // ������嵼��ҳ����תָ��tabҳ

	virtual quint64 getLoginTick() = 0; // ��ȡ��¼�ɹ�ʱ��
	virtual void switchMainFrame(QWidget *mainframe) = 0; // �������,��ȡmainframe
};
