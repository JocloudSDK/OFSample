#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"
#include "yycomponent/yycommon/yysmilefacemanager_i.h"


class DuiWidget;
/**
* @brief .���������Ľӿ�
*/
DWDEFINE_INTERFACE(IChannelInputEdit) : public IUnk 
{
/**
   * @brief invoke����.
   * @param method��Ҫ���õĺ�����inParam�����������outParam���������.
   * @return ���÷����룬��InvokeCode.
   */
	virtual int invoke(const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
	virtual DuiWidget* widget() = 0;

    /**
    * @brief �ú�����������channel input editor
    * @param json�ַ���
    * @return .
    */
    virtual void setControl(const char* json) = 0;
};
/**
* @brief .IChannelInputEdit������
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
   * @brief ���ñ༭��ӿ�.
   * @param interfacePtr���༭��ӿ�.
   */
		void setInterface(IChannelInputEditPtr interfacePtr )
		{
			m_interface = interfacePtr;
		}
/**
	* @brief ��ȡ�༭��.
	* @return �༭��widgetָ��.
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
   * @brief ���༭��������.
   * @param smile����������.
   * @return ���÷����룬��InvokeCode.
   */
		int insertSmile( const IYYSmileFaceManager::YYSmileData& smile )
		{
			typedef PL_1< const IYYSmileFaceManager::YYSmileData*> InType;
			InType inParam; inParam.size = sizeof(InType); inParam.p1 = &smile;

			return m_interface->invoke(ApiInsertSmile, &inParam, NULL);
		}

        /**
        * @brief ��ʹ������������޸��������ɫ�Ľӿ�
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
        * @brief �޸�����.
        * @param font������.
        * @return ���÷����룬��InvokeCode.
        */
        int changeFont( const QFont& font )
        {
            typedef PL_1< const QFont*> InType;
            InType inParam; inParam.size = sizeof(InType); inParam.p1 = &font;

            return m_interface->invoke(ApiChangeFont, &inParam, NULL);
        }

        /**
        * @brief �޸�������ɫ.
        * @param color��������ɫ.
        * @return ���÷����룬��InvokeCode.
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
