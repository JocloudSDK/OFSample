#include <QList>

#include "duibuttongroup.h"
#include "duiwidget_p.h"
#include "duiradiobutton.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiButtonGroup)
class DuiButtonGroupPrivate :public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiCheckBox)
public:
	DuiButtonGroupPrivate();
	~DuiButtonGroupPrivate();

	QList<DuiRadioButton*> m_radioButtons;
};	//DuiButtonGroupPrivate

DuiButtonGroupPrivate::DuiButtonGroupPrivate()
{
}

DuiButtonGroupPrivate::~DuiButtonGroupPrivate()
{
}

DuiButtonGroup::DuiButtonGroup(QGraphicsItem* parent)
:DuiWidget(*new DuiButtonGroupPrivate, parent)
{

}

void DuiButtonGroup::childEvent(QChildEvent * event)
{
	DuiWidget::childEvent(event);
	if ( event->removed() )
	{
		DuiRadioButton *button = qobject_cast<DuiRadioButton*>(event->child() );
		if ( button )
		{
			d_func()->m_radioButtons.removeOne(button);
		}
	}
}

void DuiButtonGroup::onRadionButtonStateChanged( bool bSelected )
{
	if ( !bSelected )
	{
		return ;
	}

	DuiRadioButton *button = qobject_cast<DuiRadioButton*>( sender() );
	if ( button == NULL)
	{
		return ;
	}

	DuiRadioButton *radio = NULL;
	QList<DuiRadioButton*> &buttons = d_func()->m_radioButtons;

	for (int i = 0; i < buttons.size(); ++i)
	{
		radio =  buttons[i] ;

		if ( radio != button )
		{
			radio->setCheckState( Qt::Unchecked );
			radio->update();
		}
	}

	emit radioSelected( button );
}

void DuiButtonGroup::connectChildrenSignal( QObject* child )
{
	if ( child == NULL )
	{
		return ;
	}

	DuiRadioButton *radioButton = qobject_cast<DuiRadioButton*>(child);
	if ( radioButton != NULL )
	{
		connect( radioButton, SIGNAL(stateChanged(bool)), 
			SLOT(onRadionButtonStateChanged(bool)), Qt::UniqueConnection);	//防止多次connect

		d_func()->m_radioButtons.push_back(radioButton);
	}

	QObjectList children = child->children();
	for ( int i = 0; i < children.size(); ++i )
	{
		connectChildrenSignal( children[i] );
	}	
}

void DuiButtonGroup::construct()
{
	DuiWidget::construct();
	connectChildrenSignal(this);	
}

void DuiButtonGroup::addRadioButton( DuiRadioButton* radioButton )
{
	if ( radioButton != NULL )
	{
		connect( radioButton, SIGNAL(stateChanged(bool)), 
			SLOT(onRadionButtonStateChanged(bool)), Qt::UniqueConnection);	//防止多次connect

		d_func()->m_radioButtons.push_back(radioButton);
	}
}

void DuiButtonGroup::addItem( DuiWidget* item )
{
	DuiWidget::addItem(item);

	DuiRadioButton *radioButton = qobject_cast<DuiRadioButton *>(item);
	if(item)
	{
		addRadioButton(radioButton);
	}
}

void DuiButtonGroup::removeItem(DuiWidget *item)
{
	Q_UNUSED(item);
	DW_ASSERT_X(false, __FUNCTION__, "remove item in DuiButtonGroup is unimplemented now!");
}