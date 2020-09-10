#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yytreeindicatorhandler_i.h"
#include "yycomponent/yychannel/yyweideng_i.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
/**
* @brief .频道尾灯接口
*/
DWDEFINE_INTERFACE(IYYChannelIndicator) : public IUnk
{
/**
   * @brief 向用户列表注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
	virtual int registerIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;
/**
   * @brief 向用户列表反注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
	virtual int unRegisterIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;

/**
   * @brief 向麦序列表注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
	virtual int registerMaixuIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;
/**
   * @brief 向麦序列表反注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
	virtual int unRegisterMaixuIndicatorHandler( ITreeIndicatorHandler * hanlder ) = 0;

/**
   * @brief 注册频道树头灯处理器
   * @param priority 轮询优先级 (内置的有vip-100, MobileLight-200, WebyyLight-300)
   * @param handler 处理器指针
   */
	virtual int registerHeadLightIndicator(int priority, IHeadLightIndicatorHandler *handler) = 0;

	/**
   * @brief 反注册频道树头灯处理器
   * @param handler 处理器指针
   */
	virtual int unRegisterHeadLightIndicator(IHeadLightIndicatorHandler *handler) = 0;

/**
   * @brief 向用户列表注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
    virtual int registerIndicatorHandlerById( quint32 id, ITreeIndicatorHandler * handler ) = 0;
/**
   * @brief 向用户列表反注册尾灯.
   * @param hanlder，尾灯处理器.
   * @return 暂时无用.
   */
    virtual int unRegisterIndicatorHandlerById( quint32 id ) = 0;
};

//全定制UI才需要这个接口
DWDEFINE_INTERFACE(IYYChannelIndicatorInner): public IDWQComponent 
{
	virtual QList<ITreeIndicatorHandler *> userListIndicators() const = 0;
	virtual QList<ITreeIndicatorHandler *> maixuListIndicators() const = 0;
    virtual QMap<quint32, ITreeIndicatorHandler*> treeIndicatorsMap() const = 0;

	/*
	signals:
	void userInfoUpdated(const QSet<quint32> &);
	void needUpdate();
	*/
};