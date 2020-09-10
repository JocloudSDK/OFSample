#include "stable.h"

#include "duiframeitem.h"
#include "duitab.h"
#include "duitab_p.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiTab)
DuiTab::DuiTab( QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(*(new DuiTabPrivate()), parent)
{
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	setDragPolicy(NotDragWindow);
	setColorizeWidgetFlag(DuiWidget::ColorableFlag);
// 	setColorizeIconFlag(ColorizeSelected);
	setFocusPolicy(Qt::StrongFocus);
    connect(this, SIGNAL(selectChanged(int)), this, SLOT(on_selectChanged(int)));
	//setHandlesChildEvents(true);
}

int DuiTab::insertItem(int index, const QString &text, const QString &icoPath, const QString &toolTip)
{
	return d_func()->insertItem( index, text, icoPath, toolTip, QString());
}

int DuiTab::insertItem2( int index, const QString &text, const QString &icoPath, const QString &toolTip, const QString &tabImage )
{
	return d_func()->insertItem( index, text, icoPath, toolTip, tabImage );
}

void DuiTab::removeTab( int index )
{
	d_func()->removeTab(index);
}

void DuiTab::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	d_func()->relayout( event->newSize() );
}

void DuiTab::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	d_func()->mousePressEvent(event);
}

void DuiTab::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	d_func()->hoverMoveEvent(event);
}

void DuiTab::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	d_func()->hoverLeaveEvent(event);
}

void DuiTab::keyPressEvent(QKeyEvent *event)
{
	switch( event->key() )
	{	
	case Qt::Key_Left:
	case Qt::Key_Up:
		d_func()->itemClicked(false);
		break;
	case Qt::Key_Right:
	case Qt::Key_Down:
		d_func()->itemClicked(true);
		break;
	default:
		DuiWidget::keyPressEvent(event);
	}
}

int DuiTab::tabCount() const
{
	return d_func()->tabCount();
}

int DuiTab::currentIndex() const
{
	return d_func()->currentIndex();
}

void DuiTab::setCurrentIndex(int index)
{
	d_func()->setCurrentIndex(index);
	emit selectChanged(index);
}

QString DuiTab::tabImage() const
{
	return d_func()->tabImage();
}

void DuiTab::setTabImage( const QString &tabImage )
{
	d_func()->setTabImage(tabImage);
}

QString DuiTab::itemText( int index ) const
{
	return d_func()->itemText(index);
}

void DuiTab::setItemText( int index, const QString &text )
{
	d_func()->setItemText(index, text);
}

void DuiTab::setColorizeWidgetFlag( const DuiWidget::ColorizeWidgetFlag& flag )
{
	d_func()->setColorizeWidgetFlag( flag );
}

DuiWidget::ColorizeWidgetFlag DuiTab::colorizeWidgetFlag() const
{
	return d_func()->m_colorWidgetFlag;
}

void DuiTab::setColorizeIconFlag( const ColorizeIconFlag& flag )
{
    d_func()->m_colorIconFlag = flag;
}

DuiTab::ColorizeIconFlag DuiTab::colorizeIconFlag() const
{
    return d_func()->m_colorIconFlag;
}

void DuiTab::setData(const QList<QVariant> &dataList)
{
	for ( int i = 0; i < dataList.count(); i++ )
	{
		const QMap<QString, QVariant> &m = dataList.at(i).toMap();
		QString text = m.value( "text" ).toString();
		QString path = m.value( "imagePath" ).toString();
		QString toolTip = m.value( "toolTip" ).toString();
		insertItem(-1, text, path, toolTip);
	}
	setCurrentIndex(0);
}

QList<QVariant> DuiTab::data() const
{
	DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "this method should not be invoke.");
	return QList<QVariant>();
}

void DuiTab::setDrawStyle( DuiFrameItem::DrawStyle style )
{
	d_func()->m_drawStyle = style;
}

void DuiTab::setEnableToolTip( bool enable )
{	
	d_func()->setEnableToolTip( enable );
}

void DuiTab::doColorize( QRgb color )
{
	Q_UNUSED(color);
	d_func()->doColorize(color);
}

void DuiTab::on_item_clicked(DuiFrameItem *item)
{
	d_func()->itemClicked(item);
}

void DuiTab::on_selectChanged( int newIndex )
{
    Q_UNUSED(newIndex);
    if(d_func()->m_colorIconFlag == ColorizeSelected)
    {
        d_func()->doColorize(DuiTheme::getColor( ));
        update();
    }
}
