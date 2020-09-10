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
		* @brief ��ʾ�ض��û�����Ӧ��tooltipʱ���ô˺���
		* @param uid �û�uid
		* @param pt tooltip��ʾ��λ��
		* @param handle ����������Ϣ
		*/
		// void showTooltip(quint32 uid, const QPoint& pt, qint32* handle)";
		ShowTooltip = NotificationMsgBegin,
		/**
		* @brief �����ض��û�����Ӧ��tooltipʱ���ô˺���
		* @param uid �û�uid
		* @param handle ����������Ϣ
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
	* @brief ע��֪ͨ������
	* @param priority ���ȼ�
	* @param method ע��ĺ���������ֵ�μ�namespace NotificationMsg::Method
	* @param notify ֪ͨ������ָ�룬Ҫ��notify��������invoke�������Ե��õ���ע���method������
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool registerNotifyProcessor(qint32 priority, NotificationMsg::Method method, IDWQComponentPtr notify) = 0;

	/**
	* @brief ��ע��֪ͨ������
	* @param method ��ע��ĺ���������ֵ�μ�namespace NotificationMsg::Method
	* @param notify֪ͨ������ָ��
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool unregisterNotifyProcessor(NotificationMsg::Method method, IDWQComponentPtr notify) = 0;

	/**
	* @brief ����֪ͨ����������
	* @param method ֪ͨ����������������ֵ�μ�namespace NotificationMsg::Method
	* @param args �����б�˳����ݺ���ȷ��
	* @return �Ƿ���óɹ�
	*/
	virtual bool invokeNotifyProcessor(NotificationMsg::Method method, QVariantList &args) = 0;
};