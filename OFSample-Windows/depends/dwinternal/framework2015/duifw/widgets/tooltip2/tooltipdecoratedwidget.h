#pragma once

#include "duifw/duiwidget.h"
#include "duitooltipstyle2.h"

class ToolTipDecoratedWidget : public DuiWidget
{
public:
	explicit ToolTipDecoratedWidget( DuiWidget* parent = NULL );

	void	setWidget( DuiWidget* targetWidget );
	DuiWidget*	targetWidget();

	void	setStyle( const DuiToolTipStyle2 &style );
	const DuiToolTipStyle2& style() const;

	QSize	preferredSize();

	void	setPreferredOffset( quint32 offset );

protected:
	virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ); 
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
	void	_createPath();
	void	_adjustWidget();

private:
	int					m_preferredOffset;
	DuiToolTipStyle2	m_style;
	QPainterPath		m_path;
	QPointer<DuiWidget>	m_targetWidget;
};
