#pragma once

#include "dwbase/dwcomex.h"
#include "dwbase/dwcominterface_i.h"
#include "yycomponent/yycomclsid.h"

class DuiTextEdit;

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYScreenCapture): public IDWQComponent
{
/**
   * @brief 截屏操作初始化.
   * @param textEdit 截屏后作用于的编辑框.
   * @param pParentWnd 父窗口.
   * @return .
   */
    virtual void init(DuiTextEdit *textEdit, QWidget* pParentWnd) = 0;
/**
   * @brief 截屏操作.
   * @return.
   */
    virtual void captureScreen() = 0;

  /*  
	 @brief 截屏设置窗口id.
	 @param winId 截屏时的主窗口id.
	 @return.
 */
	virtual void setWindowId(HWND winId) = 0;
  
/*  
	 @brief 获取截图路径
	 @param winId 截图的本地路径.
	 @return.
 */
	virtual void getImagePath(QString &imagePath) = 0;

/*
signals:
	void screenCaptureSuccess();
    void screenCaptureFailed();
*/
};