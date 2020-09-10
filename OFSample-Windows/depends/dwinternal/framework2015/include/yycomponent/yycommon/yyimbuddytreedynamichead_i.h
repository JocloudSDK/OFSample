#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duiimagewidget.h"

class DuiWidget;

/**
* @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYImDynamicHead): public IDWQComponent
{
	/**
	* @brief ʹ��ͼƬ·��������widget���� ֧�־�̬ͼƬ.
	* @param ��Ҫ���ŵĶ���ͼƬ·��.
	*/
	virtual void setImagePath(const QString &) = 0;

	/**
	* @brief ʹ��ͼƬ·��������widget���� ֧�־�̬ͼƬ.
	* @return �ӿ����޿���widgetʱ Ҳ ������"".
	*/
	virtual QString imagePath() const = 0;

	/**
	* @brief ʹ�ö�����������widget����.
	* @param ��������.
	*/
	virtual void setImageData(const QByteArray &) = 0;

	/**
	* @brief widget������ʾ��̬ͼƬ��ʹ��QPixmap����.
	* @param ��Ҫ���ý�widget��QPixmap.
	*/
	virtual void setPixmap(const QPixmap &) = 0;

	/**
	* @brief widget������ʾ��̬ͼƬ��ʹ��QPixmap����.
	* @return �ӿ����޿���widgetʱ Ҳ ������QPixmap().
	*/
	virtual QPixmap pixmap() = 0;

	/**
	* @brief ����widget���ڸǰ�.
	* @param mask���ļ���.
	*/
	virtual void setMaskPath(const QString &) = 0;
	
	/**
	* @brief ����widget���ڸǰ�.
	* @return �ӿ����޿���widgetʱ Ҳ ������"".
	*/
	virtual QString maskPath() const = 0;

	/**
	* @brief ���Ŷ�������ز���.
	* @param timesΪ���Ŵ�����Ĭ�ϲ�����unsigned���ܱ�ʾ�����ֵ.
	*/
	virtual void play(unsigned = 0xffffffff) = 0;

	/**
	* @brief ���Ŷ�������ز���.
	* @param timesΪ���Ŵ����������������Ĭ����һֱ����.
	*/
	virtual void stop() = 0;

	/**
	* @brief ���Ŷ�������ز���.
	* @param timesΪ���Ŵ����������������Ĭ����һֱ����.
	*/
	virtual void pause() = 0;

	/**
	* @brief ���Ŷ�������ز���.
	* @param timesΪ���Ŵ����������������Ĭ����һֱ����.
	*/
	virtual void finish() = 0;

	/**
	* @brief �������λ��widget�Ϸ��Ƿ�Ϊ����.
	* @param . 
	*/
	virtual void setHandHover(bool) = 0;
	
	/**
	* @brief �������λ��widget�Ϸ��Ƿ�Ϊ����.
	* @return ��Ϊ���λ�ӿ����޿���widget����false.
	*/
	virtual bool handHover() const = 0;

	/**
	* @brief ���ö�����̬ͼƬ��widget�е��Ų�����.
	* @param ͼƬ�Ų�����.
	*/
	virtual void setScalePolicy( DuiImageWidget::ImageScalePolicy ) = 0;

	/**
	* @brief ���ö�����̬ͼƬ��widget�е��Ų�����.
	* @return �ӿ����޿���widgetʱ Ҳ ������DuiImageWidget::ScaleNone.
	*/
	virtual DuiImageWidget::ImageScalePolicy scalePolicy() = 0;	

	/**
	* @brief ����widget�Ƿ����over�¼�.
	* @param . 
	*/
	virtual void setAcceptHover(bool) = 0;

	/**
	* @brief ����widget�Ƿ����over�¼�.
	* @return �ӿ����޿���widgetʱ Ҳ ������false.
	*/
	virtual bool acceptHover() const = 0;

	/**
	* @brief ����widget�Ƿ����˫���¼�.
	* @param . 
	*/
	virtual void setAcceptDoubleClick(bool) = 0;

	/**
	* @brief ����widget�Ƿ����˫���¼�.
	* @return �ӿ����޿���widgetʱ Ҳ ������false.
	*/
	virtual bool acceptDoubleClick() const = 0;

	/**
	* @brief ����ͷ���Ӧ��uid.
	* @param . 
	*/
	virtual void setUid(UINT32) = 0;

	/**
	* @brief ����ͷ���Ӧ��uid. 
	* @return �ӿ����޿���widgetʱ Ҳ ������0
	*/
	virtual UINT32 uid() const = 0;

	/**
	* @brief �������öԷ�ͷ��״̬ ���� ��Ϸ�� ������ͷ�����½�.
	* @param . 
	*/
	virtual void setStatusPix(const QPixmap &) = 0;

	/**
	* @brief �������öԷ�ͷ��״̬ ���� ��Ϸ�� ������ͷ�����½�.
	* @return �ӿ����޿���widgetʱ Ҳ ������QPixmap().
	*/
	virtual QPixmap statusPix() const = 0;

	/**
	* @brief ���������Լ����ڶԷ��Ƿ�����ɼ� ������ͷ�����½�.
	* @param . 
	*/
	virtual void setUnHidePix(const QPixmap &) = 0;

	/**
	* @brief ���������Լ����ڶԷ��Ƿ�����ɼ� ������ͷ�����½�.
	* @return �ӿ����޿���widgetʱ Ҳ ������QPixmap().
	*/
	virtual QPixmap unHidePix() const = 0;

	/**
	* @brief ͷ�񶯻��Ƿ����ڲ���.
	* @return �ӿ����޿���widgetʱ Ҳ ������false.
	*/
	virtual bool isPlaying() = 0;

	/**
	* @brief �ӿ����Ƿ�������õ�widget.
	* @return ���򷵻�true.
	*/
	virtual bool isWidgetAvailable() = 0;

/*
signals:
	void clicked();
	void pressed( const QPointF & );
	void released( const QPointF & );
	void doubleClicked(QGraphicsSceneMouseEvent *);
	void entered();
	void leaved();
*/
};
