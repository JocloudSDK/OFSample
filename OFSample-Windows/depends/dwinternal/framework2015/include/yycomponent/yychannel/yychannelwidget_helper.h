#pragma once

/**
* @brief ͨ��Ƶ���ؼ���Widget��ȡ��Ƶ���ؼ���ӦƵ�������ӿ�,��ýӿں���Ҫ����Release���ͷŽӿ�
* @pWidget �ؼ�ָ��
* @type �ӿ�����
* @ret  ����ֵ
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
