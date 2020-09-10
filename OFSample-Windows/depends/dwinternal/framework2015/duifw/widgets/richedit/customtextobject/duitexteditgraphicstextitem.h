#pragma once

#include "duitexteditgraphicsitem.h"

class DuiTextEditGraphicsTextItemPrivate;

class DW_DUIFW_EXPORT DuiTextEditGraphicsTextItem : public DuiTextEditGraphicsItem
{
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsTextItem )
	Q_DISABLE_COPY( DuiTextEditGraphicsTextItem )

public:
	DuiTextEditGraphicsTextItem();
	~DuiTextEditGraphicsTextItem();

	void setText( const QString& text, bool bAutoSize = true );
	QString text() const;

	void setFont( const QFont &font );
	QFont font() const ;

	void setPen( const QPen& pen );
	QPen pen() const;

	void setTextColor( const QColor& color );
	QColor textColor() const;

	void setSender(const bool isSender){m_isSender = isSender; };
	bool isSender() const {return m_isSender; };

    QSizeF textSize() const;

    void setTextFlags(int flags);
    int textFlags() const;
	
protected:
	explicit DuiTextEditGraphicsTextItem(DuiTextEditGraphicsTextItemPrivate &dd );
	virtual void paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format  );

private:
	bool m_isSender;
};