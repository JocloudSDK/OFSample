#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYFaceImage): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void channelIndexFromHeadIndexBatch( const QVector<int> &headIndexs, QVector<int>& out ) = 0;
/**
   * @brief .
   * @param .
   * @return.
   */
	virtual int	channelIndexFromHeadIndex(int headIndex ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void headIndexFromChannelIndex( const QVector<int> &channelIndexs, QVector<int> &out ) = 0;

/**
   * @brief . �õ�����ϵͳͷ��
   * @param . index ����ϵͳͷ���index
   * @return. ���ص���ͼƬ·��
   */
	virtual QString getPersonalHead( int index ) = 0;
/**
   * @brief . �õ�Ƶ��ϵͳͷ��
   * @param . index Ƶ��ϵͳͷ��index
   * @return. ���ص���ͼƬ·��
   */
	virtual QString getChannelHead( int index ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString getHeadPhotoCacheFilePath( const QString& url ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString saveHeadPhotoCacheFile( QByteArray& byteArray, const QString& url ) = 0;
};