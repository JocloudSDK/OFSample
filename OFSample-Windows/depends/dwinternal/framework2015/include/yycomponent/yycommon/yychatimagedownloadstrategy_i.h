#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duitextedit.h"

/**
* @brief ͨ��root->getServiceInterface<IChatImageDownLoadStrategy>(KSERVICECommon)����ȡ.
*/
DWDEFINE_INTERFACE(IChatImageDownLoadStrategy) : public IDWQComponent
{
	/**
	* @brief . ����textedit
	* @param . 
	* @return .
	*/
	virtual void setTextEdit( DuiTextEdit* textEdit ) = 0;
	/**
	* @brief . ����ȴ�gifͼƬλ��
	* @param .
	* @return .
	*/
	virtual void insertWaitGifPosition(int nPosition, const QString& strKey, bool bVoice) =0; //bVoice�Ƿ���������
	/**
	* @brief . ����ȴ�gifͼƬλ��
	* @param .
	* @return .
	*/
	virtual void insertWaitGifPosition2(int nPosition, const QString& url, bool bVoice)=0; //bVoice�Ƿ���������
	/**
	* @brief . ��ȡ�༭������Ҫ���ص�ͼƬurl�����ض�����
	* @param . 
	nTime �������ص�ʱ�� 
	msg �༭������
	* @return.
	*/
	virtual void addDownLoadImageAge(quint32 nTime, const QString& msg) = 0;
	/**
	* @brief . ������ͼƬ��url������ȡ��һ��url����
	* @param .
	* @return .
	*/
	virtual quint32 popDownLoadImageAge(const QString& url) =0;
	/**
	* @brief . �ϴ��Զ�����鵽�������С�
	* @param . 
	strKind ��������
	strFileName ָ�����ļ���·��
	* @return. �����������ص�ַ��������Ҫ�ϴ��������ļ�·��
	*/
	virtual QString UpLoadSmileAndShotImage(const QString& strKind, const QString& strFileName) =0; 

	/*
	signals:
	void onVoiceDownLoadResult(bool bSucc,int nIndex);  
	*/
};
