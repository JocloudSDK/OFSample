#include "stable.h"

#include "qtextend/qtreeviewex.h"
#include "duiwidget.h"
#include "duitreeview.h"
#include "duiwidget_p.h"
#include "duifw/duiframewindow.h"
#include "duifw/duiproxywidget.h"

static void initStyle(const QString &styleFile, QWidget *widget)
{
	QFile file(styleFile);

	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		widget->setStyleSheet(QTextStream(&file).readAll());
	}
	else
	{
		DW_ASSERT_X(false, "initStyle", "Load stylesheet file fail.");
	}
}

class DuiTreeViewPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiTreeView)

public:
	DuiTreeViewPrivate();
	~DuiTreeViewPrivate();

	QPointer<QTreeViewEx> m_treeview;
	DuiProxyWidget *m_proxy;

	void init();
	void initDefaultStyle();
};

DuiTreeViewPrivate::DuiTreeViewPrivate()
{
}

DuiTreeViewPrivate::~DuiTreeViewPrivate()
{

}

void DuiTreeViewPrivate::init()
{
	Q_Q( DuiTreeView );

	q->setColorizePolicy( DuiWidget::ColorableFlag );
	q->setFocusPolicy(Qt::TabFocus);
	q->setDragPolicy(DuiWidget::NotDragWindow);
	
	m_treeview = new QTreeViewEx;

	QScrollBar *srollBar = m_treeview->verticalScrollBar();
	srollBar->setContextMenuPolicy(Qt::NoContextMenu);

	srollBar = m_treeview->horizontalScrollBar();
	srollBar->setContextMenuPolicy(Qt::NoContextMenu);

	m_treeview->setFrameShape(QFrame::NoFrame);
	m_treeview->setRootIsDecorated(false);
	m_treeview->setAlternatingRowColors(false);
	m_treeview->setAllColumnsShowFocus(true);
	m_treeview->setFocusPolicy(Qt::ClickFocus);
	m_treeview->header()->setMovable( false );

	q->connect( m_treeview, SIGNAL(contextMenu(const QModelIndex&)), SIGNAL(contextMenu(const QModelIndex&)) );
	q->connect( m_treeview, SIGNAL(clicked(const QModelIndex&)), SIGNAL(clicked(const QModelIndex&)) );
	q->connect( m_treeview, SIGNAL(doubleClicked(const QModelIndex&)), SIGNAL(doubleClicked(const QModelIndex&)) );
	q->connect( m_treeview, SIGNAL(pressed(const QModelIndex&)), SIGNAL(pressed(const QModelIndex&)) );
	q->connect( m_treeview, SIGNAL(activated(const QModelIndex&)), SIGNAL(activated(const QModelIndex&)) );
	q->connect( m_treeview, SIGNAL(clicked()), SIGNAL(clicked()) );
	q->connect( m_treeview, SIGNAL(currentIndexChanged(const QModelIndex&)), SLOT(on_currentIndexChanged(const QModelIndex&)));

	m_proxy = new DuiProxyWidget(q);
	m_proxy->setInheritClip(true);
	m_proxy->setWidget(m_treeview);

	initDefaultStyle();
}

void DuiTreeViewPrivate::initDefaultStyle()
{
	initStyle(":/theme/duifw/css/tree_view_7.x.css", m_treeview);
	initStyle(":/theme/duifw/css/scrollbar_7.x.css", m_treeview->verticalScrollBar());
	initStyle(":/theme/duifw/css/scrollbar_h_7.x.css", m_treeview->horizontalScrollBar());
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiTreeView)

DuiTreeView::DuiTreeView( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget( *(new DuiTreeViewPrivate()), parent )
{
	d_func()->init();
}

DuiTreeView::DuiTreeView( DuiTreeViewPrivate &dd, QGraphicsItem *parent /*= NULL */ )
: DuiWidget( dd, parent )
{
	d_func()->init();
}

DuiTreeView::~DuiTreeView()
{
}

void DuiTreeView::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D(DuiTreeView);
	const QRectF rc(QPointF(0, 0), event->newSize());
	d->m_proxy->setGeometry(rc);
}

void DuiTreeView::setModel( QAbstractItemModel *model )
{
	if ( d_func()->m_treeview )
	{
		d_func()->m_treeview->setModel( model );
	}
}

QAbstractItemModel* DuiTreeView::model() const
{
	return d_func()->m_treeview ? d_func()->m_treeview->model() : NULL;
}

void DuiTreeView::setStyleSheet( const QString& styleSheet )
{
	if ( d_func()->m_treeview )
	{
		d_func()->m_treeview->setStyleSheet( styleSheet );
	}
}

QString DuiTreeView::styleSheet() const
{
	return d_func()->m_treeview->styleSheet();
}

QHeaderView* DuiTreeView::header() const
{
	return d_func()->m_treeview->header();
}

QModelIndex DuiTreeView::currentIndex() const
{
	return d_func()->m_treeview->currentIndex();
}

void DuiTreeView::setHeaderHidden(bool hide)
{
	d_func()->m_treeview->setHeaderHidden(hide);
}

void DuiTreeView::doColorize( QRgb color )
{
	Q_UNUSED(color);
	d_func()->initDefaultStyle();
}

QTreeView* DuiTreeView::realTreeView() const
{
	return d_func()->m_treeview;
}

void DuiTreeView::setSelectionMode( QAbstractItemView::SelectionMode mode )
{
	d_func()->m_treeview->setSelectionMode( mode );
}

QAbstractItemView::SelectionMode DuiTreeView::selectionMode() const
{
	return d_func()->m_treeview->selectionMode();
}

QItemSelectionModel * DuiTreeView::selectionModel() const
{
	return d_func()->m_treeview->selectionModel();
}

void DuiTreeView::keyPressEvent(QKeyEvent *event)
{
	DuiWidget::keyPressEvent(event);
	emit keyPress(event);
}

void DuiTreeView::focusInEvent(QFocusEvent *event)
{
	if (event->reason() == Qt::TabFocus || event->reason() == Qt::BacktabFocusReason)
	{
		DuiWidget::focusInEvent(event);
		d_func()->m_treeview->setFocus();
	}
}

void DuiTreeView::moveEvent( QGraphicsSceneMoveEvent * event )
{
	DuiWidget::moveEvent(event);
}

QVariant DuiTreeView::itemChange( GraphicsItemChange change, const QVariant & value )
{
	return DuiWidget::itemChange(change, value);
}

void DuiTreeView::on_currentIndexChanged(const QModelIndex& current)
{
	Q_UNUSED(current);
	DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccSelection);
}
