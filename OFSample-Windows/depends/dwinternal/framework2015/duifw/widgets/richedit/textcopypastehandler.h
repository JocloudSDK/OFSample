#pragma once
#include "dwglobal.h"
#include "duitexteditcopypaste.h"

class DW_DUIFW_EXPORT YYTextCopyPasteHandler : public QObject
											  , public DuiTextEditCopyPasteHandler
{
public:
	explicit YYTextCopyPasteHandler( DuiTextEdit *parent );

protected:
	virtual bool canPasteData(const QString& imieType) const;
	virtual bool canPasteData(const QMimeData *data ) const;
	virtual void beginCopyData( QMimeData *data );
	virtual void beginCopyToHtml( int pos, const QString& copyText, QString& html );
	virtual void beginCopyData( int pos, const QString& copyText, QDataStream& stream );
	virtual void beginPasteData(const QString& imieType, DuiTextCursor &tc, QDataStream &bytes );
	virtual void beginPasteData( const QMimeData *data );

private:
	void _replaceNewLine( QString& text, const QString& replaceText );
};