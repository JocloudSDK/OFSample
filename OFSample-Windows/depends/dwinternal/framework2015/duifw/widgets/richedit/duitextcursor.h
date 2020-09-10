
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

const char DEFAULT_OPEN_FILE_KEY[]	= "DEFAULT_OPEN_FILE_KEY";
const char DEFAULT_OPEN_FOLDER_AND_SELECT_FILE_KEY[]= "DEFAULT_OPEN_FOLDER_AND_SELECT_FILE_KEY";


const int KImagePath			= QTextFormat::UserProperty + 1;
const int KLinkValue			= QTextFormat::UserProperty + 2;
const int KAnimationImage		= QTextFormat::UserProperty + 3;
const int KCopyText				= QTextFormat::UserProperty + 4;

const int KTextUserProperty	= QTextFormat::UserProperty + 100;
const int KTextUserObject	= QTextFormat::UserObject + 100;
const int KTextUserFormat	= QTextFormat::UserFormat + 100;

class DuiTextCursor;
class DuiTextCompositeFormatPrivate;
class DW_DUIFW_EXPORT DuiTextCompositeFormat
{
	Q_DECLARE_PRIVATE( DuiTextCompositeFormat );
	friend DuiTextCursor;

public:
	DuiTextCompositeFormat();
	DuiTextCompositeFormat(const DuiTextCompositeFormat& other);	
	DuiTextCompositeFormat& operator=(const DuiTextCompositeFormat& other);
	virtual ~DuiTextCompositeFormat();

	void addText( const QString &text , const QColor &color);
	void addText( const QString &text, const QTextCharFormat& textFormat);

	void addLink( const QString &linkKey, const QString &linkName,
				  const QString &linkValue, const QColor &linkColor = Qt::blue );

	void addLink( const QString &linkKey, const QString &linkName,
				  const QString &linkValue, const QTextCharFormat& linkFormat );

	void addImage( const QString &imageKey, const QString &imagePath );
	void addImage( const QString &imageKey, const QImage& image );

	bool isNull() const;

	QString toPlainText() const;

protected:
	virtual void parseInput( DuiTextCursor &textCurosr ) const;

protected:
	DuiTextCompositeFormatPrivate	*d_ptr;
};

Q_DECLARE_METATYPE(QTextCharFormat)

class GifPool;
class RichTextEdit;
class DuiTextCursorPrivate;

class DW_DUIFW_EXPORT DuiTextCursor : public QTextCursor
{
	Q_DECLARE_PRIVATE( DuiTextCursor );
public:
	explicit DuiTextCursor ( RichTextEdit *edit );
	DuiTextCursor ( void );
	DuiTextCursor ( const DuiTextCursor &other);
	DuiTextCursor ( RichTextEdit *edit, const QTextCursor &tc );
	DuiTextCursor&	operator=( const DuiTextCursor& other);
	~DuiTextCursor( void );

	
public:
	void	setTextCursor( const QTextCursor &tc );

	/* 
	while insert the resource image, need to set the key value.

	notice: when you get the animation file name according to QTextImageFormat,
			use property( KImagePath ) to get it. 
			do not use name() to get the Resource's animation image.

	eg.
	format.name = ":/theme/animation.gif";
	insertImage( format, youKey );

	all the insertImage function will call insertImage(QTextImageForamt,QString) in the end.
	*/
	void	insertImage ( const QTextImageFormat &format, const QString &key = QString() );
	void	insertImage ( const QString &imagePath );
	void	insertImage ( const QString &key, const QString &imagePath );
	void	insertImage ( const QString &key, const QByteArray &data);
	void	insertImage ( const QString &key, const QByteArray &data, const QTextImageFormat &format);
	void	insertImage ( const QString &key, const QImage &image, const QTextImageFormat &format = QTextImageFormat() );

	void	insertHtml	( const QString &html );
	void	setGifPool( GifPool* gifPool );

	void	replaceKeyImage( const QString &imageKey, const QString &imagePath );

	// insert a simple text link, when click the link, 
	// you will receive the editor's signal "linkClicked(linkKey,linkValue)"
	void	insertTextLink( const QString &linkKey, const QString &link, 
							const QString &displayName, const QColor &color = Qt::blue );
	
	void	insertTextLink( const QString &linkKey, const QString &link,
							const QString &displayName, const QTextCharFormat &format );

	void	insertImageLink( const QString &linkKey, const QString &imageKey, const QString &imagePath );

	void	insertCompositeData( const DuiTextCompositeFormat & composite );

	void	intellisensInsertText ( const QString & text, const QTextCharFormat & format = QTextCharFormat() );
	
	void	insertText( const QString &text, const QTextCharFormat &format);
	
	void	insertText( const QString &text );

	GifPool* gifPool() const;

protected:
	DuiTextCursorPrivate *d_ptr;
};

