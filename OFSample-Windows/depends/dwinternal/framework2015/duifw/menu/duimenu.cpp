#include "stable.h"

#include "dwutility/dwxmldocument.h"

#include "duifw/duixmlui.h"
#include "duifw/duitheme.h"
#include "duifw/duininepatchpixmapitem.h"
#include "duifw/duidrawutil.h"
#include "duifw/duiresourceloader.h"
#include "duisigslothook.h"
#include "duifw/duiwinapi.h"

#include "duimenu.h"

const int KOffsetX = 4;
const int KOffsetY = 6;
const int KAlienArea = 0;          //主菜单边缘SIZE，7.0改为0
// const int KMainMenuSpaceOffsetX = -14; //7.0不对主菜单显示位置特殊处理
const int KMenuSpaceOffsetX = -4;
const int KTrayIconMenuContentsMarginsTop = KOffsetY+3;
const int KTrayIconMenuFirstItemTop = KTrayIconMenuContentsMarginsTop + 3;
const int KTrayIconMenuFirstItemTextOffsetX = 12;
const int KMainMenuSepMarginRight = 5;
const int KMenuSepMarginRight = 5;
const int KMainMenuSepMarginLeft = 5;
const int KMenuSepMarginLeft = 5;

const char KActionHoveredImage[] = {":/theme/duifw/menu/actionHovered.png"};
const char KDisableActionHoveredImage[] = {":/theme/duifw/menu/disableActionHovered.png"};
const char KSecondMenuArrowNormalImage[] = {":/theme/duifw/menu/secondMenu_arrow_normal.png"};
const char KSecondMenuArrowHoveredImage[] = {":/theme/duifw/menu/secondMenu_arrow_hovered.png"};
const char KMenuCheckedIcon[] = {":theme/duifw/menu/menu_checked.png"};
const char KBackgroundImage[] = {":/theme/duifw/menu/menu_frame_background.png"};
const char KTrayIconMenuForeground[] = {":/theme/duifw/menu/trayIconMenu_frame_foreground.png"};
const char KTrayIconMenuBackgroundImage[] = {":/theme/duifw/menu/trayIconMenu_frame_background.png"};

static int KActionTriggeredMethodIndex = 0;

const QColor    KMenuTextColorNormal = QColor(0x33,0x33,0x33,255);
const QColor    KMenuTextColorDisable = QColor(0xa8,0xa8,0xa8);

class DuiMenuPrivate
{
	Q_DECLARE_PUBLIC(DuiMenu)

public:
	DuiMenuPrivate();

	QAction			*actionByName( QMenu *menu, const QString &actionName );
	void			removeAction( QMenu *menu, const QString &actionName );
	QRect			adjustGeometry( const QRect &menuRect );
	void			drawPixmap(QPainter *painter, const QRect &rect, const QPixmap &image) const;
	void			drawLeftIcon(QPainter *painter, const QIcon &icon, const QRect &vCheckRect, const QStyleOption *opt, const QWidget *w) const;
	void			showAnimation();
	void			setAlienMenuType( DuiMenu::AlienType alienType);

public:
	QRect					m_alignRect;
	Qt::Alignment			m_alignment;
	DuiMenu::AlienType		m_alienMenuType;
	QRectF					m_senderRect;
	bool					m_isMainMenuReverse;

private:
	DuiMenu		*q_ptr;
};

//----------DuiCustomMenuStyle----------------------------------------
class DuiCustomMenuStyle : public QWindowsStyle
{
public:
	enum {
		windowsItemFrame        =  2, // menu item frame width
		windowsSepHeight        =  9, // separator item height
		windowsItemHMargin      =  3, // menu item hor text margin
		windowsItemVMargin      =  2, // menu item ver text margin
		windowsArrowHMargin     =  6, // arrow horizontal margin
		windowsRightBorder      = 15, // right border on windows
		windowsCheckMarkWidth   = 20  // checkmarks width on windows
	};

	void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
		const QWidget *w = 0) const;

	void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
		const QWidget *w = 0) const;

	int pixelMetric(PixelMetric pm, const QStyleOption *option = 0, const QWidget *widget = 0) const;

	QSize sizeFromContents(ContentsType ct, const QStyleOption *opt,
		const QSize &contentsSize, const QWidget *widget = 0) const;
};
Q_GLOBAL_STATIC(DuiCustomMenuStyle, _dw_CustomMenuStyle);

//---------------------------------------------------------
namespace
{
	QAction* createAction(const QString& type, QObject* parent)
	{
		if (type == "Action")
		{
			return new QAction(parent);
		}
		else if (type == "Separator")
		{
			QAction* action = new QAction(parent);
			action->setSeparator(true);
			return action;
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid action type").arg(type).toLatin1().constData());
		}
		return NULL;
	}

	void setMenuProperty(DuiMenu* menu, const DWXmlElement *child)
	{
		int count = child->attributeCount();
		for (int index = 0; index < count; ++index)
		{
			std::wstring wstrAttrName = child->attributeName(index);
			if (wstrAttrName.empty())
			{
				break;
			}
			QByteArray attrName = QStringFromWString(wstrAttrName).toLatin1();
			QString attr = QStringFromWString(child->attribute(index));
			menu->setProperty(attrName.constData(), attr);
		}
	}

	void setActionProperty(QAction* action, const DWXmlElement *child)
	{
		int count = child->attributeCount();
		for (int index = 0; index < count; ++index)
		{
			std::wstring wstrAttrName = child->attributeName(index);
			if (wstrAttrName.empty())
			{
				break;
			}
			QByteArray attrName = QStringFromWString(wstrAttrName).toLatin1();
			QString attr = QStringFromWString(child->attribute(index));
			if (attrName == "icon")
			{
				action->setIcon(QIcon(attr));
			}
			else if ( attrName == "text" )
			{
				 action->setProperty( attrName.constData(), DuiXmlUI::tr(attr) );
			}
			else if ( attrName == "uuid")
			{
				action->setProperty( attrName.constData(), attr );
			}
			else
			{
				action->setProperty(attrName.constData(), attr);
			}
		}
	}

	void constructMenu(DuiMenu* menu, const DWXmlElement *xml)
	{
		int count = xml->childCount();
		const DWXmlElement *subXml = NULL;
		for (int i = 0; i < count; ++i)
		{
			subXml = xml->child(i);
			std::wstring className = subXml->tag();
			QString qclassName = QStringFromWString(className);
			if (qclassName == "ActionGroup")
			{
				std::wstring title = subXml->attribute(L"text");
				DuiMenu* subMenu = new DuiMenu(menu);
				std::wstring objectName = subXml->attribute(L"objectName");
				subMenu->setObjectName(QStringFromWString(objectName));
				QAction *action = menu->addMenu(subMenu);
				action->setText( DuiXmlUI::tr(QStringFromWString(title)));
				action->setObjectName(QStringFromWString(objectName));
				std::wstring iconPath = subXml->attribute(L"icon");
				if (!iconPath.empty())
				{
					action->setIcon( QIcon( QStringFromWString(iconPath) ) );
				}

				constructMenu(subMenu, subXml);
			}
			else
			{
				QAction* action = createAction(qclassName, menu);
				setActionProperty(action, subXml);
				menu->addAction(action);
			}
		}
	}
}

//----------DuiMenuPrivate----------------------------------------
DuiMenuPrivate::DuiMenuPrivate()
: m_isMainMenuReverse(false)
{
	if(KActionTriggeredMethodIndex == 0)
	{
		KActionTriggeredMethodIndex = QAction::staticMetaObject.indexOfMethod("triggered(bool)");
	}
}

QRect DuiMenuPrivate::adjustGeometry( const QRect &menuRect )
{
	if ( m_alienMenuType == DuiMenu::MainMenu )
	{
		m_isMainMenuReverse = false;
	}

 	QRect screenRect = QApplication::desktop()->screenGeometry(menuRect.topLeft());
  	QRect newRect = menuRect;
 	if(newRect.right() > screenRect.right())
 	{
		if ( m_alienMenuType == DuiMenu::MainMenu )
		{
			newRect = newRect.translated(-newRect.width()+79, 0);
			m_isMainMenuReverse = true;
		}
		else
		{
			newRect = newRect.translated(screenRect.right() - newRect.right(), 0);
		}
 	}
 
 	if(newRect.bottom() > screenRect.bottom())
 	{
 		newRect = newRect.translated(0, screenRect.bottom() - newRect.bottom());
 	}
 
	if(newRect.left() < screenRect.left() && m_alienMenuType != DuiMenu::MainMenu )
 	{
 		newRect = newRect.translated(screenRect.left() - newRect.left(), 0);
 	}
 
 	if(newRect.top() < screenRect.top())
 	{
 		newRect = newRect.translated(0, screenRect.top() - newRect.top());
 	}

	return newRect;
}

QAction * DuiMenuPrivate::actionByName( QMenu *menu, const QString &actionName )
{
	if ( NULL == menu )
	{
		return NULL;
	}

	QList<QAction *> actionList = menu->actions();

	foreach( QAction *action, actionList )
	{
		if ( action->objectName() == actionName )
		{
			return action;
		}

		QMenu *subMenu = action->menu();

		action = actionByName( subMenu, actionName );
		if ( NULL != action )
		{
			return action;
		}
	}

	return NULL;
}

void DuiMenuPrivate::removeAction( QMenu *menu, const QString &actionName )
{
	if ( NULL == menu )
	{
		return;
	}

	QList<QAction *> actionList = menu->actions();

	foreach( QAction *action, actionList )
	{
		if ( action->objectName() == actionName )
		{
			menu->removeAction(action);
			continue;
		}

		QMenu *subMenu = action->menu();

		removeAction( subMenu, actionName );
	}
}

void DuiMenuPrivate::drawPixmap( QPainter *painter, const QRect &rect, const QPixmap &image ) const
{
	DW_ASSERT(painter);

	if ( DuiDrawUtil::isNinePatchsPixmap(image) )
	{
		DuiDrawUtil::drawPixmap(painter, rect, image);
	}
	else
	{
		painter->drawPixmap(rect, image);
	}
}

void DuiMenuPrivate::drawLeftIcon( QPainter *painter, const QIcon &icon, const QRect &vCheckRect, const QStyleOption *opt, const QWidget *w ) const
{
	DW_ASSERT(painter);
	
	if ( icon.isNull() )
	{
		return;
	}

	const QStyleOptionMenuItem *menuitem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
	if ( !menuitem )
	{
		return;
	}
	bool dis = !(menuitem->state & QStyle::State_Enabled);
	bool checked = (menuitem->checkType != QStyleOptionMenuItem::NotCheckable) ? menuitem->checked : false;
	bool act = menuitem->state & QStyle::State_Selected;

	QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;

	if ( act && !dis )
	{
		mode = QIcon::Active;
	}

	QPixmap pixmap;
	int pixw = -1;
	int pixh = -1;

 	if (checked)
 	{
 		pixmap = icon.pixmap(q_func()->style()->pixelMetric(DuiCustomMenuStyle::PM_SmallIconSize, opt, w), mode, QIcon::On);
 	}
 	else
 	{
 		pixmap = icon.pixmap(q_func()->style()->pixelMetric(DuiCustomMenuStyle::PM_SmallIconSize, opt, w), mode);
 	}

	pixw = pixmap.width();
	pixh = pixmap.height();
	// no icon frame
 	QRect pmr(0, 0, pixw, pixh);
 	pmr.moveCenter(vCheckRect.center());
 	painter->setPen(menuitem->palette.text().color());
 	painter->drawPixmap(pmr.topLeft(), pixmap);
}

void DuiMenuPrivate::showAnimation()
{
	Q_Q(DuiMenu);
	QAnimationGroup *animationGrp = new QParallelAnimationGroup(q);
	{
		QPropertyAnimation *animation = new QPropertyAnimation(q, "windowOpacity");
		animation->setDuration(450);
		animation->setStartValue(0.0);
		animation->setEndValue(1.0);
		animation->setEasingCurve(QEasingCurve::InExpo);
		animationGrp->addAnimation(animation);
	}
	animationGrp->start(QAbstractAnimation::DeleteWhenStopped);
}

void DuiMenuPrivate::setAlienMenuType( DuiMenu::AlienType alienType)
{
	Q_Q(DuiMenu);
	m_alienMenuType = alienType;
 	switch( m_alienMenuType )
 	{
	case DuiMenu::NormalMenu:
		q->setContentsMargins(QMargins(0, KOffsetY, 0, KOffsetY));
		break;
	case DuiMenu::MainMenu:
		q->setContentsMargins(QMargins(0, KOffsetY, 0, KOffsetY));
		q->setMinimumWidth(164);
		q->setMinimumHeight(181);
		break;
	case DuiMenu::TrayIconMenu:
		q->setContentsMargins(QMargins(0, KTrayIconMenuContentsMarginsTop, 0, KOffsetY));
		break;
	default:
		break;
 	}
}

//----------------DuiCustomMenuStyle------------------------------------------
void DuiCustomMenuStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
									   const QWidget *w) const
{
	const DuiMenu * customMenu = qobject_cast<const DuiMenu*>(w);
	DW_ASSERT(customMenu);

	if (QStyle::PE_PanelMenu == pe)
	{
		customMenu->drawPrimitivePanelMenu(p, opt, w);
	}
	else if (QStyle::PE_FrameMenu == pe)
	{
		customMenu->drawPrimitiveFrameMenu(p, opt, w);
	}
	else
	{
		__super::drawPrimitive(pe, opt, p, w);
	}
}

void DuiCustomMenuStyle::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
									 const QWidget *w/* = 0*/) const
{
	const DuiMenu * customMenu = qobject_cast<const DuiMenu*>(w);
	DW_ASSERT(customMenu);

	if (QStyle::CE_MenuItem == element)
	{
		customMenu->drawControlMenuItem(p, opt, w);
	}
	else if (QStyle::CE_MenuScroller == element)
	{
		customMenu->drawControlMenuScroller(p, opt, w);
	}
	else if (QStyle::CE_MenuTearoff == element)
	{
		customMenu->drawControlMenuTearoff(p, opt, w);
	}
	else if (QStyle::CE_MenuEmptyArea == element)
	{
		customMenu->drawControlMenuEmptyArea(p, opt, w);
	}
	else
	{
		__super::drawControl(element, opt, p, w);
	}
}

int DuiCustomMenuStyle::pixelMetric(PixelMetric pm, const QStyleOption *option/* = 0*/, 
									const QWidget *widget/* = 0*/) const
{
	if (pm == QStyle::PM_MenuTearoffHeight)
	{
		return __super::pixelMetric(pm, option, widget);
	}
	else if (pm == QStyle::PM_MenuPanelWidth)
	{
		return __super::pixelMetric(pm, option, widget);
	}
	else if (pm == QStyle::PM_SubMenuOverlap)
	{
		const DuiMenu *pMenu = qobject_cast<const DuiMenu*>(widget);
		DW_ASSERT(pMenu);
// 		if ( pMenu->alienMenuType() == DuiMenu::MainMenu )
// 		{
// 			return KMainMenuSpaceOffsetX;
// 		}
		return proxy()->pixelMetric(QStyle::PM_MenuPanelWidth, option, widget) + KMenuSpaceOffsetX;
	}

	return __super::pixelMetric(pm, option, widget);
}

QSize DuiCustomMenuStyle::sizeFromContents( ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget /*= 0*/ ) const
{
	QSize sz = QWindowsStyle::sizeFromContents(ct, opt, contentsSize, widget);
	int w = sz.width();
	int h = sz.height();

	if ( ct == CT_MenuItem )
	{
		const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
		if (mi) 
		{
			if (mi->menuItemType == QStyleOptionMenuItem::Separator) 
			{
				sz = QSize(10, 4);
				return sz;
			}

			if (mi->menuItemType == QStyleOptionMenuItem::SubMenu)
			{
				sz.setHeight( h + 2 );
			} 
			else if ( mi->icon.isNull() )
			{
				h += 2;
				sz.setHeight( h + 2 );
			}

			int maxpmw = mi->maxIconWidth;
			int checkcol = qMax<int>(maxpmw, DuiCustomMenuStyle::windowsCheckMarkWidth); // Windows always shows a check column
			w += checkcol;
			w += int(DuiCustomMenuStyle::windowsRightBorder);
			sz.setWidth(w);
			sz.setHeight( h + DuiCustomMenuStyle::windowsItemVMargin );
		}

	}
	return sz;
}

//---------------DuiMenu------------------------------------------------------
DuiMenu::DuiMenu( QWidget* parent /*= NULL*/ )
: QMenu(parent)
, d_ptr(new DuiMenuPrivate())
{
	connect(this, SIGNAL(triggered(QAction*)), SLOT(on_actionTriggered(QAction*)));
	connect(this, SIGNAL(aboutToHide()), SLOT(on_aboutToHide()));
 	setStyle(_dw_CustomMenuStyle());
 	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowTitle(tr("menu"));
	d_ptr->q_ptr = this;
	d_ptr->setAlienMenuType(DuiMenu::NormalMenu);

	// no windows shadow
	DuiWinAPI::removeDropShadowStyle(winId());
}

DuiMenu::~DuiMenu()
{
	DW_SAFE_DELETE(d_ptr);
}

bool DuiMenu::createFromFile(const QString& fileName, QObject* actionSlotReceiver, Qt::ConnectionType connectionType)
{
	return createFromFile(fileName, "", actionSlotReceiver, connectionType);
}

bool DuiMenu::createFromFile( const QString& fileName, const QString& rootPath, QObject* actionSlotReceiver, Qt::ConnectionType connectionType /*= Qt::DirectConnection*/ )
{
	return createFromFile(fileName, rootPath, actionSlotReceiver, connectionType, NULL);
}

bool DuiMenu::createFromFile( const QString& fileName, const QString& rootPath, QObject* actionSlotReceiver, Qt::ConnectionType connectionType, QTranslator *t)
{
	DW_ASSERT(connectionType == Qt::DirectConnection || connectionType == Qt::QueuedConnection);

	DuiXmlUI::AutoTranslatorSetting setter(t);		

	LogFinal("Menu") << fileName;
	QString filePath = fileName;
	if ( !rootPath.isEmpty() )
	{
		filePath = rootPath + fileName;
	}

	DWXmlDocument xml;
	if (!getResourceLoader()->loadXml(filePath, xml))
	{
		DW_ASSERT_X(false, __FUNCTION__, QString("%1 is not an valid xml file.").arg(filePath).toLatin1().constData());
		return NULL;
	}

	const DWXmlElement *child = xml.elementByPath(L"xmlui");

	if (NULL == child)
	{
		DW_ASSERT_X(false, __FUNCTION__, "child is null.");
		return NULL;
	}

	child = child->child(0);
	if (NULL == child)
	{
		DW_ASSERT_X(false, __FUNCTION__, "child is null.");
		return NULL;
	}

	std::wstring className = child->tag();

	if (className.empty())
	{
		DW_ASSERT_X(false, __FUNCTION__, "className is empty.");
		return NULL;
	}
	QString qclassName = QStringFromWString(className);
	if (qclassName == "ActionGroup")
	{
		constructMenu(this, child);
	}
	else
	{
		DW_ASSERT_X(false, __FUNCTION__, "qclassName should be ActionGroup.");
	}

	if (actionSlotReceiver != NULL)
	{
		QList<QAction*> oList = findChildren<QAction*>(QString());
		const QMetaObject* mo = actionSlotReceiver->metaObject();
		for (int i = 0; i < mo->methodCount(); ++i)
		{
			const char* slot = mo->method(i).signature();
			DW_ASSERT_X(slot, __FUNCTION__, "slot name is null.");
			QStringList sList = QString(slot).split('_');
			if (sList.count() != 3 || sList[0] != "on" || sList[2] != "triggered(bool)")
			{
				continue;
			}
			for (int j = 0; j < oList.count();++j)
			{
				if (sList[1] == oList[j]->objectName())
				{
					QByteArray slotName("1");
					slotName += slot;
					bool b = actionSlotReceiver->connect(oList[j], SIGNAL(triggered(bool)), slotName, connectionType);
					DW_ASSERT_X(b, __FUNCTION__, "can't connect slot in DuiMenu");
					Q_UNUSED(b);
				}
			}
		}
	}

	return true;
}

QAction* DuiMenu::exec()
{
	DW_ASSERT_X(false,__FUNCTION__, "deprecated method!!");

	LogFinal("Menu") << L"menu message loop begin";

	QAction* pAct = QMenu::exec(QCursor::pos());

	LogFinal("Menu") << L"menu message loop end";

	return pAct;
}

QAction* DuiMenu::exec(const QRect &alignRect, Qt::Alignment alignment)
{
	DW_ASSERT_X(false,__FUNCTION__, "deprecated method!!");

	LogFinal("Menu") << L"menu message loop begin";

	d_ptr->m_alignRect = alignRect;
	d_ptr->m_alignment = alignment;

	QAction* pAct = QMenu::exec(QCursor::pos());

	LogFinal("Menu") << L"menu message loop end";

	return pAct;
}

void DuiMenu::popup( QAction * atAction /*= 0*/ )
{
	LogFinal("Menu") << L"popup menu";
	QMenu::popup(QCursor::pos(), atAction);
}

void DuiMenu::popup( const QRect &alignRect, Qt::Alignment alignment, QAction * atAction /*= 0*/ )
{
	LogFinal("Menu") << L"popup menu";

	d_ptr->m_alignment = alignment;
	d_ptr->m_alignRect = alignRect;
	QMenu::popup(alignRect.topLeft(), atAction);
}

void DuiMenu::popup2( const QRect &alignRect, AlignCorner alignCorner /*= AlignAuto*/, QAction * atAction /*= 0*/ )
{
	LogFinal("Menu") << L"popup menu 2";

	// 为了让菜单对齐托盘图标的四个角，重新计算菜单锚点的位置，避免QT做偏移修正
	QSize sizeMenu = sizeHint();
	QSize sizeScreen(QApplication::desktop()->width(), QApplication::desktop()->height());

	// 这两个分别是给QMenu的对齐方式与对齐点
	Qt::Alignment alignment = Qt::AlignBottom;
	QPoint anchorPoint;

	switch(alignCorner)
	{
	case AlignAuto:	// 自动对齐方式，将根据屏幕的剩余空间确定菜单出现的位置，可用于托盘图标
		if (sizeScreen.height() - alignRect.bottom() >= sizeMenu.height())
		{
			alignment = Qt::AlignBottom;
		}
		else
		{
			alignment = Qt::AlignTop;
		}

		// 确定虚拟对齐点
		if ((alignment & Qt::AlignVertical_Mask & Qt::AlignBottom) == Qt::AlignBottom)
		{
			anchorPoint.setY(alignRect.bottom());
		}
		else
		{
			anchorPoint.setY(alignRect.top());
		}

		if (sizeScreen.width() - alignRect.right() >= sizeMenu.width())
		{
			// 右区域足够容纳
			anchorPoint.setX(alignRect.right());
		}
		else
		{
			// 一般菜单是左下角对齐的，这里偏移一个菜单宽度，让菜单出现在对齐物的左边
			anchorPoint.setX(alignRect.left() - sizeMenu.width());
		}

		break;
	case AlignTopLeft:
		anchorPoint = alignRect.topLeft();
		alignment = Qt::AlignTop;
		break;
	case AlignTopRight:
		anchorPoint = alignRect.topRight();
		alignment = Qt::AlignTop;
		break;
	case AlignBottomLeft:
		anchorPoint = alignRect.bottomLeft();
		alignment = Qt::AlignBottom;
		break;
	case AlignBottomRight:
		anchorPoint = alignRect.bottomRight();
		alignment = Qt::AlignBottom;
		break;
	default:
		DW_ASSERT(false);
	}

	popup(QRect(anchorPoint, anchorPoint), alignment, atAction);
}

bool DuiMenu::connectMenuAction(const QString& actionName, QObject* obj, const char* slotName)
{
	QAction* action = findChild<QAction*>(actionName);
	if (action == NULL)
	{
		DW_ASSERT_X(false, __FUNCTION__, "actionName not found!");
		return false;
	}

	return QObject::connect(action, SIGNAL(triggered(bool)), obj, slotName);
}

void DuiMenu::removeAction( const QString &actionName )
{
	d_func()->removeAction( this, actionName );
}

QAction * DuiMenu::actionByName( const QString &actionName )
{
	return d_func()->actionByName( this, actionName );
}

QAction *DuiMenu::actionByIndex(const int index)
{
	return actions().value(index);
}

void DuiMenu::showEvent( QShowEvent *event )
{
 	QRect menuRect = geometry();
 	bool isAlignToRigth = Qt::AlignRight == ((d_ptr->m_alignment & Qt::AlignHorizontal_Mask) & Qt::AlignRight);
 
 	QRect newMenuRect = menuRect;
 	switch (d_ptr->m_alignment & Qt::AlignVertical_Mask)
 	{
 	case Qt::AlignTop:
 		{
 			if (isAlignToRigth)
 			{
 				newMenuRect = QRect(d_ptr->m_alignRect.right() - menuRect.width(), d_ptr->m_alignRect.top() - menuRect.height(), menuRect.width(), menuRect.height());
 			}
 			else
 			{
 				newMenuRect = QRect(d_ptr->m_alignRect.left(), d_ptr->m_alignRect.top() - menuRect.height(), menuRect.width(), menuRect.height());
 			}
 			break;
 		}
 
 	case Qt::AlignBottom:
 		{
 			if (isAlignToRigth)
 			{
 				newMenuRect = QRect(d_ptr->m_alignRect.right() - menuRect.width(), d_ptr->m_alignRect.bottom(), menuRect.width(), menuRect.height());
 			}
 			else
 			{
 				newMenuRect = QRect(d_ptr->m_alignRect.left(), d_ptr->m_alignRect.bottom(), menuRect.width(), menuRect.height());
 			}
 			break;
 		}
 	}
 
 	newMenuRect = d_func()->adjustGeometry(newMenuRect);
    if(parent())
    {//子菜单上移5个点。
        newMenuRect.translate(0,-5);
    }
 	setGeometry(newMenuRect);
	
	BOOL ret = FALSE;
	SystemParametersInfo(SPI_GETMENUANIMATION, 0, &ret, 0);
	if(!ret)
	{
		d_func()->showAnimation();
	}
 	QMenu::showEvent(event);
}

void DuiMenu::on_actionTriggered( QAction * action )
{
	LogFinal("Menu") << action->objectName().toStdWString();

	duiCallSignalBegin(action, KActionTriggeredMethodIndex);
	duiCallSignalEnd(action, KActionTriggeredMethodIndex);
}

void DuiMenu::on_aboutToHide()
{
 	close();
}

bool DuiMenu::event( QEvent * event )
{
	if ( event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		DW_ASSERT(mouseEvent);
		QPoint globalPos = mapToGlobal( mouseEvent->pos() );
		if (d_func()->m_senderRect.contains(globalPos))
		{
			setAttribute(Qt::WA_NoMouseReplay);
			close();
			return true;
		}
	}

	if ( d_func()->m_alienMenuType == MainMenu && 
		(event->type() == QEvent::MouseButtonPress || 
		event->type() == QEvent::MouseButtonDblClick || 
		event->type() == QEvent::MouseMove)  )
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		DW_ASSERT(mouseEvent);
		QPointF pos = mouseEvent->posF();
		QRectF rc = rect();
		//Remove the shadow size
		rc.adjust(KOffsetX * 3 + 1, 0, -KOffsetX * 4 + 1, 0);
		if ( !rc.contains(pos) )
		{
			switch(event->type())
			{
			case QEvent::MouseButtonPress :
			case QEvent::MouseButtonDblClick :
				setAttribute(Qt::WA_NoMouseReplay);
				close();
				return true;
			case QEvent::MouseMove :
				event->ignore();
				return true;
			default:
				break;
			}
		}
	}
	
	return QMenu::event(event);
}

DuiMenu::AlienType DuiMenu::alienMenuType() const
{
	return d_func()->m_alienMenuType;
}

void DuiMenu::setAlienMenuType( AlienType alienType)
{
	d_func()->setAlienMenuType(alienType);
}

void DuiMenu::setSenderRect( const QRectF &senderRect )
{
	d_func()->m_senderRect = senderRect;
}

DuiMenu* DuiMenu::createPopupMenu()
{
	DuiMenu *menu = new DuiMenu();
	menu->setAttribute(Qt::WA_DeleteOnClose, true);
	return menu;
}

void DuiMenu::drawPrimitivePanelMenu(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(opt);
	Q_D(const DuiMenu);

	QRect backgroundRect = w->rect();

	QPixmap menuBackground;
 	switch( d->m_alienMenuType )
 	{
 	case DuiMenu::NormalMenu:
 		menuBackground = DuiTheme::getImage(KBackgroundImage);
 		break;
    case DuiMenu::MainMenu://<--YY 7.0将主菜单的背景下的图标去掉了，切换成普通菜单的效果。
        menuBackground = DuiTheme::getImage(KBackgroundImage);
  		break;
 	case DuiMenu::TrayIconMenu:
 		menuBackground = DuiTheme::getImage(KTrayIconMenuBackgroundImage);
 		break;
 	default:menuBackground = DuiTheme::getImage(KBackgroundImage);
 		break;
	}

	p->setRenderHint(QPainter::SmoothPixmapTransform);

	d->drawPixmap(p, backgroundRect, menuBackground);
}

void DuiMenu::drawPrimitiveFrameMenu(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(opt);
	DW_UNUSED(p);
	DW_UNUSED(w);
}

void DuiMenu::drawControlMenuItem(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(w);
	Q_D(const DuiMenu);

	const QStyleOptionMenuItem *menuitem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
	if ( !menuitem )
	{
		return;
	}

	QRect itemRect = menuitem->rect;
	//除去阴影的大小
	if ( d->m_alienMenuType == TrayIconMenu )
	{
		itemRect.adjust(KOffsetX-3, 0, -KOffsetX/2-1, 0);
	}
	else
	{
		itemRect.adjust(KOffsetX-1, 0, -KOffsetX/2-1, 0);
	}

	int x, y, width, height;
	itemRect.getRect(&x, &y, &width, &height);
	int tab = menuitem->tabWidth;
	bool dis = !(menuitem->state & QStyle::State_Enabled);
	bool checked = (menuitem->checkType != QStyleOptionMenuItem::NotCheckable) ? menuitem->checked : false;
	bool act = menuitem->state & QStyle::State_Selected;
	int checkcol = qMax<int>(menuitem->maxIconWidth, DuiCustomMenuStyle::windowsCheckMarkWidth);
	QRect vCheckRect = style()->visualRect(opt->direction, itemRect, QRect(itemRect.x(), itemRect.y(), checkcol, itemRect.height()));
	vCheckRect.adjust(0, 0, KOffsetX + KOffsetX/2, 0);

	QPixmap hoverBackgroundItemImage;
	if ( act )
	{
		if ( dis )
		{
			hoverBackgroundItemImage = DuiTheme::getImage( KDisableActionHoveredImage );
		}
		else
		{
			hoverBackgroundItemImage = DuiTheme::getImage( KActionHoveredImage );
		}
	}
	d->drawPixmap(p, itemRect, hoverBackgroundItemImage);

	if ( menuitem->menuItemType == QStyleOptionMenuItem::Separator )
	{			
		int yoff = y-1 + height/2;
		p->save();
		p->setPen(QColor(230, 237, 242));
		int x1 = x;
		int x2 = x1+width;
// 		if ( d->m_alienMenuType == DuiMenu::MainMenu )
// 		{
// 		    x1 += KMainMenuSepMarginLeft;
// 		    x2 -= KMainMenuSepMarginRight;
// 		}
// 		else
		{
            x1 += KMenuSepMarginLeft;
            x2 -= KMenuSepMarginRight;
		}
        p->drawLine(x1, yoff, x2, yoff);
        p->setPen(QColor(249, 252, 254));
        p->drawLine(x1, yoff + 1, x2, yoff + 1);

		p->restore();
	}

	if ( !menuitem->icon.isNull() )
	{
		d->drawLeftIcon(p, menuitem->icon, vCheckRect, opt, w);
	}
	else if ( checked ) 
	{
		QIcon checkIcon = DuiTheme::getImage(KMenuCheckedIcon);
		d->drawLeftIcon(p, checkIcon, vCheckRect, opt, w);
	}
	
	if (dis)
	{
		p->setPen(KMenuTextColorDisable);
	}else
	{
        p->setPen(KMenuTextColorNormal);	
	}

	int xm = int(DuiCustomMenuStyle::windowsItemFrame) + checkcol + int(DuiCustomMenuStyle::windowsItemHMargin);
	int xpos = itemRect.x() + xm;
	xpos += 2 * KOffsetX;

	QString s = menuitem->text;
	if (!s.isEmpty()) 
	{
		QRect textRect(xpos, y + DuiCustomMenuStyle::windowsItemVMargin, 
			width - xm - DuiCustomMenuStyle::windowsRightBorder - tab + 1, height - 2 * DuiCustomMenuStyle::windowsItemVMargin);
		QRect vTextRect = QStyle::visualRect(opt->direction, itemRect, textRect);
		
		p->save();
		int t = s.indexOf(QLatin1Char('\t'));
		int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;

		if ( !style()->styleHint(DuiCustomMenuStyle::SH_UnderlineShortcut, menuitem, w) )
		{
			text_flags |= Qt::TextHideMnemonic;
		}
		text_flags |= Qt::AlignLeft;

		if (t >= 0) 
		{
			QRect vShortcutRect = style()->visualRect(opt->direction, itemRect,
				QRect(textRect.topRight(), QPoint(itemRect.right(), textRect.bottom())));

			vShortcutRect.adjust(0, 0, KOffsetX, 0);

			if ( dis && !act && style()->styleHint(DuiCustomMenuStyle::SH_EtchDisabledText, opt, w) )
			{
				p->setPen(menuitem->palette.light().color());
				p->drawText(vShortcutRect.adjusted(1,1,1,1), text_flags, s.mid(t + 1));
				p->setPen(KMenuTextColorDisable);
			}
			p->drawText(vShortcutRect, text_flags, s.mid(t + 1));
			s = s.left(t);
		}

		QFont font = menuitem->font;
		//TrayIconMenu第一项文字的特殊处理
		if ( d->m_alienMenuType == TrayIconMenu )
		{
 			if ( itemRect.y() - menuitem->menuRect.y() == KTrayIconMenuFirstItemTop )
 			{
				vTextRect.adjust(KTrayIconMenuFirstItemTextOffsetX, 0, 0, 0);
			}
		}
 		if ( menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem )
 		{
			font.setBold(true);
		}

		p->setFont( font );
		if ( dis && !act && style()->styleHint(DuiCustomMenuStyle::SH_EtchDisabledText, opt, w) ) 
		{
			p->setPen( menuitem->palette.light().color() );
			p->drawText(vTextRect.adjusted(1, 1, 1, 1), text_flags, s.left(t));
			p->setPen(KMenuTextColorDisable);
		}
		p->drawText( vTextRect, text_flags, s.left(t) );
		p->restore();
	}

	if (menuitem->menuItemType == QStyleOptionMenuItem::SubMenu)
	{
		int dim = (height - 2 * DuiCustomMenuStyle::windowsItemFrame) / 2;
		DuiCustomMenuStyle::PrimitiveElement arrow;

		arrow = (opt->direction == Qt::RightToLeft) ? DuiCustomMenuStyle::PE_IndicatorArrowLeft : DuiCustomMenuStyle::PE_IndicatorArrowRight;
		xpos = x + width - DuiCustomMenuStyle::windowsArrowHMargin - DuiCustomMenuStyle::windowsItemFrame - dim;

		QRect  vSubMenuRect = style()->visualRect(opt->direction, itemRect, QRect(xpos, y + height / 2 - dim / 2, dim, dim));
		QPixmap arrowImage = DuiTheme::getImage( act ? KSecondMenuArrowHoveredImage : KSecondMenuArrowNormalImage );
		QSize arrowSize = arrowImage.size();

		vSubMenuRect.adjust( 2 * KOffsetX, 0, 0, 0 );
		vSubMenuRect.setSize(arrowSize);
		p->drawPixmap(vSubMenuRect, arrowImage);
	}

  	if ( d->m_alienMenuType == TrayIconMenu )
  	{
  		QPixmap foreground = DuiTheme::getImage(KTrayIconMenuForeground);
  		d->drawPixmap(p, w->rect(), foreground);
  	}
}

void DuiMenu::drawControlMenuScroller(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(p);
	DW_UNUSED(opt);
	DW_UNUSED(w);
}

void DuiMenu::drawControlMenuTearoff(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(p);
	DW_UNUSED(opt);
	DW_UNUSED(w);
}

void DuiMenu::drawControlMenuEmptyArea(QPainter *p, const QStyleOption *opt, const QWidget *w)const
{
	DW_UNUSED(p);
	DW_UNUSED(opt);
	DW_UNUSED(w);
}