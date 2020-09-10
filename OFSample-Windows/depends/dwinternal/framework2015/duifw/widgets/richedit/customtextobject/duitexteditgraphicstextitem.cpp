#include "stable.h"

#include "duifw/duitheme.h"
#include "duitexteditgraphicstextitem.h"
#include "duitexteditgraphicsitem_p.h"
#include "widgets/richedit/richtextbrowser.h"

class DuiTextEditGraphicsTextItemPrivate : public DuiTextEditGraphicsItemPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsTextItem )

public:
	DuiTextEditGraphicsTextItemPrivate();
	virtual ~DuiTextEditGraphicsTextItemPrivate();

	void setTextSize();

	QString m_text;
	QFont	m_font;
	QPen	m_pen;
	QSizeF  m_textSize;
    int     m_flags;
};

DuiTextEditGraphicsTextItemPrivate::DuiTextEditGraphicsTextItemPrivate()
: DuiTextEditGraphicsItemPrivate()
{
    m_flags = Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop;
}

DuiTextEditGraphicsTextItemPrivate::~DuiTextEditGraphicsTextItemPrivate()
{
}

void DuiTextEditGraphicsTextItemPrivate::setTextSize()
{
	QFontMetrics fm( m_font );
	//m_textSize = fm.boundingRect( m_text ).size();
    m_textSize = fm.size(m_flags, m_text);
}


//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsTextItem::DuiTextEditGraphicsTextItem( ) 
: DuiTextEditGraphicsItem( *(new DuiTextEditGraphicsTextItemPrivate) )
{
}

DuiTextEditGraphicsTextItem::DuiTextEditGraphicsTextItem( DuiTextEditGraphicsTextItemPrivate &dd )
: DuiTextEditGraphicsItem( dd ) 
{
}

DuiTextEditGraphicsTextItem::~DuiTextEditGraphicsTextItem()
{
}


void DuiTextEditGraphicsTextItem::paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	Q_D( DuiTextEditGraphicsTextItem );
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	Q_UNUSED( painter );
	Q_UNUSED( rect );
	
	if ( d->m_textSize.isNull() )
	{
		return;
	}

	painter->setFont( d->m_font );
	QPointF pos = absolutePos( rect );
	QRectF rc(pos, size());

	RichTextEdit* editor = qobject_cast<RichTextEdit*>(d->m_containerWidget);
	bool selectedPaint = (editor != NULL);
	if (selectedPaint)
	{
		DuiTextCursor tc = editor->textCursor();
		selectedPaint = (tc.hasSelection() && (tc.selectionStart() <= posInDocument  && tc.selectionEnd() > posInDocument));
	}
	if (selectedPaint)
	{
		QColor colorBak = d->m_pen.color();
		d->m_pen.setColor(colorBak.lighter(200));
		painter->drawText(rc, d->m_flags, text());
		d->m_pen.setColor(colorBak);
	}
	else
	{
		painter->setPen( d->m_pen );
		painter->drawText(rc, d->m_flags, text());
	}
}

void DuiTextEditGraphicsTextItem::setFont( const QFont &font )
{
	d_func()->m_font = font;
	d_func()->setTextSize();
}

QFont DuiTextEditGraphicsTextItem::font() const
{
	return d_func()->m_font;
}

void DuiTextEditGraphicsTextItem::setTextColor( const QColor& color )
{
	d_func()->m_pen.setColor( color );
}

QColor DuiTextEditGraphicsTextItem::textColor() const
{
	return d_func()->m_pen.color();
}

void DuiTextEditGraphicsTextItem::setText( const QString& text, bool bAutoSize/* = true */ )
{
	d_func()->m_text = text;
	d_func()->setTextSize();
    if(bAutoSize)
    {
        setSize(textSize());
    }
	update();
}

QString DuiTextEditGraphicsTextItem::text() const
{
	return d_func()->m_text;
}

void DuiTextEditGraphicsTextItem::setPen( const QPen& pen )
{
	d_func()->m_pen = pen;	
}

QPen DuiTextEditGraphicsTextItem::pen() const
{
	return d_func()->m_pen;
}

QSizeF DuiTextEditGraphicsTextItem::textSize() const
{
    return d_func()->m_textSize;
}

void DuiTextEditGraphicsTextItem::setTextFlags(int flags)
{
    d_func()->m_flags = flags;
    d_func()->setTextSize();
}

int DuiTextEditGraphicsTextItem::textFlags() const
{
    return d_func()->m_flags;
}
