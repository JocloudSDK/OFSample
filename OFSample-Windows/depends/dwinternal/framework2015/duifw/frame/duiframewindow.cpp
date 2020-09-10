#include "stable.h"

#include "duitheme.h"
#include "duiframestyles.h"
#include "duiframewindow.h"
#include "duirootitem.h"
#include "duiwidget.h"
#include "duiframeitem.h"
#include "duiimagewidget.h"
#include "duiframewindow_p.h"
#include "duiapplication.h"
#include "duipushbutton.h"
#include "duifwstyle.h"
#include "utility/averagecolor.h"
#include "platform/windows/duiwinapi.h"
#include "duidrawutil.h"
#include "duilabelwidget.h"
#include "duicheckbox.h"

#include "winevenfilter/dwwinevenfilter_i.h"
#include "dwbase/perflog.h"
#include "dwbase/log.h"

//------------------------------------------------------------------------------
const int KGradientOffset = 30;
const qreal KFadeInOutZoomInDelta = 0.01;
const qreal KFadeInOutZoomOutDelta = -0.01;
//------------------------------------------------------------------------------
DuiBackgroundItem::DuiBackgroundItem()
: m_opacity(1.0)
, m_needDecorateImage(false)
{
	m_shadowImage.load(":/theme/duifw/shadow.png");
	setGradientType(DuiTheme::PopupGradient);
}

static const char* KLinearGradientImage[] =
{
	":/theme/duifw/mask_mainframe.png",
	":/theme/duifw/mask_thicktitle.png",
	":/theme/duifw/mask_dialog.png",
	":/theme/duifw/mask_popup.png",
	":/theme/duifw/mask_settingframe.png",
	":/theme/duifw/mask_channel.png",
	"", //imgroupmask
	":/theme/duifw/mask_mainframe_wonder.png" //YY大面板
};

void DuiBackgroundItem::setGradientType(DuiTheme::LinearGradientType type)
{
	enum { ImageCount = sizeof(KLinearGradientImage) / sizeof(KLinearGradientImage[0]) };
	DW_ASSERT(type < ImageCount || type == DuiTheme::NoGradient);
	if(type < ImageCount && type >= 0)
	{
		m_gradientType = type;

		if (QString(KLinearGradientImage[m_gradientType]).isEmpty())
		{
			m_decorateGradient = QPixmap();
		}
		else
		{
			m_decorateGradient.load(KLinearGradientImage[m_gradientType]);		
		}
	}
	else
	{
		m_decorateGradient = QPixmap();	//make it null
	}
}

DuiTheme::LinearGradientType DuiBackgroundItem::gradientType()
{
	return m_gradientType;
}

QString DuiBackgroundItem::imagePath() const
{
	return m_imagePath;
}

void DuiBackgroundItem::setImagePath(const QString &imagePath)
{
	m_imagePath = imagePath;
	m_image =  DuiTheme::getImage(imagePath);
}

void DuiBackgroundItem::setDecorationImage(const QImage &image)
{
	m_decorationColor = calculatePerceptualBodyColor(image);
	QImage decorationImage = image;
	if(!decorationImage.hasAlphaChannel() || decorationImage.depth() != 32)
	{
		decorationImage = decorationImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	}

	//QPixmap decorationMask(":/theme/duifw/decorate_fadeout.png");
	QPixmap decorationMask(":/theme/duifw/largearea_decorate_fadeout.png");
	QPainter p(&decorationImage);
	p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
	DuiDrawUtil::drawPixmap(&p, decorationImage.rect(), decorationMask);

	m_decorationImage = QPixmap::fromImage(decorationImage);
}

void DuiBackgroundItem::resetDecorationImage()
{
    m_decorationColor = QColor();
    m_decorationImage = QPixmap();
}

const QColor& DuiBackgroundItem::decorationColor() const
{
    return m_decorationColor;
}

qreal DuiBackgroundItem::opacity() const
{
	return m_opacity;
}

void DuiBackgroundItem::setOpacity(qreal opacity)
{
	m_opacity = opacity;
}

bool DuiBackgroundItem::needDecorationImage()
{
	return m_needDecorateImage;
}

void DuiBackgroundItem::setNeedDecorationImage(bool useDecorationImage)
{
	m_needDecorateImage = useDecorationImage;
}

void DuiBackgroundItem::drawBackground(QPainter * painter, const QRect &rect)
{
	PerfLogF(F_DUI);

	static const QMargins defMargins(0, 0, 0, 0);
	QRect windowRect = rect;

	//draw
	if(m_imagePath.isEmpty())
	{
		static QRect lastWindowRect;
		static QRegion curWindowRgn;

		if (lastWindowRect != windowRect) 
		{
			lastWindowRect = windowRect;
			curWindowRgn = DuiTheme::calcFrameClipRegion(windowRect);
		}

		painter->setOpacity( opacity() );
		painter->setClipRegion(curWindowRgn);

 		QPixmap decorate;
		//fill with decoration color
		QColor c;
		if(m_decorationColor.isValid())
		{
			c = m_decorationColor;
			decorate = m_decorationImage;
		}
		else
		{
			c = DuiTheme::decorationBackgroundColor();

			if (gradientType() == DuiTheme::ImGroupGradient)
			{
				decorate = DuiTheme::largeAreaDecorationImage();
			}
			else if (gradientType() == DuiTheme::ChannelGradient
				&& DuiTheme::themeStyle() == DuiTheme::LightTheme
				)
			{
				decorate = DuiTheme::getImage(":/theme/duifw/channel_default_theme.png");
			}
			else
			{
				decorate = DuiTheme::decorationImage();
			}
		}

		if (gradientType() == DuiTheme::ImGroupGradient)
		{
			// 1->draw basecolor
			// 2->draw 5% opacity
			painter->fillRect(windowRect, c);
			painter->fillRect(windowRect, QColor(255,255,255, 240));
		}else if ( (gradientType() == DuiTheme::PopupGradient || gradientType() == DuiTheme::SettingframeGradient)
			&& DuiTheme::themeStyle() == DuiTheme::LightTheme
			)
		{
			c = QColor(98,201,244);
			painter->fillRect(windowRect, c);
		}
		else if (gradientType() == DuiTheme::ChannelGradient 
			&& DuiTheme::themeStyle() == DuiTheme::LightTheme
			&& !m_decorationColor.isValid()
			)
		{
			c = QColor(98,201,244);
			painter->fillRect(windowRect, c);
		}
		else
		{
			painter->fillRect(windowRect, c);
		}

		//draw image
		if (m_needDecorateImage)
		{
			if(DuiTheme::TiledFill == DuiTheme::decorationFillMode())
			{
				qDrawBorderPixmap(painter, windowRect, 
					defMargins, decorate, decorate.rect(), 
					defMargins, QTileRules(Qt::RepeatTile));
			}
			else
			{
				//temp caizhiru
				QPoint pos;
				if (windowRect.width() > decorate.rect().width())
				{
					pos = windowRect.topRight();
					pos.rx() -= decorate.rect().width();
					painter->drawPixmap(pos, decorate);
				}
				else
				{
					QRect rc = decorate.rect();
					rc.moveLeft(decorate.rect().width()-windowRect.width());
					painter->drawPixmap(windowRect.topLeft(), decorate, rc);
				}				
			}
		}

		painter->setOpacity(1.0);

		//draw decorate mask
		if(!m_decorateGradient.isNull())
		{
			
            if(DuiDrawUtil::isNinePatchsPixmap(m_decorateGradient))
            {
                DuiDrawUtil::drawPixmap(painter, windowRect, m_decorateGradient);
            }
            else
            {
                DuiTheme::drawNinePiecesBoarder(painter, windowRect, m_decorateGradient);
            }
			
		}

		//draw border and shadow
        if (DuiDrawUtil::isNinePatchsPixmap(m_shadowImage))
        {
            DuiDrawUtil::drawPixmap(painter, windowRect, m_shadowImage);
        }
        else
        {
            DuiTheme::drawNinePiecesBoarder(painter, windowRect, m_shadowImage);
        }
	}
	else
	{
		DuiFrameItemDrawer drawer(m_image, DuiFrameItemDrawer::NinePieces);
		drawer.paint(painter, windowRect, defMargins);
	}
}


void DuiBackgroundItem::setDecorationImage(const QImage &image, const QString &maskPath)
{
    m_decorationColor = calculatePerceptualBodyColor(image);
    QImage decorationImage = image;
    if(!decorationImage.hasAlphaChannel() || decorationImage.depth() != 32)
    {
        decorationImage = decorationImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    //QPixmap decorationMask(":/theme/duifw/decorate_fadeout.png");
    QPixmap decorationMask(maskPath);
    QPainter p(&decorationImage);
    p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    DuiDrawUtil::drawPixmap(&p, decorationImage.rect(), decorationMask);

    m_decorationImage = QPixmap::fromImage(decorationImage);
}

//------------------------------------------------------------------------------
DuiFrameWindowPrivate::DuiFrameWindowPrivate()
: q_ptr(NULL)
, m_frameStyle(NULL)
, m_scene(NULL)
, m_rootItem(NULL)
, m_titlebar(NULL)
, m_backgroundItem(NULL)
, m_contentWidget(NULL)
, m_frameStyleFlags(DuiFrameWindow::Null)
, m_animation(NULL)
, m_hWindowBrush(NULL)
, m_closeOnEscape(false)
, m_animateInOut(false)
, m_windowPlacement(DuiFrameWindow::DefaultPlacement)
, m_ignoreComposition(false)
, m_isVisible(false)
{
}

DuiFrameWindowPrivate::~DuiFrameWindowPrivate()
{
	DW_SAFE_DELETE(m_frameStyle);
    DW_SAFE_DELETE(m_backgroundItem);

	if(m_hWindowBrush)
	{
		::DeleteObject(m_hWindowBrush);
		m_hWindowBrush = NULL;
	}
}

void DuiFrameWindowPrivate::init()
{
	if (m_frameStyle)
	{
		DW_ASSERT_X(false, __FUNCTION__, "error: construct more than once!");
		return ;
	}

	Q_Q( DuiFrameWindow );

	if(m_frameStyleFlags.testFlag(DuiFrameWindow::Child))
	{
		m_frameStyle = new DuiFrameStyle_Child( q );
	}
	else if(m_frameStyleFlags.testFlag(DuiFrameWindow::FrameLess))
	{
		if(DuiWinAPI::IsCompositionEnabled())
		{
			m_frameStyle = new DuiFrameStyle_Vista( q );
		}
		else
		{
			m_frameStyle = new DuiFrameStyle_WinXP( q );
		}
	}
	else if(m_frameStyleFlags.testFlag(DuiFrameWindow::ToolTip))
	{
		m_frameStyle = new DuiFrameStyle_Tooltip( q );
	}
	else
	{
		m_frameStyle = new DuiFrameStyle_System( q );
	}

	if( m_frameStyleFlags.testFlag(DuiFrameWindow::Popup))
	{
		m_frameStyle->setPopup(true);
	}
	if( m_frameStyleFlags.testFlag(DuiFrameWindow::Resizable) )
	{
		m_frameStyle->setResizable( true );
	}
	if( m_frameStyleFlags.testFlag(DuiFrameWindow::ToolWindow) )
	{
		m_frameStyle->setToolWindow( true );
	}
	if( m_frameStyleFlags.testFlag(DuiFrameWindow::NoRoundCorner) )
	{
		m_frameStyle->setRoundCorner( false );
	}

	m_frameStyle->init();

	bool hasTitlebar =false ,hasMaxButton=false,hasMinButton=false;
	hasTitlebar = m_frameStyleFlags.testFlag(DuiFrameWindow::CloseButton);
	if (!hasTitlebar)
	{
		m_titlebar->titleButton(DuiTitlebar::Button_Close)->hide();
	}

	hasMaxButton = m_frameStyleFlags.testFlag(DuiFrameWindow::MaxButton);
	m_titlebar->setHasMaximizeButton(hasMaxButton);

	hasMinButton = m_frameStyleFlags.testFlag(DuiFrameWindow::MinButton);
	m_titlebar->setHasMinimizeButton(hasMinButton);

	if( !m_frameStyleFlags.testFlag(DuiFrameWindow::ToolWindow) )
	{
		UINT32 winStyle = 0;
		if( hasMinButton )
		{
			winStyle |= WS_MINIMIZEBOX;
		}
		if( hasMaxButton )
		{
			winStyle |= WS_MAXIMIZEBOX;
		}
		if(winStyle)
		{
			DuiWinAPI::addWindowStyle((UINT32)q->winId(), WS_SYSMENU | winStyle );
		}
	}

	q->setAttribute(Qt::WA_DeleteOnClose, true);
	if(m_frameStyleFlags.testFlag(DuiFrameWindow::AppWindow))
	{
		q->setAttribute(Qt::WA_QuitOnClose, true);
	}
	else
	{
		q->setAttribute(Qt::WA_QuitOnClose, false);
	}

	m_backgroundItem->setOpacity(m_frameStyle->opacity());

	_initWindowBrush();
}

void DuiFrameWindowPrivate::showActive()
{
	Q_Q(DuiFrameWindow);

	if(DuiWinAPI::IsMinimized(q->winId()))
	{
		q->systemButtonClicked(DuiTitlebar::Restore);
		return;
	}

	QRect newGeometry = adjustedGeometry(q->realFrameGeometry());

	q->QGraphicsView::show();

    HWND hWnd = NULL;
    if (KDuiIsEditorMode)
    {
        hWnd = ::GetDesktopWindow();
    }

	DuiWinAPI::setWindowPos(q->winId(), 
		hWnd, 
		newGeometry.left(), newGeometry.top(), newGeometry.width(), newGeometry.height(),
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	if (q->isAnimateInOut() && m_frameStyle->isAnimationEnabled())
	{
		q->showAnimation();
	}
}

void DuiFrameWindowPrivate::emitVisibleChanged(bool visible)
{
	Q_Q(DuiFrameWindow);
	if(m_isVisible != visible)
	{
		m_isVisible = visible;
		emit q->visibleChanged(m_isVisible);
	}
}

QRect DuiFrameWindowPrivate::adjustedGeometry(const QRect &oldGeometry)
{
	Q_Q(DuiFrameWindow);

	QRect newGeometry = oldGeometry;
	if(m_windowPlacement == DuiFrameWindow::ScreenCenter)
	{		
		QRect screenRect = QApplication::desktop()->screenGeometry(QCursor::pos());
		newGeometry.moveCenter( screenRect.center() );
	}
	else if(m_windowPlacement == DuiFrameWindow::ParentCenter)
	{
		QWidget *parentWidget = qobject_cast<QWidget*>(q->parent());
		if(parentWidget)
		{
			QRect parentRect = parentWidget->geometry();
			newGeometry.moveCenter( parentRect.center() );

			QRect screenRect = QApplication::desktop()->screenGeometry(QCursor::pos());
			if(!screenRect.contains(newGeometry))
			{
				newGeometry.moveCenter( screenRect.center() );
			}
		}
	}

	return newGeometry;
}

void DuiFrameWindowPrivate::_initWindowBrush()
{
	Q_Q(DuiFrameWindow);

	LOGBRUSH lBrush;
	lBrush.lbColor = duiFrameKeyColor();
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbHatch = 0;
	m_hWindowBrush = CreateBrushIndirect(&lBrush);
	SetClassLongPtr(q->winId(), GCL_HBRBACKGROUND, (LONG)m_hWindowBrush);
}

//------------------------------------------------------------------------------
DUI_IMPLEMENT_DYNAMIC_FRAME_CREATE_SELF(DuiFrameWindow)
DuiFrameWindow::DuiFrameWindow(QWidget* parent, FrameStyles frameStyles)
: QGraphicsView(parent)
, d_ptr(new DuiFrameWindowPrivate())
{
	Q_D(DuiFrameWindow);
	d->q_ptr = this;
	d->m_frameStyleFlags = frameStyles;
	_initScene();
}

DuiFrameWindow::DuiFrameWindow( DuiFrameWindowPrivate &dd, QWidget *parent, FrameStyles frameStyles /*= Window*/ )
: QGraphicsView(parent)
, d_ptr(&dd)
{
	Q_D(DuiFrameWindow);
	d->q_ptr = this;
	d->m_frameStyleFlags = frameStyles;
	_initScene();
}

DuiFrameWindow::~DuiFrameWindow()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiFrameWindow::doConstruct()
{
	PerfLogF(F_DUI);
	Q_D(DuiFrameWindow);

	d->init();
	construct();

	// colorize after construction, prevent missing some widgets.
	QRgb currentColor = DuiTheme::getColor();
	_colorizeWidgets(currentColor);

	d->m_titlebar->refreshButtonName();
}

void DuiFrameWindow::construct()
{
}

void DuiFrameWindow::resizeEvent(QResizeEvent *event)
{
	PerfLogF(F_DUI);
	Q_D(DuiFrameWindow);
	d->m_frameStyle->resizeEvent(event);
	
	QRect rect(QPoint(0, 0), event->size());
	setSceneRect(rect);

	d->m_rootItem->setGeometry(rect);

	QRect contentRect = d->m_frameStyle->calcContentRect(rect);
	d->m_contentWidget->setGeometry(d->m_titlebar->cutNcArea(contentRect));

    QRect rcTitle =d->m_titlebar->geometryHint(d->m_frameStyle->calcContentRect(rect));
	d->m_titlebar->setGeometry(rcTitle);
	
	emit resized();
}

bool DuiFrameWindow::winEvent(MSG *message, long *result)
{
	PerfLogF(F_DUI);
	Q_D(DuiFrameWindow);

	for (int i = 0; i < d->m_winEvenFilters.count(); ++i)
	{
		if (d->m_winEvenFilters[i].isNull())
		{
			continue;
		}

		if (d->m_winEvenFilters[i]->winEvent(message, result))
		{
			break;
		}
	}

	if( message->message == WM_SIZE && NULL != d->m_titlebar )
	{
		//to handle drag to maximize on windows7
		if( message->wParam == SIZE_MAXIMIZED )
		{
			d->m_titlebar->setMaximized( true );
		}
		else if( message->wParam == SIZE_RESTORED )
		{
			d->m_titlebar->setMaximized( false );
		}
	}

	bool retVal = false;
	if( d->m_frameStyle )
	{
		retVal = d->m_frameStyle->winEvent( message, result );
	}

	if(message->message == WM_SYSCOMMAND)
	{
		bool handled = true;
		switch(message->wParam)
		{
			case SC_CLOSE:
				this->systemButtonClicked(DuiTitlebar::Close);
				break;
			case SC_MAXIMIZE:
				this->systemButtonClicked(DuiTitlebar::Maximize);
				break;
			case SC_MINIMIZE:
				this->systemButtonClicked(DuiTitlebar::Minimize);
				break;
			case SC_RESTORE:
				this->systemButtonClicked(DuiTitlebar::Restore);
				break;
			default:
				handled = false;
		}
		return handled;
	}
	else if(message->message == WM_WINDOWPOSCHANGED)
	{		
		d->emitVisibleChanged(isVisible());
	}
	else if(message->message == WM_SHOWWINDOW)
	{
		d->emitVisibleChanged(message->wParam != 0);
	}

	return retVal;
}

void DuiFrameWindow::installWinEvenFilter(DWAbstractWinEvenFilter *filter)
{
	Q_D(DuiFrameWindow);
	d->m_winEvenFilters.removeAll((DWAbstractWinEvenFilter*)0);
	d->m_winEvenFilters.removeAll(filter);
	d->m_winEvenFilters.prepend(filter);
}

void DuiFrameWindow::removeWinEvenFilter(DWAbstractWinEvenFilter *filter)
{
	Q_D(DuiFrameWindow);
	for (int i = 0; i < d->m_winEvenFilters.count(); ++i)
	{
		if (d->m_winEvenFilters.at(i) == filter)
		{
			d->m_winEvenFilters[i] = 0;
		}
	}
}

void DuiFrameWindow::_initScene()
{
	PerfLogF(F_DUI);
	Q_D(DuiFrameWindow);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//avoid lots of background redrawin
	setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

	//init scene
	d->m_scene = new QGraphicsScene(this);
	{
#define L_setScene "setScene"
		PerfLog(1, L_setScene, F_DUI);
		setScene(d->m_scene);
	}

	d->m_rootItem = new DuiRootItem();
	d->m_rootItem->setParent(d->m_scene);
	d->m_scene->addItem(d->m_rootItem);

	//init background item
	d->m_backgroundItem = new DuiBackgroundItem();

	//init title bar
	d->m_titlebar = new DuiTitlebar(d->m_rootItem);
	d->m_titlebar->setWindowId(this);
	d->m_titlebar->setParent(d->m_rootItem);
	d->m_titlebar->setZValue(1);
	connect(d->m_titlebar, SIGNAL(buttonClicked(DuiTitlebar::titlebarClick)), SLOT(onTitleButtonClicked(DuiTitlebar::titlebarClick)));
	if( !d->m_frameStyleFlags.testFlag(DuiFrameWindow::Child) )
	{// set child
	}
	//init content item
	d->m_contentWidget = new DuiWidget(d->m_rootItem);
	d->m_contentWidget->setParent(d->m_rootItem);
	d->m_contentWidget->setDragPolicy( DuiWidget::DragWindow );
	d->m_contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


	d->m_contentWidget->setLayout(DuiWidget::VBox);

	//保证每个frame必定有名字
	static int s_index = 0;
	QString name = "NoNameFrame" + QString::number(s_index++);
	setObjectName(name);
}

void DuiFrameWindow::systemButtonClicked(DuiTitlebar::titlebarClick click)
{
	DuiSysCommandRequest ev(click);
	emit sysCommand(&ev);
	if(ev.accepted())
	{
		switch(click)
		{
			case DuiTitlebar::Minimize:
				{
					if( d_func()->m_frameStyleFlags.testFlag(DuiFrameWindow::ToolWindow) )
					{
						hide();
					}
					else
					{
						showMinimized();
					}
					break;
				}
			case DuiTitlebar::Maximize:
				showMaximized();
				break;

			case DuiTitlebar::Restore:
				DuiWinAPI::ShowWindow(winId(), SW_RESTORE);
				break;

			case DuiTitlebar::Close:
				close();
				break;

			default:
				break;
		}
	}
}

DuiWidget * DuiFrameWindow::rootWidget() const
{
	return d_func()->m_contentWidget;
}

void DuiFrameWindow::show()
{
	PERFLOG;

	Q_D(DuiFrameWindow);
	d->showActive();
}

void DuiFrameWindow::animatedMoveTo(const QRect& rc, int ms)
{
	Q_D(DuiFrameWindow);
	if(	!d->m_animation )
	{
		d->m_animation = new QPropertyAnimation(this);
	}
	d->m_animation->setPropertyName("realFrameGeometry");
	d->m_animation->setTargetObject(this);
	d->m_animation->setDuration(ms);
	d->m_animation->setStartValue(realFrameGeometry());
	d->m_animation->setEndValue(rc);

	d->m_animation->setEasingCurve(QEasingCurve::Linear);

	d->m_animation->start();
}

void DuiFrameWindow::setTitle(const I18nString &title)
{
	d_func()->m_titlebar->setTitle(title);
	setWindowTitle(title);
}

I18nString DuiFrameWindow::title() const
{
	return I18nString(d_func()->m_titlebar->title());
}

void DuiFrameWindow::setTitleIcon( const QString &icon )
{
	d_func()->m_titlebar->setIcon(icon);
}

QString DuiFrameWindow::titleIcon() const
{
	return d_func()->m_titlebar->icon()->imagePath();
}

void DuiFrameWindow::setFrameStyle(FrameStyles styles)
{
	Q_D(DuiFrameWindow);
	d->m_frameStyleFlags = styles;
}

DuiFrameWindow::FrameStyles DuiFrameWindow::frameStyle() const
{
	Q_D(const DuiFrameWindow);
	return d->m_frameStyleFlags;
}

DuiTitlebar * DuiFrameWindow::titlebar( void ) const
{
	return d_func()->m_titlebar;
}

QRect DuiFrameWindow::clientAreaRect() const
{
	return rect().adjusted(1, 1, -1, -1);
}

void DuiFrameWindow::setBackgroundImage(const QString &imageFileName)
{
	d_func()->m_backgroundItem->setImagePath(imageFileName);
}

QString DuiFrameWindow::backgroundImage() const
{
	return d_func()->m_backgroundItem->imagePath();
}

bool DuiFrameWindow::event( QEvent * event )
{
	if( event->type() == DuiColorizeEvent::EventType)
	{
		colorizeEvent( (DuiColorizeEvent*)event );
		return true;
	}
    else
    {
#if (defined DEBUG || defined _DEBUG)
        // monitor for long text
        if (event->type() == QEvent::Paint)
        {
            foreach (DuiWidget *w, qFindChildren<DuiWidget *>(this))
            {
                int width = w->width();
                if (!w->isVisible() || 0 == width) // invisible
                    continue;

                DuiLabelWidget *lbl = qobject_cast<DuiLabelWidget *>(w);
                DuiCheckBox *checkBox = qobject_cast<DuiCheckBox *>(w);
                if (!lbl && !checkBox)
                    continue;
                
                int preferedWidth = 0;
                QString txt, name, className;

                if (lbl)
                {
                    QFontMetrics fm(lbl->font());
                    txt = lbl->text();
                    preferedWidth = fm.width(txt);
                }
                if (checkBox)
                {
                    preferedWidth = checkBox->preferredWidth();
                }
                if (preferedWidth <= width)
                    continue;
                if (lbl)
                {
                    name = lbl->objectName();
                    className = lbl->metaObject()->className();
                    if (name.startsWith("NoNameWidget") &&
                        qobject_cast<DuiPushButton *>(lbl->parent()))
                    {
                        name = lbl->parent()->objectName();
                        className = lbl->parent()->metaObject()->className();
                    }
                }
                if (checkBox)
                {
                    className = checkBox->metaObject()->className();
                    name = checkBox->objectName();
                    txt = checkBox->text();
                }
                QString inheritChain;
                QObject *p= w->parent();
                while (p)
                {
                    inheritChain += " ";
                    inheritChain += p->objectName();
                    p = p->parent();
                }
                LogDev(LOG::KDuiFw)
                    << className.toStdString().c_str()
                    << " " << name.toStdString().c_str()
                    << " " << preferedWidth
                    << " " << width
                    // a special bound @_@
                    << " @_@" << txt.toStdString().c_str() << "@_@"
                    << inheritChain.toStdString().c_str();
            }
        }
#endif
		//  此处保证scene中焦点链是一个循环链
		if (scene() && event->type() == QEvent::KeyPress)
		{
			QKeyEvent *k = static_cast<QKeyEvent *>(event);
			if (k->key() == Qt::Key_Tab || k->key() == Qt::Key_Backtab) 
			{
				QGraphicsItem *oldFocusItem = scene()->focusItem();
				bool result = QGraphicsView::event(event);
				if (hasFocus() && oldFocusItem == scene()->focusItem())
				{
					clearFocus();
					setFocus(k->key() == Qt::Key_Tab ? Qt::TabFocusReason : Qt::BacktabFocusReason);
				}
				return result;
			}
		}
        return QGraphicsView::event(event);
	}
}

void DuiFrameWindow::colorizeEvent(DuiColorizeEvent *event)
{
	resetCachedContent();
	_colorizeWidgets(event->color());
}

void DuiFrameWindow::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	emit posChanged();
}

//virtual
void DuiFrameWindow::drawBackground( QPainter * painter, const QRectF & rect )
{
	Q_UNUSED(rect);
	Q_D(DuiFrameWindow);

	QRect r = d->m_frameStyle->calcContentRect(this->rect());
	d->m_backgroundItem->drawBackground(painter, r);
}

void DuiFrameWindow::_colorizeWidgets(QRgb color)
{
	foreach (QGraphicsItem* item, items())
	{
		DuiWidget* widget = qobject_cast<DuiWidget*>( item->toGraphicsObject() );
		if ( widget )
		{
			widget->colorize(color);
		}
	}

	QList<QWidget*> childWidgets = findChildren<QWidget*>();
	foreach (QWidget* widget, childWidgets)
	{
		DuiColorizeEvent colorEvent(color);
		qApp->sendEvent(widget, &colorEvent);
	}

	update();
}

void DuiFrameWindow::filterNcMessage( MSG *message )
{
	if(message->message == WM_NCLBUTTONDOWN)
	{
		emit captionAreaClicked();
	}
}

QRect DuiFrameWindow::realFrameGeometry() const
{
	RECT rcWin = {0};
	::GetWindowRect(winId(), &rcWin);

	return QRect(rcWin.left, rcWin.top, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top);
}

void DuiFrameWindow::setRealFrameGeometry(const QRect &rect)
{
	::MoveWindow(winId(), rect.left(), rect.top(), rect.width(), rect.height(), TRUE);
}

void DuiFrameWindow::closeEvent( QCloseEvent *event )
{
	emit aboutToClose();

	event->accept();
	QGraphicsView::closeEvent( event );
}

void DuiFrameWindow::setFixSize(const QSize &size)
{
	resize(size);
	setMinimumSize(size);
	setMaximumSize(size);
}

QSize DuiFrameWindow::fixSize() const
{
	return minimumSize();
}

QSize DuiFrameWindow::defaultSize() const
{
	return d_func()->m_defaultSize;
}

void DuiFrameWindow::setDefaultSize(const QSize &size)
{
	d_func()->m_defaultSize = size;
	resize(size);
}

void DuiFrameWindow::setTitlebarHeight(int height)
{
	d_func()->m_titlebar->setHeight(height);
}

int DuiFrameWindow::titlebarHeight() const
{
	return d_func()->m_titlebar->height();
}

QString DuiFrameWindow::windowTitle() const
{
	return QGraphicsView::windowTitle();
}

void DuiFrameWindow::setWindowTitle( const QString &text )
{
	return QGraphicsView::setWindowTitle(text);
}

void DuiFrameWindow::showMinimized()
{
	DuiWinAPI::ShowWindow(winId(), SW_SHOWMINIMIZED);
}

void DuiFrameWindow::showMaximized()
{
	DuiWinAPI::ShowWindow(winId(), SW_SHOWMAXIMIZED);
}

void DuiFrameWindow::hide()
{
	DuiWinAPI::ShowWindow(winId(), SW_HIDE);

	//don't call QGraphicsView::hide, or the windows won't restore to correct status when show again with maximized state.
//	QGraphicsView::hide();
}

bool DuiFrameWindow::isVisible() const
{
    WId wid = effectiveWinId();
    if(wid)
    {
        return IsWindowVisible(wid);
    }

    return false;
}

void DuiFrameWindow::setVisible2(bool b)
{
	b ? show() : hide();
}

bool DuiFrameWindow::isMinimized()
{
	return DuiWinAPI::IsMinimized(winId());
}

bool DuiFrameWindow::isMaximized()
{
	return DuiWinAPI::IsMaximized(winId());
}

QString DuiFrameWindow::styleId()const
{
	Q_D(const DuiFrameWindow);
	return d->m_styleId;
}

void DuiFrameWindow::setStyleId(const QString& styleId)
{
	Q_D(DuiFrameWindow);
	if (d->m_styleId != styleId)
	{
		d->m_styleId = styleId;
		duiGetStyleManager()->applyStyleInfo(this, d->m_styleId);
	}
}

quint32 DuiFrameWindow::hwnd() const
{
	return quint32(winId());
}

void DuiFrameWindow::onTitleButtonClicked( DuiTitlebar::titlebarClick click )
{
	systemButtonClicked(click);
}

QString DuiFrameWindow::decorationImage() const
{
	return QString();
}

void DuiFrameWindow::setDecorationImage( const QString &imageFileName )
{
	Q_D(DuiFrameWindow);

	QImage image(imageFileName);

	if(image.isNull())
	{
		LogFinal(LOG::KDuiFw) << "Fail to load image from file " << imageFileName;
		return;
	}
	d->m_backgroundItem->setDecorationImage(image);
	resetCachedContent();
}

void DuiFrameWindow::setDecorationImage(const QString &imageFileName, const QString &maskPath)
{
    Q_D(DuiFrameWindow);

    QImage image(imageFileName);

    if(image.isNull())
    {
        LogFinal(LOG::KDuiFw) << "Fail to load image from file " << imageFileName;
        return;
    }

    QImage mask(maskPath);
    if (mask.isNull())
    {
        LogFinal(LOG::KDuiFw) << "Fail to load image from file " << maskPath;
        return;
    }

    d->m_backgroundItem->setDecorationImage(image, maskPath);
    resetCachedContent();

}

void DuiFrameWindow::resetDecorationImage()
{
    Q_D(DuiFrameWindow);

    d->m_backgroundItem->resetDecorationImage();
    resetCachedContent();
}

const QColor& DuiFrameWindow::decorationColor() const
{
    return d_func()->m_backgroundItem->decorationColor();
}

void DuiFrameWindow::keyPressEvent( QKeyEvent *event )
{
	QGraphicsView::keyPressEvent(event);
	if (!event->isAccepted())
	{
		switch(event->key())
		{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			{
				//当为Key_Enter时，会有一个keypadmodifier标志，表示是小键盘的回车键
				Qt::KeyboardModifiers modifier = event->modifiers();
				if ( modifier.testFlag(Qt::KeypadModifier) )
				{
					modifier ^= Qt::KeypadModifier;
				}
				onEnterPressed(modifier);
				break;
			}
		case Qt::Key_Escape:
			{
				if(d_func()->m_closeOnEscape)
				{
					systemButtonClicked(DuiTitlebar::Close);
				}
				break;
			}

		default:
			break;
		}
	}
}

void DuiFrameWindow::onEnterPressed( Qt::KeyboardModifiers modifiers )
{
	emit enterPressed(modifiers);
}

void DuiFrameWindow::setGradientType( int type )
{
	d_func()->m_backgroundItem->setGradientType( (DuiTheme::LinearGradientType)type );
}

int DuiFrameWindow::gradientType() const
{
	Q_D(const DuiFrameWindow);
	return (int)d->m_backgroundItem->gradientType();
}

void DuiFrameWindow::showNoActive()
{
	setAttribute(Qt::WA_WState_ExplicitShowHide, false);
	setAttribute(Qt::WA_ShowWithoutActivating, true);
	QGraphicsView::show();
}

void DuiFrameWindow::showMinNoActive()
{
	// 为实现最小化不激活显示,在show之前就设置状态为最小化
	// 那么qt就会采用 SW_SHOWMINNOACTIVE 的方式了
	setWindowState(Qt::WindowMinimized);
	QGraphicsView::show();
}

void DuiFrameWindow::setPos(const QPoint &pt)
{
	setGeometry(pt.x(), pt.y(), width(), height());
}

bool DuiFrameWindow::closeOnEscape() const
{
	return d_func()->m_closeOnEscape;
}

void DuiFrameWindow::setCloseOnEscape( bool b )
{
	d_func()->m_closeOnEscape = b;
}

bool DuiFrameWindow::isAnimateInOut() const
{
	return d_func()->m_animateInOut;
}

void DuiFrameWindow::setAnimateInOut(bool animateInOut)
{
	d_func()->m_animateInOut = animateInOut;
}

void DuiFrameWindow::showAnimation()
{
	//setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QAnimationGroup *animationGrp = new QParallelAnimationGroup(this);
	{
		QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
		animation->setDuration(450);
		animation->setStartValue(0.0);
		animation->setEndValue(1.0);
		animation->setEasingCurve(QEasingCurve::InExpo);
		animationGrp->addAnimation(animation);
	}	
	{	
		QRect orgin = realFrameGeometry();
		QRect start = _zoomRect(orgin, KFadeInOutZoomOutDelta);
		QRect to = _zoomRect(orgin, KFadeInOutZoomInDelta);
		QAnimationGroup *seqanimationGrp = new QSequentialAnimationGroup(this);
		{
			QPropertyAnimation *animation = new QPropertyAnimation(this, "realFrameGeometry");
			animation->setDuration(300);
			animation->setStartValue(start);
			animation->setEndValue(to);
			animation->setEasingCurve(QEasingCurve::Linear);
			seqanimationGrp->addAnimation(animation);	
		}
		{
			QPropertyAnimation *animation = new QPropertyAnimation(this, "realFrameGeometry");
			animation->setDuration(100);
			animation->setStartValue(to);
			animation->setEndValue(orgin);
			animation->setEasingCurve(QEasingCurve::Linear);
			seqanimationGrp->addAnimation(animation);	
		}
		animationGrp->addAnimation(seqanimationGrp);
	}
	connect(animationGrp, SIGNAL(finished()), SLOT(onFadeInFinished()));
	animationGrp->start(QAbstractAnimation::DeleteWhenStopped);
}

void DuiFrameWindow::closeAnimation()
{
	QAnimationGroup *animationGrp = new QParallelAnimationGroup(this);
	{
		QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
		animation->setDuration(300);
		animation->setStartValue(1.0);
		animation->setEndValue(0.0);
		animation->setEasingCurve(QEasingCurve::OutExpo);
		animationGrp->addAnimation(animation);
	}	
	{
		QRect orgin = realFrameGeometry();
		QRect to = _zoomRect(orgin, KFadeInOutZoomInDelta);
		QPropertyAnimation *animation = new QPropertyAnimation(this, "realFrameGeometry");
		animation->setDuration(300);
		animation->setStartValue(orgin);
		animation->setEndValue(to);
		animation->setEasingCurve(QEasingCurve::OutQuad);
		animationGrp->addAnimation(animation);	
	}
	connect(animationGrp, SIGNAL(finished()), SLOT(onFadeOutFinished()));
	animationGrp->start(QAbstractAnimation::DeleteWhenStopped);
}

void DuiFrameWindow::onFadeInFinished()
{
	DuiWinAPI::SetWindowLong(winId(),
		GWL_EXSTYLE,
		GetWindowLong(winId(), GWL_EXSTYLE) | WS_EX_LAYERED);
	DuiWinAPI::SetLayeredWindowAttributes(winId(), duiFrameKeyColor(), 0, LWA_COLORKEY);
}

void DuiFrameWindow::onFadeOutFinished()
{
	QWidget::close();
}

bool DuiFrameWindow::close()
{
	if (isAnimateInOut() && d_func()->m_frameStyle->isAnimationEnabled())
	{
		closeAnimation();
        setAnimateInOut(false);
	}
	else
	{
		QWidget::close();
	}

	return true;
}

QRect DuiFrameWindow::_zoomRect( const QRectF& rect, qreal delta ) const
{
	DW_ASSERT(rect.width() > 0);
	delta *= -1;
	qreal dw = rect.width() * delta;
	qreal scale = rect.height() / rect.width();
	qreal dh = dw * scale;
	return QRect(rect.left() + dw, rect.top() + dh, rect.width() - dw * 2, rect.height() - dh * 2);
}

DuiFrameWindow::WindowPlacement DuiFrameWindow::windowPlacement() const
{
	return d_func()->m_windowPlacement;
}

void DuiFrameWindow::setWindowPlacement(DuiFrameWindow::WindowPlacement wp)
{
	d_func()->m_windowPlacement = wp;
}

bool DuiFrameWindow::needDecorationImage()
{
	Q_D(DuiFrameWindow);
	return d->m_backgroundItem->needDecorationImage();
}

void DuiFrameWindow::setNeedDecorationImage( bool useDecorationImage )
{
	Q_D(DuiFrameWindow);
	d->m_backgroundItem->setNeedDecorationImage(useDecorationImage);
}
//------------------------------------------------------------------------------
DuiSysCommandRequest::DuiSysCommandRequest(DuiTitlebar::titlebarClick type)
	: d_ptr(new DuiSysCommandRequestPrivate(type))
{
}

DuiSysCommandRequest::~DuiSysCommandRequest()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiSysCommandRequest::reject()
{
	Q_D(DuiSysCommandRequest);

	d->m_accepted = false;
}

bool DuiSysCommandRequest::accepted() const
{
	Q_D(const DuiSysCommandRequest);
	return d->m_accepted;
}

DuiTitlebar::titlebarClick DuiSysCommandRequest::type() const
{
	Q_D(const DuiSysCommandRequest);
	return d->m_type;
}
