#pragma once

#include "duifw/yynotificationboxitem.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

DWDEFINE_INTERFACE(IYYChannelMsgCenter) : public IUnk
{
    /**
    * @brief ���һ�����ݵ����½�
    * @param imagePath, ����ͼ��
    * @param text, ���ݵ��ı�
    * @param msgNum, ���ݵ���������ӵ�ԭ������
    * @param item, ���ݵ��������Զ��壩����
    * @return ���ر�ʶid
    */
    virtual void addMsg(const QString& imgPath, const QString& text, int msgNum, IPropBagWrapperPtr item) = 0;


    /**
    * @brief �Ƴ�һ�����½ǵ�����
    * @param id, ���ݱ�ʶ
    * @param cnt, �Ƴ���������-1��ʾ���������Ƴ���>0 ��ʾ����cnt������
    * @return
    */
    virtual void removeMsg(IPropBagWrapperPtr item) = 0;

    
    typedef _def_boost_signals2_signal_type<void (YYNotificationBoxItem::ExecuteReason reason, IPropBagWrapperPtr spItem)>::type _SIG_ITEM_EXECUTED;
    virtual _SIG_ITEM_EXECUTED* getSigItemExecuted() = 0;
};