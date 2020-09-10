#pragma once

#include "duitexteditgraphicsitem.h"
#include "duiitemdrawers.h"

class DuiTextEditGraphicsPixmapItemPrivate;
 
class DW_DUIFW_EXPORT DuiTextEditGraphicsPixmapItem : public DuiTextEditGraphicsItem
{
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsPixmapItem )
	Q_DISABLE_COPY( DuiTextEditGraphicsPixmapItem )

public:
	DuiTextEditGraphicsPixmapItem();
	~DuiTextEditGraphicsPixmapItem();

	void	setFrameType( DuiFrameItemDrawer::FrameType type );
	DuiFrameItemDrawer::FrameType frameType() const;

	void	setImagePath( const QString& normalPath, const QString& hoverPath, const QString& pressPath );
	void	setPixmap(const QPixmap &normal, const QPixmap &hover, const QPixmap &press);

    void setText( const QString& text );
    QString text() const;

    void setFont( const QFont &font );
    QFont font() const ;

    void setPen( const QPen& pen );
    QPen pen() const;

    void setTextColor( const QColor& color );
    QColor textColor() const;

    QSizeF textSize() const;

protected:
	explicit DuiTextEditGraphicsPixmapItem(DuiTextEditGraphicsPixmapItemPrivate &dd );
	
	virtual void paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format  );
	virtual void hoverEnterEvent ( QMouseEvent * event, quint32 textObjectIndex );
	virtual void hoverLeaveEvent ( QMouseEvent * event, quint32 textObjectIndex );
	virtual void mousePressEvent( QMouseEvent *event, quint32 textObjectIndex );
	virtual void mouseReleaseEvent(QMouseEvent *event, quint32 textObjectIndex );
};
 
