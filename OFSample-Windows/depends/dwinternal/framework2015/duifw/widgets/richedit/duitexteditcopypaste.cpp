#include "stable.h"
#include "duitexteditcopypaste.h"


class DuiTextEditCopyPasteHandlerPrivate
{
public:
	explicit DuiTextEditCopyPasteHandlerPrivate();
	~DuiTextEditCopyPasteHandlerPrivate();

	DuiTextEditCopyPasteHandler	*m_nextChain;
	DuiTextEdit					*m_textEdit;
};

DuiTextEditCopyPasteHandlerPrivate::DuiTextEditCopyPasteHandlerPrivate()
: m_nextChain(NULL)
, m_textEdit(NULL)
{

}

DuiTextEditCopyPasteHandlerPrivate::~DuiTextEditCopyPasteHandlerPrivate()
{

}

//////////////////////////////////////////////////////////////////////////

DuiTextEditCopyPasteHandler::DuiTextEditCopyPasteHandler( DuiTextEdit* textEdit )
: d_ptr( new DuiTextEditCopyPasteHandlerPrivate)
{
	d_func()->m_textEdit = textEdit;
}

DuiTextEditCopyPasteHandler::~DuiTextEditCopyPasteHandler()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiTextEditCopyPasteHandler::setNextChain( DuiTextEditCopyPasteHandler* nextCopyPaste )
{
	d_func()->m_nextChain = nextCopyPaste;
}

void DuiTextEditCopyPasteHandler::beginCopyData( int pos, const QString& copyText, QDataStream& stream)
{
	Q_D(DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		d->m_nextChain->beginCopyData( pos, copyText, stream );
	}
}

void DuiTextEditCopyPasteHandler::beginCopyData( QMimeData *data )
{
	Q_D(DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		d->m_nextChain->beginCopyData(data);
	}
}

void DuiTextEditCopyPasteHandler::beginPasteData( const QString& copyText, DuiTextCursor& tc, QDataStream& stream)
{
	Q_D(DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		d->m_nextChain->beginPasteData(copyText,tc,stream);
	}
}

void DuiTextEditCopyPasteHandler::beginPasteData(const QMimeData * mimeData)
{
	Q_D(DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		d->m_nextChain->beginPasteData(mimeData);
	}
}

void DuiTextEditCopyPasteHandler::beginCopyToHtml( int pos, const QString& copyText, QString& html )
{
	Q_D(DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain)
	{
		d->m_nextChain->beginCopyToHtml(pos,copyText,html);
	}
}

bool DuiTextEditCopyPasteHandler::canPasteData( const QString& imieType) const
{
	Q_D(const DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		return d->m_nextChain->canPasteData(imieType);
	}

	return false;
}

bool DuiTextEditCopyPasteHandler::canPasteData( const QMimeData *data ) const
{
	Q_D(const DuiTextEditCopyPasteHandler);
	if ( d->m_nextChain )
	{
		return d->m_nextChain->canPasteData(data);
	}
	return false;
}

DuiTextEditCopyPasteHandler* DuiTextEditCopyPasteHandler::nextChain() const
{
	return d_func()->m_nextChain;
}

DuiTextEdit* DuiTextEditCopyPasteHandler::textEdit() const
{
	return d_func()->m_textEdit;
}
