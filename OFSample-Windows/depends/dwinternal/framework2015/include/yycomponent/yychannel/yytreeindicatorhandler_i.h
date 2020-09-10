#pragma once
#include "dwbase/dwcomex.h"

struct ITreeItemInfo
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 uid() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int indentLevel() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int userLevel() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString userNick() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString userSign() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual quint32 userTestIcon() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString gameCardKey() const = 0;
};

class DuiWidget;
struct ITreeIndicatorHandler
{
	virtual ~ITreeIndicatorHandler(){}
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int preferredPosition(quint32 uid) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int preferredWidth(ITreeItemInfo *userInfo, int level, quint32* revision) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual qreal draw(QPainter *painter, const QRectF &rect, ITreeItemInfo *userInfo, int level) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual DuiWidget *tooltipWidget(ITreeItemInfo *userInfo) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void showCustomTooltip(QWidget *parentWidget, ITreeItemInfo *itemInfo, const QPointF &offset, int level) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onClicked(quint32 uid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QObject *queryNotifyObject() = 0;

// signals:
// 需要主动刷新，请在NotifyObject中实现此信号
// void userInfoUpdated(const QSet<quint32> &uids);
};
