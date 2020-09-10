#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

/**
* @brief ���дʹ���.
*/
DWDEFINE_INTERFACE(IYYChannelKeywordFilter) : public IUnk
{
/**
   * @brief Invoke�ӿ�.
   * @param method����Ҫ���õĺ���.
   * @param inParam���������.
   * @param outParam���������.
   */
	virtual int Invoke( const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
};

/**
* @brief IYYChannelKeywordFilter�ĸ�����.
*/
namespace APIChannelKeywordFilter
{
	const char ApiFilter[] = "filter";

	class Invoker
	{
	public:
/**
   * @brief �����๹�캯��.
   * @param filter�� IYYChannelKeywordFilteָ��.
   */
		explicit Invoker(IYYChannelKeywordFilterPtr filter ):m_interface(filter){}

/**
   * @brief �ؼ��ֹ���.
   * @param message��Ҫ���˵�ԭʼ��Ϣ.
   * @return ���˺����Ϣ�����Ϊ�գ����дʹ��˲�ͨ������ԭʼ�źŲ�һ���������дʱ��滻�����һ����ԭʼ��Ϣû�����д�.
   */
		QString filter( const QString& message )
		{
			typedef PL_1<QString> FilterParam;

			FilterParam inParam; inParam.size = sizeof(inParam); inParam.p1 = message;
			FilterParam outParam; outParam.size = sizeof(outParam);

			int code = m_interface->Invoke(ApiFilter,&inParam,&outParam);
			DW_ASSERT(code == InvokeCode::InvokeOK );

			return outParam.p1;
		}

	private:
		IYYChannelKeywordFilterPtr m_interface;
	};
}