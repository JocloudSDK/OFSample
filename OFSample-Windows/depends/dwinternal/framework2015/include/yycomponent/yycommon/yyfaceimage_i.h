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
   * @brief . 得到个人系统头像
   * @param . index 个人系统头像的index
   * @return. 返回的是图片路径
   */
	virtual QString getPersonalHead( int index ) = 0;
/**
   * @brief . 得到频道系统头像
   * @param . index 频道系统头像index
   * @return. 返回的是图片路径
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