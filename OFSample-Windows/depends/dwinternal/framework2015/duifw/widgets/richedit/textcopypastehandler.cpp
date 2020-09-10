#include "stable.h"
#include "textcopypastehandler.h"
#include "duitextedit.h"
#include "duitextcursor.h"

const char KTextStart[] = "[Text]";
const char KTextEnd[]	="[/Text]";

YYTextCopyPasteHandler::YYTextCopyPasteHandler( DuiTextEdit *parent /*= NULL */ )
: QObject(parent)
, DuiTextEditCopyPasteHandler(parent)
{

}

void YYTextCopyPasteHandler::beginCopyData( int pos, const QString& copyText, QDataStream& stream )
{
	Q_UNUSED(pos);
	Q_UNUSED(copyText);
	Q_UNUSED(stream);
	
	QString text = copyText;
	_replaceNewLine(text, "\n");

	if ( !text.isEmpty() )
	{
		stream << QString(KTextStart) << text << QString(KTextEnd);
	}
}

void YYTextCopyPasteHandler::beginCopyData( QMimeData *data )
{
	DuiTextCursor tc = textEdit()->textCursor();

	QString text = tc.selectedText();

	int selectStartPos = tc.selectionStart();
	
	int index = 0;
	int start = 0;

	QString copyOutText;

	while ( (index = text.indexOf(QChar(QChar::ObjectReplacementCharacter), index)) != -1 )
	{
		if ( start < index )
		{
			copyOutText += text.mid(start, index - start);
			
			start = index + 1;
		}

		tc.setPosition( selectStartPos + index + 1);
		QTextCharFormat format = tc.charFormat();
		copyOutText += format.stringProperty(KCopyText);

		index++;
	}

	copyOutText += text.mid(start);

	_replaceNewLine(copyOutText, "\n");

	data->setText(copyOutText);

	DuiTextEditCopyPasteHandler::beginCopyData(data);
}

void YYTextCopyPasteHandler::beginPasteData( const QString& imieType, DuiTextCursor &tc, QDataStream &stream )
{
	if ( textEdit()->isReadOnly() )
	{
		return ;
	}

	if ( imieType == KTextStart )
	{
		QString text;
		QString textEnd;

		stream >> text >> textEnd;
		DW_ASSERT( textEnd == KTextEnd );
		
		if ( !text.isEmpty() )
		{
			tc.insertText(text);
		}
	}
	else if ( nextChain() )
	{
		nextChain()->beginPasteData(imieType, tc, stream);
	}
}

void YYTextCopyPasteHandler::beginPasteData( const QMimeData *source )
{
	if ( source->hasText() )
	{
		DuiTextCursor tc = textEdit()->textCursor();
		tc.insertText( source->text() );
	}

	if ( nextChain() )
	{
		nextChain()->beginPasteData(source);
	}
}

void YYTextCopyPasteHandler::beginCopyToHtml( int pos, const QString& copyText, QString& html )
{
	Q_UNUSED(pos);

	QString planText = Qt::escape(copyText);
	_replaceNewLine(planText, "<br>");

	html += planText;
}

bool YYTextCopyPasteHandler::canPasteData( const QString& imieType) const
{
	if ( !textEdit()->isReadOnly() )
	{

		if ( imieType == KTextStart )
		{
			return true;
		}
	}

	if ( nextChain() )
	{
		return nextChain()->canPasteData( imieType );
	}

	return false;
}

bool YYTextCopyPasteHandler::canPasteData( const QMimeData *data ) const
{
	if ( !textEdit()->isReadOnly() && data->hasText() )
	{
		return true;
	}

	if ( nextChain() )
	{
		return nextChain()->canPasteData(data);
	}

	return false;
}

void YYTextCopyPasteHandler::_replaceNewLine( QString& text, const QString& replaceText )
{
	QString regexp = QString("[%1%2]")
		.arg(QTextBeginningOfFrame)
		.arg(QChar(QChar::ObjectReplacementCharacter));

	text.remove( QRegExp(regexp));

	text.replace(QTextEndOfFrame,replaceText);
	text.replace(QTextEndOfRow, replaceText);
	text.replace(QChar::ParagraphSeparator, replaceText);
}
