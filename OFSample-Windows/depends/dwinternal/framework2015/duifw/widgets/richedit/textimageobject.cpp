#include "stable.h"
#include "textimageobject.h"
#include "richtextbrowser.h"
#include "duitheme.h"

const int KZoomIconMinHeight = 30;

QImage getImage(QTextDocument *doc, const QTextFormat &format)
{
	QString name = format.stringProperty(QTextCharFormat::ImageName);
	if (name.startsWith(QLatin1String(":/"))) // auto-detect resources
	{
		name.prepend(QLatin1String("qrc"));
	}

	QUrl url = QUrl::fromEncoded(name.toUtf8());
	const QVariant data = doc->resource(QTextDocument::ImageResource, url);

	if (data.type() == QVariant::Pixmap || data.type() == QVariant::Image) 
	{
		return qvariant_cast<QImage>(data);
	}

	return QImage();
}

QSize getImageSize(QTextDocument *doc, const QTextFormat &format)
{
	if(format.hasProperty(QTextFormat::ImageWidth) && format.hasProperty(QTextFormat::ImageHeight))
	{
		return QSize(format.doubleProperty(QTextFormat::ImageWidth), format.doubleProperty(QTextFormat::ImageHeight));
	}

	QImage image = getImage(doc, format);
	return image.size();
}

QSize calcScaledSize(int texteditWidth, QTextDocument *doc, const QTextFormat &format)
{
    if(texteditWidth < 0)
    {
       LogFinal(LOG::KDuiFw) << "calcScaledSize " << "texteditWidth = " << texteditWidth;
    }
	QSize imageSize = getImageSize(doc, format);
	if ( imageSize.width() > texteditWidth )
	{
		qreal scale = 1.0;
		if(imageSize.width() == 0)
		{
            LogFinal(LOG::KDuiFw) << "calcScaledSize " << "imageSize.width() = 0";
		}else
        {
            scale = (qreal) texteditWidth / imageSize.width();
        }

		return imageSize * scale;
	}

	return imageSize;
}

DuiTextImageObject::DuiTextImageObject( RichTextEdit* textedit )
: QObject( textedit )
, m_textedit(textedit)
, m_zoomIconNormal(":/theme/duifw/zoom_normal.png")
, m_zoomIconHover(":/theme/duifw/zoom_hover.png")
, m_isHovered(false)
, m_hasZoomIcon(false)
{

}

QSizeF DuiTextImageObject::intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	int textWidth = m_textedit->width() - doc->documentMargin() * 2;
	if ( m_textedit->verticalScrollBar()->isVisible() )
	{
		textWidth -= m_textedit->verticalScrollBar()->width();
	}
 
	QTextBlock block = doc->findBlock(posInDocument);
	textWidth -= block.blockFormat().leftMargin();

	QTextCursor tc = QTextCursor(block);
	if (tc.currentTable())
	{
		QTextTable *table = tc.currentTable();
		QTextTableCell msgCell = table->cellAt(tc);
		const quint32 headImageWidth = 40;
		quint32 marginsWidth = headImageWidth + msgCell.format().toTableCellFormat().leftPadding() + msgCell.format().toTableCellFormat().rightPadding() + 1;
		marginsWidth += (table->format().leftMargin() + table->format().rightMargin());
		textWidth -= marginsWidth;
	}

	return calcScaledSize(textWidth, doc, format);
}

void DuiTextImageObject::drawObject( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	const QImage image = getImage(doc, format);

	if(image.size() == rect.size().toSize())
	{
		painter->drawImage(rect.topLeft(), image);
	}
	else
	{
		painter->drawImage( rect.topLeft(), image.scaledToWidth(rect.width(), Qt::SmoothTransformation) );
	}

	if (!m_hasZoomIcon)
	{
		return;
	}

	if (image.height() < KZoomIconMinHeight)
	{
		return;
	}

	if ( image.width() > rect.width() )
	{
		m_objectHash.insert( posInDocument, rect );

		QPixmap zoomIcon;
		if ( m_hoveredPos == posInDocument && m_isHovered )
		{	
			zoomIcon = m_zoomIconHover;
		}
		else
		{
			zoomIcon = m_zoomIconNormal;
		}

		QSize szIcon = zoomIcon.size();
		QRectF iconRect( rect.right() - szIcon.width(), rect.bottom() - szIcon.height(), szIcon.width(), szIcon.height() );
		painter->drawPixmap( iconRect, zoomIcon, zoomIcon.rect() );
	}
}

void DuiTextImageObject::setZoomHovered( bool hovered )
{
	if ( m_isHovered == hovered )
	{
		return;
	}

	m_isHovered = hovered;
}

bool DuiTextImageObject::isZoomHovered() const
{
	return m_isHovered;
}

void DuiTextImageObject::setHoveredPos( int pos )
{
	m_hoveredPos = pos;
}

QRectF DuiTextImageObject::objectRect( int posInDoc ) const
{ 
	return m_objectHash.value(posInDoc);
}

bool DuiTextImageObject::hasZoomIcon() const
{
	return m_hasZoomIcon;
}

void DuiTextImageObject::setZoomIcon( bool hasZoomIcon )
{
	m_hasZoomIcon = hasZoomIcon;
}