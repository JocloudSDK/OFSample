#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#define KDefaultSkinPageName "normalSkin"
#define KVipSkinPageName     "vipSkin"
/**
  * @brief �����л�, ͨ��Root->getServiceInterface<IYYThemeColorFrame>(KSERVICEMainframe)��ȡ.
  */
class DuiWidget;
class QPoint;
class DuiFrameWindow;

DWDEFINE_INTERFACE(IYYThemeColorFrame) : public IDWQComponent
{
	/**
   * @brief ��ʾƤ�����ô���.activePageName�ǵ�ǰ����active״̬��ҳ,������ֻ����ʾƤ������
   * @return ��.
   */
	virtual void showThemeColorFrame( const std::wstring& activePageName ) = 0;

	/**
   * @brief �����Զ���Ƥ����ť, button ������ͷ�
   * @return ��.
   */
	virtual bool setCustomizeButton( DuiWidget* button ) = 0;

	/**
   * @brief ����Ƥ�����ô������widget
   * @return ��.
   */
	virtual DuiWidget* entranceWidget() = 0;

	/**
   * @brief ��ȡƤ������,�����ǰû�д�Ƥ�����ڣ��᷵��NULL
   * @return DuiFrameWindow*.
   */
	virtual DuiFrameWindow* getThemeColorFrame(void) = 0;
};