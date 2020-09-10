#include "stable.h"
#include "duiframeitemex.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiFrameItemEx);
DuiFrameItemEx::DuiFrameItemEx(QGraphicsItem *parent/* = NULL*/)
:DuiWidget(parent)
,m_background(NULL)
,m_acceptClick(false)
,m_acceptHover(false)
,m_acceptDoubleClick(false)
,m_acceptRightClick(false)
{	
	m_background = new DuiFrameItem(this);
	m_background->setParent(this);	
	setColorizePolicy(DuiWidget::ColorableFlag);	// ±³¾°Ä¬ÈÏ»»·ô
}

DuiFrameItemEx::DuiFrameItemEx(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent)
: DuiWidget( parent )
,m_background(NULL)
,m_acceptClick(false)
,m_acceptHover(false)
{	
	m_background = new DuiFrameItem(this);
	m_background->setParent(this);
	m_background->setImage(frameImageName);
	m_background->setFrameType(type);	
	setColorizePolicy(DuiWidget::ColorableFlag);	// ±³¾°Ä¬ÈÏ»»·ô
}

void DuiFrameItemEx::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverEnterEvent(event);
	emit entered();
}
void DuiFrameItemEx::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverLeaveEvent(event);
	emit leaved();
}

void DuiFrameItemEx::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverMoveEvent(event);
}

void DuiFrameItemEx::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	Q_UNUSED(event);
    m_background->setPressed(true);
}

void DuiFrameItemEx::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    m_background->setPressed(false);
	if(m_acceptClick && rect().contains( event->pos() ))
	{
		if (event->button() == Qt::LeftButton)
		{
			emit clicked();
		}
		else if (event->button() == Qt::RightButton)
		{
			emit rightClicked();
		}
	}	
}

void DuiFrameItemEx::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if(m_acceptDoubleClick && rect().contains( event->pos() ))
	{
		emit doubleClicked();
	}
}

void DuiFrameItemEx::resizeEvent(QGraphicsSceneResizeEvent * event)
{
	m_background->setGeometry(0, 0, event->newSize().width(), event->newSize().height());
}

void DuiFrameItemEx::setImage(const QString &image)
{
	m_background->setImage(image);
}

void DuiFrameItemEx::setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown)
{
	m_background->setImage( imageNormal, imageHover, imageDown);
}

QString DuiFrameItemEx::imagePath() const
{
	return m_background->imagePath();
}

void DuiFrameItemEx::setAcceptClick(bool b)
{
	m_acceptClick = b;
}

bool DuiFrameItemEx::acceptClick() const
{
	return m_acceptClick;
}

bool DuiFrameItemEx::acceptHover() const
{
	return m_acceptHover;
}

void DuiFrameItemEx::setAcceptHover(bool b)
{
	m_acceptHover = b;
	setAcceptHoverEvents(m_acceptHover);
	m_background->setAcceptHoverEvents(m_acceptHover);
	setDragPolicy(m_acceptHover? NotDragWindow:DragWithParentFlag);	
}

void DuiFrameItemEx::setPressed(bool pressed)
{
	m_background->setPressed(pressed);
}

int DuiFrameItemEx::frameType() const
{
	return  m_background->frameType();
}

void DuiFrameItemEx::setFrameType( int frameType )
{
	m_background->setFrameType(frameType);
}

void DuiFrameItemEx::setFrameType( const QString& frameType )
{
	if (m_background)
	{
		m_background->setFrameType(frameType);
	}
}

QString	 DuiFrameItemEx::frameTypeStr() const
{
	DW_ASSERT_X(false, __FUNCTION__, "this method should not be called.");
	return "";
}

void DuiFrameItemEx::setColorizePolicy( ColorizeWidgetFlag flag )
{
	PERFLOG;
	if (m_background)
	{
		m_background->setColorizePolicy(flag);
	}
}

void DuiFrameItemEx::colorize(QRgb color)
{
	if (m_background)
	{
		m_background->colorize(color);
	}

	// ÓÀÔ¶²»»»×ÔÒÑ£¬·ñÔò×ÓÔªËØ¶¼»á»»
	return;
}

bool DuiFrameItemEx::acceptDoubleClick() const
{
	return m_acceptDoubleClick;
}

void DuiFrameItemEx::setAcceptDoubleClick(bool b)
{
	m_acceptDoubleClick = b;
}

void DuiFrameItemEx::setAcceptRightClick(bool b)
{
	m_acceptRightClick = b;
}

bool DuiFrameItemEx::acceptRightClick() const
{
	return m_acceptRightClick;
}

void DuiFrameItemEx::setThemeColorType( int colorType )
{
	if ( m_background == NULL )
	{
		return;
	}

	m_background->setThemeColorType( colorType );
}
