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
	* @brief 初始化.
	* @param 需要绘制头像的窗口.
	*/	
	virtual void init(DuiWidget*) = 0;
	
	/**
	* @brief 设置是否为高清头像 高清头像有折角.
	* @param true为显示高清头像.
	*/
	virtual void setShowHD(bool) = 0;
	
	/**
	* @brief 设置折角的位置.
	* @param .
	*/
	virtual void setConnerArea(QRect) = 0;

	/**
	* @brief 用于绘制高清头像.
	* @param 画笔.
	* @param 绘制头像的区域.
	* @param 头像图片.
	*/	
	virtual void paintHead( QPainter*,QRect,QPixmap& ) = 0;

	/**
	* @brief 可用于绘制头像的状态 隐身 隐身可见.
	* @param 画笔.
	* @param 绘制logo的区域.
	* @param logo图片.
	*/	
	virtual void paintLogo( QPainter*,QRect,QPixmap& ) = 0;

	/**
	* @brief 处理高清头像折角的卷动.
	* @param 响应事件的区域.
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
	* @brief 返回头像路径 有的话.
	* @return . 
	*/
	virtual QString imagePath();

	/**
	* @brief 设置高清头像.
	* @param 头像的文件路径.
	*/
	virtual void setImagePath( QString );

	/**
	* @brief 头像的左下角标志.
	* @param 头像的文件路径.
	*/
	virtual void setLeftLogoPath( QString );

	/**
	* @brief 头像的右下角标志.
	* @param 头像的文件路径.
	*/
	virtual void setRightLogoPath( QString );

	/**
	* @brief 返回是否显示高清头像.
	* @return 显示返回true.
	*/
	virtual bool isShowHD() = 0;

	/**
	* @brief 设置是否为高清头像 高清头像有折角.
	* @param true为显示高清头像.
	*/	
	virtual void setShowHD(bool) = 0;

	/**
	* @brief 返回头像.
	* @return . 
	*/
	virtual QPixmap headImage() = 0;

	/**
	* @brief 设置头像.
	* @param 头像图片.
	*/
	virtual void setHeadImage(QPixmap&) = 0;

	/**
	* @brief 设置头像左下角的log 如隐身可见.
	* @param logo图片.
	*/
	virtual void setLeftBottomLogo(QPixmap&) = 0;

	/**
	* @brief 设置头像右下角的log 如离开.
	* @param logo图片.
	*/
	virtual void setRightBottomLogo(QPixmap&) = 0;

	/**
	* @brief 返回高清头像的折角区域.
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