#include "stable.h"

#include "duilineargradientframestyle.h"
#include "duiframeitem.h"
#include "duitheme.h"
#include "duidrawutil.h"
#include "dwbase/perflog.h"
#include "duiframeitem_p.h"
#include "duifw/duitextutility.h"

namespace
{
	static QBrush loadBrush(const QString &path)
	{
		if (path.isEmpty())
		{
			return QBrush();
		}

		DuiLinearGradientObject obj;
		obj.load(path);
		return obj.getBrush();
	}

	static bool imageIsVaild(const QPixmap &pixmap, int cnt, int idx)
	{
		if (idx < 0 || idx >= cnt)
			return false;

		const QRect sourceRect = DuiDrawUtil::calcPixmapSourceRect(pixmap, idx);
		return sourceRect.width() > 0 && sourceRect.height() > 0;
	}
}

//------------------------------------------------------------------------------
DuiFrameItemPrivate::DuiFrameItemPrivate()
: m_type(DuiFrameItemDrawer::OnePiece)
, m_drawStyle(DuiFrameItem::PixmapDrawStyle)
, m_hovered(false)
, m_pressed(false)
, m_enabled(true)
, m_checked(false)
, m_animationOff(false) // For DuiAnimationFrameItem
, m_keepNormal(true) // For DuiAnimationFrameItem
, m_baseColor(Qt::transparent)
, m_composeMode(QPainter::CompositionMode_SourceOver)
{

}

DuiFrameItemPrivate::~DuiFrameItemPrivate()
{
    ;
}

void DuiFrameItemPrivate::init()
{
	Q_Q(DuiFrameItem);

	q->setFocusPolicy(Qt::NoFocus);
	q->setThemeColorType(DuiTheme::BaseColor);
	q->setColorizePolicy(DuiWidget::ColorableFlag);

	updateGradientBrushes();
}

void DuiFrameItemPrivate::updateGradientBrushes()
{
	if (m_drawStyle != DuiFrameItem::LinearGradientDrawStyle)
		return;

	for (int i = 0; i < DuiFrameItem::StateMax; ++i)
	{
		m_brush[i] = loadBrush(m_image[i]);
	}
}

void DuiFrameItemPrivate::setupImageStates(const QString &image)
{
	QPixmap pixmap = DuiTheme::getImage(image);

	int cnt = 0;
	DuiDrawUtil::isNinePatchsPixmap(pixmap, &cnt);

	for (int i = 0; i < DuiFrameItem::StateMax; ++i)
	{
		m_image[i] = imageIsVaild(pixmap, cnt, i) ? image : QString();
	}
}

//------------------------------------------------------------------------------
/**
@brief 这里的状态返回，存在一个优先级问题，当存在多个状态时，按如下
优先级返回状态的Pixmap :
diabled > pressingDown > checked > hovered > normal

*/
const DuiFrameItem::FrameState DuiFrameItemPrivate::currentState(const QStyleOptionGraphicsItem *option) const
{
	if (!m_image[DuiFrameItem::StateDisable].isEmpty())
	{
		if (!m_enabled)
		{
			return DuiFrameItem::StateDisable;
		}
	}

	if (!m_image[DuiFrameItem::StatePressed].isEmpty() && m_enabled)
	{
		if (m_pressed)
		{
			return DuiFrameItem::StatePressed;
		}
	}

	// 兼容旧的4态用法 : 没有设置 check 状态的时候，服用 down 的资源,    
	// see void DuiFrameItem::setImage(
	if (!m_image[DuiFrameItem::StateCheck].isEmpty() || !m_image[DuiFrameItem::StatePressed].isEmpty())
	{
		if (m_checked && m_enabled)
		{
			if (!m_image[DuiFrameItem::StateCheck].isEmpty())
			{
				return DuiFrameItem::StateCheck;
			}
			else
			{
				return DuiFrameItem::StatePressed;
			}
		}
	}

	if (!m_image[DuiFrameItem::StateHover].isEmpty())
	{
		if (((option && option->state.testFlag(QStyle::State_MouseOver)) || m_hovered) && m_enabled)
		{
			return DuiFrameItem::StateHover;
		}
	}

	return DuiFrameItem::StateNormal;
}

void DuiFrameItemPrivate::drawMask(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state)
{
	if (m_baseColor.alpha() == 0 || m_drawStyle != DuiFrameItem::PixmapDrawStyle)
		return;

	Q_UNUSED(option);
	Q_UNUSED(widget);
	Q_UNUSED(state);
	Q_Q(DuiFrameItem);

	const QRect rc(q->rect().toAlignedRect());
	QPixmap pm(rc.size());

	{
		pm.fill(QColor(0, 0, 0, 0));
		QPainter p(&pm);
		p.fillRect(rc, m_baseColor);
		if (!m_backgoundMask.isEmpty())
		{
			p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
			DuiFrameItemDrawer drawer(m_backgoundMask, m_type);
			drawer.paint(&p, rc, m_frameMargin);
		}
	}

	{
		DuiFrameItemDrawer drawer(pm, m_type);
		drawer.paint(painter, rc, m_frameMargin);
	}
}

void DuiFrameItemPrivate::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state)
{
	const QPainter::CompositionMode compositionMode = painter->compositionMode();
	painter->setCompositionMode(m_composeMode);

	switch (m_drawStyle)
	{
	case DuiFrameItem::LinearGradientDrawStyle:
		drawBrush(painter, option, widget, state);
		break;
	default:
		drawPixmap(painter, option, widget, state);
		break;
	}

	painter->setCompositionMode(compositionMode);
}

void DuiFrameItemPrivate::drawPixmap(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	Q_Q(DuiFrameItem);

	const QString &imageDraw = m_image[state];

	DuiFrameItemDrawer drawer(imageDraw, m_type);
	drawer.paint(painter, q->rect().toAlignedRect(), m_frameMargin, state);
}

void DuiFrameItemPrivate::drawBrush(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	Q_Q(DuiFrameItem);

	const QBrush &brush = m_brush[state];

	QRectF fillRect = q->rect();
	if (!m_frameMargin.isNull())
	{
		fillRect.adjust(m_frameMargin.left(), m_frameMargin.top(), -m_frameMargin.right(), -m_frameMargin.bottom());
	}

	painter->fillRect(fillRect, brush);
}

//------------------------------------------------------------------------------
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiFrameItem)
DuiFrameItem::DuiFrameItem(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent)
: DuiWidget(*(new DuiFrameItemPrivate()),  parent )
{
	Q_D(DuiFrameItem);
	d->m_type = type;
    setImage(frameImageName);

	d->init();
}

DuiFrameItem::DuiFrameItem( QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(*(new DuiFrameItemPrivate()), parent) 
{
	Q_D(DuiFrameItem);
	d->init();
}

DuiFrameItem::DuiFrameItem(DuiFrameItemPrivate &dd, QGraphicsItem *parent)
: DuiWidget(dd, parent)
{
	Q_D(DuiFrameItem);
	d->init();
}

DuiFrameItem::~DuiFrameItem()
{
	;
}

//------------------------------------------------------------------------------
/**
	@brief 绘制FrameItem 流程：
	1. 如果设置了基准色则用基准色填充矩形
		1.1 如果设置了蒙版则用蒙版混合基准色矩形
	2. 设置混合模式
	3. 绘制FrameItem的状态图
*/
void DuiFrameItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = NULL*/ )
{
	PerfLogF(F_DUI);

	Q_D(DuiFrameItem);
	const FrameState state = d->currentState(option);

	d->drawMask(painter, option, widget, state);
	d->drawBackground(painter, option, widget, state);
}

void DuiFrameItem::setMargin(const QMargins &margin)
{
	Q_D(DuiFrameItem);
	d->m_frameMargin = margin;
}

void DuiFrameItem::setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown, const QString &imageDisable/* = ""*/, const QString &imageCheck/* = ""*/)
{
	Q_D(DuiFrameItem);

	d->m_image[StateNormal] = imageNormal;
	d->m_image[StateHover] = imageHover;
	d->m_image[StatePressed] = imageDown;
	d->m_image[StateDisable] = imageDisable;
    // 兼容旧的4态用法
    d->m_image[StateCheck] = imageCheck.isEmpty() ? imageDown : imageCheck;

	bool isLinearGradientDrawStyle = false;
	isLinearGradientDrawStyle = isLinearGradientDrawStyle | d->m_image[StateNormal].endsWith(".xml", Qt::CaseInsensitive);
	isLinearGradientDrawStyle = isLinearGradientDrawStyle | d->m_image[StateHover].endsWith(".xml", Qt::CaseInsensitive);
	isLinearGradientDrawStyle = isLinearGradientDrawStyle | d->m_image[StatePressed].endsWith(".xml", Qt::CaseInsensitive);
	isLinearGradientDrawStyle = isLinearGradientDrawStyle | d->m_image[StateDisable].endsWith(".xml", Qt::CaseInsensitive);
    isLinearGradientDrawStyle = isLinearGradientDrawStyle | d->m_image[StateCheck].endsWith(".xml", Qt::CaseInsensitive);

	if (isLinearGradientDrawStyle)
	{
		setDrawStyle(LinearGradientDrawStyle);
	}

	d->updateGradientBrushes();

	update();
}

QString DuiFrameItem::imagePath() const
{
	Q_D(const DuiFrameItem);
	return d->m_image[StateNormal];
}

void DuiFrameItem::setImage(const QString &image )
{
	Q_D(DuiFrameItem);

    if (image.indexOf('|') == -1 && !image.isEmpty())
    {
        d->m_image[StateNormal] = image;

        bool isLinearGradientDrawStyle = d->m_image[StateNormal].endsWith(".xml", Qt::CaseInsensitive);
        if (isLinearGradientDrawStyle)
        {
            setDrawStyle(LinearGradientDrawStyle);
        }

		d->setupImageStates(image);
		d->updateGradientBrushes();
        update();
    }
    else
    {
        QStringList images = duitextutils::resolvingStates(image);
        DW_ASSERT(images.count() >= StateMax);

        setImage(images[StateNormal], 
            images[StateHover], 
            images[StatePressed], 
            images[StateDisable],
            images[StateCheck]);
    }
}


void DuiFrameItem::setPressed(bool pressed)
{
	Q_D(DuiFrameItem);
	d->m_pressed = pressed;
	update();
}

bool DuiFrameItem::isChecked()const
{
	Q_D(const DuiFrameItem);
    return d->m_checked;
}

void DuiFrameItem::setChecked(bool checked)
{
	Q_D(DuiFrameItem);
	d->m_checked = checked;
    update();
}


bool DuiFrameItem::isHovered()const
{
	Q_D(const DuiFrameItem);
	return d->m_hovered;
}

void DuiFrameItem::setHovered(bool hovered)
{
	Q_D(DuiFrameItem);
	d->m_hovered = hovered;
	update();
}

void DuiFrameItem::setEnable(bool enabled)
{
	Q_D(DuiFrameItem);
	d->m_enabled = enabled;

	ColorizeWidgetFlag colorizeFlag = enabled ? d->m_enabledColorizeFlag : UnColorableFlag;
	setColorizePolicy(colorizeFlag);

	colorize(DuiTheme::getColor((DuiTheme::ColorType)themeColorType()));

	update();
}

bool DuiFrameItem::enable() const
{
	return d_func()->m_enabled;
}

int DuiFrameItem::frameType() const
{
	Q_D(const DuiFrameItem);
	return  (int)d->m_type;
}

void DuiFrameItem::setFrameType( int f )
{
	Q_D(DuiFrameItem);
	d->m_type = (DuiFrameItemDrawer::FrameType)f;
}

QString	 DuiFrameItem::frameTypeStr() const
{
	return QString();
}

void DuiFrameItem::setFrameType( const QString& frameType )
{
	setFrameType(DuiFrameItemDrawer::string2Enum(frameType));
}

void DuiFrameItem::polishEvent()
{
	colorize(DuiTheme::getColor((DuiTheme::ColorType)themeColorType()));
}

void DuiFrameItem::setDrawStyle( DrawStyle style )
{
	Q_D(DuiFrameItem);
	if(d->m_drawStyle != style)
	{
		d->m_drawStyle = style;
		d->updateGradientBrushes();
	}
}

DuiFrameItem::DrawStyle DuiFrameItem::drawStyle() const
{
	Q_D(const DuiFrameItem);
	return d->m_drawStyle;
}

void DuiFrameItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_UNUSED(event);
	update();
}

void DuiFrameItem::doColorize( QRgb color )
{
	Q_D(DuiFrameItem);
	switch ( d->m_drawStyle )
	{
	case DuiFrameItem::LinearGradientDrawStyle:
		d->updateGradientBrushes();
		break;
	default:
		{
			if (d->m_baseColor.alpha() != 0)
			{
				setBaseColor(DuiTheme::getCurrentPerceptualColor());
			}
			else
			{
				DuiWidget::doColorize( color );
			}
		}	
	}
}

void DuiFrameItem::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	DuiWidget::resizeEvent(event);
	update();
}

bool DuiFrameItem::isPressed() const
{
	Q_D(const DuiFrameItem);
	return d->m_pressed;
}

void DuiFrameItem::setColorizePolicy( ColorizeWidgetFlag flag )
{
	Q_D(DuiFrameItem);
	if ( d->m_enabled )
	{
		d->m_enabledColorizeFlag = flag;
	}

	DuiWidget::setColorizePolicy(flag);
}

//------------------------------------------------------------------------------
/**
	@brief 设置Item的基准色(不透明的基准色),
	设置基准色的同时，会打破原有的QGraphicsEffect的着色方案
	DuiFrameItem 会绘制一个纯底色                                       
*/
void DuiFrameItem::setBaseColor(const QColor& color)
{
	Q_D(DuiFrameItem);
	d->m_baseColor = color;
}

//------------------------------------------------------------------------------
/**
*/
QColor DuiFrameItem::baseColor()const
{
	Q_D(const DuiFrameItem);
	return d->m_baseColor;
}

//------------------------------------------------------------------------------
/**
	@brief 设置混合模式
	默认的混合模式为：QPainter::CompositionMode_SourceOver
*/
void DuiFrameItem::setComposeMode(QPainter::CompositionMode composeMode)
{
	Q_D(DuiFrameItem);
	d->m_composeMode = composeMode;
}

//------------------------------------------------------------------------------
/**
*/
QPainter::CompositionMode DuiFrameItem::composeMode()const
{
	Q_D(const DuiFrameItem);
	return d->m_composeMode;
}

//------------------------------------------------------------------------------
/**
*/
void DuiFrameItem::setBackgroundMask(const QString& m)
{
	Q_D(DuiFrameItem);
	d->m_backgoundMask = m;
}

//------------------------------------------------------------------------------
/**
*/
QString DuiFrameItem::backgroundMask()const
{
	Q_D(const DuiFrameItem);
	return d->m_backgoundMask;
}
