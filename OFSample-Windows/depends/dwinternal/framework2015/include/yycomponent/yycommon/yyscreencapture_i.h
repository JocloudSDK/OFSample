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
   * @brief ����������ʼ��.
   * @param textEdit �����������ڵı༭��.
   * @param pParentWnd ������.
   * @return .
   */
    virtual void init(DuiTextEdit *textEdit, QWidget* pParentWnd) = 0;
/**
   * @brief ��������.
   * @return.
   */
    virtual void captureScreen() = 0;

  /*  
	 @brief �������ô���id.
	 @param winId ����ʱ��������id.
	 @return.
 */
	virtual void setWindowId(HWND winId) = 0;
  
/*  
	 @brief ��ȡ��ͼ·��
	 @param winId ��ͼ�ı���·��.
	 @return.
 */
	virtual void getImagePath(QString &imagePath) = 0;

/*
signals:
	void screenCaptureSuccess();
    void screenCaptureFailed();
*/
};