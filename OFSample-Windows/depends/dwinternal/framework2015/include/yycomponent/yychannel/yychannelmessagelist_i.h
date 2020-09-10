#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"
#include "yycomponent/yychannel/yycompositeformat_i.h"

// 频道默认公屏接口.

DWDEFINE_INTERFACE(IYYChannelMessageList) : public IUnk
{
/**
   * @brief .invoke
   * @param .method，要调用的函数；inParam，输入参数；outParam，输出参数
   * @return .调用返回码，见InvokeCode
   */
	virtual int invoke( const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
};

namespace APIChannelMessageList
{
	const char KAppendEchoMessage[] = "appendEchoMessage";
	const char KAppendIconMessage[] = "appendIconMessage";
	const char KAppendCompositeFormat[] = "appendCompoisteFormat";
	const char KCreateCompositeFormat[] = "createFormat";
	const char KIsImageAllowed[] = "isImageAllowed";
	const char KIsTextAllowed[] = "isTextAllowed";

	class Invoker
	{
	public:
/**
   * @brief .辅助类
   * @param .listPtr，公屏列表指针
   * @return .无
   */
		explicit Invoker( IYYChannelMessageListPtr listPtr )
		{
			m_listPrt = listPtr;
		}

/**
   * @brief .往公屏插入一条通知信息；消息前面用默认图标
   * @param .message，通知信息
   * @return .调用返回码，见InvokeCode
   */
		int appendEchoMessage( const QString& message )
		{
			PL_1<QString> inParam;inParam.size = sizeof(inParam);inParam.p1 = message;
			return m_listPrt->invoke(KAppendEchoMessage, &inParam, NULL );
		}

/**
   * @brief .往公屏插入一条通知信息
   * @param .message，通知信息；icon，消息自定义图标
   * @return .调用返回码，见InvokeCode
   */
		int appendEchoMessage( const QString& icon,const QString& message )
		{
			PL_2<QString,QString> inParam;inParam.size=sizeof(inParam);
			inParam.p1 = icon;
			inParam.p2 = message;
			return m_listPrt->invoke(KAppendIconMessage,&inParam,NULL);
		}


/**
   * @brief .创建消息格式，这个提供的format自定义消息格式
   * @param .无
   * @return .format
   */
		ICompositeFormatPtr createCompositeFormat()
		{
			PL_1<ICompositeFormatPtr> outParam; outParam.size = sizeof(outParam);
			outParam.p1 = NULL;

			m_listPrt->invoke(KCreateCompositeFormat, NULL, &outParam);

			return outParam.p1;
		}

/**
   * @brief .插入格式
   * @param .format，格式
   * @return .调用返回码，见InvokeCode
   */
		int appendCompositeFormat( ICompositeFormatPtr format )
		{
			PL_1<ICompositeFormatPtr> inParam;inParam.size=sizeof(inParam);
			inParam.p1 = format;

			return m_listPrt->invoke(KAppendCompositeFormat,&inParam,NULL);
		}

/**
   * @brief .判断公屏是否允许接受图片
   * @param .无
   * @return .true，允许接受图片，否则false
   */
		bool isImageAllowed()
		{
			return m_listPrt->invoke(KIsImageAllowed, NULL, NULL) == 1;
		}

/**
   * @brief .判断公屏是否允许接受文字
   * @param .无
   * @return .true，允许接受文字，否则false
   */
		bool isTextAllowed()
		{
			return m_listPrt->invoke(KIsTextAllowed, NULL, NULL) == 1;
		}

	private:
		IYYChannelMessageListPtr m_listPrt;
	};
}
