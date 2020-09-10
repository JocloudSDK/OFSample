#pragma once

#include "dwbase/dwcomex.h"

/**
  * @brief YY����ҳ��ӿڣ���ʾ��������ҳ�桢ͷ������ҳ�棩, use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYSettingFrameWrap) : public IDWQComponent
{
	/**
	* @brief ��ʾ�������ÿ�.
	* @param activePageName Ĭ����ʾ��ҳ.
	*/
	virtual void showPersonalSettingFrame( const QString activePageName ) = 0;	

	/**
	* @brief �رո������ÿ�.
	*/
	virtual void closePersonalSettingFrame() = 0;

	/**
	* @brief ��ʾͷ��༭��.
	*/
	virtual void showHeadFaceFrame() = 0;

	/**
	* @brief �ر�ͷ��༭��.
	*/
	virtual void closeHeadFaceFrame() = 0;

	/**
	* @brief ��ʾ��̬ͷ��ѡ���.
	*/
	virtual void showDynamicHeadFaceFrame() = 0;
};
