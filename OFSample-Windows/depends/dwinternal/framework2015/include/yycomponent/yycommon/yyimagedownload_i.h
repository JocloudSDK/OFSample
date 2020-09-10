#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYImageDownLoad): public IDWQComponent
{
	/**
	* @brief 下载聊天消息中的图片，，注意strKind都不会去修改，原原本本回传给调用者,调用者用于判断结果回调是否属于自己关心的.
	* @param strKind  任务类型.
	* @param strMsg YY消息，里面包含了一到多个图片的下载地址.
	* @param strSavePath 保存目录.
	*/
	virtual void DownloadImageInYYMsg(UINT32 sendTime, const QString& strKind, const QString& strMsg) = 0;

	/**
	* @brief 增加一个聊天图片下载任务（包括截图、发图片和自定义表情），注意strTaskID和strKind都不会去修改，原原本本回传给调用者,调用者用于判断结果回调是否属于自己关心的.
	* @param strTaskID 任务ID.
	* @param strKind 任务类型.
	* @param strURL 下载地址.
	* @param strSavePath 保存目录.
	*/
	virtual void AddChatImageTask(const QString& strTaskID, const QString& strKind, const QString& strURL, const QString& strSavePath, bool useTcp=true)  = 0;

	/**
	* @brief 给定一个文件，进行下载, strTaskID和strKind都不会去修改，原原本本回传给调用者,调用者用于判断结果回调是否属于自己关心的.
	* @param strTaskID 任务ID.
	* @param strKind 任务类型.
	* @param strURL 下载地址.
	* @param strSavePath 保存目录.
	*/
	virtual void AddFileTask(const QString& strTaskID, const QString& strKind, const QString& strURL, const QString& strSavePath, bool useIECache=true)  = 0;

	/*
	signals:
	void imageDownloadResult(const QString &strTaskID, const QString &strKind, const QString &strURL, const QString& strImgFile, bool bSucc); //下载结果通知
	*/
};