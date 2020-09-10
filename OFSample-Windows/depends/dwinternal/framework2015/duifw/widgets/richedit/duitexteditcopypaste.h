#pragma once

#include "dwglobal.h"

class DuiTextEdit;
class DuiTextCursor;
class DuiTextEditCopyPasteHandlerPrivate;

class DW_DUIFW_EXPORT DuiTextEditCopyPasteHandler
{
	Q_DECLARE_PRIVATE( DuiTextEditCopyPasteHandler );
public:
	explicit	DuiTextEditCopyPasteHandler( DuiTextEdit* textEdit );
	virtual		~DuiTextEditCopyPasteHandler();

public:
	void	setNextChain( DuiTextEditCopyPasteHandler* nextCopyPaste );

public:
	//exchange the data with other application.
	virtual void beginCopyToHtml( int pos, const QString& copyText, QString& html );
	//exchange the internal data.
	virtual void beginCopyData( int pos, const QString& copyText, QDataStream& stream );

	virtual void beginCopyData( QMimeData *data );
	virtual void beginPasteData(const QString& imieType, DuiTextCursor &tc, QDataStream &bytes );
	virtual void beginPasteData(const QMimeData *source);

	virtual bool canPasteData( const QString& imieType) const;
	virtual bool canPasteData(const QMimeData *data ) const;

protected:
	DuiTextEditCopyPasteHandler*	nextChain() const;
	DuiTextEdit*					textEdit() const;

protected:
	DuiTextEditCopyPasteHandlerPrivate	*d_ptr;
};