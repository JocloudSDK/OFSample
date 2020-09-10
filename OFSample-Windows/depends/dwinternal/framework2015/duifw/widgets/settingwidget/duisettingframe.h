
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duifw/duiframewindow.h"

class QModelIndex;
class DuiSettingPageBase;
class DuiSettingFramePrivate;

class DW_DUIFW_EXPORT DuiSettingFrame : public DuiFrameWindow
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSettingFrame )
	DUI_DECLARE_DYNAMIC_FRAME_CREATE_SELF( DuiSettingFrame )
	Q_DECLARE_PRIVATE( DuiSettingFrame )

public:
	explicit DuiSettingFrame( QWidget* parent = NULL,FrameStyles frameStyles = DuiFrameWindow::Dialog);
	explicit DuiSettingFrame( DuiSettingFramePrivate &dd, QGraphicsItem *parent = NULL );

	virtual ~DuiSettingFrame();

	virtual void activePage( const QString &pageName );

	void addCategory( const QString& categoryText, const QString& icon );
	void addCategoryItem( const QString& categoryText, const QString& itemText, const QString& itemIcon, const QString& pageName );
	void addPage( DuiSettingPageBase* settingPage );

	void removePage( DuiSettingPageBase* settingPage );
	void removePage( const int index );
	void removeCategory( const QString& categoryText );
	void removeCategoryItem( const QString& categoryText, const QString& itemText );

    QString getCategoryByPage(const QString& pageName) const;
    QString getCategoryByPage(const int nPageIndex) const;

	DuiSettingPageBase* getPageByName( const QString& pageName ) const ;
	DuiSettingPageBase* getPageByIndex( const int index ) const;
	int currentPageIndex();
	QString currentPageName();

	int pageIndex( const QString &pageName ) const;
	int pageIndex( const DuiSettingPageBase *page ) const;
	int	pageCount();

	bool isRightButtonAvailable() const;
	void setRightButtonAvailable( bool available = true );

	// return DuiSettingPageBase::SaveEvent
	int clickedButtonType() const;
	

protected:
	virtual void construct();
	virtual void closeEvent( QCloseEvent* event );
	virtual void colorizeEvent(DuiColorizeEvent *event);

    virtual void okImpl();
    virtual void applyImpl();
    virtual void cancelImpl();

	DuiSettingPageBase* findPage( const QString &pageName );
	virtual void onlyActivePage( const QString &pageName );
    virtual void initStackWidget();

	/**
	 * @bug by huqiuyun for https://yypm.com:8443/browse/YYXMII-13318
	*/
	virtual void onEnterPressed( Qt::KeyboardModifiers modifiers );
signals:
	void closeSettingFrame(); 
	void currentPageChanged( const int index );
	void cancelButtonClicked();

private slots:
	void on_pageDataChanged( const QString& pageName );
	void on_selectPageButtonList_leftButtonClicked( const QModelIndex& index );
	void on_okButton_clicked();
	void on_cancelButton_clicked();
	void on_applyButton_clicked();

	void addtaskRef();
	void releaseTaskRef( bool isClose );
};