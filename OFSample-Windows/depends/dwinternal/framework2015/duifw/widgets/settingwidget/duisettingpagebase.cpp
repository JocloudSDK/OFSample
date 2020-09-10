#include "stable.h"

#include "duiwidget_p.h"
#include "duisettingpagebase.h"
#include "duisettingframe.h"

class DuiSettingPageBasePrivate : public DuiWidgetPrivate	
{
	Q_DECLARE_PUBLIC( DuiSettingPageBase );

public:
	DuiSettingPageBasePrivate();
	~DuiSettingPageBasePrivate();

	void  init();

public:
	DuiSettingFrame* m_parentFrame;
	bool			 m_isChanged;
	bool			 m_isInitializing;
	bool			 m_hasInitialized;
};

DuiSettingPageBasePrivate::DuiSettingPageBasePrivate()
: m_parentFrame( NULL )
, m_isChanged( false )
, m_isInitializing(false)
, m_hasInitialized(false)
{

}

DuiSettingPageBasePrivate::~DuiSettingPageBasePrivate()
{

}

void DuiSettingPageBasePrivate::init()
{
}


//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageBase );


DuiSettingPageBase::DuiSettingPageBase( QGraphicsItem* parent /*= NULL */ )
: DuiWidget( *new DuiSettingPageBasePrivate, parent )
{
	d_func()->init();
}

DuiSettingPageBase::DuiSettingPageBase( DuiSettingPageBasePrivate &dd, QGraphicsItem *parent /*= NULL */ )
: DuiWidget( dd, parent )
{
	d_func()->init();
}

DuiSettingPageBase::~DuiSettingPageBase()
{

}

void DuiSettingPageBase::setParentFrame( DuiSettingFrame* parentFrame )
{
	d_func()->m_parentFrame = parentFrame;
}

void DuiSettingPageBase::onPageActivity()
{

}

void DuiSettingPageBase::onPageLeave(const QString&)
{

}

bool DuiSettingPageBase::validatePage()
{
	return true;
}

void DuiSettingPageBase::onPageSave(DuiSettingPageBase::SaveEvent saveEvent)
{
	Q_UNUSED(saveEvent)
}

void DuiSettingPageBase::onPageRevert()
{

}

bool DuiSettingPageBase::dataChanged()
{
	return d_func()->m_isChanged;
}

void DuiSettingPageBase::setDataChanged()
{
	if (d_func()->m_isInitializing)
	{
		return;
	}
	d_func()->m_isChanged = true;

	emit pageDataChanged( objectName() );	
}

void DuiSettingPageBase::finishSave()
{
	d_func()->m_isChanged = false;
}

DuiSettingFrame* DuiSettingPageBase::parentFrame()
{
	return d_func()->m_parentFrame;
}

void DuiSettingPageBase::beginInitialize()
{
	d_func()->m_isInitializing = true;
}

void DuiSettingPageBase::endInitialize()
{
	d_func()->m_isInitializing = false;
}

void DuiSettingPageBase::setInitializeStatus( bool hasInitialized )
{
	d_func()->m_hasInitialized = hasInitialized;
}

bool DuiSettingPageBase::hasInitialized() const
{
	return d_func()->m_hasInitialized;
}

void DuiSettingPageBase::taskWait()
{	
	emit addtaskRef();
}

void DuiSettingPageBase::taskFinish( bool isClose )
{
	emit releaseTaskRef( isClose );
}

DuiSettingPageInitHelper::DuiSettingPageInitHelper( DuiSettingPageBase *basePage )
: m_basePage(basePage)
{
	if (NULL != m_basePage)
	{
		m_basePage->beginInitialize();
	}
}

DuiSettingPageInitHelper::~DuiSettingPageInitHelper()
{
	if (NULL != m_basePage)
	{
		m_basePage->endInitialize();
	}
}
