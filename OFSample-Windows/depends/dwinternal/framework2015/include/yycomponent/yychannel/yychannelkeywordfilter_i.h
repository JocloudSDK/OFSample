#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

/**
* @brief 敏感词过滤.
*/
DWDEFINE_INTERFACE(IYYChannelKeywordFilter) : public IUnk
{
/**
   * @brief Invoke接口.
   * @param method，将要调用的函数.
   * @param inParam，输入参数.
   * @param outParam，输出参数.
   */
	virtual int Invoke( const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
};

/**
* @brief IYYChannelKeywordFilter的辅助类.
*/
namespace APIChannelKeywordFilter
{
	const char ApiFilter[] = "filter";

	class Invoker
	{
	public:
/**
   * @brief 辅助类构造函数.
   * @param filter， IYYChannelKeywordFilte指针.
   */
		explicit Invoker(IYYChannelKeywordFilterPtr filter ):m_interface(filter){}

/**
   * @brief 关键字过滤.
   * @param message，要过滤的原始信息.
   * @return 过滤后的信息，如果为空，敏感词过滤不通过；与原始信号不一样，则敏感词被替换；如果一样，原始信息没有敏感词.
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