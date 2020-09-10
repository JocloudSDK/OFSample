#include "stable.h"

#include "duifw.h"
#include "duiwidget_p.h"
#include "duiwidget.h"
#include "duitheme.h"
#include "duifw/duiapplication.h"
#include "duifwstyle.h"

//------------------------------------------------------------------------------
namespace
{
	class DuiWidgetXmlPathMgr
	{
	public:
		void addWidgetXmlPath(DuiWidget *widget, const QString &path);
		QString getWidgetXmlPath(const DuiWidget *widget) const;
		void removeWidgetXmlPath(DuiWidget* widget);

	private:
		QMap<DuiWidget*, QString> m_widget2Path;
	};

	void DuiWidgetXmlPathMgr::addWidgetXmlPath(DuiWidget *widget, const QString &path)
	{
		if (m_widget2Path.contains(widget))
		{
			DW_ASSERT_X(false, __FUNCTION__, QString("widget %1 already exist.").arg(widget->objectName()).toLatin1().constData());
		}

		m_widget2Path.insert(widget, path);
	}

	QString DuiWidgetXmlPathMgr::getWidgetXmlPath(const DuiWidget *widget) const
	{
		return m_widget2Path.value(const_cast<DuiWidget*>(widget));
	}

	void DuiWidgetXmlPathMgr::removeWidgetXmlPath(DuiWidget* widget)
	{
		m_widget2Path.remove(widget);
	}

	Q_GLOBAL_STATIC(DuiWidgetXmlPathMgr, _dw_GetWidgetXmlPathMgr);

	void _dw_SetAnchor(DuiWidget *widget, DuiWidget *anchorWidget, Qt::AnchorPoint from, Qt::AnchorPoint to, int distance)
	{
		DW_ASSERT_X(widget, __FUNCTION__, "widget is null");
		DW_ASSERT_X(anchorWidget, __FUNCTION__, "anchorWidget is null");
		if (!widget || !anchorWidget)
		{
			return;
		}

		DuiWidget* p = qobject_cast<DuiWidget*>(widget->parent());
		DW_ASSERT_X(p, __FUNCTION__, "parent is null");
		DW_ASSERT_X(KDuiIsEditorMode || p->layoutType() == DuiWidget::Anchor, __FUNCTION__, "layout type should be anchor");
        if (KDuiIsEditorMode && p->layoutType() != DuiWidget::Anchor)
            return ;
		QGraphicsLayout *layout = p->layout();

		QGraphicsAnchorLayout *anchorLayout = static_cast<QGraphicsAnchorLayout*>(layout);
		if (anchorLayout == NULL)
		{
			DW_ASSERT_X(false, __FUNCTION__, "layout is not group layout");
			return ;
		}

		QGraphicsAnchor *anchor = NULL;
		if (anchorWidget == p)
		{
			anchor = anchorLayout->addAnchor(widget, from, anchorLayout, to);
		}
		else
		{
			anchor = anchorLayout->addAnchor(widget, from, anchorWidget, to);
		}
		if (anchor != NULL)
		{
			anchor->setSpacing(distance);
		}
		else
		{
			DW_ASSERT_X(false, __FUNCTION__, "anchor is invalide");
		}
	}
}

//------------------------------------------------------------------------------
DuiWidgetPrivate::DuiWidgetPrivate()
: m_dropFlag(DuiWidget::DragWithParentFlag)
, m_colorizeFlag(DuiWidget::FollowParent)
, m_themeColorType(DuiTheme::InvalidColor)
, m_layoutType(DuiWidget::None)
, m_splitPolicy(QSizePolicy::Expanding)
, m_isAutoWidth(false)
, m_isAutoHeight(false)
, m_isAnchorSetted(false)
, m_constructed(false)
, m_backgroundColor(0, 0, 0, 0)
, m_anchorInfo(NULL)
, m_sizeCache(NULL)
{
}

DuiWidgetPrivate::~DuiWidgetPrivate()
{
	DW_SAFE_DELETE(m_anchorInfo);
    DW_SAFE_DELETE(m_sizeCache);
}

void DuiWidgetPrivate::updateSize()
{
	Q_Q(DuiWidget);
	if (q->autoWidth())
	{
		q->setPreferredWidth(calcAutoWidth());
	}
    if (q->autoHeight())
    {
        q->setPreferredHeight(calcAutoHeight());
    }
}

int DuiWidgetPrivate::calcAutoWidth()
{
    DW_ASSERT(KDuiIsEditorMode); // not support
    return 0;
}

int DuiWidgetPrivate::calcAutoHeight()
{
    DW_ASSERT(KDuiIsEditorMode); // not support
    return 0;
}

void DuiWidgetPrivate::graphicsColorize( QRgb color )
{
	Q_Q( DuiWidget );

	QRgb colorizeColor;
	if ( m_themeColorType != DuiTheme::InvalidColor )
	{
		colorizeColor = DuiTheme::getColor( m_themeColorType );
	}
	else
	{
		colorizeColor = color;
	}

	QGraphicsColorizeEffect* effect = qobject_cast<QGraphicsColorizeEffect* >( q->graphicsEffect() );
	if ( effect == NULL )
	{
		effect = new QGraphicsColorizeEffect( q );
		q->setGraphicsEffect( effect );
	}

	effect->setColor( colorizeColor );
}

void DuiWidgetPrivate::init()
{
	Q_Q(DuiWidget);
	q->setContentsMargins(0, 0, 0, 0);

	static int s_index = 0;
	QString name = QString("NoNameWidget%1").arg(s_index++);
	q->setObjectName(name);
	q->setFocusPolicy(Qt::ClickFocus);
}

//------------------------------------------------------------------------------
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiWidget)
DuiWidget::DuiWidget( QGraphicsItem *parent /* = NULL*/ )
: DuiWidgetBase(*new DuiWidgetPrivate(), parent) 
{
	d_func()->init();
}

DuiWidget::DuiWidget( DuiWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ ) 
: DuiWidgetBase(dd, parent)
{
	d_func()->init();
}

DuiWidget::~DuiWidget()
{
	_dw_GetWidgetXmlPathMgr()->removeWidgetXmlPath(this);
}

int DuiWidget::height() const
{
	return static_cast<int>(size().height());
}

void DuiWidget::setHeight(int h)
{
	QSizeF s = size();
	s.setHeight(h);

	resize(s);
}

int DuiWidget::width() const
{
	// Rounds value to the nearest integer.
	return qRound((size().width()));
}

void DuiWidget::setWidth(int w)
{
	QSizeF s = size();
	s.setWidth(w);

	resize(s);
}

int DuiWidget::spacing() const
{
	return 0;
}

void DuiWidget::setSpacing(int s)
{
	QGraphicsLayout *lay = NULL;
	switch (layoutType())
	{
	case VBox:
	case HBox:
		{
			QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>(layout());
			if (l != NULL)
			{
				l->setSpacing(s);
			}
			lay = l;
		}
		break;
	case Anchor:
		{
			QGraphicsAnchorLayout* l = static_cast<QGraphicsAnchorLayout*>(layout());
			if (l != NULL)
			{
				l->setSpacing(s);
			}
			lay = l;
		}
		break;
	default:
		DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "layout is null.");
		break;
	}
	DW_ASSERT_X(lay || KDuiIsEditorMode, __FUNCTION__, "no layout, no spacing");
}

int DuiWidget::itemSpacing() const
{
	return 0;
}

void DuiWidget::setItemSpacing(int s)
{
	DuiWidget* p = qobject_cast<DuiWidget*>( parent() );
	if ( p != NULL && p->layout() != NULL)
	{
		if (p->layoutType() != VBox &&
			p->layoutType() != HBox )
		{
			DW_ASSERT_X(false, __FUNCTION__, "only VBox, HBox can use itemSpacing");
		}
		else
		{
			QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( p->layout() );
			for (int i = 0; i < l->count(); i++)
			{
				if (l->itemAt(i) == this)
				{
					l->setItemSpacing(i, s);
				}
			}
		}
	}
	else
	{
		DW_ASSERT_X(false, __FUNCTION__, "no layout, no itemSpacing");
	}
}

Qt::Alignment DuiWidget::alignment() const
{
	return Qt::AlignCenter;
}

void DuiWidget::setAlignment(Qt::Alignment align)
{
	DuiWidget* p = static_cast<DuiWidget*>(parent());
	if (p != NULL && p->layout() != NULL)
	{
		if (p->layoutType() != HBox &&
			p->layoutType() != VBox)
		{
			DW_ASSERT_X(false, __FUNCTION__, "only VBox, HBox can use Alignment");
		}
		else
		{
			QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>(p->layout());
			l->setAlignment(this, align);
		}
	}
}

QSizeF DuiWidget::fixSize() const
{
	return size();
}

void DuiWidget::setFixSize(const QSizeF &s)
{
	setMaximumSize(s);
	setMinimumSize(s);
	setPreferredSize(s);
}

int DuiWidget::fixHeight() const
{
	return size().height();
}

void DuiWidget::setFixHeight(int h)
{
	setMaximumHeight(h);
	setMinimumHeight(h);
	setPreferredHeight(h);
}

int DuiWidget::fixWidth() const
{
	return size().width();
}

void DuiWidget::setFixWidth(int w)
{
	setMaximumWidth(w);
	setMinimumWidth(w);
	setPreferredWidth(w);
}

QMargins DuiWidget::margins() const
{
	if (layout())
	{
		qreal left = 0;
		qreal top = 0;
		qreal right = 0;
		qreal bottom = 0;
		layout()->getContentsMargins(&left, &top, &right, &bottom);
		return QMargins(left, top, right, bottom);
	}

	return QMargins();
}

void DuiWidget::setMargins(const QMargins& m)
{
	DW_ASSERT_X(KDuiIsEditorMode || layout(), __FUNCTION__, "no layout");
	if (layout())
	{
		layout()->setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
	}
}

void DuiWidget::doConstruct()
{
	Q_D(DuiWidget);
	if (d->m_constructed)
	{
		return ;
	}

	d->m_constructed = true;
	QString xml = xmlPath();
	if (!xml.isEmpty() && DuiEditorMode != KDuiCurrentEditorMode)
	{
		QVariant v = property(KRootPathProperty);
		if (v.isValid())
		{
			xml = v.toString() + xml;
		}
		DuiXmlUI::createFromFile(xml, this);
	}
	construct();  

}

void DuiWidget::construct()
{
    if (layoutType() != Anchor)
    {
        return;
    }

    QList<DuiWidget*> widgetList = qFindChildren<DuiWidget*>(this);
    foreach(DuiWidget* widget, widgetList)
    {
        widget->delaySetAnchor();
    }
}

DuiWidget *DuiWidget::getXmlNode()
{
	return this;
}

void DuiWidget::addItem(DuiWidget* item)
{
	insertItem(item,-1);
}

int DuiWidget::insertItem(DuiWidget* item,int index)
{
	switch (layoutType())
	{
	case VBox:
	case HBox:
		{
			QGraphicsLinearLayout* parentLayout = static_cast<QGraphicsLinearLayout*>(layout());
			DW_ASSERT_X(parentLayout, __FUNCTION__, "layout is null, can't add child.");
			parentLayout->insertItem(index,item);
		}
		break;
	case Anchor:
		{
			QGraphicsAnchorLayout *parentLayout = static_cast<QGraphicsAnchorLayout*>(layout());
			DW_ASSERT_X(parentLayout, __FUNCTION__, "layout is null, can't add child.");
			//anchor layout can't add item here, add item in setAnchor
		}
		break;
	default:
		DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "layout error!");
		break;
	}
	return 0;
}

void DuiWidget::removeItem(DuiWidget *item)
{
	switch (layoutType())
	{
	case VBox:
	case HBox:
		{
			QGraphicsLinearLayout* parentLayout = static_cast<QGraphicsLinearLayout*>(layout());
			DW_ASSERT_X(parentLayout, __FUNCTION__, "layout is null, can't add child.");
			parentLayout->removeItem(item);
		}
		break;
	case Anchor:
		{
			QGraphicsAnchorLayout *parentLayout = static_cast<QGraphicsAnchorLayout*>(layout());
			DW_ASSERT_X(parentLayout, __FUNCTION__, "layout is null, can't add child.");
			DW_ASSERT_X(false, __FUNCTION__, "anchor layout can't remove item now!");
		}
		break;
	default:
		DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "layout error!");
		break;
	}
}

void DuiWidget::setLayout(QGraphicsLayout *layout)
{
	PerfLogF(F_DUI);
	if (KDuiIsEditorMode) // reparent child items in editor mode
	{
		switch (layoutType())
		{
		case VBox:
		case HBox: { ;
			QGraphicsLinearLayout* linearLayout = static_cast<QGraphicsLinearLayout*>(layout);
			DW_ASSERT_X(linearLayout, __FUNCTION__, "layout is null.");
			QGraphicsLayout *oldLayout = DuiWidget::layout();
			if (!oldLayout)
                break;
			for (int i = 0; i < oldLayout->count(); ++ i)
			{
				QGraphicsLayoutItem *item = oldLayout->itemAt(i);
				DuiWidget *w = static_cast<DuiWidget *>(item);
				linearLayout->addItem(w);
			}
			break;
			; }
		case Anchor: { ;
			QGraphicsAnchorLayout *anchorLayout = static_cast<QGraphicsAnchorLayout*>(layout);
			DW_ASSERT_X(anchorLayout, __FUNCTION__, "layout is null.");

			QGraphicsLayout *oldLayout = DuiWidget::layout();
			if (!oldLayout)
                break;
			for (int i = 0; i < oldLayout->count(); ++ i)
			{
				QGraphicsLayoutItem *item = oldLayout->itemAt(i);
				DuiWidget *w = static_cast<DuiWidget *>(item);
				AnchorInfo anchorInfo = w->anchor();

				Qt::AnchorPoint dummy[] = {Qt::AnchorLeft, Qt::AnchorRight, Qt::AnchorTop, Qt::AnchorBottom};
				for (int i = 0; i < 4; i ++)
				{
					QGraphicsAnchor *anchor = anchorLayout->addAnchor(w, 
						dummy[i],
						anchorLayout,
						anchorInfo.anchorPoint[i]);
					DW_ASSERT(anchor);
					anchor->setSpacing(anchorInfo.distance[i]);
				}
			}
			break;
			; }
		default:
			DW_ASSERT_X(false, __FUNCTION__, "layout error!");
			break;
		}
	}
	QGraphicsWidget::setLayout(layout);
	setMargins(QMargins(0, 0, 0, 0));
	setSpacing(0);
}

QGraphicsLayout *DuiWidget::layout() const
{
	return QGraphicsWidget::layout();
}

QColor DuiWidget::backgroundColor() const
{
	Q_D(const DuiWidget);
	return d->m_backgroundColor;
}

void DuiWidget::setBackgroundColor( QColor backgroundColor )
{
	Q_D(DuiWidget);

	if(d->m_backgroundColor == backgroundColor)
	{
		return;
	}

	d->m_backgroundColor = backgroundColor;

	//when the background color changed.
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, backgroundColor);
	setPalette(p);
}

QString DuiWidget::toolTip() const
{
	return d_func()->m_toolTipText;
}

void DuiWidget::setToolTip( const QString& toolTip )
{
	d_func()->m_toolTipText = toolTip;
}

void DuiWidget::setDragPolicy( DragWidgetFlag flag )
{
	d_func()->m_dropFlag = flag;
}

DuiWidget::DragWidgetFlag DuiWidget::dragPolicy() const
{
	return d_func()->m_dropFlag;
}

void DuiWidget::colorize(QRgb color)
{
	ColorizeWidgetFlag flag = colorizePolicy();
	if (flag == ColorableFlag)
	{
		doColorize( color );
	}
	else if (flag == UnColorableFlag)
	{
		setGraphicsEffect(NULL);
	}	
}

void DuiWidget::doColorize( QRgb color )
{
	Q_D( DuiWidget );

	d->graphicsColorize( color );
}

void DuiWidget::focusInEvent(QFocusEvent *event)
{
	DuiWidgetBase::focusInEvent(event);
	duiUpdateAccessibility(this, AccFocus);
}

void DuiWidget::duiUpdateAccessibility(DuiWidget *duiWidget, AccEvent reason)
{
	if (QAccessible::isActive() && duiWidget->scene() && duiWidget->graphicsView())
	{
		int indexInScene = (duiWidget->scene()->items()).indexOf(duiWidget);
		switch (reason)
		{
		case AccFocus:
		case AccSelection:
		case AccStateChanged:
		case AccLocationChanged:
			QAccessible::updateAccessibility(duiWidget->graphicsView(), indexInScene + 1, QAccessible::Focus);
			break;
		default:
			break;
		}
		
	}
}

void DuiWidget::setXmlPath(const QString& xmlPath)
{
	_dw_GetWidgetXmlPathMgr()->addWidgetXmlPath(this, xmlPath);
}

QString DuiWidget::xmlPath() const
{
	return _dw_GetWidgetXmlPathMgr()->getWidgetXmlPath(this);
}

void DuiWidget::setColorizePolicy( ColorizeWidgetFlag flag )
{
	d_func()->m_colorizeFlag = flag;
	//colorize(DuiTheme::getColor(DuiTheme::BaseColor));
}

DuiWidget::ColorizeWidgetFlag DuiWidget::colorizePolicy() const
{
	return d_func()->m_colorizeFlag;
}

int DuiWidget::themeColorType() const
{
	return (int)d_func()->m_themeColorType;
}

void DuiWidget::setThemeColorType( int colorType )
{
	d_func()->m_themeColorType = (DuiTheme::ColorType)colorType;
}

void DuiWidget::setAnchor(const AnchorInfo &data)
{
	AnchorInfo2 data2(data);
	setAnchor2(data2);
}

// TODO : remove this in yy5.x
AnchorInfo DuiWidget::anchor() const
{
	return AnchorInfo();
}

AnchorInfo2 DuiWidget::anchor2() const
{
	Q_D(const DuiWidget);
	if(d->m_anchorInfo)
	{
		return *(d->m_anchorInfo);
	}

	return AnchorInfo2();
}
//isDynamic: true( dynamic create object),false(default xml create object)
void DuiWidget::setAnchor2(const AnchorInfo2 &data ,bool isDynamic/*=false*/)
{
	Q_D(DuiWidget);
    d->m_isAnchorSetted = false;
	if( !d->m_anchorInfo )
	{
		d->m_anchorInfo = new AnchorInfo2();
	}

    *(d->m_anchorInfo) = data;
    if (canSetAnchor())
    {
        delaySetAnchor();
    }
}

DuiWidget::LayoutType DuiWidget::layoutType() const
{
	Q_D(const DuiWidget);
	return d->m_layoutType;
}

void DuiWidget::setLayout(LayoutType type)
{
	Q_D(DuiWidget);
	QGraphicsLayout *layout = NULL;
	switch (type)
	{
	case VBox:
		layout = new QGraphicsLinearLayout(Qt::Vertical);
		d->m_layoutType = VBox;
		break;
	case HBox:
		layout = new QGraphicsLinearLayout(Qt::Horizontal);
		d->m_layoutType = HBox;
		break;
	case Anchor:
		layout = new QGraphicsAnchorLayout();
		d->m_layoutType = Anchor;
		break;
	default:
		DW_ASSERT_X(false, __FUNCTION__, "unknown layout!");
		return ;
	}

	setLayout(layout);
}

void DuiWidget::setDisableStatus()
{
	QGraphicsColorizeEffect* effect = qobject_cast<QGraphicsColorizeEffect* >( graphicsEffect() );
	if ( effect == NULL )
	{
		effect = new QGraphicsColorizeEffect( this );
		effect->setColor( Qt::gray );
		setGraphicsEffect( effect );
	}
}

DuiWidget::DragWidgetFlag DuiWidget::hitTest( const QPointF& point ) const
{
	Q_UNUSED(point);
	return d_func()->m_dropFlag;
}

QSizePolicy::Policy DuiWidget::splitPolicy() const
{
	Q_D(const DuiWidget);
	return d->m_splitPolicy;
}

void DuiWidget::setSplitPolicy( QSizePolicy::Policy policy)
{
	Q_D(DuiWidget);
	d->m_splitPolicy = policy;
}

bool DuiWidget::createFromFile(const QString &fileName)
{
	return DuiXmlUI::createFromFile(fileName, this);
}

QString DuiWidget::styleId()const
{
	Q_D(const DuiWidget);
	return d->m_styleId;
}

void DuiWidget::setStyleId(const QString& styleId)
{
	Q_D(DuiWidget);
	if (d->m_styleId != styleId)
	{
		d->m_styleId = styleId;
		duiGetStyleManager()->applyStyleInfo(this, d->m_styleId);
	}
}
void DuiWidget::setUuid(const QString &uuid)
{
	d_func()->m_uuid = uuid;
}

QString DuiWidget::uuid() const
{
	return d_func()->m_uuid;
}

void  DuiWidget::setAutoWidth(bool isAutoWidth)
{
	Q_D( DuiWidget);
	if (autoWidth() == isAutoWidth)
        return ;
	if(isAutoWidth)
	{
        QSizePolicy sp = sizePolicy();
		setSizePolicy(QSizePolicy::Maximum, sp.verticalPolicy());
	}
	else
	{
		DW_ASSERT(0); // not support dynamic alter
	}

	d->m_isAutoWidth = isAutoWidth;
	d->updateSize();
}

bool DuiWidget::autoWidth() const
{
	Q_D(const DuiWidget);
	return d->m_isAutoWidth;
}

void  DuiWidget::setAutoHeight(bool isAutoHeight)
{
    Q_D(DuiWidget);
    if (autoHeight() == isAutoHeight)
        return;
    if (isAutoHeight)
    {
        QSizePolicy sp = sizePolicy();
        setSizePolicy(sp.horizontalPolicy(), QSizePolicy::Maximum);
    }
    else
    {
        DW_ASSERT(0); // not support dynamic set
    }
    d->m_isAutoHeight = isAutoHeight;
    d->updateSize();
}

bool DuiWidget::autoHeight() const
{
	Q_D(const DuiWidget);
	return d->m_isAutoHeight;
}

void DuiWidget::setAnchorString(const QString &anchorString)
{
	int index = DuiWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
	QMetaEnum enu = DuiWidget::staticMetaObject.enumerator(index);
	QStringList list = anchorString.split('|');
	DW_ASSERT_X(list.count()==4, __FUNCTION__, QString("%1 is not valide anchor info").arg(anchorString).toLatin1().constData());
	AnchorInfo info;
	for (int i = 0; i < list.count() && i < 4; ++i)
	{
		QStringList innerList = list[i].split(':');
		DW_ASSERT_X(innerList.count()==2, __FUNCTION__, QString("%1 is not valide anchor info").arg(anchorString).toLatin1().constData());
		if (innerList.count() >= 2)
		{
			int value = enu.keyToValue(innerList[0].toLatin1().constData());
			DW_ASSERT_X(value != -1, __FUNCTION__, (innerList[0] + "is not a valid enum value").toLatin1().constData());
			if (value == -1)
			{
				value = 0;
			}
			info.anchorPoint[i] = (Qt::AnchorPoint)value;
			info.distance[i] = innerList[1].toInt();
		}
	}
	setAnchor(info);
}

QString DuiWidget::anchorString() const
{
    return QString();
}

bool DuiWidget::canSetAnchor()
{
    Q_D(DuiWidget);
    if (!d->m_anchorInfo || d->m_isAnchorSetted)
        return true;

    for (int i = 0; i < d->m_anchorInfo->anchorNum; i++)
    {
        DW_ASSERT(parent());
        const QString &objectName = d->m_anchorInfo->objectName[i];
        DuiWidget *w = 0;
        if (objectName.isNull()         ||
            KAnchorParent == objectName ||
            objectName == parent()->objectName())
        {
            continue;
        }
        else
        {
            w = parent()->findChild<DuiWidget*>(d->m_anchorInfo->objectName[i]);
            if (!w)
            {
                return false;
            }
        }
    }
    return true;
}

void DuiWidget::delaySetAnchor()
{
    Q_D(DuiWidget);
    if (!d->m_anchorInfo || d->m_isAnchorSetted)
        return ;

    d->m_isAnchorSetted = true;
    for (int i = 0; i < d->m_anchorInfo->anchorNum; i++)
    {
        DW_ASSERT(parent());
        const QString &objectName = d->m_anchorInfo->objectName[i];
        DuiWidget *w = 0;
        if (objectName.isEmpty()         ||
            KAnchorParent == objectName ||
            objectName == parent()->objectName())
        {
            w = qobject_cast<DuiWidget *>(parent());
        }
        else
        {
            w = parent()->findChild<DuiWidget*>(d->m_anchorInfo->objectName[i]);
        }
        DW_ASSERT(w);
        ::_dw_SetAnchor(this, 
            w, 
            d->m_anchorInfo->point[i],
            d->m_anchorInfo->anchorPoint[i], 
            d->m_anchorInfo->distance[i]);
    }
}

QWidget * DuiWidget::topQWidget() const
{
	return graphicsView();	
}

QGraphicsView * DuiWidget::graphicsView() const
{
	QGraphicsScene *s = scene();
	if(s)
	{
		QList<QGraphicsView*> views = s->views();
		if(views.count() > 0)
		{
			return views[0];
		}
	}

	//DW_ASSERT(false);
	return NULL;

}

QPoint DuiWidget::mapToGlobal(const QPoint & pos) const
{
	QGraphicsView *view = graphicsView();
	if (!view)
	{
		return QPoint();
	}
	QPointF scenePointF = mapToScene(pos);
	QPoint viewPoint = view->mapFromScene(scenePointF);
	QPoint globalPoint = view->mapToGlobal(viewPoint);
	return QPoint(globalPoint.x(), globalPoint.y());
}

void DuiWidget::setVisible2(bool visible)
{
    QGraphicsItem::setVisible(visible);
    Q_D(DuiWidget);
    if (!visible)
    {
        if (!d->m_sizeCache)
        {
            d->m_sizeCache = new DuiWidgetPrivate::SizeCache;
        }
        QSizeF minimum = minimumSize();
        QSizeF preferred = preferredSize();
        QSizeF maximum = maximumSize();
        if (minimum != QSizeF(0, 0)   ||
            preferred != QSizeF(0, 0) ||
            maximum != QSizeF(0, 0))
        {
            d->m_sizeCache->minimumSizeCache = minimum;
            d->m_sizeCache->perferedSizeCache = preferred;
            d->m_sizeCache->maximumSizeCache = maximum;
            setFixSize(QSizeF(0, 0));
        }
    }
    else
    {
        if (d->m_sizeCache)
        {
            setMinimumSize(d->m_sizeCache->minimumSizeCache);
            setPreferredSize(d->m_sizeCache->perferedSizeCache);
            setMaximumSize(d->m_sizeCache->maximumSizeCache);
        }
    }
}

// name description help of blinder read ui 
QString DuiWidget::accessibleName() const
{
	Q_D(const DuiWidget);
	return d->m_accessibleName;
}

void DuiWidget::setAccessibleName(const QString &name)
{
	Q_D(DuiWidget);
	d->m_accessibleName = name;
}

QString DuiWidget::accessibleDescription() const
{
	Q_D(const DuiWidget);
	return d->m_accessibleDescription;
}

void DuiWidget::setAccessibleDescription( const QString & description )
{
	Q_D(DuiWidget);
	d->m_accessibleDescription = description;
}

QString DuiWidget::whatsThis() const
{
	Q_D(const DuiWidget);
	return d->m_whatsThis;
}

void DuiWidget::setWhatsThis(const QString &s)
{
	Q_D(DuiWidget);
	d->m_whatsThis = s;
}