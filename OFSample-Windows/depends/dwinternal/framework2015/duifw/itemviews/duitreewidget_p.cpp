#include "stable.h"
#include "duitreewidget_p.h"
#include "duiscrollbar.h"

DuiTreeWidgetPrivate::DuiTreeWidgetPrivate()
: m_itemContainer( NULL )
{
}

DuiTreeWidgetPrivate::~DuiTreeWidgetPrivate()
{
}

void DuiTreeWidgetPrivate::init()
{
	Q_Q(DuiTreeWidget);

	m_itemContainer = new DuiTreeItemContainer(q);
	//以下两句用于自动化测试，请勿删
	m_itemContainer->setParent(q);
	m_itemContainer->setObjectName("selectPageButtonListContainer");

	q->setFocusPolicy(Qt::StrongFocus);
	q->setContentWidget(m_itemContainer);
	m_scrollBar->setSingleStep(40);
	q->connect(q, SIGNAL(scrollPosChanged(int)), q, SLOT(on_scrollPosChanged(int)));
	q->connect(m_itemContainer, SIGNAL(folderLeftButtonClicked(const QModelIndex &)), q, SIGNAL(folderLeftButtonClicked(const QModelIndex &)));
	q->connect(m_itemContainer, SIGNAL(leftButtonClicked(const QModelIndex &)), q, SIGNAL(leftButtonClicked(const QModelIndex &)));
	q->connect(m_itemContainer, SIGNAL(rightButtonClicked(const QModelIndex &)), q, SIGNAL(rightButtonClicked(const QModelIndex &)));
	q->connect(m_itemContainer, SIGNAL(leftButtonDoubleClicked(const QModelIndex &)), q, SIGNAL(leftButtonDoubleClicked(const QModelIndex &)));
	q->connect(m_itemContainer, SIGNAL(beginDrag(QWidget*)), q, SLOT(on_containerDrag(QWidget*)) );
	q->connect(m_itemContainer, SIGNAL(canSelectInMultiSectionMode(const QModelIndex &,bool &)), q, SIGNAL(canSelectInMultiSectionMode(const QModelIndex &,bool &)));
	q->connect( m_itemContainer, SIGNAL( pinItemUpdated( DuiTreeViewItem* ) ), SIGNAL( pinItemUpdated( DuiTreeViewItem* ) ) );

	q->setDragPolicy( DuiWidget::NotDragWindow );
}

void DuiTreeWidgetPrivate::setModel( QStandardItemModel* model )
{
	m_itemContainer->setModel(model);
}

QStandardItemModel* DuiTreeWidgetPrivate::model() const
{
	return m_itemContainer->model();
}
