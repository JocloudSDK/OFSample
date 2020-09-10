#include "stable.h"

#include "duirootitem.h"

DuiRootItem::DuiRootItem( QGraphicsItem *parent /* = NULL*/ )
	: DuiWidget(parent)
{
	setFlag(QGraphicsItem::ItemHasNoContents);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);

	setDragPolicy( DuiWidget::DragWindow );
}

DuiRootItem::~DuiRootItem( )
{
}

void DuiRootItem::setContentWidget( DuiWidgetBase *widget )
{
	if( !layout() )
	{
		setLayout(DuiWidget::VBox);
	}

	if( layout()->count() > 0 )
	{
		QGraphicsLayoutItem *itemToDelete = layout()->itemAt(0);
		layout()->removeAt(0);
		delete itemToDelete;
	}

	DW_ASSERT(layoutType() == DuiWidget::VBox);
	(static_cast<QGraphicsLinearLayout*>(layout()))->addItem( widget );
	widget->setParent(this);
}
