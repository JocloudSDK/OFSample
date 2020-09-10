#pragma once

#include "dwbase/dwcomex.h"

class DuiWidget;

/*
	IYYImChatFrameManagerPtr frameMgr = Root->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	if (frameMgr == NULL || !frameMgr->isOpenChatWnd(uid))
	{
		return;
	}
	IYYImChatFramePtr chatFrame = frameMgr->findImFrame( uid );
	if (chatFrame == NULL)
	{
		return;
	}

	IYYNoticeTipWidgetPtr pNoticeTip;
	chatFrame->queryInterface( pNoticeTip );
*/

/*
IYYNoticeTipCommonItemPtr prop = root->getServiceInterface<IYYNoticeTipCommonItem>(KSERVICENoticeTipHtmlItem);
IYYNoticeTipSimpleItemPtr prop = root->getServiceInterface<IYYNoticeTipSimpleItem>(KSERVICENoticeTipSimpleItem);
IYYNoticeTipHtmlItemPtr prop = root->getServiceInterface<IYYNoticeTipHtmlItem>(KSERVICENoticeTipHtmlItem);
IYYNoticeTipDefaultItemPtr prop = root->getServiceInterface<IYYNoticeTipDefaultItem>(KSERVICENoticeTipDefaultItem);

IYYNoticeTipCommonItemPtr item = XXXXX;
IYYRealWidgetWrapPtr realWidgetPtr;
item->queryInterface(realWidgetPtr);

//œ»”√IYYRealWidgetWrap¿¥create
*/
DWDEFINE_INTERFACE(IYYNoticeTipCommonItem) : public IDWQComponent
{
	virtual void setInterval( int interval ) = 0;
	virtual int  interval()  = 0 ;

	virtual void setParam( int key, QString& param ) = 0;
	virtual QString param( int key ) = 0;

	virtual void setBackgroundStyle( const QColor& bkColor, const QColor& topLineColor, const QColor& bottomLineColor ) = 0;

//signals:
//	void closeTip( YYNoticeTipCommonItem* item );
};

DWDEFINE_INTERFACE(IYYNoticeTipDefaultItem) : public IDWQComponent
{
	virtual void setText( const QString& text ) = 0;
	virtual void setValue( const QString& value ) = 0;
	virtual void setAcceptClick( bool enabled ) = 0;
	virtual void setTextColor( const QColor& textColor ) = 0;
	virtual void setIcon( const QString &icon ) = 0;

	virtual QString text() = 0;
	virtual QString value() = 0;

	virtual void addCloseButton( ) = 0;

//signals:
//	void labelClicked( const QString& objectName, const QString& value );
//	void closeTipButtonClicked( const QString& objectName );
};

DWDEFINE_INTERFACE(IYYNoticeTipHtmlItem) : public IDWQComponent
{
	virtual void setHtml( const QString& html ) = 0;
	virtual void addCloseButton( ) = 0;

//signals:
//	void linkActivated( const QString & objectName, const QString& link );
//	void closeTipButtonClicked( const QString& objectName );
};

DWDEFINE_INTERFACE(IYYNoticeTipSimpleItem) : public IDWQComponent
{
	virtual void setText( const QString& text ) = 0;
	virtual void setTextColor( const QColor& textColor ) = 0;
	virtual void update( const QString& text ) = 0;
	virtual void addCloseButton( ) = 0;

//signals:
//	void closeTipButtonClicked( const QString& objectName );
};

DWDEFINE_INTERFACE(IYYNoticeTipWidget) : public IDWQComponent
{
	virtual void removeItem( const QString& objectName ) = 0;
	virtual void insertItem( IYYNoticeTipCommonItemPtr item, int interval /*= -1*/, int index /*= -1*/ ) = 0;

	virtual bool getItem( const QString& objectName, IYYNoticeTipDefaultItemPtr defaultItemWrap ) = 0;

//signals:
//	void closeTipClicked( const QString& objectName );
};