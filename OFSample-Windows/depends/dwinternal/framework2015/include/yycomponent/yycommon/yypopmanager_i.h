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
   * @brief 显示自定义泡泡，在右下角显示出来（popFrame会自动给一个objectname，手动关闭泡泡时需要用到）.
   * @param popFrame 泡泡窗口.
   * @param interval 泡泡自动退出的时间，默认-1为不自动退出.
   * @return. 
   */
	virtual void show( DuiFrameWindow* popFrame, int interval/* = -1 */) = 0;

/**
   * @brief 关闭自定义泡泡.
   * @param objectName 泡泡窗口的objectName.
   * @return.
   */
	virtual void hide( const QString& objectName ) = 0;

/**
   * @brief 关闭所有泡泡窗口.
   * @return.
   */
	virtual void clear() = 0;
};