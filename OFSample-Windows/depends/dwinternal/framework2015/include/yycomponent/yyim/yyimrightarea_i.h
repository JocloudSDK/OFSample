#pragma once

#include "dwbase/dwcomex.h"

class DuiWidget;

//��ҵ����ʾ���ȼ�
/*
*�ͷ� 0XF,������ȼ�
*��Ե 0xFF
*Ƶ��� 0XFFFF
*ʱװ�� Ĭ��0xFFF,���ò�������ʾ����0xFFFF
*��Ϸ��ɫ 0xFFFF
ʹ�÷�����
IYYImChatFrameManagerPtr frameMgr = comRoot->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
if(NULL == frameMgr)
	return;
IYYImChatWidgetPtr chatFrame = frameMgr->findImWidget(userId);
if(NULL == chatFrame)
	return;
IYYImRightAreaPtr rightMgr;
if ( !chatFrame->queryInterface(rightMgr) )
{
	DW_ASSERT(false);
	return;
}
*/
DWDEFINE_INTERFACE(IYYImRightArea) : public IDWQComponent
{
	/*
	* ������չʾ����һ��ҳ��,������ʾnPriorityС��ҳ�棬ͬһ��ֵ�ֲ���ʾ����ͨҳ����0xFFFF��
	* 8.4���ϣ�����8.4����֧������ӿ���.
	*/
	virtual void insertBuddyViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/*
	* ��չʾ�Լ���������һ��ҳ��,������ʾnPriorityС��ҳ�棬ͬһ��ֵ�ֲ���ʾ����ͨҳ����0xFFFF��
	* 8.4���ϣ�����8.4����֧������ӿ���.
	*/
	virtual void insertMyViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/*
	*ͨ��xml����page,���xml�к���DuiTextEdit,Ӧ���������������
	* strObjectName�Ƿ��ض�Ӧ��widget��widget.
	* 8.4���ϣ�����8.4����֧������ӿ���.
	*/
	virtual DuiWidget* insertBuddyViewPageEx(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;
	virtual DuiWidget* insertMyViewPageEx(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;

    /*
    * ȥ��һ��widget,���ڽӿ�����ɾ��
    */
    virtual void removeBuddyWidget(DuiWidget* page) = 0;
    virtual void removeMyWidget(DuiWidget* page) = 0;

    /*
    *  �������ȼ�,8.4���ϣ�����8.4����֧������ӿ���.
    */
    virtual void setBuddyWidgetPriority(DuiWidget* widget, UINT32 nPriority) = 0;
    virtual void setMyWidgetPriority(DuiWidget* widget, UINT32 nPriority) = 0;

    /*
    *  ����չʾ����ĸ߶�,8.4���ϣ�����8.4����֧������ӿ���.
    *  ��Ӧ�����ź�
    *  signals:
    *      void myViewHeightUpdate(int newHeight, int oldHeight);
    *      void buddyViewHeightUpdate(int newHeight, int oldHeight);
    */
    virtual void setMyWidgetMinHeight(DuiWidget* widget, int minHeight) = 0;

	/**
	  * @brief ��Y������������һ��ҳ�棬�汾:8.4��
	  * @param page Ҫ���ӵ�ҳ�档
	  * @param nPriority ������ʾnPriorityС��ҳ�棬ͬһ��ֵ�ֲ���ʾ����ͨҳ����0xFFFF��
	  * @return  true��ʾ�ɹ���false��ʾʧ�ܡ�
	  */
	virtual bool insertPersonViewPage(DuiWidget* page, UINT32 nPriority) = 0;

	/**
	  * @brief ��Y��������,ͨ��xml����page,���xml�к���DuiTextEdit,Ӧ����������������汾:8.4��
	  * @param strPath Ҫ���ӵ�ҳ�档
	  * @param strObjectName ���ض�Ӧ��widget��widget��
	  * @param nPriority ������ʾnPriorityС��ҳ�棬ͬһ��ֵ�ֲ���ʾ����ͨҳ����0xFFFF��
	  * @return  ��0��ʾ�ɹ���NULL��ʾʧ�ܡ�
	  */
	virtual DuiWidget* insertPersonViewPageByXml(std::wstring& strPath, std::wstring& strObjectName, UINT32 nPriority) = 0;

	/**
	  * @brief ��Y��������ɾ��һ��ҳ�棬�汾:8.4��
	  * @param page Ҫɾ����ҳ�档
	  * @return  true��ʾ�ɹ���false��ʾʧ�ܡ�
	  */
	virtual bool removePersonViewPage(DuiWidget* page) = 0;

	/**
	  * @brief ����ҳ�����ȼ����汾:8.4��
	  * @param page Ҫ���õ�ҳ�档
	  * @return  true��ʾ�ɹ���false��ʾʧ�ܡ�
	  */
	virtual bool setPersonViewPagePriority(DuiWidget* page, UINT32 nPriority) = 0;
};