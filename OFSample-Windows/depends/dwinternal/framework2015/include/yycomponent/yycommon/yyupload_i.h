#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/**
  * @brief �����л�, ͨ��Root->getServiceInterface<IYYUpload>(KSERVICECommon)��ȡ.
  */
DWDEFINE_INTERFACE(IYYUpload): public IUnk
{

	/**
   * @brief �ϴ�ͼƬ.
   * @param imagePath ͼƬ�ľ���·��.
   * @param format  ͼƬ�ĸ�ʽ( "png", "jpg"�ȣ� ���QT Assisant: Reading and Writing Image Files ).
   * @param imageType ���������Ҫ�����������.Ƥ��������Ϊ��"skin".
   * @return �����ϴ��ɹ�������true, ����false �ϴ����ͨ����_SIG_UPLOADIMAGE_RES������.
   */
	virtual bool uploadImage( const std::wstring& imagePath, const char* format, const char* imageType, unsigned int sessionId ) = 0;	

	/**
   * @brief �ϴ�ͼƬ.
   * @param imagePath ͼƬ�ľ���·��.
   * @param format  ͼƬ�ĸ�ʽ( "png", "jpg"�ȣ� ���QT Assisant: Reading and Writing Image Files ).
   * @param imageType ���������Ҫ�����������.Ƥ��������Ϊ��"skin".
   * @return �����ϴ��ɹ�������true, ����false �ϴ����ͨ����_SIG_UPLOADIMAGE_RES������.
   */
	virtual bool uploadCustomizeSkin( const std::wstring& imagePath, const char* format, bool bShow ) = 0;

	/**
   * @brief �������ͼƬ�ϴ��Ļص��ź�.
   * @param bool true �ϴ��ɹ�,����ʧ��.
   * @param imagePath �ϴ���ͼƬ·��.
   * @param url �ϴ����url·��.
   * @return .
   */
	typedef _def_boost_signals2_signal_type<void (bool, const std::wstring& imagePath, const std::wstring& url)>::type _SIG_UPLOADIMAGE_RES;
	virtual _SIG_UPLOADIMAGE_RES *uploadImageResSignal() = 0;
};

