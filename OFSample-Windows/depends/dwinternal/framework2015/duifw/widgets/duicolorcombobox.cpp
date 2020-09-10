#include <QGraphicsRectItem>
#include "duifw/duilineedit.h"
#include "duicolorcombobox.h"
#include "itemviews/duilistwidget.h"

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiColorComboBox)
DuiColorComboBox::DuiColorComboBox(QGraphicsItem* parent/* = NULL*/)
:DuiComboBox(parent)
{
	addItemPrototype(new DuiListColorItem());
	lineEdit()->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
}

DuiColorComboBox::~DuiColorComboBox()
{

}

void DuiColorComboBox::setSelectedItem( QStandardItem* item )
{
	if ( item != NULL )
	{
		if ( !item->text().isEmpty() )
		{
			uint uColor = item->text().toUInt(0,16);

			int b = uColor & 0x000000FF;
			int g = ( uColor & 0x0000FF00 ) >> 8;
			int r = ( uColor & 0x00FF0000 ) >> 16;

			this->lineEdit()->setText("");
			this->lineEdit()->setBackgroundColor(QColor(r,g,b));
		}
	}
}
