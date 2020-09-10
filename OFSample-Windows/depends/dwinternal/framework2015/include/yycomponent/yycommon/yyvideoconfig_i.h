#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief ��Ƶ��������.
*YY video config interface��ͨ��root->getServiceInterface<IYYVideoConfig>(KSERVICECommon)��ȡ.
*/
DWDEFINE_INTERFACE(IYYVideoConfig): public IUnk
{
public:
   /**
   * @brief ��Ƶ����ģʽ
   */
	enum VIDEO_MODE
	{ 
		MODE_FLUENCY  = 1, //����
		MODE_DISTINCT = 2,//����
	};

public:

	/**
   * @brief �����豸����.
   */
	virtual void setDeviceName( const std::wstring& deviceName ) = 0;
	
	/**
    * @brief ���ص�ǰ�豸����.
    */
	virtual std::wstring deviceName() = 0;


	/**
    * @brief ������Ƶ����.
    */
	virtual void setVideoMode( VIDEO_MODE mode ) = 0;
	
	/**
    * @brief ���ص�ǰ���õ���Ƶ����.
    */
	virtual VIDEO_MODE videoMode() = 0;

	/**
	* @brief ���ص�ǰ�Ƿ������Ƶ����.
	*/
	virtual bool isVideoSettingFrameOpen() = 0;
};
