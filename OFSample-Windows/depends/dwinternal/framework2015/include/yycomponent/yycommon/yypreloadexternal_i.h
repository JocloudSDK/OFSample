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
   * @brief 准备预启动的yyexternal, 注：YY基础内部使用.
   */
	virtual bool prepareExternal(void) = 0;

/**
   * @brief 使用预启动的yyexternal.exe加载业务的dll, 成功则返回yyexternal的id.
   * @param strDllFile  dll路径.
   */
	virtual quint32 loadDll(const QString& strDllFile ) = 0;

/**
   * @brief 释放回收yyexternal进程.
   * @param nId  yyexternal的id.
   */
	virtual bool releaseDll(quint32 nId) = 0;


/**
   * @brief 重启yyexternal进程.
   * @param nId  yyexternal的id.
   */
	virtual bool restart(quint32 nId) = 0;

/**
   * @brief 重启yyexternal.exe后调这个函数加载业务的dll, 成功则返回true.
   * @param strDllFile  dll路径.
   */
	virtual bool reLoadDll(quint32 nId, const QString& strDllFile) = 0;

/**
   * @brief 透传数据给yyexternal.exe.
   * @param nId  yyexternal的id.
   */
	virtual bool sendTransferData(quint32 nId, quint32 dataType, const BYTE *data, quint32 nLength) = 0;

/**
   * @brief 使用预启动的yyexternal.exe加载业务的dll, 成功则返回yyexternal的id.
   * @param strDllFile  dll路径.
   * @param strExpand   初始化dll透传参数.
   */
	virtual quint32 loadDll2(const QString& strDllFile, const QString& strExpand) = 0;

/**
   * @brief 重启yyexternal.exe后调这个函数加载业务的dll, 成功则返回true.
   * @param strDllFile  dll路径.
   * @param strExpand   初始化dll透传参数.
   */
	virtual bool reLoadDll2(quint32 nId, const QString& strDllFile, const QString& strExpand) = 0;
};
