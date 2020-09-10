#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duitextedit.h"

/**
* @brief 通过root->getServiceInterface<IChatImageDownLoadStrategy>(KSERVICECommon)来获取.
*/
DWDEFINE_INTERFACE(IChatImageDownLoadStrategy) : public IDWQComponent
{
	/**
	* @brief . 设置textedit
	* @param . 
	* @return .
	*/
	virtual void setTextEdit( DuiTextEdit* textEdit ) = 0;
	/**
	* @brief . 插入等待gif图片位置
	* @param .
	* @return .
	*/
	virtual void insertWaitGifPosition(int nPosition, const QString& strKey, bool bVoice) =0; //bVoice是否声音留言
	/**
	* @brief . 插入等待gif图片位置
	* @param .
	* @return .
	*/
	virtual void insertWaitGifPosition2(int nPosition, const QString& url, bool bVoice)=0; //bVoice是否声音留言
	/**
	* @brief . 提取编辑框内需要下载的图片url入下载队列中
	* @param . 
	nTime 请求下载的时间 
	msg 编辑框内容
	* @return.
	*/
	virtual void addDownLoadImageAge(quint32 nTime, const QString& msg) = 0;
	/**
	* @brief . 从下载图片的url队列中取出一个url下载
	* @param .
	* @return .
	*/
	virtual quint32 popDownLoadImageAge(const QString& url) =0;
	/**
	* @brief . 上传自定义表情到服务器中。
	* @param . 
	strKind 任务类型
	strFileName 指表情文件的路径
	* @return. 返回完整下载地址，参数是要上传的完整文件路径
	*/
	virtual QString UpLoadSmileAndShotImage(const QString& strKind, const QString& strFileName) =0; 

	/*
	signals:
	void onVoiceDownLoadResult(bool bSucc,int nIndex);  
	*/
};
