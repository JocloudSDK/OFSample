#include "stable.h"

#include "duifw/duiwidget.h"
#include "tooltipdecoratedwidget.h"

namespace
{
	const quint32 margin = 1;

	QSize widgetDecoratedSize( DuiWidget* widget, const DuiToolTipStyle2& style )
	{
		QSize widgetSize = widget->size().toSize();
		if ( widgetSize.isNull() )
		{
			widgetSize = widget->preferredSize().toSize();
			DW_ASSERT( !widgetSize.isNull() );
		}

		quint32 triangleValue = style.triangleHeight();

		switch( style.arrowType() )
		{
		case Qt::LeftArrow:
		case Qt::RightArrow:
			{
				widgetSize.rwidth() += triangleValue;
				break;
			}

		case Qt::UpArrow:
		case Qt::DownArrow:
			{
				widgetSize.rheight() += triangleValue;
				break;
			}
		default:
			break;
		}

		widgetSize += QSize(margin * 2, margin * 2);
		return widgetSize;
	}
	
	QPainterPath createPainterPath( const QSize& widgetSize, const DuiToolTipStyle2& style, quint32 preferredOffset )
	{
		QRect rect(0, 0, widgetSize.width() + margin * 2, widgetSize.height() + margin * 2);

		quint32 offset = 0;
		if ( preferredOffset != 0 )
		{
			offset = preferredOffset;
		}
		else
		{
			if ( style.triangleOffset() == 0 && style.triangleOffsetPercent() != 0 )
			{
				offset = widgetSize.width() * ( style.triangleOffsetPercent() / 100.0 );
			}
			else
			{
				offset = style.triangleOffset();
			}
		}

		//三角型边长
		quint32 triangleLen = style.triangleHeight();
		//三角型高.
		qreal triangleHeight = triangleLen * 0.5;
		
		offset -= triangleHeight ; //鼠标点指向三角顶点。

		QPolygonF trianglePolygon;
		switch( style.arrowType() )
		{
			case Qt::LeftArrow:
				{
					trianglePolygon << QPointF(0,0)
									<< QPointF(triangleHeight * -1, triangleHeight)
									<< QPointF(0,triangleLen);

					trianglePolygon.translate(QPoint(triangleLen,offset));
					rect.translate(QPoint(triangleLen,0));
				}
				break;
			case Qt::RightArrow:
				{
					trianglePolygon << QPoint(0,0)
									<< QPoint(triangleHeight, triangleHeight )
									<< QPoint(0,triangleLen);

					trianglePolygon.translate( QPoint(rect.width(),offset) );
				}
				break;
			case Qt::UpArrow:
				{
					trianglePolygon << QPoint(0,0)
									<< QPoint(triangleHeight, triangleHeight * -1)
									<< QPoint(triangleLen, 0); 

					trianglePolygon.translate(QPoint(offset, triangleLen));
					rect.translate(QPoint(0, triangleLen));
				}
				break;
			case Qt::DownArrow:
				{
					trianglePolygon << QPoint(0,0)
									<< QPoint(triangleHeight, triangleHeight)
									<< QPoint(triangleLen,0);

					trianglePolygon.translate( QPoint(offset, rect.height()) );
				}
				break;
			default:
				break;
		}

		QPainterPath path;
		path.addRoundedRect( rect, style.roundedRect(), style.roundedRect() );
		path.addPolygon( trianglePolygon );

		return path.simplified();
	}

}
//////////////////////////////////////////////////////////////////////////

void ToolTipDecoratedWidget::setWidget( DuiWidget* widget )
{
	if ( m_targetWidget != widget )
	{
		m_targetWidget = widget;
	}
}

void ToolTipDecoratedWidget::setStyle( const DuiToolTipStyle2 &style )
{
	m_style = style;
}

QSize ToolTipDecoratedWidget::preferredSize()
{
	if ( m_targetWidget )
	{
		return widgetDecoratedSize( m_targetWidget, m_style ); 
	}

	return QSize();
}

void ToolTipDecoratedWidget::_createPath()
{
	if ( m_targetWidget )
	{
		QSizeF widgetSize = m_targetWidget->size();
		if ( widgetSize.isNull() )
		{
			widgetSize = m_targetWidget->preferredSize();
		}

		m_path = createPainterPath( widgetSize.toSize(), m_style, m_preferredOffset );
	}
}

void ToolTipDecoratedWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	DuiWidget::paint( painter, option, widget );

	if ( m_path.isEmpty() )
	{
		_createPath();
		_adjustWidget();
	}

	QBrush backgroundBrush = m_style.background();
	QGradient *pGradient = const_cast<QGradient*>( backgroundBrush.gradient() );
	if ( pGradient )
	{
		if ( pGradient->type() == QGradient::LinearGradient )
		{
			QLinearGradient* linearGradient = static_cast< QLinearGradient* >(pGradient);
			if ( linearGradient )
			{
				linearGradient->setStart(0,0);
				linearGradient->setFinalStop(0, height());
			}
		}
	}

	//painter->setRenderHint(QPainter::Antialiasing);
	painter->fillPath( m_path, backgroundBrush );
	painter->setPen( m_style.border() );
	painter->drawPath( m_path );
	
}

ToolTipDecoratedWidget::ToolTipDecoratedWidget( DuiWidget* parent /*= NULL */ )
: DuiWidget( parent )
, m_preferredOffset(0)
{

}

void ToolTipDecoratedWidget::_adjustWidget()
{
	if ( !m_targetWidget )
	{
		return;
	}

	m_targetWidget->setPos(0,0);

	switch( m_style.arrowType() )
	{
	case Qt::UpArrow:
		{
			m_targetWidget->setPos(margin, m_style.triangleHeight() + margin);
			break;
		}
	case Qt::DownArrow:
	case Qt::RightArrow:
		{
			m_targetWidget->setPos( margin, margin);
			break;
		}
	case Qt::LeftArrow:
		{
			m_targetWidget->setPos( m_style.triangleHeight() + margin, margin);
			break;
		}
	default:
		break;
	}
}

void ToolTipDecoratedWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	DuiWidget::resizeEvent(event);

	if ( m_targetWidget )
	{
		_createPath();
		_adjustWidget();
	}
}

const DuiToolTipStyle2& ToolTipDecoratedWidget::style() const
{
	return m_style;
}

void ToolTipDecoratedWidget::setPreferredOffset( quint32 offset )
{
	m_preferredOffset = offset;
}

DuiWidget* ToolTipDecoratedWidget::targetWidget()
{
	return m_targetWidget;
}