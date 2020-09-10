
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiStackedWidgetPrivate;

class DW_DUIFW_EXPORT DuiStackedWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiStackedWidget )
	Q_DECLARE_PRIVATE( DuiStackedWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiStackedWidget )

public:
	enum EffectType
	{
		NullEffect,
		SlideInOutEffect
	};
	Q_ENUMS(EffectType)
	Q_PROPERTY( EffectType effectType READ effectType WRITE setEffectType )

	enum ChildWidgetSizeMode
	{
		fixedMode,
		adjustMode
	};
	Q_ENUMS(ChildWidgetSizeMode)

	explicit DuiStackedWidget( QGraphicsItem *parent = NULL );
	~DuiStackedWidget();

	virtual void addItem( DuiWidget* item );
	virtual void removeItem( DuiWidget* item );

	void removeItem( int index );
	void removeAll();

	int insertWidget(int index, DuiWidget *view);

	int currentIndex() const;
	Q_INVOKABLE void setCurrentIndex(int index);
	Q_INVOKABLE void setCurrentWidget( const DuiWidget *widget );

	DuiWidget *widgetAt(int index) const;
	DuiWidget *widgetAtByName( const QString& objectName ) const;
	int widgetIndex( const DuiWidget* widget );

	int count() const;

	EffectType effectType() const;
	void setEffectType(EffectType effect);

	void setChildWidgetSizeMode( DuiStackedWidget::ChildWidgetSizeMode mode );

signals:
	void currentIndexChanged(int oldIndex, int currentIndex);

protected slots:
	void onAnimationStateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
	void onAnimationFinished();

protected:
	DuiStackedWidget( DuiStackedWidgetPrivate &dd, QGraphicsItem* parent = NULL);

	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void setLayout( QGraphicsLayout *layout );
	virtual QGraphicsLayout *layout () const;
	virtual void construct();

private:
	void _resetLayout();
};