#include "stable.h"

#include "duiwidget_p.h"
#include "duiimagewidget.h"
#include "duitheme.h"
#include "duiframeitem.h"
#include "duisliderwidget.h"
#include "utility/duicolorformat.h"
#include "duifw/duitooltip2.h"
#include "duifw/duitooltipstyle2.h"
#include "duifw/duidrawutil.h"

const int KThumbWidth = 20;
const int KThumbHeight = 20;

const int KSelectPartOffsetWidth	= 4;
const int KSelectPartOffsetHeight	= 6;
const char KThumbImageHorizontal[]	= ":/theme/duifw/slider_thumb.png";
const char KThumbImageVertical[]	= ":/theme/duifw/slider_thumb.png";

enum ThumbImage
{
	NormalImage = 0,
	HoverImage,
	PressImage,
	DisableImage
};

class DuiSliderWidgetPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiSliderWidget);
public:
	DuiSliderWidgetPrivate();
	~DuiSliderWidgetPrivate();

	QGraphicsLineItem   *m_progressBar;
	DuiFrameItem*		m_selectedPart;
	DuiFrameItem*		m_thumb;
	DuiFrameItem*		m_backgroundImage;
	QVector<QString>	m_thumbImage;
	QString				m_selectedPartImage;
	qreal				m_value;
	qreal				m_progressValue;
	bool				m_isDragging;
	QPointF				m_pointInThumb;
	bool				m_progressCombo;
	QSize				m_thumbSize;
	Qt::Orientation		m_orientation;
	QSize				m_selectPartOffset;
	QSize				m_backgroundItemSize;
	bool				m_enableTips;
	DuiTextToolTip*		m_toolTip;		
};

DuiSliderWidgetPrivate::DuiSliderWidgetPrivate()
: m_selectedPart(NULL)
, m_progressCombo(false)
, m_progressBar(NULL)
, m_thumb(NULL)
, m_value(0.0)
, m_backgroundImage(NULL)
, m_selectedPartImage(":/theme/duifw/slider_selected_part.png")
, m_isDragging(false)
, m_progressValue(0.0)
, m_orientation(Qt::Horizontal)
, m_selectPartOffset(KSelectPartOffsetWidth, KSelectPartOffsetHeight)
, m_backgroundItemSize(KThumbWidth, KThumbHeight)
, m_toolTip(NULL)
, m_enableTips(false)
{
	m_thumbImage.resize(4);
	m_thumbImage[NormalImage]= KThumbImageHorizontal;
	m_thumbImage[HoverImage] = m_thumbImage[NormalImage];
	m_thumbImage[PressImage] = m_thumbImage[NormalImage];
}

DuiSliderWidgetPrivate::~DuiSliderWidgetPrivate()
{
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSliderWidget)
DuiSliderWidget::DuiSliderWidget( QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(*new DuiSliderWidgetPrivate, parent)
{
	_init();
}

DuiSliderWidget::DuiSliderWidget( DuiWidgetPrivate &dd, QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(dd, parent)
{
	_init();
}

DuiSliderWidget::~DuiSliderWidget()
{

}

void DuiSliderWidget::_init()
{
	Q_D(DuiSliderWidget);
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	setDragPolicy(NotDragWindow);

	d->m_backgroundImage = new DuiFrameItem(":/theme/duifw/slider_background.png", DuiFrameItemDrawer::NinePieces, this);
	d->m_backgroundImage->setThemeColorType( DuiTheme::MoreDarkColor );
	d->m_backgroundImage->setColorizePolicy(DuiWidget::UnColorableFlag);

	d->m_selectedPart = new DuiFrameItem(d->m_selectedPartImage, DuiFrameItemDrawer::NinePieces, this);
	d->m_selectedPart->setThemeColorType(DuiTheme::DarkColor);
	d->m_selectedPart->setColorizePolicy(DuiWidget::UnColorableFlag);

	d->m_progressBar = new QGraphicsLineItem(this);

	QPen pen;
	pen.setColor(Qt::green);
	pen.setWidth(4);
	pen.setStyle(Qt::DotLine);
	d->m_progressBar->setPen(pen);

	d->m_thumb = new DuiFrameItem(d->m_thumbImage[NormalImage], DuiFrameItemDrawer::ThreePiecesVertical, this);
	d->m_thumb->setThemeColorType(DuiTheme::DarkColor);
	d->m_thumb->setColorizePolicy(DuiWidget::UnColorableFlag);

	_relayout();
}

void DuiSliderWidget::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	Q_D(DuiSliderWidget);
	QRectF thumbRect = d->m_thumb->geometry();
	if ( thumbRect.contains(event->pos()) )
	{
		d->m_thumb->setPressed(true);
		d->m_isDragging = true;
		d->m_pointInThumb = event->pos() - thumbRect.topLeft();
		emit thumbPressed();
	}
	else
	{
		qreal theValue = 0;
		if (d->m_orientation == Qt::Horizontal)
		{
			theValue = event->pos().x() / _totalValueH();
		}
		else
		{
			theValue = event->pos().y() / _totalValueV();
		}
		if(setValue(theValue))
		{
			emit positionChanged(d->m_value);
		}
	}

	_showToolTip();
}

void DuiSliderWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);
    Q_D(DuiSliderWidget);

    d->m_thumb->setPressed(false);
	d->m_isDragging = false;
	emit mouseRelease();

	_hideToolTip();
}

void DuiSliderWidget::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	Q_D(DuiSliderWidget);
	if (d->m_isDragging)
	{
		qreal theValue = 0;
		if (d->m_orientation == Qt::Horizontal)
		{
			theValue = event->pos().x() - d->m_pointInThumb.x();
			theValue = theValue / _totalValueH();
		}
		else
		{
			theValue = event->pos().y() - d->m_pointInThumb.y();
			theValue = theValue / _totalValueV();
		}

		if(setValue( theValue ))
		{
			emit positionChanged(d->m_value);
			_showToolTip();
		}
	}
}

void DuiSliderWidget::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event);
	Q_D(DuiSliderWidget);
    d->m_thumb->setHovered(true);
}

void DuiSliderWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event);
	Q_D(DuiSliderWidget);
    d->m_thumb->setHovered(false);
}

qreal DuiSliderWidget::value() const
{
	Q_D(const DuiSliderWidget);
	return d->m_value;
}

bool DuiSliderWidget::setValue(qreal value)
{
	Q_D(DuiSliderWidget);

	if (value < 0)
	{
		value = 0;
	}
	else if (value > 1.0)
	{
		value = 1.0;
	}

	if( qFuzzyCompare( d->m_value, value) == false )
	{
		d->m_value = value;
		
		_relayout();

		emit valueChanged(d->m_value);

		return true;
	}

	return false;
}

QString DuiSliderWidget::thumbImage() const
{
	DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "this method should not be called.");
	return QString();
}

void DuiSliderWidget::setThumbImage( const QString &thumbImage )
{
	Q_D(DuiSliderWidget);
	QStringList list = thumbImage.split(",");
	int listSize = list.size();
	int index = 0;

	while ( listSize > index && index < 4 )
	{
		d->m_thumbImage[index] = list[index];
		index++;
	}

	d->m_thumbSize = DuiDrawUtil::calcPixmapSize(QPixmap(list[0]), 0);

    QString standardList = thumbImage;
    standardList.replace(',', '|');
	d->m_thumb->setImage(standardList);
}

QString DuiSliderWidget::backImage() const
{
	DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "this method should not be called.");
	return QString();
}

void DuiSliderWidget::setBackImage( const QString &backImage )
{
	Q_D(DuiSliderWidget);
	d->m_backgroundImage->setImage(backImage);
}

QString DuiSliderWidget::selectImage() const
{
	DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "this method should not be called.");
	return QString();
}

void DuiSliderWidget::setSelectImage( const QString &selectImage )
{
	Q_D(DuiSliderWidget);
	d->m_selectedPartImage = selectImage;
	d->m_selectedPart->setImage( selectImage );
}

void DuiSliderWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);

	_relayout();
}

void DuiSliderWidget::_relayout()
{
	if(size().isEmpty())
	{
		return;
	}

	_resizeBackground();
	_resizeSelectedPart();
	_resizeProcessBar();
	_resizeThumb();
}

void DuiSliderWidget::_resizeBackground()
{
	Q_D(DuiSliderWidget);

	QRectF backgroundRect(0, 0, 0, 0);
	if (d->m_orientation == Qt::Horizontal)
	{
		if (size().height() > d->m_backgroundItemSize.height())
		{
			backgroundRect.setTop((size().height() - d->m_backgroundItemSize.height()) / 2);
		}
		backgroundRect.setWidth(size().width());
		backgroundRect.setHeight(d->m_backgroundItemSize.height());
	}
	else if (d->m_orientation == Qt::Vertical)
	{
		if (size().width() > d->m_backgroundItemSize.width())
		{
			backgroundRect.setLeft((size().width() - d->m_backgroundItemSize.width()) / 2);
		}
		backgroundRect.setWidth(d->m_backgroundItemSize.width());
		backgroundRect.setHeight(size().height());
	}

	d->m_backgroundImage->setGeometry(backgroundRect);
}

void DuiSliderWidget::_resizeSelectedPart()
{
	Q_D(DuiSliderWidget);

	QRectF selectedpartRect(0, 0, 0, 0);
	if (d->m_orientation == Qt::Horizontal)
	{
		qreal leftpart = d->m_value * _totalValueH();
		selectedpartRect.setTop((size().height() - (d->m_backgroundItemSize.height() - d->m_selectPartOffset.height())) / 2);
		selectedpartRect.setLeft(d->m_selectPartOffset.width()/2);
		selectedpartRect.setWidth(leftpart);
		selectedpartRect.setHeight(d->m_backgroundItemSize.height() - d->m_selectPartOffset.height());
	}
	else if (d->m_orientation == Qt::Vertical)
	{
		qreal leftpart = d->m_value * _totalValueV();
		selectedpartRect.setTop(d->m_selectPartOffset.height()/2);
		selectedpartRect.setLeft((size().width() - (d->m_backgroundItemSize.width() - d->m_selectPartOffset.width())) / 2);
		selectedpartRect.setWidth(d->m_backgroundItemSize.width() - d->m_selectPartOffset.width());
		selectedpartRect.setHeight(leftpart);
	}
	d->m_selectedPart->setGeometry(selectedpartRect);
}

void DuiSliderWidget::_resizeProcessBar()
{
	Q_D(DuiSliderWidget);

	if(d->m_progressCombo)
	{
		d->m_progressBar->show();
		d->m_selectedPart->hide();
	}
	else
	{
		d->m_progressBar->hide();
		d->m_selectedPart->show();
	}

	QLineF progressLine(0, 0, 0, 0);
	if (d->m_orientation == Qt::Horizontal)
	{
		//progress line
		qreal progressYPos = size().height() / 2 + 1;
		progressLine.setLine(4, progressYPos, (size().width() - 8) * d->m_progressValue, progressYPos);
	}
	else if (d->m_orientation == Qt::Vertical)
	{
		//progress line
		qreal progressXPos = size().width() / 2 + 1;
		progressLine.setLine(progressXPos, 4, progressXPos, (size().height() - 8) * d->m_progressValue);
	}

	d->m_progressBar->setLine(progressLine);
}

void DuiSliderWidget::_resizeThumb()
{
	Q_D(DuiSliderWidget);

	QRectF thumbRect(0, 0, 0, 0);
	
	if (d->m_orientation == Qt::Horizontal)
	{
		qreal selectPart = d->m_value * _totalValueH();
		int thumbLimit = Dui::clamp<qreal>(d->m_thumbSize.width() - 2, KThumbWidth, _totalValueH());

		if (selectPart > size().width() - thumbLimit)
		{
			selectPart = size().width() - thumbLimit;
		}	
		if ( d->m_thumbSize.isValid() )
		{
			qreal top = (size().height() - KThumbHeight) / 2 -  ( d->m_thumbSize.height() - KThumbHeight  ) / 2;
			thumbRect.setLeft(selectPart);
			thumbRect.setTop(top);
			thumbRect.setWidth(d->m_thumbSize.width());
			thumbRect.setHeight(d->m_thumbSize.height());
		}
		else
		{
			thumbRect.setLeft(selectPart);
			thumbRect.setTop(0);
			thumbRect.setWidth(KThumbWidth);
			thumbRect.setHeight(size().height());
		}
	}
	else if (d->m_orientation == Qt::Vertical)
	{
		//thumb rect
		qreal selectPart = d->m_value * _totalValueV();
		int thumbLimit = Dui::clamp<qreal>(d->m_thumbSize.height() - 2, d->m_thumbSize.height() - 2, _totalValueV());
		if (selectPart > size().height() - thumbLimit)
		{
			selectPart = size().height() - thumbLimit;
		}	
		if ( d->m_thumbSize.isValid() )
		{
			qreal left = (size().width() - KThumbWidth) / 2 -  ( d->m_thumbSize.width() - KThumbWidth  ) / 2;
			thumbRect.setLeft(left);
			thumbRect.setTop(selectPart);
			thumbRect.setWidth(d->m_thumbSize.width());
			thumbRect.setHeight(d->m_thumbSize.height());
		}
		else
		{
			thumbRect.setLeft(0);
			thumbRect.setTop(selectPart);
			thumbRect.setWidth(size().width());
			thumbRect.setHeight(KThumbWidth);
		}
	}

	d->m_thumb->setGeometry(thumbRect);
}

bool DuiSliderWidget::progressCombo() const
{
	return d_func()->m_progressCombo;
}

void DuiSliderWidget::setProgressCombo(bool combo)
{
	Q_D(DuiSliderWidget);

	d->m_progressCombo = combo;
}

qreal DuiSliderWidget::progressValue() const
{
	return d_func()->m_progressValue;
}

void DuiSliderWidget::setProgressValue(qreal value)
{
	Q_D(DuiSliderWidget);

	if(value > 1.0)
	{
		value = 1.0;
	}
	if(value < 0)
	{
		value = 0;
	}

	d->m_progressValue = value;

	_relayout();
}

Qt::Orientation DuiSliderWidget::sliderOrientation() const
{
	return d_func()->m_orientation;
}

void DuiSliderWidget::setSliderOrientation(Qt::Orientation value)
{
	Q_D(DuiSliderWidget);
	
	d->m_orientation = value;
	if (d->m_orientation == Qt::Horizontal)
	{
		d->m_thumb->setFrameType(DuiFrameItemDrawer::ThreePiecesVertical);
	}
	else
	{
		d->m_thumb->setFrameType(DuiFrameItemDrawer::ThreePiecesHorizontal);

		//默认使用水平滑动条的滑动块，如果改变了方向而不换图片，会导致绘制出问题
		if (d->m_thumbImage[NormalImage] == KThumbImageHorizontal)
		{
			d->m_thumbImage[NormalImage] = KThumbImageVertical;
			d->m_thumb->setImage(KThumbImageVertical);
		}
		if (d->m_thumbImage[HoverImage] == KThumbImageHorizontal)
		{
			d->m_thumbImage[HoverImage]= KThumbImageVertical;
		}
	}
	_relayout();
}
void DuiSliderWidget::setColorizePolicy( ColorizeWidgetFlag flag )
{
	Q_D( DuiSliderWidget );
	d->m_selectedPart->setColorizePolicy( flag );
	d->m_thumb->setColorizePolicy( flag );
	d->m_backgroundImage->setColorizePolicy( flag );
}

void DuiSliderWidget::setEnabled( bool enabled )
{
	DuiWidget::setEnabled( enabled );
	doColorize( Qt::gray );

	if ( !enabled )
	{
		_hideToolTip();
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	}
	else
	{
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges,false);
	}
}

QSize DuiSliderWidget::selectPartOffset()const
{
	Q_D( const DuiSliderWidget );
	return d->m_selectPartOffset;
}

void DuiSliderWidget::setSelectPartOffset(const QSize& offset)
{
	Q_D( DuiSliderWidget );
	d->m_selectPartOffset = offset;
}

QSize DuiSliderWidget::backgroundItemSize()const
{
	Q_D( const DuiSliderWidget );
	return d->m_backgroundItemSize;
}

void DuiSliderWidget::setBackgroundItemSize(const QSize& size)
{
	Q_D( DuiSliderWidget );
	d->m_backgroundItemSize = size;
}

void DuiSliderWidget::doColorize( QRgb color )
{
	Q_UNUSED(color);
	if ( !isEnabled() )
	{
		setDisableStatus();
	}
	else
	{
		setGraphicsEffect( NULL );
	}
}

int DuiSliderWidget::thumbColorType() const
{
	return d_func()->m_thumb->themeColorType();
}

void DuiSliderWidget::setThumbColorType( int thumbColorType )
{
	Q_D( DuiSliderWidget );
	d->m_thumb->setThemeColorType( thumbColorType );
}

int DuiSliderWidget::backColorType() const
{
	return d_func()->m_backgroundImage->themeColorType();
}

void DuiSliderWidget::setBackColorType( int backColorType )
{
	Q_D( DuiSliderWidget );
	d->m_backgroundImage->setThemeColorType( backColorType );
}

int DuiSliderWidget::selectColorType() const
{
	return d_func()->m_selectedPart->themeColorType();
}

void DuiSliderWidget::setSelectColorType( int selectColorType )
{
	Q_D( DuiSliderWidget );
	d->m_selectedPart->setThemeColorType( selectColorType );
}

void DuiSliderWidget::_showToolTip()
{
	Q_D(DuiSliderWidget);
	
	if ( !d->m_enableTips )
	{
		return;
	}

	if ( d->m_orientation == Qt::Horizontal)
	{
		QPoint tipPos = d->m_thumb->mapToGlobal(QPoint(d->m_thumb->width() / 2, 0));
		d->m_toolTip->forceShowToolTip( QString::number( (int)(d->m_value * 100) ), tipPos);
	}
}

void DuiSliderWidget::_hideToolTip()
{
	Q_D(DuiSliderWidget);

	if ( !d->m_enableTips )
	{
		return;
	}

	if ( d->m_toolTip->isVisible() && 
		 d->m_orientation == Qt::Horizontal )
	{
		d->m_toolTip->forceHideTooltip();
	}
}

void DuiSliderWidget::_initTooltip()
{
	Q_D(DuiSliderWidget);

	DuiToolTipStyle2 style;
	style.setShowStyle(DuiToolTipStyle2::NormalShow);
	style.setArrowType(Qt::DownArrow);
	style.setTriangleHeight(6);
	style.setReferenceSpacing(-5);
	style.setTriangleOffsetPercent(50);
	style.setReferenceOffsetPercent(-50);

	d->m_toolTip = new DuiTextToolTip(this);
	d->m_toolTip->setStyle( style );
}

void DuiSliderWidget::setEnableTips( bool isEnable )
{
	Q_D(DuiSliderWidget);
	d->m_enableTips = isEnable;

	if ( d->m_enableTips && d->m_toolTip == NULL )
	{
		_initTooltip();
	}
}

bool DuiSliderWidget::enableTips() const
{
	Q_D(const DuiSliderWidget);
	return d->m_enableTips;
}

QVariant DuiSliderWidget::itemChange( GraphicsItemChange change, const QVariant &value )
{
	if ( change == QGraphicsItem::ItemPositionHasChanged )
	{
		QPointF pos = value.toPointF();
		setGeometry( pos.x(), pos.y(), width(), height() );
	}

	return DuiWidget::itemChange(change, value);
}

qreal DuiSliderWidget::_totalValueH()
{
	//宽度： |——————|—
	//滑块：  \ 有效区域 /↑↑
	return size().width() - d_func()->m_thumbSize.width();
}

qreal DuiSliderWidget::_totalValueV()
{
	return size().height() - d_func()->m_thumbSize.height();
}
