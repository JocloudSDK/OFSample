#pragma once

#include "duifw/yynotificationboxitem.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

DWDEFINE_INTERFACE(IYYChannelMsgCenter) : public IUnk
{
    /**
    * @brief 添加一个泡泡到右下角
    * @param imagePath, 泡泡图标
    * @param text, 泡泡的文本
    * @param msgNum, 泡泡的数量，会加到原数量上
    * @param item, 泡泡的其他（自定义）属性
    * @return 返回标识id
    */
    virtual void addMsg(const QString& imgPath, const QString& text, int msgNum, IPropBagWrapperPtr item) = 0;


    /**
    * @brief 移除一个右下角的泡泡
    * @param id, 泡泡标识
    * @param cnt, 移除的数量，-1表示整个泡泡移除；>0 表示减少cnt个数量
    * @return
    */
    virtual void removeMsg(IPropBagWrapperPtr item) = 0;

    
    typedef _def_boost_signals2_signal_type<void (YYNotificationBoxItem::ExecuteReason reason, IPropBagWrapperPtr spItem)>::type _SIG_ITEM_EXECUTED;
    virtual _SIG_ITEM_EXECUTED* getSigItemExecuted() = 0;
};