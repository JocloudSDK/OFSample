#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
signals:
	void transferDataArrived(quint32 nId, quint32 dataType, const QByteArray &buffer);
	void connected(quint32 nId, quint32 connCount);
	void disconnected(quint32 nId, quint32 connCount);
	void peerQuited(quint32 nId, quint32 connCount, qint32 errcode);
*/

DWDEFINE_INTERFACE( IPreloadExternal ) : public IDWQComponent
{
/**
   * @brief ׼��Ԥ������yyexternal, ע��YY�����ڲ�ʹ��.
   */
	virtual bool prepareExternal(void) = 0;

/**
   * @brief ʹ��Ԥ������yyexternal.exe����ҵ���dll, �ɹ��򷵻�yyexternal��id.
   * @param strDllFile  dll·��.
   */
	virtual quint32 loadDll(const QString& strDllFile ) = 0;

/**
   * @brief �ͷŻ���yyexternal����.
   * @param nId  yyexternal��id.
   */
	virtual bool releaseDll(quint32 nId) = 0;


/**
   * @brief ����yyexternal����.
   * @param nId  yyexternal��id.
   */
	virtual bool restart(quint32 nId) = 0;

/**
   * @brief ����yyexternal.exe��������������ҵ���dll, �ɹ��򷵻�true.
   * @param strDllFile  dll·��.
   */
	virtual bool reLoadDll(quint32 nId, const QString& strDllFile) = 0;

/**
   * @brief ͸�����ݸ�yyexternal.exe.
   * @param nId  yyexternal��id.
   */
	virtual bool sendTransferData(quint32 nId, quint32 dataType, const BYTE *data, quint32 nLength) = 0;

/**
   * @brief ʹ��Ԥ������yyexternal.exe����ҵ���dll, �ɹ��򷵻�yyexternal��id.
   * @param strDllFile  dll·��.
   * @param strExpand   ��ʼ��dll͸������.
   */
	virtual quint32 loadDll2(const QString& strDllFile, const QString& strExpand) = 0;

/**
   * @brief ����yyexternal.exe��������������ҵ���dll, �ɹ��򷵻�true.
   * @param strDllFile  dll·��.
   * @param strExpand   ��ʼ��dll͸������.
   */
	virtual bool reLoadDll2(quint32 nId, const QString& strDllFile, const QString& strExpand) = 0;
};
