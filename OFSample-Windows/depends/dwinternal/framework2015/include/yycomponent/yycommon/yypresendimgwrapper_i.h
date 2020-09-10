#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duitextcursor.h" 
#include "duifw/duitextedit.h"

enum CHECK_IMAGE_LIMIT
{
	CHECK_IMAGE_LIMIT_OK,       // ���Ϸ�������
	CHECK_IMAGE_LIMIT_SIZE,     // �ֽڴ�С����
	CHECK_IMAGE_LIMIT_WIDTH,    // ��ȳ���
	CHECK_IMAGE_LIMIT_HEIGHT,   // �߶ȳ���
	CHECK_IMAGE_LIMIT_NOFILE,   // Ҫ�����ļ�������
};

#define IMAGE_FILE_SIZE_LIMIT_SIZE    1048576  // 1M
#define IMAGE_FILE_SIZE_LIMIT_HEIGHT  4000
#define IMAGE_FILE_SIZE_LIMIT_WIDTH   4000

const char dyimg_begin[]	= "[dyimg]";
const char dyimg_end[]		= "[/dyimg]";
const char dysnd_begin[]	= "[dysnd]"; //dyvoice�޸ĳɺ��ֻ���ʽһ���ĸ�ʽdysnd
const char dysnd_end[]		= "[/dysnd]";
const int KImageCountLimitInMsg = 5;

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYPreSendImageWrapper): public IDWQComponent
{
	/**
	* @brief ��ͼƬ�ļ����ص��Ű����.
	* @param strFile ͼƬ�ļ�·��.
	* @param textCursor ������α�.
	* @return. 
	*/
	virtual void loadIntoEdit(const QString& strFile, DuiTextCursor &textCursor) = 0;

	/**
	* @brief �����Զ�����鵽duitextedit��.
	* @param strFile ͼƬ�ļ�·��.
	* @param strImgUrl ͼƬ�ļ�url.
	* @return.
	*/ 
	virtual void insertSelfSmileToEdit(const QString& strFile, const QString& strImgUrl) = 0;

	/**
	* @brief ͼƬ���ͳ�����Ϣ����.
	* @param limit ������.
	* @return ��������������Ϣ.
	*/
	virtual QString checkImageLimitStr(CHECK_IMAGE_LIMIT limit) = 0;

	/**
	* @brief ��ָ������ͼƬ�ļ�ת����Э���ʽ.
	* @param strFile ͼƬ�ļ�·��.
	* @param strImgUrl ͼƬ�ļ�url.
	* @param [output] strNewFile ת�����Э���ʽ.
	* @return.
	*/
	virtual void parserToDyimg(const QString& strFile, QString& strImgUrl, QString& strNewFile) = 0;
	/**
	* @brief ��ʼ������.
	* @param pEdit �󶨵ı༭��.
	* @param pParentWnd ������.
	* @return.
	*/ 
	virtual void init(DuiTextEdit* pEdit, QWidget* pParentWnd) = 0;

   /*  
   @brief ��ȡ��ͼ��������
   @param winId ��ͼ�ı���·��.
   @return ��ͼ����������
   */
   virtual CHECK_IMAGE_LIMIT checkImageLimit(const QString& filename) = 0;

};