#include "duivectorbutton.h"
#include "duiwidget_p.h"

class DuiVectorButtonPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiVectorButton );

public:
	DuiVectorButtonPrivate()
		: DuiWidgetPrivate(),
		timerId(0),
		pressed(false),
		enabled(true),
		penColor(QColor::fromRgba(0)),
		backgroundColor(QColor::fromRgba(0)),
		foregroundColor(Qt::white),
		hoverBackgroundColor(QColor::fromRgba(0)),
		hoverForegroundColor(Qt::blue),
		pressedBackgroundColor(QColor::fromRgba(0)),
		pressedForegroundColor(Qt::cyan),
		disabledBackgroundColor(QColor::fromRgba(0)),
		disabledForegroundColor(Qt::darkGray)
	{
	}

	~DuiVectorButtonPrivate() 
	{
	}

private:
	int timerId;
	QPainterPath path;
	bool pressed;
	bool enabled;

	QColor penColor;
	QColor backgroundColor;
	QColor foregroundColor;
	QColor pressedBackgroundColor;
	QColor pressedForegroundColor;
	QColor hoverBackgroundColor;
	QColor hoverForegroundColor;
	QColor disabledBackgroundColor;
	QColor disabledForegroundColor;
};

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiVectorButton);

DuiVectorButton::DuiVectorButton( QGraphicsItem* parent )
			:	DuiWidget(*new DuiVectorButtonPrivate(), parent)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	setDragPolicy(DuiWidget::NotDragWindow);
}

void DuiVectorButton::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRectF &rect) {
	Q_D(DuiVectorButton);

	painter->save();
	painter->setPen(d->penColor);
	painter->setOpacity(0.6);
	if(!this->isEnabled()) {
		painter->setBrush(d->disabledBackgroundColor);
	} else if(d->pressed) {
		painter->setBrush(d->pressedBackgroundColor);
	} else if (option->state.testFlag(QStyle::State_MouseOver)){
		painter->setBrush(d->hoverBackgroundColor);
	} else {
		painter->setBrush(d->backgroundColor);
	}
	painter->drawRoundedRect(rect.adjusted(0,0,-1,-1), 5, 5);
	painter->restore();
}

void DuiVectorButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
	Q_UNUSED(widget);

	Q_D(DuiVectorButton);

	if( d->path.isEmpty())
		return;

	painter->setRenderHint(QPainter::Antialiasing);
	drawBackground(painter, option, this->rect());

	if(!this->isEnabled()) 
	{
		painter->setBrush(d->disabledForegroundColor);
	} 
	else if(d->pressed) 
	{
		painter->setBrush(d->pressedForegroundColor);
	}
	else if (option->state.testFlag(QStyle::State_MouseOver))
	{
		painter->setBrush(d->hoverForegroundColor);
	} 
	else 
	{
		painter->setBrush(d->foregroundColor);
	}
	painter->drawPath(d->path);
}

QPainterPath DuiVectorButton::painterPath() const
{
	Q_D(const DuiVectorButton);
	return d->path;
}

void DuiVectorButton::setPainterPath(const QPainterPath &path) 
{
	Q_D(DuiVectorButton);
	d->path = path;
}

void DuiVectorButton::setBackgroundColor( const QColor &color) 
{
	Q_D(DuiVectorButton);
	d->backgroundColor = color;
}

QColor DuiVectorButton::backgroundColor( void ) const 
{
	Q_D(const DuiVectorButton);
	return d->backgroundColor;
}

void DuiVectorButton::setForegroundColor( const QColor &color) 
{
	Q_D(DuiVectorButton);
	d->foregroundColor = color;
}

QColor DuiVectorButton::foregroundColor( void ) const 
{
	Q_D(const DuiVectorButton);
	return d->foregroundColor;
}

void DuiVectorButton::setHoverBackgroundColor( const QColor &color) 
{
	Q_D(DuiVectorButton);
	d->hoverBackgroundColor = color;
}

QColor DuiVectorButton::hoverBackgroundColor( void ) const 
{
	Q_D(const DuiVectorButton);
	return d->hoverBackgroundColor;
}

void DuiVectorButton::setHoverForegroundColor( const QColor &color) 
{
	Q_D(DuiVectorButton);
	d->hoverForegroundColor = color;
}

QColor DuiVectorButton::hoverForegroundColor( void ) const 
{
	Q_D(const DuiVectorButton);
	return d->hoverForegroundColor;
}

void DuiVectorButton::setPressedBackgroundColor( const QColor &color) 
{
	Q_D(DuiVectorButton);
	d->pressedBackgroundColor = color;
}

QColor DuiVectorButton::pressedBackgroundColor( void ) const {
	Q_D(const DuiVectorButton);
	return d->pressedBackgroundColor;
}

void DuiVectorButton::setPressedForegroundColor( const QColor &color) {
	Q_D(DuiVectorButton);
	d->pressedForegroundColor = color;
}

QColor DuiVectorButton::pressedForegroundColor( void ) const {
	Q_D(const DuiVectorButton);
	return d->pressedForegroundColor;
}

void DuiVectorButton::setDisabledBackgroundColor( const QColor &color) {
	Q_D(DuiVectorButton);
	d->disabledBackgroundColor = color;
}

QColor DuiVectorButton::disabledBackgroundColor( void ) const {
	Q_D(const DuiVectorButton);
	return d->disabledBackgroundColor;
}

void DuiVectorButton::setDisabledForegroundColor( const QColor &color) {
	Q_D(DuiVectorButton);
	d->disabledForegroundColor = color;
}

QColor DuiVectorButton::disabledForegroundColor( void ) const {
	Q_D(const DuiVectorButton);
	return d->disabledForegroundColor;
}

void DuiVectorButton::setPenColor( const QColor &color) {
	Q_D(DuiVectorButton);
	d->penColor= color;
}

QColor DuiVectorButton::penColor( void ) const {
	Q_D(const DuiVectorButton);
	return d->penColor;
}

void DuiVectorButton::mousePressEvent( QGraphicsSceneMouseEvent * event ) 
{
	Q_D(DuiVectorButton);

	event->accept();
	d->pressed = true;
	emit pressed(event->pos());
	update();
}

void DuiVectorButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) 
{
	Q_D(DuiVectorButton);
	Q_UNUSED(event);

	d->pressed = false;
	emit released(event->pos());

	if( rect().contains( event->pos() ) )
	{
		emit clicked();
	}

	update();
}
