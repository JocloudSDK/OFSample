#include "stable.h"

#include "duiwidget_p.h"
#include "duiview.h"

class DuiViewPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiView)

	DuiViewPrivate();
	~DuiViewPrivate();

	void init();

	QGraphicsWidget *m_widget;
	QGraphicsLinearLayout *m_layout;
	QString		m_title;
	QString		m_iconNormalPath;
	QString		m_iconSelectedPath;
};

DuiViewPrivate::DuiViewPrivate()
: m_widget(NULL)
, m_layout(NULL) 
{
}


DuiViewPrivate::~DuiViewPrivate()
{
	delete m_widget;
}

void DuiViewPrivate::init()
{
}


DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiView)
DuiView::DuiView(QGraphicsItem *parent) 
: DuiWidget(*new DuiViewPrivate, parent)
{
}

DuiView::DuiView( DuiViewPrivate &dd, QGraphicsItem *parent /*= NULL*/ )
	: DuiWidget(dd, parent)
{
}

DuiView::DuiView()
:DuiWidget(*new DuiViewPrivate,NULL)
{

}
DuiView::~DuiView()
{
}

void DuiView::setWidget(QGraphicsWidget *widget)
{
	Q_D(DuiView);

	if(d->m_widget == widget)
		return;

	if(widget)
	{
		if(d->m_layout)
		{
			d->m_layout->removeAt(0);
			d->m_layout->addItem(widget);
		}
		else
		{
			d->m_layout = new QGraphicsLinearLayout(this);
			d->m_layout->addItem(widget);
			d->m_layout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
			setLayout(d->m_layout);
		}
		if(d->m_widget)
		{
			delete d->m_widget;
		}
		d->m_widget = widget;
	}
	else
	{
		if(d->m_layout)
		{
			setLayout(NULL);
			d->m_layout = NULL;
		}
		delete d->m_widget;
		d->m_widget = NULL;
	}
}

QString DuiView::title() const
{
	Q_D( const DuiView);
	return d->m_title;
}

void DuiView::setTitle( const QString &title )
{
	Q_D(DuiView);
	d->m_title = title;
}

QString DuiView::normalIcon() const
{
	Q_D( const DuiView);
	return d->m_iconNormalPath;
}

void DuiView::setNormalIcon( const QString &iconPath )
{
	Q_D(DuiView);
	d->m_iconNormalPath = iconPath;
}

QString DuiView::selectedIcon() const
{
	Q_D( const DuiView);
	return d->m_iconSelectedPath;
}

void DuiView::setSelectedIcon( const QString &iconPath )
{
	Q_D(DuiView);
	d->m_iconSelectedPath = iconPath;
}

void DuiView::onActivated()
{

}

void DuiView::onDeactivated()
{

}

void DuiView::construct()
{
	DuiWidget::construct();
	d_func()->init();
}

void DuiView::doExpandOrCollapse()
{

}
