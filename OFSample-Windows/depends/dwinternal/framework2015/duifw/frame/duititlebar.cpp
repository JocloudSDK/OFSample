#include "stable.h"

#include "duititlebar.h"
#include "duipushbutton.h"
#include "duilabelwidget.h"
#include "duiwidget_p.h"
#include "duititlebar_p.h"
#include "duifw/duiwinapi.h"
#include "duifw/duitooltip2.h"
#include "duifwstyle.h"

//------------------------------------------------------------------------------
const int KTitleBarPreferredHeight = 30;
const char KTitleShadowStyleName[] = "normalTitleShadowLabel";

//------------------------------------------------------------------------------
DuiTitlebar::DuiTitlebar(QGraphicsItem *parent)
: DuiWidget(*new DuiTitlebarPrivate(), parent)
{
	Q_D(DuiTitlebar);

	d->init();
}

void DuiTitlebar::on_minimizeButton_clicked( void )
{
	Q_D(DuiTitlebar);
	d->on_minimizeButton_clicked();
}

void DuiTitlebar::on_maximizeButton_clicked( void )
{
	Q_D(DuiTitlebar);
	d->on_maximizeButton_clicked();
}

void DuiTitlebar::on_restoreButton_clicked( void )
{
	Q_D(DuiTitlebar);
	d->on_restoreButton_clicked();
}

void DuiTitlebar::on_closeButton_clicked( void )
{
	Q_D(DuiTitlebar);
	d->on_closeButton_clicked();
}

void DuiTitlebar::setMaximized( bool maximized )
{
	Q_D(DuiTitlebar);
	d->setMaximized(maximized);
}

void DuiTitlebar::setTitle( const QString& title )
{
	Q_D(DuiTitlebar);
	d->m_windowTitleItem->setText(title);
}

void DuiTitlebar::setHasMinimizeButton( bool hasMinimizeButton )
{
	d_func()->setHasMinimizeButton( hasMinimizeButton );
}

void DuiTitlebar::setHasMaximizeButton( bool hasMaximizeButton )
{
	d_func()->setHasMaximizeButton( hasMaximizeButton );
}

//------------------------------------------------------------------------------
/**
	@brief transfer the item's ownership to titlebar, do NOT delete it yourself
*/
void DuiTitlebar::addButton( DuiPushButton *item )
{
	Q_D(DuiTitlebar);
	DW_ASSERT(!d->m_buttons.contains(item));
	d->m_buttons.push_back(item);
	this->connect(item, SIGNAL(visibleChanged()), SLOT(reLayout()));

	// sunny: setParent() is a must for test automation, please contact dw_zhuangxuhui before deleting it
	item->setParent(this);
	item->setParentItem(this);
	duiToolTip()->attatchWidget(item);
	_reLayout(size());
}

//------------------------------------------------------------------------------
/**
	@brief take the item's ownership from titlebar, do REMEMBER to delete it yourself
*/
bool DuiTitlebar::takeButton( DuiPushButton *item )
{
	Q_D(DuiTitlebar);
	int idx = d->m_buttons.indexOf(item);
	if(idx < 0)
	{
		return false;
	}
	else if(idx >= Button_User)
	{
		//remove any connection between title bar and the button
		item->disconnect(this);
		return d->m_buttons.removeOne(item);
	}
	DW_ASSERT(false && "do NOT take builtin system buttons");
	return false;
}

DuiPushButton * DuiTitlebar::titleButton( TitlebarButton button )
{
	return d_func()->titleButton( button );
}

QString DuiTitlebar::title() const
{
	Q_D(const DuiTitlebar);
	return d->m_windowTitleItem->text();
}

void DuiTitlebar::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	_reLayout(event->newSize());
}

void DuiTitlebar::setIcon(const QString &icon)
{
	Q_D(DuiTitlebar);
	d->setIcon(icon);
}

void DuiTitlebar::setIcon(const QPixmap &icon)
{
	Q_D(DuiTitlebar);
	d->setIcon(icon);
}

DuiImageWidget* DuiTitlebar::icon()
{
	Q_D(const DuiTitlebar);
	return d->m_icon;
}

const DuiImageWidget* DuiTitlebar::icon() const
{
	Q_D(const DuiTitlebar);
	return d->m_icon;
}

int DuiTitlebar::height()  const
{
	Q_D(const DuiTitlebar);
	return d->m_height;
}

void DuiTitlebar::setHeight(int h)
{
	Q_D(DuiTitlebar);
	d->setHeight(h);
}

void DuiTitlebar::setWindowId( QWidget *window  )
{
	d_func()->setWindowId(window);
}

void DuiTitlebar::refreshButtonName()
{
	d_func()->refreshButtonName();
}

void DuiTitlebar::setMargins(const QMargins& m)
{
	d_func()->setMargins(m);
}

void DuiTitlebar::setAlignment(Qt::Alignment align)
{
	d_func()->setAlignment(align);
}

void DuiTitlebar::setSpacing(int s)
{
	d_func()->setSpacing(s);
}

QRect DuiTitlebar::buttonsRect()
{
	Q_D(DuiTitlebar);
	const int KDelta = 10;
	int width = KDelta - d->m_spacing;

	foreach(const DuiWidget *w, d->m_buttons)
	{
		if (w->isVisible())
		{
			width += w->width() + d->m_spacing;
		}
	}
	return QRect(0, 0, width, preferredHeight());
}

QRect DuiTitlebar::geometryHint(QRect windowRect)
{
	Q_D(DuiTitlebar);
	windowRect.adjust(d->m_margins.left(), d->m_margins.top(), -d->m_margins.right(), -d->m_margins.bottom());
	if(d->m_windowTitleItem->text().isEmpty() && d->m_icon->pixmap().isNull())
	{
		windowRect.adjust(windowRect.width() - buttonsRect().width(), 0, 0, 0);
	}

	windowRect.setHeight(d->m_height);
	return windowRect;
}

QRect DuiTitlebar::cutNcArea(QRect windowRect)
{
	Q_D(DuiTitlebar);
	if(!d->m_windowTitleItem->text().isEmpty() || !d->m_icon->pixmap().isNull()) {
		windowRect.adjust(0, d->m_height, 0, 0);
	}
	return windowRect;
}

void DuiTitlebar::reLayout()
{
	this->_reLayout(size());
}

void DuiTitlebar::_reLayout(const QSizeF& newSize)
{
	Q_D(DuiTitlebar);

	QPointF pos(newSize.width(), 0);//7.0标题栏按钮没有边缘透明，需要向左偏移0个象素。
	foreach(DuiWidget *w, d->m_buttons)
	{
		if(w->isVisible())
		{
			pos.rx() -= w->preferredWidth();
			QRectF target(pos, w->preferredSize());
			if (d->m_alignment | Qt::AlignVCenter)
			{
				target.moveTop((d->m_height - target.height()) / 2);
			}
			else if (d->m_alignment | Qt::AlignBottom)
			{
				target.moveBottom(d->m_height);
			}
			w->setGeometry(target);
			pos.rx() -= d->m_spacing;
		}
	}

	d->m_left->setFixWidth(newSize.width() - buttonsRect().width());
}

//------------------------------------------------------------------------------
DuiTitlebarPrivate::DuiTitlebarPrivate()
: m_maximized(false)
, m_hasMinimizeButton(true)
, m_hasMaximizeButton(true)
, m_windowTitleItem(NULL)
, m_window(NULL)
, m_left(NULL)
, m_icon(NULL)
, m_height(KTitleBarPreferredHeight)
, m_margins(1, 1, 1, 1)
, m_alignment(Qt::AlignTop)
, m_spacing(0)
{
}

void DuiTitlebarPrivate::on_minimizeButton_clicked()
{
	Q_Q(DuiTitlebar);
	DuiTitlebar::titlebarClick click = DuiTitlebar::Minimize;
	emit q->buttonClicked(click);
}

void DuiTitlebarPrivate::on_maximizeButton_clicked()
{
	Q_Q(DuiTitlebar);
	DuiTitlebar::titlebarClick click = DuiTitlebar::Maximize;
	emit q->buttonClicked(click);
	setMaximized(true);
}

void DuiTitlebarPrivate::on_restoreButton_clicked()
{
	Q_Q(DuiTitlebar);
	DuiTitlebar::titlebarClick click = DuiTitlebar::Restore;
	emit q->buttonClicked(click);
	setMaximized(false);
}

void DuiTitlebarPrivate::on_closeButton_clicked()
{
	Q_Q(DuiTitlebar);
	DuiTitlebar::titlebarClick click = DuiTitlebar::Close;
	emit q->buttonClicked(click);
}

static const char* kButtonTable[][4] =
{
	//name_prefix, pic, text, slot
	{"close", "close.png", "Close", SLOT(on_closeButton_clicked())},
	{"maximize", "max.png", "Maximize", SLOT(on_maximizeButton_clicked())},
	{"restore", "restore.png", "Restore", SLOT(on_restoreButton_clicked())},
	{"minimize", "min.png", "Minimize", SLOT(on_minimizeButton_clicked())},
};

#define IMPLEMENT_BUTTON(id) \
	do { \
		static const QString strToopTip[4] = {DuiTitlebar::tr("Close"), DuiTitlebar::tr("Maximize"), DuiTitlebar::tr("Restore"), DuiTitlebar::tr("Minimize")};\
		\
		const char** entry = kButtonTable[id];\
		DuiPushButton *tmp = new DuiPushButton(q); \
		tmp->setObjectName(entry[0]); \
		tmp->setBackgroundColorable( DuiWidget::UnColorableFlag ); \
        tmp->setAnimateInOut(false);\
		q->connect(tmp, SIGNAL(clicked()), entry[3]); \
		tmp->setToolTip(strToopTip[id]);\
		tmp->setImagePath(":/theme/duifw/" + QString(entry[1])); \
		tmp->setFlag(QGraphicsItem::ItemIsFocusable, false); \
		tmp->setFocusPolicy(Qt::NoFocus);\
		q->addButton(tmp); \
		DW_ASSERT(m_buttons.indexOf(tmp) == id);\
	} while(0)

void DuiTitlebarPrivate::init()
{
	Q_Q(DuiTitlebar);

	q->setAcceptHoverEvents(false);
	q->setPreferredHeight(m_height);
	q->setDragPolicy( DuiWidget::DragWindow );
	_initTitleText();

	IMPLEMENT_BUTTON(DuiTitlebar::Button_Close);
	IMPLEMENT_BUTTON(DuiTitlebar::Button_Maximize);
	IMPLEMENT_BUTTON(DuiTitlebar::Button_Restore);
	IMPLEMENT_BUTTON(DuiTitlebar::Button_Minimize);
	m_buttons[DuiTitlebar::Button_Restore]->setVisible(false);

	_adjustButtonSize();
	q->_reLayout(q->size());
}

#undef IMPLEMENT_BUTTON

void DuiTitlebarPrivate::setMaximized( bool maxzimized )
{
	if (maxzimized == m_maximized)
	{
		return;
	}
	m_maximized = maxzimized;
	if (!m_hasMaximizeButton)
	{
		return;
	}

	m_buttons[DuiTitlebar::Button_Maximize]->setVisible(!m_maximized);
	m_buttons[DuiTitlebar::Button_Restore]->setVisible(m_maximized);
}

void DuiTitlebarPrivate::setHasMinimizeButton( bool hasMinimizeButton )
{
	if (hasMinimizeButton == m_hasMinimizeButton)
	{
		return;
	}

	m_hasMinimizeButton = hasMinimizeButton;
	m_buttons[DuiTitlebar::Button_Minimize]->setVisible(m_hasMinimizeButton);

	if (m_window)
	{
		if (m_hasMinimizeButton)
		{
			DuiWinAPI::addWindowStyle((UINT32)m_window->winId(), WS_MINIMIZEBOX | WS_SYSMENU);
		}
		else
		{
			DuiWinAPI::removeWindowStyle((UINT32)m_window->winId(), WS_MINIMIZEBOX | WS_SYSMENU);
		}
	}
}

void DuiTitlebarPrivate::setHasMaximizeButton( bool hasMaximizeButton )
{
	if (hasMaximizeButton == m_hasMaximizeButton)
	{
		return;
	}
	m_hasMaximizeButton = hasMaximizeButton;
	if (m_hasMaximizeButton)
	{
		if(m_maximized)
		{
			m_buttons[DuiTitlebar::Button_Restore]->setVisible(true);
		}
		else
		{
			m_buttons[DuiTitlebar::Button_Maximize]->setVisible(true);
		}
	}
	else
	{
		m_buttons[DuiTitlebar::Button_Restore]->setVisible(false);
		m_buttons[DuiTitlebar::Button_Maximize]->setVisible(false);
	}

	if (m_window)
	{
		if (m_hasMaximizeButton)
		{
			DuiWinAPI::addWindowStyle((UINT32)m_window->winId(), WS_MAXIMIZEBOX | WS_SYSMENU);
		}
		else
		{
			DuiWinAPI::removeWindowStyle((UINT32)m_window->winId(), WS_MAXIMIZEBOX | WS_SYSMENU);
		}
	}
}

DuiPushButton * DuiTitlebarPrivate::titleButton( DuiTitlebar::TitlebarButton button )
{
	if(button < m_buttons.size())
	{
		return qobject_cast<DuiPushButton*>(m_buttons[button]);
	}
	DW_ASSERT(false);
	return NULL;
}

void DuiTitlebarPrivate::_adjustButtonSize()
{
    //yy7.0把标题栏按钮大小统一改为30*30
	m_buttons[DuiTitlebar::Button_Minimize]->setFixSize(QSizeF(30, 30));
	m_buttons[DuiTitlebar::Button_Maximize]->setFixSize(QSizeF(30, 30));
	m_buttons[DuiTitlebar::Button_Restore]->setFixSize(QSizeF(30, 30));
	m_buttons[DuiTitlebar::Button_Close]->setFixSize(QSizeF(30, 30));
}

void DuiTitlebarPrivate::_initTitleText()
{
	Q_Q(DuiTitlebar);

	m_left = new DuiWidget(q);
	m_left->setLayout(DuiWidget::HBox);
	m_left->setFixHeight(m_height);
	m_left->setSpacing(6);
	m_left->setMargins(QMargins(8, 2, 0, 0));

	m_icon = new DuiImageWidget(m_left);
	m_left->addItem(m_icon);
	m_windowTitleItem = new DuiLabelWidget(m_left);
	m_windowTitleItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_windowTitleItem->setFocusPolicy(Qt::ClickFocus);
    m_windowTitleItem->setShadowBlurRadius(3);
    m_windowTitleItem->setShadowSize(3);
    m_windowTitleItem->setFontSize(14);
	m_left->addItem(m_windowTitleItem);

	if(duiGetStyleManager()->hasStyleInfo(KTitleShadowStyleName))
	{
		m_windowTitleItem->setStyleId(KTitleShadowStyleName);
	}
	else
	{
		m_windowTitleItem->setDropShadow(true);
		m_windowTitleItem->setShadowBlurRadius(3);		
		m_windowTitleItem->setShadowOpacity(0.5);
		m_windowTitleItem->setShadowColor( QColor(255, 255, 255, 180) );
		m_windowTitleItem->setTextColor(Qt::black);
	}	
}

void DuiTitlebarPrivate::setWindowId( QWidget *window  )
{
	m_window = window;
}

void DuiTitlebarPrivate::refreshButtonName()
{
	if (m_window)
	{
		QString windowName = m_window->objectName();

		m_buttons[DuiTitlebar::Button_Minimize]->setObjectName(windowName + "MinimizeButton");
		m_buttons[DuiTitlebar::Button_Maximize]->setObjectName(windowName + "MaximizeButton");
		m_buttons[DuiTitlebar::Button_Restore]->setObjectName(windowName + "RestoreButton");
		m_buttons[DuiTitlebar::Button_Close]->setObjectName(windowName + "CloseButton");
	}
}

void DuiTitlebarPrivate::setIcon(const QPixmap &icon)
{
	Q_Q(DuiTitlebar);

	m_icon->setPixmap(icon);
	q->_reLayout(q->size());
}

void DuiTitlebarPrivate::setIcon(const QString &icon)
{
	Q_Q(DuiTitlebar);

	m_icon->setImagePath(icon);
	q->_reLayout(q->size());
}

void DuiTitlebarPrivate::setHeight(int h)
{
	Q_Q(DuiTitlebar);

	m_height = h;
	m_left->setFixHeight(h-4); //减4为了平衡之前setMargin的top为2
	q->_reLayout(q->size());
}

void DuiTitlebarPrivate::setMargins(const QMargins& m)
{
	if (m_margins == m)
		return;

	Q_Q(DuiTitlebar);
	m_margins = m;
	q->_reLayout(q->size());
}

void DuiTitlebarPrivate::setAlignment(Qt::Alignment align)
{
	if (m_alignment == align)
		return;

	Q_Q(DuiTitlebar);
	m_alignment = align;
	q->_reLayout(q->size());
}

void DuiTitlebarPrivate::setSpacing(int s)
{
	if (m_spacing == s)
		return;

	Q_Q(DuiTitlebar);
	m_spacing = s;
	q->_reLayout(q->size());
}
