#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

class DuiFrameWindow;
class DuiWidget;

DWDEFINE_INTERFACE(IChannelPrivateChatAutoReply) : public IUnk 
{
	/**
	* @brief �����Զ��ظ�����.
	* @return �Զ��ظ�����ָ��.
	*/
	virtual DuiFrameWindow*	create(DuiWidget* parent) = 0;

	/**
   * @brief ��ʾ����.
   * @param relateWidget���Զ��ظ����ڻ�����������ָ��Ĵ����Զ������ں��ʵ�λ������ʾ����.
   */
	virtual void show( DuiWidget* relateWidget ) = 0;

	/**
   * @brief ���ش���.
   */
	virtual void hide(void) = 0;
};