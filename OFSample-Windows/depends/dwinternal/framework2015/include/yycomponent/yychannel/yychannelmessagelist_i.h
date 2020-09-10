#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"
#include "yycomponent/yychannel/yycompositeformat_i.h"

// Ƶ��Ĭ�Ϲ����ӿ�.

DWDEFINE_INTERFACE(IYYChannelMessageList) : public IUnk
{
/**
   * @brief .invoke
   * @param .method��Ҫ���õĺ�����inParam�����������outParam���������
   * @return .���÷����룬��InvokeCode
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
   * @brief .������
   * @param .listPtr�������б�ָ��
   * @return .��
   */
		explicit Invoker( IYYChannelMessageListPtr listPtr )
		{
			m_listPrt = listPtr;
		}

/**
   * @brief .����������һ��֪ͨ��Ϣ����Ϣǰ����Ĭ��ͼ��
   * @param .message��֪ͨ��Ϣ
   * @return .���÷����룬��InvokeCode
   */
		int appendEchoMessage( const QString& message )
		{
			PL_1<QString> inParam;inParam.size = sizeof(inParam);inParam.p1 = message;
			return m_listPrt->invoke(KAppendEchoMessage, &inParam, NULL );
		}

/**
   * @brief .����������һ��֪ͨ��Ϣ
   * @param .message��֪ͨ��Ϣ��icon����Ϣ�Զ���ͼ��
   * @return .���÷����룬��InvokeCode
   */
		int appendEchoMessage( const QString& icon,const QString& message )
		{
			PL_2<QString,QString> inParam;inParam.size=sizeof(inParam);
			inParam.p1 = icon;
			inParam.p2 = message;
			return m_listPrt->invoke(KAppendIconMessage,&inParam,NULL);
		}


/**
   * @brief .������Ϣ��ʽ������ṩ��format�Զ�����Ϣ��ʽ
   * @param .��
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
   * @brief .�����ʽ
   * @param .format����ʽ
   * @return .���÷����룬��InvokeCode
   */
		int appendCompositeFormat( ICompositeFormatPtr format )
		{
			PL_1<ICompositeFormatPtr> inParam;inParam.size=sizeof(inParam);
			inParam.p1 = format;

			return m_listPrt->invoke(KAppendCompositeFormat,&inParam,NULL);
		}

/**
   * @brief .�жϹ����Ƿ��������ͼƬ
   * @param .��
   * @return .true���������ͼƬ������false
   */
		bool isImageAllowed()
		{
			return m_listPrt->invoke(KIsImageAllowed, NULL, NULL) == 1;
		}

/**
   * @brief .�жϹ����Ƿ������������
   * @param .��
   * @return .true������������֣�����false
   */
		bool isTextAllowed()
		{
			return m_listPrt->invoke(KIsTextAllowed, NULL, NULL) == 1;
		}

	private:
		IYYChannelMessageListPtr m_listPrt;
	};
}
