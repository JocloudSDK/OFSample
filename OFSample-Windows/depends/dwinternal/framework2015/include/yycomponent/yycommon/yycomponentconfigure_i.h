#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief ����������ýӿڣ�ͨ��root->getServiceInterface<IDWComponentConfigure>(KSERVICECommonComponentLevel)����ȡ.
*/
DWDEFINE_INTERFACE(IDWComponentConfigure): public IUnk
{
	enum Position
	{
		MainFrame_AppTab,     //������Ӧ��TAB��
		MainFrame_AppBox,	  //������Ӧ�ù�����
	};

	/**
   * @brief . �������Ӧ����ĳ��λ������ʾ״̬
   * @param . pos ��ʾ��λ��(Position)
   * @param . visible �Ƿ���ʾ
   * @return. ��
   */
	virtual void setVisible( Position pos, bool visible ) = 0;

   /**
   * @brief . �������Ӧ����ĳ��λ�����Ƿ���ʾ
   * @param . pos ��ʾ��λ��(Position)
   * @return. ���ڸ�λ���ϴ�����ʾ״̬ true, ���� false
   */
	virtual bool visible( Position pos ) = 0;

};