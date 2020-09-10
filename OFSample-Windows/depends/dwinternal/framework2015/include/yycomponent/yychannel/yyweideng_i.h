#pragma once

#include "yycomponent/yychannel/yytreeindicatorhandler_i.h"
#include "yycomponent/yycomclsid.h"

/*
    caution: only for YY internal use
*/
define_interface(IChannel);

struct IHeadLightIndicatorHandler : public ITreeIndicatorHandler
{
	//ITreeIndicatorHandler
	virtual int preferredPosition(quint32) const { return 2;}
	virtual int preferredWidth( ITreeItemInfo *, int, quint32* ) { return 13; }
	virtual qreal draw(QPainter *, const QRectF &, ITreeItemInfo *, int ) { return 0;}
	virtual void showCustomTooltip(QWidget *, ITreeItemInfo *, const QPointF &, int ) {}
	virtual QObject *queryNotifyObject(){return NULL;}

	//目前只需要实现以下4个接口
	//按照注册的优先级轮询
	//如果需要响应uid，则返回true
	virtual bool canHandle(quint32 uid) = 0;

	//返回需要响应的pixmap
	virtual QPixmap fetchPixmap(ITreeItemInfo *userInfo) = 0;

	//virtual DuiWidget *tooltipWidget(ITreeItemInfo *userInfo) = 0;
	//virtual void onClicked(quint32 uid) = 0;
};

/**
* @brief 内部尾灯接口，用初始化，组件勿调用.
*/
DWDEFINE_INTERFACE(IYYWeidengInner): public IUnk
{
/**
   * @brief 初始化.
   * @param spChannel，频道指针.
   */
	virtual void init(IChannelPtr spChannel) = 0;
/**
   * @brief 反初始化.
   */
	virtual void unInit() = 0;

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
    * @brief 注册js插件proxy
    */
    virtual void registeScriptObjectProxy() = 0;

};

/**
* @brief 尾灯接口.
*/
DWDEFINE_INTERFACE(IYYWeideng):public IUnk
{

	/**
	* @brief 麦序尾灯列表.
	* @return 麦序尾灯列表.
	*/
	virtual QList<ITreeIndicatorHandler *> getMaixuIndicator() = 0; // for maixu
	/**
	* @brief 用户树尾灯列表.
	* @return 用户树尾灯列表.
	*/
	virtual QList<ITreeIndicatorHandler *> getUserlistIndicator() = 0; // for userlist
/**
   * @brief 尾灯清理.
   */
	virtual void clear() = 0;
};