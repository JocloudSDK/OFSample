#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/

namespace NotificationMsg
{
	typedef enum
	{
		NotificationMsgBegin = 0,

		/**
		* @brief 显示特定用户所对应的tooltip时调用此函数
		* @param uid 用户uid
		* @param pt tooltip显示的位置
		* @param handle 函数返回信息
		*/
		// void showTooltip(quint32 uid, const QPoint& pt, qint32* handle)";
		ShowTooltip = NotificationMsgBegin,
		/**
		* @brief 隐藏特定用户所对应的tooltip时调用此函数
		* @param uid 用户uid
		* @param handle 函数返回信息
		*/
		// void hideTooltip(quint32 uid, qint32* handle)";
		HideTooltip,
		NotificationMsgEnd,
	} Method;
}

class QGraphicsWidget;
class QWidget;
DWDEFINE_INTERFACE(IYYNotifier) : public IUnk
{
	/**
	* @brief .
	* @param .
	* @return.
	*/
	virtual void show(
		QWidget *parentWidget, 
		const QString &text, 
		QMessageBox::Icon icon /*= QMessageBox::Information */, 
		int durationTime /*= 3000*/) = 0;

	/**
	* @brief .
	* @param .
	* @return.
	*/
	virtual void show(
		QWidget *parentWidget, 
		const QRect &rect, 
		const QString &text, 
		QMessageBox::Icon icon /*= QMessageBox::Information*/, 
		int durationTime /*= 3000*/,
		Qt::ArrowType arrowType /*= Qt::NoArrow*/, 
		int arrowOffset /* = 0*/) = 0;

	/**
	* @brief .
	* @param .
	* @return.
	*/
	virtual void showWidget(
		QWidget *parentWidget, 
		const QRect &rect, 
		QGraphicsWidget *widget,
		QMessageBox::Icon icon /*= QMessageBox::Information*/, 
		int durationTime /*= 3000*/,
		Qt::ArrowType arrowType /*= Qt::NoArrow*/, 
		int arrowOffset /*= 0*/) = 0;

};

DWDEFINE_INTERFACE(IYYNotifierEx) : public IUnk
{
/**
	* @brief .
	* @param .
	* @return.
	*/
	virtual void showWidget( QWidget *parentWidget, const QRect &rect, QGraphicsWidget *widget, QMessageBox::Icon icon /*= QMessageBox::Information*/, 
		Qt::ArrowType arrowType /*= Qt::NoArrow*/, int arrowOffset /*= 0*/, QGraphicsWidget* positionBaseWidget, int xOffset, int yOffset, bool popup ) = 0;

	/**
	* @brief 注册通知处理器
	* @param priority 优先级
	* @param method 注册的函数，可用值参见namespace NotificationMsg::Method
	* @param notify 通知处理器指针，要求notify处理器的invoke函数可以调用到所注册的method函数。
	* @return 是否注册成功
	*/
	virtual bool registerNotifyProcessor(qint32 priority, NotificationMsg::Method method, IDWQComponentPtr notify) = 0;

	/**
	* @brief 反注册通知处理器
	* @param method 反注册的函数，可用值参见namespace NotificationMsg::Method
	* @param notify通知处理器指针
	* @return 是否反注册成功
	*/
	virtual bool unregisterNotifyProcessor(NotificationMsg::Method method, IDWQComponentPtr notify) = 0;

	/**
	* @brief 调用通知处理器函数
	* @param method 通知处理器函数，可用值参见namespace NotificationMsg::Method
	* @param args 参数列表，顺序根据函数确定
	* @return 是否调用成功
	*/
	virtual bool invokeNotifyProcessor(NotificationMsg::Method method, QVariantList &args) = 0;
};