#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiFrameWindow;
/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYPopManager) : public IUnk
{
/**
   * @brief ��ʾ�Զ������ݣ������½���ʾ������popFrame���Զ���һ��objectname���ֶ��ر�����ʱ��Ҫ�õ���.
   * @param popFrame ���ݴ���.
   * @param interval �����Զ��˳���ʱ�䣬Ĭ��-1Ϊ���Զ��˳�.
   * @return. 
   */
	virtual void show( DuiFrameWindow* popFrame, int interval/* = -1 */) = 0;

/**
   * @brief �ر��Զ�������.
   * @param objectName ���ݴ��ڵ�objectName.
   * @return.
   */
	virtual void hide( const QString& objectName ) = 0;

/**
   * @brief �ر��������ݴ���.
   * @return.
   */
	virtual void clear() = 0;
};