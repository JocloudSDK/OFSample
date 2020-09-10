#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYImageDownLoad): public IDWQComponent
{
	/**
	* @brief ����������Ϣ�е�ͼƬ����ע��strKind������ȥ�޸ģ�ԭԭ�����ش���������,�����������жϽ���ص��Ƿ������Լ����ĵ�.
	* @param strKind  ��������.
	* @param strMsg YY��Ϣ�����������һ�����ͼƬ�����ص�ַ.
	* @param strSavePath ����Ŀ¼.
	*/
	virtual void DownloadImageInYYMsg(UINT32 sendTime, const QString& strKind, const QString& strMsg) = 0;

	/**
	* @brief ����һ������ͼƬ�������񣨰�����ͼ����ͼƬ���Զ�����飩��ע��strTaskID��strKind������ȥ�޸ģ�ԭԭ�����ش���������,�����������жϽ���ص��Ƿ������Լ����ĵ�.
	* @param strTaskID ����ID.
	* @param strKind ��������.
	* @param strURL ���ص�ַ.
	* @param strSavePath ����Ŀ¼.
	*/
	virtual void AddChatImageTask(const QString& strTaskID, const QString& strKind, const QString& strURL, const QString& strSavePath, bool useTcp=true)  = 0;

	/**
	* @brief ����һ���ļ�����������, strTaskID��strKind������ȥ�޸ģ�ԭԭ�����ش���������,�����������жϽ���ص��Ƿ������Լ����ĵ�.
	* @param strTaskID ����ID.
	* @param strKind ��������.
	* @param strURL ���ص�ַ.
	* @param strSavePath ����Ŀ¼.
	*/
	virtual void AddFileTask(const QString& strTaskID, const QString& strKind, const QString& strURL, const QString& strSavePath, bool useIECache=true)  = 0;

	/*
	signals:
	void imageDownloadResult(const QString &strTaskID, const QString &strKind, const QString &strURL, const QString& strImgFile, bool bSucc); //���ؽ��֪ͨ
	*/
};