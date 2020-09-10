#include <QFontMetrics>
#include <QPixmap>

#include "stable.h"
#include "duiradiobutton.h"
#include "duiitemdrawers.h"
#include "duiwidget_p.h"
#include "duifw/duitheme.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiRadioButton)

DuiRadioButton::DuiRadioButton(QGraphicsItem *parent)
:DuiCheckBox(parent)
{
	init();
}

DuiRadioButton::DuiRadioButton(DuiWidgetPrivate &dd, QGraphicsItem* parent)
:DuiCheckBox(dd, parent)
{
	init();
}

void DuiRadioButton::init()
{
// 	setImage(":/theme/duifw/radiobox_selected_normal.png", 
// 		":/theme/duifw/radiobox_selected_hover.png", 
// 		":/theme/duifw/radiobox_selected_pressdown.png",
// 		":/theme/duifw/radiobox_selected_disable.png",	// CheckedDisable
// 		":/theme/duifw/radiobox_default_normal.png", 
// 		":/theme/duifw/radiobox_default_hover.png", 
// 		":/theme/duifw/radiobox_default_pressdown.png",
// 		":/theme/duifw/radiobox_default_disable.png"); // UncheckedDisable
	QString path(":/theme/duifw/radiobox.png");
	setImage(path, path, path, path, path, path, path, path); // UncheckedDisable
}

void DuiRadioButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if( rect().contains( event->pos() ) )
	{
		Qt::CheckState state = checkState();

		if ( state == Qt::Unchecked )
		{
			setCheckState( Qt::Checked );
			emit radioCheckedWithMouse();
		}
	}
}

void DuiRadioButton::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	event->accept();
}
