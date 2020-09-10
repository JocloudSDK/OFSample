#include "stable.h"
#include "dwbase/log.h"
#include "duitextcursor.h"
#include "duitextedit.h"
#include "richedit/richtextbrowser.h"
#include "richedit/gifpool.h"

class DuiTextCursorPrivate
{
public:
	explicit DuiTextCursorPrivate( RichTextEdit *edit );
	virtual	~DuiTextCursorPrivate();

	void setTextBlockAnimationFlag(DuiTextCursor *textCursor, bool hasAnimation);
	QTextImageFormat cloneTextImageFormat(const QTextImageFormat &format, const QString &key) const;

public:
	RichTextEdit	*m_edit;
	GifPool			*m_gifPool;
};

DuiTextCursorPrivate::DuiTextCursorPrivate( RichTextEdit *edit )
: m_edit( edit )
, m_gifPool( NULL )
{

}

DuiTextCursorPrivate::~DuiTextCursorPrivate()
{
}

void DuiTextCursorPrivate::setTextBlockAnimationFlag(DuiTextCursor *textCursor, bool hasAnimation)
{
	QTextBlockFormat blockFormat = textCursor->blockFormat();
	if (!blockFormat.hasProperty(KAnimationImage))
	{
		blockFormat.setProperty(KAnimationImage, hasAnimation);
	}
	textCursor->setBlockFormat(blockFormat);
}

QTextImageFormat DuiTextCursorPrivate::cloneTextImageFormat( const QTextImageFormat &format, const QString &key ) const
{
	QTextImageFormat imageFormat(format);
	imageFormat.clearProperty(QTextFormat::ImageWidth);
	imageFormat.clearProperty(QTextFormat::ImageHeight);
	imageFormat.setName( key );

	return imageFormat;
}

DuiTextCursor::DuiTextCursor(void)
:d_ptr( new DuiTextCursorPrivate(NULL) )
{

}

DuiTextCursor::DuiTextCursor( RichTextEdit *edit )
: QTextCursor( edit->document() )
, d_ptr( new DuiTextCursorPrivate(edit) )
{

}

DuiTextCursor::DuiTextCursor( RichTextEdit *edit, const QTextCursor &tc )
: QTextCursor(tc)
, d_ptr( new DuiTextCursorPrivate(edit) )
{
}

DuiTextCursor::DuiTextCursor( const DuiTextCursor &other )
: QTextCursor( other )
, d_ptr(NULL)
{
	* this = other;
}

DuiTextCursor& DuiTextCursor::operator=( const DuiTextCursor& other )
{
	if (this != &other)
	{
		if (d_ptr)
		{
			delete d_ptr;
		}
		d_ptr = new DuiTextCursorPrivate( *(other.d_ptr) );
	}
	return *this;
}

DuiTextCursor::~DuiTextCursor(void)
{
	DW_SAFE_DELETE( d_ptr );
}

void DuiTextCursor::insertImage( const QTextImageFormat &format, const QString &key )
{
	DW_ASSERT( !key.isEmpty() );
	Q_D(DuiTextCursor);

	QTextImageFormat imageFormat = d->cloneTextImageFormat(format, key);
	imageFormat.setProperty( KImagePath, format.name() );

	if ( GifPool::isAnimationGif( format.name() ) )
	{
		DW_ASSERT( !key.isEmpty() );

		d->m_gifPool->insertKeyImage( key, format.name() );

		QImage gifImage = d->m_gifPool->currentImage(key);
		imageFormat.setWidth(gifImage.width());
		imageFormat.setHeight(gifImage.height());
		imageFormat.setProperty(KAnimationImage, true);

		document()->addResource(QTextDocument::ImageResource, QUrl(key), gifImage);

		d->setTextBlockAnimationFlag(this, true);
	}
	else
	{
		QImage image( format.name() );
		if ( !image.isNull() )
		{
			document()->addResource(QTextDocument::ImageResource,QUrl(key), image );
		}
		else
		{
			LogFinal(LOG::KDuiFw) << L"insert image fail! image:" << format.name(); 
		}
	}

	QTextCursor::insertImage( imageFormat );
}

void DuiTextCursor::insertImage( const QString & imagePath )
{
	QTextImageFormat format;
	format.setName(imagePath);
	format.setFont( charFormat().font() );

	insertImage( format, imagePath );
}

void DuiTextCursor::insertImage( const QString &key, const QString &imagePath )
{
	QTextImageFormat imageFormat;
	imageFormat.setName( imagePath );
	insertImage( imageFormat, key );
}

void DuiTextCursor::insertImage(const QString &key, const QByteArray &data)
{
	insertImage(key, data, QTextImageFormat());	
}

void DuiTextCursor::insertImage(const QString &key, const QByteArray &data, const QTextImageFormat &format)
{
	Q_D(DuiTextCursor);

	QTextImageFormat imageFormat = d->cloneTextImageFormat(format, key);
	if ( GifPool::isAnimationGif(data) || GifPool::isAnimationGif(imageFormat.name()) )
	{
		DW_ASSERT( !key.isEmpty() );

		d->m_gifPool->insertKeyImage( key, data );

		QImage gifImage = d->m_gifPool->currentImage(key);
		document()->addResource(QTextDocument::ImageResource, QUrl(key), gifImage);
		imageFormat.setWidth(gifImage.width());
		imageFormat.setHeight(gifImage.height());
		imageFormat.setProperty(KAnimationImage, true);

		d->setTextBlockAnimationFlag(this, true);
	}
	else
	{
		imageFormat.setProperty( KImagePath, format.name() );

		QImage image;
		if(image.loadFromData(data))
		{
			document()->addResource(QTextDocument::ImageResource,QUrl(key), image );
		}
	}

	QTextCursor::insertImage(imageFormat);
}

void DuiTextCursor::insertImage(  const QString &key, const QImage &image, const QTextImageFormat &format )
{
	DW_ASSERT( !key.isEmpty() );

	QTextImageFormat imageFormat = d_func()->cloneTextImageFormat(format, key);
	document()->addResource(QTextDocument::ImageResource, QUrl(key), image);
	QTextCursor::insertImage( imageFormat );
}

void DuiTextCursor::insertHtml( const QString &html )
{
	QTextCursor::insertHtml(html);
}

void DuiTextCursor::setGifPool( GifPool* gifPool )
{
	d_func()->m_gifPool = gifPool;
}

void DuiTextCursor::replaceKeyImage( const QString &imageKey, const QString &imagePath )
{
	Q_D(DuiTextCursor);
	DW_ASSERT( d->m_gifPool );

	d->m_gifPool->replaceKeyImage( imageKey, imagePath );
}

GifPool* DuiTextCursor::gifPool() const
{
	return d_func()->m_gifPool;
}

void DuiTextCursor::setTextCursor( const QTextCursor &tc )
{
	QTextCursor::operator =( tc );
}

void DuiTextCursor::intellisensInsertText( const QString & text, const QTextCharFormat & format )
{
	Q_D(DuiTextCursor);
	DW_ASSERT( d->m_edit );

	beginEditBlock();

	QTextCharFormat textFormat;
	if ( format.isValid() )
	{
		textFormat = format;
	}
	else
	{
		textFormat = d->m_edit->charFormat();
	}

	d->m_edit->intellisensInsertText( text, *this, textFormat );

	endEditBlock();
}

void DuiTextCursor::insertTextLink( const QString &linkKey, const QString &link, 
								    const QString &displayName, const QColor &color /*= Qt::blue */ )
{
	Q_D(DuiTextCursor);

	QTextCharFormat linkFormat = d->m_edit->charFormat();

	linkFormat.setFontUnderline( true );
	linkFormat.setForeground( color );

	insertTextLink(linkKey, link, displayName, linkFormat);
}

void DuiTextCursor::insertTextLink( const QString &linkKey, const QString &link, const QString &displayName, const QTextCharFormat &format )
{

	QTextCharFormat linkFormat = format;

	linkFormat.setAnchor( true );
	linkFormat.setAnchorName( displayName );
	linkFormat.setAnchorHref( linkKey );
	linkFormat.setProperty( KLinkValue, link );

	insertText( displayName, linkFormat );
}

void DuiTextCursor::insertImageLink( const QString &linkKey, const QString &imageKey , const QString &imagePath )
{
	QTextImageFormat imageFormat;

	imageFormat.setAnchor(true);
	imageFormat.setAnchorHref(linkKey);
	imageFormat.setName(imagePath);
	imageFormat.setProperty( KLinkValue, imageKey );

	insertImage( imageFormat, imageKey );
}

void DuiTextCursor::insertCompositeData( const DuiTextCompositeFormat & composite )
{
	composite.parseInput( *this );
}

void DuiTextCursor::insertText( const QString &text, const QTextCharFormat &format )
{
	QTextCursor::insertText(text,format);
}

void DuiTextCursor::insertText( const QString &text )
{
	Q_D(DuiTextCursor);
	QTextCursor::insertText(text, d->m_edit->charFormat() );
}

//////////////////////////////////////////////////////////////////////////

class DuiTextCompositeFormatPrivate
{
public:
	enum CompositeType
	{
		TextFormatType,
		LinkFormatType,
		ImageFormatType
	};

	struct ContentData
	{
		CompositeType			Type;
		QMap<QString, QVariant> Attributes;
	};

	DuiTextCompositeFormatPrivate();
	~DuiTextCompositeFormatPrivate();

	QList< ContentData > m_comositeDatas;
};

DuiTextCompositeFormatPrivate::DuiTextCompositeFormatPrivate()
{

}

DuiTextCompositeFormatPrivate::~DuiTextCompositeFormatPrivate()
{

}

DuiTextCompositeFormat::DuiTextCompositeFormat()
: d_ptr(new DuiTextCompositeFormatPrivate)
{

}

DuiTextCompositeFormat::DuiTextCompositeFormat( const DuiTextCompositeFormat& other )
: d_ptr(NULL)
{
	*this = other;
}

DuiTextCompositeFormat& DuiTextCompositeFormat::operator=( const DuiTextCompositeFormat& other )
{
	if (this != &other)
	{
		if (d_ptr)
		{
			delete d_ptr;
		}
		d_ptr = new DuiTextCompositeFormatPrivate(*(other.d_ptr));
	}
	return *this;
}

DuiTextCompositeFormat::~DuiTextCompositeFormat()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiTextCompositeFormat::addText( const QString &text , const QColor &color )
{
	QTextCharFormat format;
	format.setForeground(color);

	addText(text, format);
}

void DuiTextCompositeFormat::addText( const QString &text, const QTextCharFormat& textFormat )
{
	DuiTextCompositeFormatPrivate::ContentData data;
	data.Type = DuiTextCompositeFormatPrivate::TextFormatType;

	data.Attributes.insert("text", text);

	QVariant var;
	var.setValue(textFormat);
	data.Attributes.insert("textFormat", var );

	d_func()->m_comositeDatas.push_back(data);
}

void DuiTextCompositeFormat::addLink( const QString &linkKey, const QString &linkName, const QString &linkValue, const QColor &linkColor /*= Qt::blue */ )
{
	QTextCharFormat format;
	format.setForeground(linkColor);
	format.setFontUnderline(true);

	addLink(linkKey,linkName,linkValue,format);
}

void DuiTextCompositeFormat::addLink( const QString &linkKey, const QString &linkName, const QString &linkValue, const QTextCharFormat& linkFormat )
{
	DuiTextCompositeFormatPrivate::ContentData data;
	data.Type = DuiTextCompositeFormatPrivate::LinkFormatType;

	data.Attributes.insert("linkKey", linkKey);
	data.Attributes.insert("linkName", linkName);
	data.Attributes.insert("linkValue", linkValue);
	
	QVariant var;
	var.setValue(linkFormat);
	data.Attributes.insert("linkFormat", var );

	d_func()->m_comositeDatas.push_back(data);
}

void DuiTextCompositeFormat::addImage( const QString& imageKey, const QString &imagePath )
{
	DuiTextCompositeFormatPrivate::ContentData data;
	
	data.Type = DuiTextCompositeFormatPrivate::ImageFormatType;

	data.Attributes.insert("imagePath", imagePath);
	data.Attributes.insert("imageKey", imageKey );

	d_func()->m_comositeDatas.push_back(data);
}

void DuiTextCompositeFormat::addImage( const QString& imageKey, const QImage& image )
{
	DuiTextCompositeFormatPrivate::ContentData data;

	data.Type = DuiTextCompositeFormatPrivate::ImageFormatType;

	data.Attributes.insert("imageKey", imageKey);
	data.Attributes.insert("imageData", image );

	d_func()->m_comositeDatas.push_back(data);
}

void DuiTextCompositeFormat::parseInput( DuiTextCursor &textCurosr ) const
{
	Q_D(const DuiTextCompositeFormat);
	QVariant var;

	for ( QList<DuiTextCompositeFormatPrivate::ContentData>::const_iterator it = d_func()->m_comositeDatas.begin();
		  it != d->m_comositeDatas.end(); ++it )
	{
		switch ( it ->Type )
		{
		case DuiTextCompositeFormatPrivate::TextFormatType:
			{
				QString text;
				QColor	color;
				QTextCharFormat textFormat;

				var =  it->Attributes.value("text");

				if ( var.isValid() )
				{
					text = var.toString();
				}

				var = it->Attributes.value("textFormat");
				if ( var.isValid() )
				{
					textFormat = var.value<QTextCharFormat>();
				}

				textCurosr.insertText(text, textFormat);

				break;
			}
		case DuiTextCompositeFormatPrivate::LinkFormatType:
			{
				QString linkKey;
				QString linkValue;
				QString linkName;
				QColor	linkColor;
				QTextCharFormat	linkformat;

				var = it->Attributes.value("linkKey");
				if ( var.isValid() )
				{
					linkKey = var.toString();
				}

				var = it->Attributes.value("linkValue");
				if ( var.isValid() )
				{
					linkValue = var.toString();
				}

				var = it->Attributes.value("linkName");
				if ( var.isValid() )
				{
					linkName = var.toString();
				}

				var = it->Attributes.value("linkFormat");
				if (var.isValid())
				{
					linkformat = var.value<QTextCharFormat>();
				}

				if ( linkformat.isValid() )
				{
					textCurosr.insertTextLink(linkKey,linkValue,linkName,linkformat);
				}
				break;
			}
		case DuiTextCompositeFormatPrivate::ImageFormatType:
			{
				QString imageKey;

				var = it->Attributes.value("imageKey");

				if ( var.isValid() )
				{
					imageKey = var.toString();
				}

				var = it->Attributes.value("imagePath");
				if ( var.isValid() )
				{
					DW_ASSERT( !imageKey.isEmpty() );

					textCurosr.insertImage(imageKey, var.toString() );
				}

				break;
			}
		}
	}
}

bool DuiTextCompositeFormat::isNull() const
{	
	return d_func()->m_comositeDatas.isEmpty();
}

QString DuiTextCompositeFormat::toPlainText() const
{
	Q_D(const DuiTextCompositeFormat);

	QVariant var;
	QString plainText;

	for ( QList<DuiTextCompositeFormatPrivate::ContentData>::const_iterator it = d->m_comositeDatas.begin();
		it != d->m_comositeDatas.end(); ++it )
	{
		switch ( it ->Type )
		{
		case DuiTextCompositeFormatPrivate::TextFormatType:
			{
				var =  it->Attributes.value("text");
				if ( var.isValid() )
				{
					plainText += var.toString();
				}

				break;
			}
		case DuiTextCompositeFormatPrivate::LinkFormatType:
			{
				var = it->Attributes.value("linkName");
				if ( var.isValid() )
				{
					plainText += var.toString();
				}

				break;
			}
		}//switch
	}//for

	return plainText;
}
