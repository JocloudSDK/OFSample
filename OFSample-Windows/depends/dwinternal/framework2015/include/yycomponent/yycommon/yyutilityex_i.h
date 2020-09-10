#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

class QRect;
class QPoint;
class QPointF;
class QString;
class QObject;
class QGraphicsWidget;
class QGraphicsView;
class QGraphicsItem;

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYUtilityEx): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QPoint mapItemToScreen( QGraphicsView* view, QGraphicsItem *item ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QPoint mapItemToView( QGraphicsView*, QGraphicsItem* item) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QRect mapWidgetToScreen( QGraphicsWidget *widget ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QPointF mapScreenToWidget( const QPoint &pos, QGraphicsWidget *widget ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QGraphicsView *graphicsViewFromWidget( QGraphicsWidget *widget ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void clearBizConnections(QObject *obj) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString toHalfWide( const QString& querySring ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int fixPageRange(const int nTotal, const int nInput) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString filterBulletinText( const QString& text ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int yyLinkCount( const QString& text ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int urlCount( const QString& text ) = 0;

/**
   * @brief 添加进程ID到列表.
   * @param .
   * @return .
   */
	virtual void addWorkingPidToList( quint32 pid ) = 0;

/**
   * @brief 判断前端进程对应的PID是否在列表内.
   * @return .
   */
	virtual bool isForegroundPidInList() = 0;

	virtual int yyCountAnyLink( const QString& text) = 0;
};

DWDEFINE_INTERFACE(IYYUtilityMiscSignal): public IUnk
{
	typedef _def_boost_signals2_signal_type<void (bool)>::type _SIG_AUTORUN_CONFIG_CHANGED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_AUTORUN_CONFIG_CHANGED *getAutoRunConfigChanged() = 0;
};