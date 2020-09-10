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
	* @brief 使用图片路径名设置widget动画 支持静态图片.
	* @param 需要播放的动画图片路径.
	*/
	virtual void setImagePath(const QString &) = 0;

	/**
	* @brief 使用图片路径名设置widget动画 支持静态图片.
	* @return 接口中无可用widget时 也 将返回"".
	*/
	virtual QString imagePath() const = 0;

	/**
	* @brief 使用动画数据设置widget动画.
	* @param 动画数据.
	*/
	virtual void setImageData(const QByteArray &) = 0;

	/**
	* @brief widget可以显示静态图片，使用QPixmap设置.
	* @param 需要设置进widget的QPixmap.
	*/
	virtual void setPixmap(const QPixmap &) = 0;

	/**
	* @brief widget可以显示静态图片，使用QPixmap设置.
	* @return 接口中无可用widget时 也 将返回QPixmap().
	*/
	virtual QPixmap pixmap() = 0;

	/**
	* @brief 设置widget的掩盖板.
	* @param mask的文件名.
	*/
	virtual void setMaskPath(const QString &) = 0;
	
	/**
	* @brief 设置widget的掩盖板.
	* @return 接口中无可用widget时 也 将返回"".
	*/
	virtual QString maskPath() const = 0;

	/**
	* @brief 播放动画的相关操作.
	* @param times为播放次数，默认参数是unsigned所能表示的最大值.
	*/
	virtual void play(unsigned = 0xffffffff) = 0;

	/**
	* @brief 播放动画的相关操作.
	* @param times为播放次数，若不填参数则默认是一直播放.
	*/
	virtual void stop() = 0;

	/**
	* @brief 播放动画的相关操作.
	* @param times为播放次数，若不填参数则默认是一直播放.
	*/
	virtual void pause() = 0;

	/**
	* @brief 播放动画的相关操作.
	* @param times为播放次数，若不填参数则默认是一直播放.
	*/
	virtual void finish() = 0;

	/**
	* @brief 设置鼠标位于widget上方是否为手形.
	* @param . 
	*/
	virtual void setHandHover(bool) = 0;
	
	/**
	* @brief 设置鼠标位于widget上方是否为手形.
	* @return 不为手形或接口中无可用widget返回false.
	*/
	virtual bool handHover() const = 0;

	/**
	* @brief 设置动画或静态图片在widget中的排布策略.
	* @param 图片排布策略.
	*/
	virtual void setScalePolicy( DuiImageWidget::ImageScalePolicy ) = 0;

	/**
	* @brief 设置动画或静态图片在widget中的排布策略.
	* @return 接口中无可用widget时 也 将返回DuiImageWidget::ScaleNone.
	*/
	virtual DuiImageWidget::ImageScalePolicy scalePolicy() = 0;	

	/**
	* @brief 设置widget是否接受over事件.
	* @param . 
	*/
	virtual void setAcceptHover(bool) = 0;

	/**
	* @brief 设置widget是否接受over事件.
	* @return 接口中无可用widget时 也 将返回false.
	*/
	virtual bool acceptHover() const = 0;

	/**
	* @brief 设置widget是否接受双击事件.
	* @param . 
	*/
	virtual void setAcceptDoubleClick(bool) = 0;

	/**
	* @brief 设置widget是否接受双击事件.
	* @return 接口中无可用widget时 也 将返回false.
	*/
	virtual bool acceptDoubleClick() const = 0;

	/**
	* @brief 设置头像对应的uid.
	* @param . 
	*/
	virtual void setUid(UINT32) = 0;

	/**
	* @brief 设置头像对应的uid. 
	* @return 接口中无可用widget时 也 将返回0
	*/
	virtual UINT32 uid() const = 0;

	/**
	* @brief 用于设置对方头像状态 隐身 游戏等 覆盖在头像右下角.
	* @param . 
	*/
	virtual void setStatusPix(const QPixmap &) = 0;

	/**
	* @brief 用于设置对方头像状态 隐身 游戏等 覆盖在头像右下角.
	* @return 接口中无可用widget时 也 将返回QPixmap().
	*/
	virtual QPixmap statusPix() const = 0;

	/**
	* @brief 用于设置自己对于对方是否隐身可见 覆盖在头像左下角.
	* @param . 
	*/
	virtual void setUnHidePix(const QPixmap &) = 0;

	/**
	* @brief 用于设置自己对于对方是否隐身可见 覆盖在头像左下角.
	* @return 接口中无可用widget时 也 将返回QPixmap().
	*/
	virtual QPixmap unHidePix() const = 0;

	/**
	* @brief 头像动画是否正在播放.
	* @return 接口中无可用widget时 也 将返回false.
	*/
	virtual bool isPlaying() = 0;

	/**
	* @brief 接口中是否包含可用的widget.
	* @return 有则返回true.
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
