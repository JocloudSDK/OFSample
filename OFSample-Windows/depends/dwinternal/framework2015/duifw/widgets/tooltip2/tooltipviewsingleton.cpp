#include "stable.h"
#include "tooltipviewsingleton.h"
#include "duitooltipview.h"

Q_GLOBAL_STATIC(ToolTipViewSingleton, _tooltipViewSingleton);

ToolTipViewSingleton* toolTipViewSingleton()
{
	return _tooltipViewSingleton();
}

namespace
{
	const int KCursorHeight = 20;

	int referenceOffset( const DuiToolTipStyle2& style, const QSize& viewSize )
	{
		int refecenceLen = 0;
		int	referenceOffset = 0;

		switch( style.arrowType() )
		{
		case Qt::DownArrow:
		case Qt::UpArrow:
			refecenceLen = viewSize.width() - KShadowEffectOffset;
			break;

		case Qt::LeftArrow:
		case Qt::RightArrow:
			refecenceLen = viewSize.height();
			break;
		default:
			DW_ASSERT(false);
		}

		if ( style.referenceOffset() == 0 && style.referencePercent() != 0 )
		{
			referenceOffset = style.referencePercent() / 100.0 * refecenceLen;
		}
		else
		{
			referenceOffset = style.referenceOffset();
		}

		return referenceOffset;
	}

	QRect viewGeometry( const DuiToolTipStyle2& style, const QSize& viewSize )
	{
		DW_ASSERT( style.referenceType() == DuiToolTipStyle2::ReferenceMouse );

		QPoint	pos;
		QPoint	mousePoint = QCursor::pos();
		quint32	spacing = style.referenceSpacing();

		int	offset = referenceOffset(style,viewSize);

		switch( style.arrowType() )
		{
		case Qt::LeftArrow:
			{
				pos = QPoint( mousePoint.x()+ KCursorHeight + spacing, mousePoint.y() + offset  );
				break;
			}
		case Qt::RightArrow:
			{
				pos = QPoint( mousePoint.x() - spacing - viewSize.width() , mousePoint.y() + offset );
				break;
			}
		case Qt::UpArrow:
			{
				pos = QPoint( mousePoint.x() + offset, mousePoint.y() + KCursorHeight + spacing );
				break;
			}
		case Qt::DownArrow:
			{
				pos = QPoint( mousePoint.x() + offset, mousePoint.y() - viewSize.height() - spacing );
				break;
			}
		default:
			break;
		}

		return QRect(pos,viewSize);
	}

	bool ensureFullRectVisible( QRect& viewRect, DuiToolTipStyle2& style )
	{
		int screen = QApplication::desktop()->screenNumber(QCursor::pos());
		QRect validRect = QApplication::desktop()->screenGeometry(screen);

		for (int i = 0; i < QApplication::desktop()->numScreens(); ++i)
		{
			if (i == screen)
			{
				continue;
			}

			QRect rc = QApplication::desktop()->screenGeometry(i);
			if (validRect.right() + 1 == rc.left())
			{
				validRect.setRight(rc.right());
			}
			else if (rc.right() + 1 == validRect.left())
			{
				validRect.setLeft(rc.left());
			}

			if (validRect.bottom() + 1 == rc.top())
			{
				validRect.setBottom(rc.bottom());
			}
			else if (rc.bottom() + 1 == validRect.top())
			{
				validRect.setTop(rc.top());
			}
		}

		int maxWidgth = validRect.width()- viewRect.width();
		int maxHeight = validRect.height() - viewRect.height();

		bool fixUpStyle = false;

		QPoint newTopLeftPoint = viewRect.topLeft();

		if ( newTopLeftPoint.x() < validRect.left() )
		{
			if ( style.arrowType() == Qt::UpArrow || 
				 style.arrowType() == Qt::DownArrow )
			{
				newTopLeftPoint.setX(validRect.left());
			}
			else if ( style.arrowType() == Qt::RightArrow )
			{
				fixUpStyle = true;
				style.setArrowType( Qt::LeftArrow );
			}
		}
		else if ( newTopLeftPoint.x() > maxWidgth )
		{
			if ( style.arrowType() == Qt::UpArrow || 
				 style.arrowType() == Qt::DownArrow )
			{
				newTopLeftPoint.setX( maxWidgth );
			}
			else if ( style.arrowType() == Qt::LeftArrow )
			{
				fixUpStyle = true;
				style.setArrowType( Qt::RightArrow );
			}
		}

		if ( newTopLeftPoint.y() < validRect.top() )
		{
			if ( style.arrowType() == Qt::LeftArrow || 
				 style.arrowType() == Qt::RightArrow )
			{
				newTopLeftPoint.setY(validRect.top());
			}
			else if ( style.arrowType() == Qt::DownArrow )
			{
				fixUpStyle = true;
				style.setArrowType(Qt::UpArrow);
			}
		}
		else if ( newTopLeftPoint.y() > maxHeight )
		{
			if ( style.arrowType() == Qt::LeftArrow || 
				 style.arrowType() == Qt::RightArrow )
			{
				newTopLeftPoint.setY( maxHeight );
			}
			else if ( style.arrowType() == Qt::UpArrow )
			{
				fixUpStyle = true;
				style.setArrowType(Qt::DownArrow);
			}
		}

		viewRect.moveTo(newTopLeftPoint);
		return fixUpStyle;
	}

	quint32 preferredTriangleOffset( const QPoint& mousePoint, const QRect& viewRect, const DuiToolTipStyle2& style )
	{
		switch ( style.arrowType() )
		{
		case Qt::UpArrow:
		case Qt::DownArrow:
			{
				return mousePoint.x() - viewRect.topLeft().x();
				break;
			}
		case Qt::LeftArrow:
		case Qt::RightArrow:
			{
				return mousePoint.y() - viewRect.topLeft().y();
				break;
			}
		}
		return 0;
	}

	QRect viewGeometry( const DuiToolTipStyle2& style, const QSize& viewSize, const QRect& referenceRect )
	{
		//DW_ASSERT( style.referenceType() == DuiToolTipStyle2::ReferenceWidget );

		QPoint pos;
		QPoint topLeft = referenceRect.topLeft();
		
		int offset = referenceOffset(style, viewSize);
		int spacing = style.referenceSpacing();

		switch( style.arrowType() )
		{
		case Qt::UpArrow:
			{
				pos.setX( topLeft.x() + offset);
				pos.setY( topLeft.y() + referenceRect.height() + spacing );
				break;
			}
		case Qt::DownArrow:
			{
				pos.setX( topLeft.x() + offset);
				pos.setY( topLeft.y() - viewSize.height() - spacing );
				break;
			}
		case Qt::LeftArrow:
			{
				pos.setY( topLeft.y() + offset);
				pos.setX( topLeft.x() + referenceRect.width() + spacing );
				break;
			}
		case Qt::RightArrow:
			{
				pos.setY( topLeft.y() + offset);
				pos.setX( topLeft.x() - viewSize.width() - spacing );
				break;
			}
		default:
			break;
		}

		return QRect(pos,viewSize);
	}
}// namespace

ToolTipViewSingleton::ToolTipViewSingleton()
{

}

ToolTipViewSingleton::~ToolTipViewSingleton()
{

}

void ToolTipViewSingleton::closeToolTipView()
{
	if ( m_view )
	{
		LogFinal(LOG::KDuiFw) << "view hide and close begin : " << m_view.data();
		m_view->hide();
		m_view->close();
		m_view = NULL;
		LogFinal(LOG::KDuiFw) << "view hide and close end.";
	}
}

void ToolTipViewSingleton::setWidget( DuiWidget* widget, bool tackOwnerShip /*= true */ )
{
	DW_ASSERT( m_view );

	if(m_view)
	{
		m_view->setWidget(widget, tackOwnerShip);
	}
}

void ToolTipViewSingleton::setStyle( const DuiToolTipStyle2& style )
{
	DW_ASSERT(m_view);
	if(m_view)
	{
		m_view->setToolTipStyle(style);
	}
}

void ToolTipViewSingleton::initView()
{
	if ( m_view == NULL )
	{
		m_view = new DuiToolTipView(NULL);
		m_view->setObjectName("toolTips");
	}
	else
	{
		m_view->clearScene();
	}
}

void ToolTipViewSingleton::showToolTipView()
{
	DW_ASSERT( m_view );
	if ( !m_view )
	{
		return;
	}

	QSize viewSize = m_view->prefreedViewSize();
	QRect viewRect;

	quint32 triangleOffset = 0;

	if ( m_view->toolTipStyle().referenceType() == DuiToolTipStyle2::ReferenceMouse )
	{
		DuiToolTipStyle2 style = m_view->toolTipStyle();
		viewRect = viewGeometry(style, viewSize);

		if ( ensureFullRectVisible(viewRect,style) )
		{
			m_view->setToolTipStyle(style);
			viewRect = viewGeometry(style,viewSize);
			ensureFullRectVisible(viewRect, style);
		}

		triangleOffset = preferredTriangleOffset( QCursor::pos(), viewRect, m_view->toolTipStyle() );
	}
	else if ( m_view->toolTipStyle().referenceType() == DuiToolTipStyle2::ReferenceWidget )
	{
		DuiToolTipStyle2 style = m_view->toolTipStyle();
		viewRect = viewGeometry(style, viewSize, m_referenceRect);

		if ( ensureFullRectVisible(viewRect,style) )
		{
			m_view->setToolTipStyle(style);
			viewRect = viewGeometry(style, viewSize, m_referenceRect);
		}

		triangleOffset = preferredTriangleOffset( m_referenceRect.topLeft(), viewRect, m_view->toolTipStyle() );
	}

	m_view->setPrefeffedTriangleOffset(triangleOffset);

	if ( m_view->isVisible() )
	{
		_animationMoveView(viewRect);
	}
	else
	{
		m_view->setGeometry(viewRect);
		_animationShowView();
	}
}

DuiWidget* ToolTipViewSingleton::rootWidget()
{
	DW_ASSERT(m_view);

	if(m_view)
	{
		return m_view->rootWidget();
	}

	return NULL;
}

void ToolTipViewSingleton::setReferenceRect( const QRect& rect /*= QRect() */ )
{
	m_referenceRect = rect;
}

QRect ToolTipViewSingleton::tooltipViewRect()
{
	DW_ASSERT(m_view);

	if(m_view)
	{
		return m_view->geometry();
	}

	return QRect();
}

void ToolTipViewSingleton::showTooltipView( const QPoint& pos, QWidget* parent )
{
	DW_ASSERT( m_view );
	if ( !m_view )
	{
		return;
	}

	if ( parent )
	{
		m_view->setParent(parent);
	}

	QSize viewSize = m_view->prefreedViewSize();

	QPoint viewPos = pos;
	if ( viewPos.isNull() )
	{
		viewPos = viewGeometry( m_view->toolTipStyle(), viewSize ).topLeft();
	}
	else
	{
		viewPos = viewGeometry( m_view->toolTipStyle(), viewSize, QRect(viewPos,QSize(0,0)) ).topLeft();
	}

	if ( m_view->isVisible() )
	{
		_animationMoveView( QRect(viewPos, viewSize) );
	}
	else
	{
		m_view->setGeometry(QRect(viewPos, viewSize));
		_animationShowView();
	}
}

void ToolTipViewSingleton::_animationMoveView(const QRect& targetRect)
{
	DW_ASSERT(m_view);

	if(!m_view)
	{
		return;
	}

	if ( m_view->toolTipStyle().showStyle() == DuiToolTipStyle2::AnimationShow )
	{
		QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup( m_view );
		QPropertyAnimation *moveAnimation = new QPropertyAnimation(animationGroup);

		moveAnimation->setDuration(100);
		moveAnimation->setTargetObject(m_view);
		moveAnimation->setPropertyName("geometry");
		moveAnimation->setStartValue( m_view->geometry() );
		moveAnimation->setEndValue(targetRect);

		animationGroup->start( QAbstractAnimation::DeleteWhenStopped );
	}
	else if ( m_view->toolTipStyle().showStyle() == DuiToolTipStyle2::NormalShow )
	{
		m_view->setGeometry(targetRect);
	}
}

bool ToolTipViewSingleton::isVisible()
{
	if ( m_view )
	{
		return m_view->isVisible();
	}

	return false;
}

bool ToolTipViewSingleton::isUnderMouse(void)
{
	if (NULL == m_view)
	{
		return false;
	}
	QRect rc ;
	rc = m_view->frameGeometry();
	QPoint pt ;
	pt = QCursor::pos();
	return rc.contains(pt.x(),pt.y());
}

void ToolTipViewSingleton::_animationShowView()
{
	DW_ASSERT(m_view);
	if(!m_view)
	{
		return;
	}

	if ( m_view->toolTipStyle().showStyle() == DuiToolTipStyle2::AnimationShow )
	{
		m_view->setWindowOpacity(0.0);
		m_view->show();

		QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup( m_view );
		QPropertyAnimation *moveAnimation = new QPropertyAnimation(animationGroup);

		moveAnimation->setDuration(300);
		moveAnimation->setTargetObject(m_view);
		moveAnimation->setPropertyName("windowOpacity");
		moveAnimation->setStartValue( m_view->windowOpacity() );
		moveAnimation->setEndValue(1.0);

		animationGroup->start( QAbstractAnimation::DeleteWhenStopped );
	}
	else if ( m_view->toolTipStyle().showStyle() == DuiToolTipStyle2::NormalShow )
	{
		m_view->show();
	}
}

void ToolTipViewSingleton::setViewId( const QString& viewId )
{
	m_viewId = viewId;
}

QString ToolTipViewSingleton::viewId() const
{
	return m_viewId;
}
