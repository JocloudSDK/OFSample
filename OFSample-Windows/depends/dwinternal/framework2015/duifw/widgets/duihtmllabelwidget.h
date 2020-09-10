#pragma once

#include "duiwidget.h"

class DuiHtmlLabelWidgetPrivate;
class DW_DUIFW_EXPORT DuiHtmlLabelWidget:public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY(DuiHtmlLabelWidget);
	Q_DECLARE_PRIVATE(DuiHtmlLabelWidget);
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiHtmlLabelWidget);

	Q_PROPERTY( I18nString text WRITE setText READ text );
    Q_PROPERTY( QString link WRITE setLink READ link );
    Q_PROPERTY( QColor defaultTextColor WRITE setDefaultTextColor READ defaultTextColor );
    Q_PROPERTY( QColor shadowColor WRITE setShadowColor READ shadowColor );
    Q_PROPERTY( qreal shadowBlurRadius WRITE setShadowBlurRadius READ shadowBlurRadius );
    Q_PROPERTY( qreal shadowOpacity WRITE setShadowOpacity READ shadowOpacity);
	Q_PROPERTY( bool acceptClick WRITE setAcceptClick READ acceptClick );
	Q_PROPERTY( bool underlineOnHover WRITE setUnderlineOnHover READ underlineOnHover );
	Q_PROPERTY( bool dropShadow WRITE setDropShadow READ dropShadow );
	Q_PROPERTY( bool autoMultiLine READ autoMultiLine WRITE setAutoMultiLine );
    Q_PROPERTY( Qt::Alignment alignment READ alignment WRITE setAlignment );
    Q_PROPERTY( qreal documentMargin WRITE setDocumentMargin READ documentMargin );

public:
	explicit DuiHtmlLabelWidget(QGraphicsItem* parent = NULL);
	~DuiHtmlLabelWidget();

	QString text() const;
	QString plateText() const;
	void setText( const QString& text );
	void setPlainText(const QString& text);

    void setDocumentMargin(  qreal margin );
    qreal	documentMargin () const;

	void setLink( const QString& url );
	QString link() const;

	void setAcceptClick(bool enabled);
	bool acceptClick() const;

	void setUnderlineOnHover( bool underlineOnHover );
    bool underlineOnHover() const;

    void setDefaultTextColor(const QColor &c);
    QColor defaultTextColor() const;

	QTextOption::WrapMode defaultTextWrapMode() const;
	void setDefaultTextWrapMode(QTextOption::WrapMode mode);

	bool dropShadow() const;
	void setDropShadow(bool shadow);

	QColor shadowColor() const;
    void setShadowColor(const QColor &color);

    qreal shadowBlurRadius() const;
    void setShadowBlurRadius(qreal blurRadius);

    qreal shadowOpacity() const;
    void setShadowOpacity(qreal opacity);

	bool autoMultiLine() const;
	void setAutoMultiLine(bool b);

	Qt::Alignment alignment() const;
	void setAlignment( Qt::Alignment align );

    QRectF htmlItemBoundingRect();
signals:
	void clicked();

protected:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event ); 
	virtual void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );	

private:
	void _updateShadowEffect();
    void setTextGraphicsEffect( QGraphicsEffect *effect );
    QTextDocument *document() const;
};