#pragma once

#include "duiwidget.h"

class DuiHtmlItemWidgetPrivate;
class DW_DUIFW_EXPORT DuiHtmlItemWidget:public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY(DuiHtmlItemWidget);
	Q_DECLARE_PRIVATE(DuiHtmlItemWidget);
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiHtmlItemWidget);

	Q_PROPERTY( I18nString html WRITE setHtml READ html );
	Q_PROPERTY( qreal documentMargin WRITE setDocumentMargin READ documentMargin );
	Q_PROPERTY( bool openExternalLinks WRITE setOpenExternalLinks READ openExternalLinks );

public:
	explicit DuiHtmlItemWidget(QGraphicsItem* parent = NULL);
	~DuiHtmlItemWidget();

	QString html() const;
	void setHtml( const QString& text );
	
	void setDocumentMargin(  qreal margin );
	qreal	documentMargin () const;

    void setDefaultTextColor(const QColor &c);
    QColor defaultTextColor() const;

	QRectF htmlItemBoundingRect();
	QTextDocument *document() const;
	
	void setTextFont( QFont& font );
	QFont textFont( );

	bool	openExternalLinks () const;
	void	setOpenExternalLinks ( bool open );

signals:
	void	linkActivated(const QString &link);
	void	linkHovered ( const QString & link );

private slots:
	void on_linkHovered( const QString & link );

protected:	
    virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );
};