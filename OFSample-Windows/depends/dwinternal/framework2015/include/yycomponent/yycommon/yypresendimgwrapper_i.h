#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duitextcursor.h" 
#include "duifw/duitextedit.h"

enum CHECK_IMAGE_LIMIT
{
	CHECK_IMAGE_LIMIT_OK,       // 符合发送条件
	CHECK_IMAGE_LIMIT_SIZE,     // 字节大小超标
	CHECK_IMAGE_LIMIT_WIDTH,    // 宽度超标
	CHECK_IMAGE_LIMIT_HEIGHT,   // 高度超标
	CHECK_IMAGE_LIMIT_NOFILE,   // 要检查的文件不存在
};

#define IMAGE_FILE_SIZE_LIMIT_SIZE    1048576  // 1M
#define IMAGE_FILE_SIZE_LIMIT_HEIGHT  4000
#define IMAGE_FILE_SIZE_LIMIT_WIDTH   4000

const char dyimg_begin[]	= "[dyimg]";
const char dyimg_end[]		= "[/dyimg]";
const char dysnd_begin[]	= "[dysnd]"; //dyvoice修改成和手机格式一样的格式dysnd
const char dysnd_end[]		= "[/dysnd]";
const int KImageCountLimitInMsg = 5;

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYPreSendImageWrapper): public IDWQComponent
{
	/**
	* @brief 把图片文件加载到排版框内.
	* @param strFile 图片文件路径.
	* @param textCursor 插入的游标.
	* @return. 
	*/
	virtual void loadIntoEdit(const QString& strFile, DuiTextCursor &textCursor) = 0;

	/**
	* @brief 插入自定义表情到duitextedit内.
	* @param strFile 图片文件路径.
	* @param strImgUrl 图片文件url.
	* @return.
	*/ 
	virtual void insertSelfSmileToEdit(const QString& strFile, const QString& strImgUrl) = 0;

	/**
	* @brief 图片发送出错信息翻译.
	* @param limit 出错码.
	* @return 中文描述出错信息.
	*/
	virtual QString checkImageLimitStr(CHECK_IMAGE_LIMIT limit) = 0;

	/**
	* @brief 给指定发送图片文件转换成协议格式.
	* @param strFile 图片文件路径.
	* @param strImgUrl 图片文件url.
	* @param [output] strNewFile 转换后的协议格式.
	* @return.
	*/
	virtual void parserToDyimg(const QString& strFile, QString& strImgUrl, QString& strNewFile) = 0;
	/**
	* @brief 初始化操作.
	* @param pEdit 绑定的编辑框.
	* @param pParentWnd 父窗口.
	* @return.
	*/ 
	virtual void init(DuiTextEdit* pEdit, QWidget* pParentWnd) = 0;

   /*  
   @brief 获取截图限制类型
   @param winId 截图的本地路径.
   @return 截图的限制类型
   */
   virtual CHECK_IMAGE_LIMIT checkImageLimit(const QString& filename) = 0;

};