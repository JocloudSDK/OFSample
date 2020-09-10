#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"
#include "yycomponent/yycommon/yysmilefacemanager_i.h"


class DuiWidget;
/**
* @brief .代表输入框的接口
*/
DWDEFINE_INTERFACE(IChannelInputEdit) : public IUnk 
{
/**
   * @brief invoke函数.
   * @param method，要调用的函数；inParam，输入参数；outParam，输出参数.
   * @return 调用返回码，见InvokeCode.
   */
	virtual int invoke(const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
	virtual DuiWidget* widget() = 0;

    /**
    * @brief 该函数用于配置channel input editor
    * @param json字符串
    * @return .
    */
    virtual void setControl(const char* json) = 0;
};
/**
* @brief .IChannelInputEdit辅助类
*/
namespace APIChannelInputEdit
{
	const char ApiInsertSmile[]	="insertSmile";
	const char ApiInputWidget[]	="inputWidget";
    const char ApiChangeFontOrColor[] = "changeFontOrColor";
    const char ApiChangeFont[] = "changeFont";
    const char ApiChangeFontColor[] = "changeFontColor";

	class Invoker
	{
	public:
		Invoker(IChannelInputEditPtr interfacePtr):m_interface(interfacePtr)
		{

		}

/**
   * @brief 设置编辑框接口.
   * @param interfacePtr，编辑框接口.
   */
		void setInterface(IChannelInputEditPtr interfacePtr )
		{
			m_interface = interfacePtr;
		}
/**
	* @brief 获取编辑框.
	* @return 编辑框widget指针.
	*/
		DuiWidget* inputWidget()
		{
			PL_1<DuiWidget*> outParam; outParam.size = sizeof(PL_1<DuiWidget*>);
			if ( m_interface->invoke(ApiInputWidget, NULL, &outParam) == InvokeCode::InvokeOK )
			{
				return outParam.p1;
			}
			return NULL;
		}

/**
   * @brief 往编辑框插入表情.
   * @param smile，表情数据.
   * @return 调用返回码，见InvokeCode.
   */
		int insertSmile( const IYYSmileFaceManager::YYSmileData& smile )
		{
			typedef PL_1< const IYYSmileFaceManager::YYSmileData*> InType;
			InType inParam; inParam.size = sizeof(InType); inParam.p1 = &smile;

			return m_interface->invoke(ApiInsertSmile, &inParam, NULL);
		}

        /**
        * @brief 请使用另外的两个修改字体和颜色的接口
        * @param
        * @return
        */
        int changeFontOrColor( const IYYSmileFaceManager::YYFontConfig& config )
        {
            typedef PL_1< const IYYSmileFaceManager::YYFontConfig*> InType;
            InType inParam; inParam.size = sizeof(InType); inParam.p1 = &config;

            return m_interface->invoke(ApiChangeFontOrColor, &inParam, NULL);
        }

        /**
        * @brief 修改字体.
        * @param font，字体.
        * @return 调用返回码，见InvokeCode.
        */
        int changeFont( const QFont& font )
        {
            typedef PL_1< const QFont*> InType;
            InType inParam; inParam.size = sizeof(InType); inParam.p1 = &font;

            return m_interface->invoke(ApiChangeFont, &inParam, NULL);
        }

        /**
        * @brief 修改字体颜色.
        * @param color，字体颜色.
        * @return 调用返回码，见InvokeCode.
        */
        int changeFontColor( const QColor& color )
        {
            typedef PL_1< const QColor*> InType;
            InType inParam; inParam.size = sizeof(InType); inParam.p1 = &color;

            return m_interface->invoke(ApiChangeFontColor, &inParam, NULL);
        }

	private:
		IChannelInputEditPtr m_interface;
	};
};
