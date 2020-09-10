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

	//Ŀǰֻ��Ҫʵ������4���ӿ�
	//����ע������ȼ���ѯ
	//�����Ҫ��Ӧuid���򷵻�true
	virtual bool canHandle(quint32 uid) = 0;

	//������Ҫ��Ӧ��pixmap
	virtual QPixmap fetchPixmap(ITreeItemInfo *userInfo) = 0;

	//virtual DuiWidget *tooltipWidget(ITreeItemInfo *userInfo) = 0;
	//virtual void onClicked(quint32 uid) = 0;
};

/**
* @brief �ڲ�β�ƽӿڣ��ó�ʼ������������.
*/
DWDEFINE_INTERFACE(IYYWeidengInner): public IUnk
{
/**
   * @brief ��ʼ��.
   * @param spChannel��Ƶ��ָ��.
   */
	virtual void init(IChannelPtr spChannel) = 0;
/**
   * @brief ����ʼ��.
   */
	virtual void unInit() = 0;

/**
   * @brief ע��Ƶ����ͷ�ƴ�����
   * @param priority ��ѯ���ȼ� (���õ���vip-100, MobileLight-200, WebyyLight-300)
   * @param handler ������ָ��
   */
	virtual int registerHeadLightIndicator(int priority, IHeadLightIndicatorHandler *handler) = 0;

	/**
   * @brief ��ע��Ƶ����ͷ�ƴ�����
   * @param handler ������ָ��
   */
	virtual int unRegisterHeadLightIndicator(IHeadLightIndicatorHandler *handler) = 0;

    /**
    * @brief ע��js���proxy
    */
    virtual void registeScriptObjectProxy() = 0;

};

/**
* @brief β�ƽӿ�.
*/
DWDEFINE_INTERFACE(IYYWeideng):public IUnk
{

	/**
	* @brief ����β���б�.
	* @return ����β���б�.
	*/
	virtual QList<ITreeIndicatorHandler *> getMaixuIndicator() = 0; // for maixu
	/**
	* @brief �û���β���б�.
	* @return �û���β���б�.
	*/
	virtual QList<ITreeIndicatorHandler *> getUserlistIndicator() = 0; // for userlist
/**
   * @brief β������.
   */
	virtual void clear() = 0;
};