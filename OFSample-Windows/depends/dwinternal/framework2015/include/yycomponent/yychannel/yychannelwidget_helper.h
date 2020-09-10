#pragma once

/**
* @brief 通过频道控件的Widget获取与频道控件对应频道操作接口,获得接口后需要调用Release来释放接口
* @pWidget 控件指针
* @type 接口类型
* @ret  返回值
*/
#define QUERYYYCHANNELINTERFACE(pWidget,type) __QueryChannelInterface<type>(pWidget, #type)

template <typename T>
T __QueryChannelInterface(DuiWidget *pWidget,const char *typeName)
{
    T ret = NULL;
    QMetaObject::invokeMethod(pWidget, "getYYChannelInterface", QReturnArgument<T>(typeName, ret));
    if(ret) ret->addRef();
    return ret;
}
