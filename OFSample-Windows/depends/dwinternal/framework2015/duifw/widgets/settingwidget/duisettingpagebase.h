
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiscrollarea.h"

class DuiSettingFrame;
class DuiSettingPageBasePrivate;

const int KPageId					= Qt::UserRole+0x30;
const int KPageIndex			= Qt::UserRole+0x31;
const int KPageName			= Qt::UserRole+0x32;
const int KPageUrl				= Qt::UserRole+0x33;
const int KPageTargetUid		= Qt::UserRole+0x34;
const int KPageMyUid			= Qt::UserRole+0x35;

class DW_DUIFW_EXPORT DuiSettingPageBase : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSettingPageBase )
	Q_DECLARE_PRIVATE( DuiSettingPageBase )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageBase )

public:

	enum SaveEvent
	{
		ClickedNone,
		ClickedOK,
		ClickedApply
	};

	explicit DuiSettingPageBase( QGraphicsItem* parent = NULL );
	explicit DuiSettingPageBase( DuiSettingPageBasePrivate &dd, QGraphicsItem *parent = NULL );
	virtual ~DuiSettingPageBase();

	void setParentFrame( DuiSettingFrame* parentFrame );
	DuiSettingFrame* parentFrame();

	void setDataChanged();
	bool dataChanged();

	//only duisettingframe call
	void finishSave();

	void beginInitialize();
	void endInitialize();

	void setInitializeStatus( bool hasInitialized );
	bool hasInitialized() const;

	virtual void onPageActivity();
	virtual void onPageLeave(const QString& selPageName);
	virtual bool validatePage();
	virtual void onPageSave(SaveEvent saveEvent);
	virtual void onPageRevert();

	//call this, when click ok button, the frame will close after all page finish from server.
	void taskWait();
	void taskFinish( bool isClose );

signals:
	void pageDataChanged( const QString& pageName );
	void addtaskRef();
	void releaseTaskRef( bool isClose );
};


class DW_DUIFW_EXPORT DuiSettingPageInitHelper
{
	Q_DISABLE_COPY( DuiSettingPageInitHelper )
public:
	explicit DuiSettingPageInitHelper(DuiSettingPageBase *basePage);
	~DuiSettingPageInitHelper();
private:
	DuiSettingPageBase	*m_basePage;
};