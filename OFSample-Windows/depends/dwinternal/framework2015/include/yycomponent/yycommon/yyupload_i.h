#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/**
  * @brief 主题切换, 通过Root->getServiceInterface<IYYUpload>(KSERVICECommon)获取.
  */
DWDEFINE_INTERFACE(IYYUpload): public IUnk
{

	/**
   * @brief 上传图片.
   * @param imagePath 图片的绝对路径.
   * @param format  图片的格式( "png", "jpg"等， 详见QT Assisant: Reading and Writing Image Files ).
   * @param imageType 这个类型需要与服务器商量.皮肤的类型为　"skin".
   * @return 发起上传成功　返回true, 否则　false 上传结果通过　_SIG_UPLOADIMAGE_RES　返回.
   */
	virtual bool uploadImage( const std::wstring& imagePath, const char* format, const char* imageType, unsigned int sessionId ) = 0;	

	/**
   * @brief 上传图片.
   * @param imagePath 图片的绝对路径.
   * @param format  图片的格式( "png", "jpg"等， 详见QT Assisant: Reading and Writing Image Files ).
   * @param imageType 这个类型需要与服务器商量.皮肤的类型为　"skin".
   * @return 发起上传成功　返回true, 否则　false 上传结果通过　_SIG_UPLOADIMAGE_RES　返回.
   */
	virtual bool uploadCustomizeSkin( const std::wstring& imagePath, const char* format, bool bShow ) = 0;

	/**
   * @brief 返回完成图片上传的回调信号.
   * @param bool true 上传成功,否则失败.
   * @param imagePath 上传的图片路径.
   * @param url 上传后的url路径.
   * @return .
   */
	typedef _def_boost_signals2_signal_type<void (bool, const std::wstring& imagePath, const std::wstring& url)>::type _SIG_UPLOADIMAGE_RES;
	virtual _SIG_UPLOADIMAGE_RES *uploadImageResSignal() = 0;
};

