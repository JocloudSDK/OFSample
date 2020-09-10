#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use coCreateComInstance function.
*/
class DuiWidget;

DWDEFINE_INTERFACE(IYYImHighDefinitionHeadHandler): public IUnk
{

	/**
	* @brief ��ʼ��.
	* @param ��Ҫ����ͷ��Ĵ���.
	*/	
	virtual void init(DuiWidget*) = 0;
	
	/**
	* @brief �����Ƿ�Ϊ����ͷ�� ����ͷ�����۽�.
	* @param trueΪ��ʾ����ͷ��.
	*/
	virtual void setShowHD(bool) = 0;
	
	/**
	* @brief �����۽ǵ�λ��.
	* @param .
	*/
	virtual void setConnerArea(QRect) = 0;

	/**
	* @brief ���ڻ��Ƹ���ͷ��.
	* @param ����.
	* @param ����ͷ�������.
	* @param ͷ��ͼƬ.
	*/	
	virtual void paintHead( QPainter*,QRect,QPixmap& ) = 0;

	/**
	* @brief �����ڻ���ͷ���״̬ ���� ����ɼ�.
	* @param ����.
	* @param ����logo������.
	* @param logoͼƬ.
	*/	
	virtual void paintLogo( QPainter*,QRect,QPixmap& ) = 0;

	/**
	* @brief �������ͷ���۽ǵľ�.
	* @param ��Ӧ�¼�������.
	*/	
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent*, QRectF) = 0;
};

class DuiWidget;
/**
* @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYImHighDefinitionHeadWidget): public IDWQComponent
{
	
	/**
	* @brief ����ͷ��·�� �еĻ�.
	* @return . 
	*/
	virtual QString imagePath();

	/**
	* @brief ���ø���ͷ��.
	* @param ͷ����ļ�·��.
	*/
	virtual void setImagePath( QString );

	/**
	* @brief ͷ������½Ǳ�־.
	* @param ͷ����ļ�·��.
	*/
	virtual void setLeftLogoPath( QString );

	/**
	* @brief ͷ������½Ǳ�־.
	* @param ͷ����ļ�·��.
	*/
	virtual void setRightLogoPath( QString );

	/**
	* @brief �����Ƿ���ʾ����ͷ��.
	* @return ��ʾ����true.
	*/
	virtual bool isShowHD() = 0;

	/**
	* @brief �����Ƿ�Ϊ����ͷ�� ����ͷ�����۽�.
	* @param trueΪ��ʾ����ͷ��.
	*/	
	virtual void setShowHD(bool) = 0;

	/**
	* @brief ����ͷ��.
	* @return . 
	*/
	virtual QPixmap headImage() = 0;

	/**
	* @brief ����ͷ��.
	* @param ͷ��ͼƬ.
	*/
	virtual void setHeadImage(QPixmap&) = 0;

	/**
	* @brief ����ͷ�����½ǵ�log ������ɼ�.
	* @param logoͼƬ.
	*/
	virtual void setLeftBottomLogo(QPixmap&) = 0;

	/**
	* @brief ����ͷ�����½ǵ�log ���뿪.
	* @param logoͼƬ.
	*/
	virtual void setRightBottomLogo(QPixmap&) = 0;

	/**
	* @brief ���ظ���ͷ����۽�����.
	* @return .
	*/
	virtual QRectF getConnerArea() = 0;

/*
signals:
	void press(QGraphicsSceneMouseEvent *event);
	void release(QGraphicsSceneMouseEvent * event);
	void doubleClicked(QGraphicsSceneMouseEvent * event);
	void mouseMove(QGraphicsSceneHoverEvent * event);
	void entered();
	void leaved();
*/
};