#include "stable.h"

#include "duipushbutton.h"
#include "duistackedwidget.h"

#include "frame/duiframewindow_p.h"
#include "duisettingpagebuttontree.h"
#include "duisettingpagebase.h"

#include "DuiSettingFrame.h"

class DuiSettingFramePrivate : public DuiFrameWindowPrivate
{
	Q_DECLARE_PUBLIC( DuiSettingFrame );

	DuiSettingFramePrivate();
	~DuiSettingFramePrivate();

	void showDefaultButtonTreeItem();
	bool savePages(DuiSettingPageBase::SaveEvent saveEvent);
	void revertPages();

	//void initStackWidget();

public:
	DuiStackedWidget*			m_stackWidget;
	DuiSettingPageButtonTree*	m_settingPageButtonTree;
	DuiPushButton*				m_ApplyButton;
	int							m_taskRef;
	DuiSettingPageBase::SaveEvent					m_clickButtonType;
};

DuiSettingFramePrivate::DuiSettingFramePrivate()
: m_stackWidget( NULL )
, m_settingPageButtonTree( NULL )
, m_ApplyButton( NULL )
, m_taskRef( 0 )
, m_clickButtonType( DuiSettingPageBase::ClickedNone )
{

}

DuiSettingFramePrivate::~DuiSettingFramePrivate()
{

}

void DuiSettingFramePrivate::showDefaultButtonTreeItem()
{
}

bool DuiSettingFramePrivate::savePages(DuiSettingPageBase::SaveEvent saveEvent)
{
	Q_Q( DuiSettingFrame );

	//check current page is ok
	DuiSettingPageBase* currentPage = q->getPageByIndex( q->currentPageIndex() );
	if ( currentPage && currentPage->dataChanged() )
	{
		if ( !currentPage->validatePage() ) 
		{
			return false;
		}
	}

	for ( int i = 0; i < m_stackWidget->count(); i++ )
	{
		DuiSettingPageBase* page = qobject_cast<DuiSettingPageBase*> ( m_stackWidget->widgetAt( i ) );
		if ( page == NULL )
		{
			continue;
		}

		if ( page->dataChanged() )
		{
			page->onPageSave(saveEvent);
			page->finishSave();
		}
	}

	return true;
}

void DuiSettingFramePrivate::revertPages()
{
	Q_Q( DuiSettingFrame );

	//check current page is ok
	DuiSettingPageBase* currentPage = q->getPageByIndex( q->currentPageIndex() );
	if ( currentPage && currentPage->dataChanged() )
	{
		if ( !currentPage->validatePage() ) 
		{
			return ;
		}
	}

	for ( int i = 0; i < m_stackWidget->count(); i++ )
	{
		DuiSettingPageBase* page = qobject_cast<DuiSettingPageBase*> ( m_stackWidget->widgetAt( i ) );
		if ( page == NULL )
		{
			continue;
		}

		if ( page->dataChanged() )
		{
			page->onPageRevert();
			page->finishSave();
		}
	}

}


//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_FRAME_CREATE_SELF( DuiSettingFrame )

DuiSettingFrame::DuiSettingFrame( QWidget* parent,FrameStyles frameStyles )
: DuiFrameWindow( *new DuiSettingFramePrivate, parent, frameStyles )
{

}

DuiSettingFrame::~DuiSettingFrame()
{
}

void DuiSettingFrame::construct()
{
	Q_D( DuiSettingFrame );

	DuiFrameWindow::construct();

	setCloseOnEscape( true );

    DuiSettingFrame::initStackWidget();

	d->m_ApplyButton = findChild<DuiPushButton*>( "applyButton" );
	if ( d->m_ApplyButton )
	{
		d->m_ApplyButton->setEnabled(false);
	}
	
	d->m_settingPageButtonTree = findChild<DuiSettingPageButtonTree*>( "selectPageButtonList" );

	DW_ASSERT( d->m_settingPageButtonTree );

	if (NULL == d->m_settingPageButtonTree->model())
	{
		return;
	}
    d->m_settingPageButtonTree->expandAllItem();

//	d->m_contentWidget->setBackgroundThemeColor( DuiTheme::MoreLightColor );
//	d->m_contentWidget->setColorizePolicy( ColorableFlag );
}

void DuiSettingFrame::on_selectPageButtonList_leftButtonClicked( const QModelIndex& index )
{
	Q_D( DuiSettingFrame );

	QModelIndex parent = index.parent();
	if (!parent.isValid())
	{
		DW_ASSERT_X(false, __FUNCTION__, "parent index is not valid.");
		return;
	}

	QString pageName = d->m_settingPageButtonTree->pageName(index);
	if ( pageName.isEmpty() )
	{
		return ;
	}

	onlyActivePage( pageName );
}


void DuiSettingFrame::on_okButton_clicked()
{	
	okImpl();
}

void DuiSettingFrame::on_cancelButton_clicked()
{
    cancelImpl();
}

void DuiSettingFrame::on_applyButton_clicked()
{
    applyImpl();
}

void DuiSettingFrame::okImpl()
{
    d_func()->m_clickButtonType = DuiSettingPageBase::ClickedOK;
    bool isSuccess = d_func()->savePages(DuiSettingPageBase::ClickedOK);
    if (isSuccess && d_func()->m_taskRef <= 0 )
    {
        close();
    }
}

void DuiSettingFrame::applyImpl()
{
    d_func()->m_clickButtonType = DuiSettingPageBase::ClickedApply;
    bool isSuccess = d_func()->savePages(DuiSettingPageBase::ClickedApply);
    if (isSuccess)
    {
        d_func()->m_ApplyButton->setEnabled(false);
    }
}

void DuiSettingFrame::cancelImpl()
{
    emit cancelButtonClicked();
    close();
}

int DuiSettingFrame::currentPageIndex()
{
	Q_D( DuiSettingFrame );

	if ( d->m_stackWidget == NULL )
		return 0;

	return d->m_stackWidget->currentIndex();
}


DuiSettingPageBase* DuiSettingFrame::getPageByName( const QString& pageName ) const
{
	DuiSettingPageBase *page =  qobject_cast<DuiSettingPageBase*> ( d_func()->m_stackWidget->widgetAtByName( pageName ) );
	return page;
}

DuiSettingPageBase* DuiSettingFrame::getPageByIndex( const int index ) const
{
	Q_D( const DuiSettingFrame );

	if ( d->m_stackWidget == NULL )
	{
		return NULL;
	}

	if ( index == -1 )
	{
		return NULL;
	}

	return qobject_cast<DuiSettingPageBase*> ( d->m_stackWidget->widgetAt( index ) );
}

int DuiSettingFrame::pageCount()
{
	return d_func()->m_stackWidget->count();
}

void DuiSettingFrame::setRightButtonAvailable( bool available /*= true */ )
{
	Q_D(DuiSettingFrame);
	d->m_settingPageButtonTree->setRightButtonAvailable(available);
}

bool DuiSettingFrame::isRightButtonAvailable() const
{
	return d_func()->m_settingPageButtonTree->isRightButtonAvailable();
}

int DuiSettingFrame::clickedButtonType() const
{
	return d_func()->m_clickButtonType;
}

void DuiSettingFrame::addPage( DuiSettingPageBase* settingPage )
{
	if ( settingPage->objectName().isEmpty() )
	{
		return;
	}

	d_func()->m_stackWidget->addItem( settingPage );
}

void DuiSettingFrame::removePage( DuiSettingPageBase* settingPage )
{
	d_func()->m_stackWidget->removeItem( settingPage );
}

void DuiSettingFrame::removePage( const int index )
{
	d_func()->m_stackWidget->removeItem( index );
}

void DuiSettingFrame::closeEvent( QCloseEvent* event )
{
	Q_UNUSED( event );
	emit closeSettingFrame();	
}

void DuiSettingFrame::addCategory( const QString& categoryText, const QString& icon )
{
	return d_func()->m_settingPageButtonTree->addCategory( categoryText, icon );
}

void DuiSettingFrame::addCategoryItem( const QString& categoryText, const QString& itemText, const QString& itemIcon, const QString& pageName )
{
	return d_func()->m_settingPageButtonTree->addCategoryItem( categoryText, itemText, itemIcon, pageName );
}


DuiSettingPageBase* DuiSettingFrame::findPage( const QString &pageName )
{
	DuiSettingPageBase *pageItem = QObject::findChild<DuiSettingPageBase*>(pageName);
	if ( pageItem )
	{
		return pageItem;
	}
	return NULL;
}

void DuiSettingFrame::activePage( const QString &pageName )
{
	onlyActivePage( pageName );
	if (NULL == d_func()->m_settingPageButtonTree->model())
	{
		return;
	}
	d_func()->m_settingPageButtonTree->expandItem( pageName );
}

int DuiSettingFrame::pageIndex( const QString &pageName ) const
{
	Q_D(const DuiSettingFrame);

	DuiSettingPageBase *page = getPageByName( pageName );
	if ( page != NULL )
	{
		return d->m_stackWidget->widgetIndex(page);
	}

	return -1;
}

int DuiSettingFrame::pageIndex( const DuiSettingPageBase *page ) const
{
	return d_func()->m_stackWidget->widgetIndex(page);
}
QString DuiSettingFrame::currentPageName()
{
	if ( currentPageIndex() == -1 )
	{
		return QString("");
	}

	return getPageByIndex( currentPageIndex() )->objectName() ;
}

void DuiSettingFrame::removeCategory( const QString& categoryText )
{
	d_func()->m_settingPageButtonTree->removeCategory( categoryText );
}

void DuiSettingFrame::removeCategoryItem( const QString& categoryText, const QString& itemText )
{
	d_func()->m_settingPageButtonTree->removeCategoryItem( categoryText, itemText );
}

QString DuiSettingFrame::getCategoryByPage(const QString& pageName) const
{
    Q_D(const DuiSettingFrame);

    return d->m_settingPageButtonTree->categoryText(pageName);
}

QString DuiSettingFrame::getCategoryByPage(const int nPageIndex) const
{
    Q_D(const DuiSettingFrame);

    if (DuiWidget* page = d->m_stackWidget->widgetAt(nPageIndex))
    {
        return d->m_settingPageButtonTree->categoryText(page->objectName());
    }
    return QString();
}

void DuiSettingFrame::onlyActivePage( const QString &pageName )
{
	DuiSettingPageBase* prePage = getPageByIndex( currentPageIndex() );
	if ( prePage )
	{
		if (prePage->dataChanged())
		{
			if ( !prePage->validatePage() )
			{
				return;
			}
		}

		prePage->onPageLeave(pageName);
	}

	DuiSettingPageBase *page = getPageByName( pageName );
	if ( page )
	{
		d_func()->m_stackWidget->setCurrentWidget( page );
		page->onPageActivity();
	}
}

void DuiSettingFrame::on_pageDataChanged( const QString& pageName )
{
	Q_UNUSED( pageName );
	if ( d_func()->m_ApplyButton )
	{
		d_func()->m_ApplyButton->setEnabled(true);
	}
}

void DuiSettingFrame::colorizeEvent( DuiColorizeEvent *event )
{
	DuiFrameWindow::colorizeEvent( event );

	//d_func()->m_contentWidget->setBackgroundThemeColor( DuiTheme::MoreLightColor );
}

void DuiSettingFrame::addtaskRef()
{
	Q_D( DuiSettingFrame );

	if ( d->m_clickButtonType != DuiSettingPageBase::ClickedOK )
	{
		return;
	}

	d->m_taskRef ++;
}

void DuiSettingFrame::releaseTaskRef( bool isClose )
{
	Q_D( DuiSettingFrame );

	if ( d->m_clickButtonType != DuiSettingPageBase::ClickedOK )
	{
		return;
	}

	d->m_taskRef --;

	if ( d->m_taskRef <= 0 && isClose )
	{
		close();
	}
}

// @bug by huqiuyun for https://yypm.com:8443/browse/YYXMII-13318 
void DuiSettingFrame::onEnterPressed( Qt::KeyboardModifiers modifiers )
{
	DuiFrameWindow::onEnterPressed( modifiers );

	on_okButton_clicked();
}


void DuiSettingFrame::initStackWidget()
{
    Q_D( DuiSettingFrame );

    d->m_stackWidget = findChild<DuiStackedWidget*>("settingPageStack");
    if ( d->m_stackWidget == NULL )
    {
        //DW_ASSERT( d->m_stackWidget );
        return;
    }

    d->m_stackWidget->setCurrentIndex( 0 );

    for ( int i = 0; i < d->m_stackWidget->count(); i++ )
    {
        DuiSettingPageBase* item = qobject_cast<DuiSettingPageBase*>( d->m_stackWidget->widgetAt( i ) );

        if ( item == NULL )
        {
            continue;
        }

        item->setParentFrame( this );
        connect( item, SIGNAL(pageDataChanged(const QString&)), SLOT(on_pageDataChanged(const QString&)) );
        connect( item, SIGNAL(addtaskRef()), SLOT(addtaskRef()) );
        connect( item, SIGNAL(releaseTaskRef(bool)), SLOT(releaseTaskRef(bool)) );

    }
}