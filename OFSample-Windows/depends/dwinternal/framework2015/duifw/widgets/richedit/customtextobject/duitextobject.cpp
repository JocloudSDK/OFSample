#include "stable.h"

#include "duifw/duitextcursor.h"
#include "../gifpool.h"
#include "./animation/duitexteditgraphicsgifstate.h"
#include "duitexteditgraphicsitem.h"
#include "duitexteditgraphicsitem_p.h"
#include "duitextobject.h"
#include "dwbase/log.h"

//////////////////////////////////////////////////////////////////////////

typedef QList<DuiTextEditGraphicsItem*> GraphicsItemList;

class DuiTextObjectPrivate
{
	Q_DECLARE_PUBLIC( DuiTextObject );

public:
	DuiTextObjectPrivate();
	virtual ~DuiTextObjectPrivate();

	void clear();
	void init(DuiTextEdit* editor);
	void setAnimationTextObject( const QString& gifPath, const quint32 textObjectIndex );

protected:
	DuiTextObject *q_ptr;

	QHash<int, GraphicsItemList* >	m_graphicsItemListHash;
	DuiTextEdit	*m_edit;
	QHash<int, QRectF> m_objectHash;	
};

DuiTextObjectPrivate::DuiTextObjectPrivate()
: q_ptr( NULL )
, m_edit( NULL )
{

}

DuiTextObjectPrivate::~DuiTextObjectPrivate()
{
	clear();
}

void DuiTextObjectPrivate::clear()
{
	QList<GraphicsItemList*> list =  m_graphicsItemListHash.values();

	for ( int i = 0; i < list.count(); i++ )
	{
		GraphicsItemList* graphicsItemList = list.at( i );
		if ( graphicsItemList == NULL )
		{
			continue;
		}

		if ( graphicsItemList->count() )
		{
			qDeleteAll( *graphicsItemList );
			graphicsItemList->clear();
		}

		delete graphicsItemList;
		graphicsItemList = NULL;
	}

	m_graphicsItemListHash.clear();	
}

void DuiTextObjectPrivate::init(DuiTextEdit* editor)
{
	m_edit = editor;
	if (m_edit && m_edit->document())
	{
		q_func()->setParent(m_edit->document());
		QObject::connect(m_edit->realWidget(), SIGNAL(textEditCleared()), q_func(), SLOT(on_textEditCleared()));
	}
}

void DuiTextObjectPrivate::setAnimationTextObject( const QString& gifPath, const quint32 textObjectIndex )
{
	QStringList list;
	if ( q_ptr->property(KAnimationTextObject).isValid() )
	{
		list = q_ptr->property(KAnimationTextObject).toStringList();
	}

	if (list.indexOf(gifPath) == -1 )
	{
		list.append(gifPath);
	}

	q_ptr->setProperty(KAnimationTextObject, list);

	if (m_edit->textCursor().blockCharFormat().objectType() == QTextFormat::TableCellObject)
	{
		if (m_edit->textCursor().charFormat().property(KTextObjectIndex).toUInt() == textObjectIndex)
		{
			QTextCharFormat charFormat = m_edit->textCursor().blockCharFormat();
			charFormat.setProperty(KTextObjectIndex, textObjectIndex);
			m_edit->textCursor().setBlockCharFormat(charFormat);
		}
	}
	else
	{
		m_edit->textCursor().setBlockCharFormat(m_edit->textCursor().charFormat());
	}
}

//////////////////////////////////////////////////////////////////////////

DuiTextObject::DuiTextObject( DuiTextEdit* editor ) 
: d_ptr( new DuiTextObjectPrivate )
{
	d_ptr->q_ptr = this;
	d_func()->init(editor);
}

DuiTextObject::DuiTextObject( DuiTextObjectPrivate &dd, DuiTextEdit* editor )
: d_ptr( &dd )
{
	d_ptr->q_ptr = this;
	d_func()->init(editor);
}

DuiTextObject::~DuiTextObject()
{
	DW_ASSERT_X(d_ptr, __FUNCTION__, "private data is null.");
	delete d_ptr;
	d_ptr = NULL;
}

QSizeF DuiTextObject::intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format ) const
{
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	return QSizeF( 80.0, 36.0 );
}

void DuiTextObject::drawObject(QPainter *painter, 
												const QRectF &rect, 
												QTextDocument *doc, 
												int posInDocument, 
												const QTextFormat &format )
{
	PerfLogF(F_DUI);
	Q_D( DuiTextObject );

	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	Q_UNUSED( painter );
	Q_UNUSED( rect );
	Q_UNUSED( d );

	int textObjectIndex = format.property( KTextObjectIndex ).toUInt();

	GraphicsItemList* graphicsItemList = d->m_graphicsItemListHash.value( textObjectIndex );
	if ( graphicsItemList == NULL )
	{
		return ;
	}

	 d->m_objectHash.insert(textObjectIndex, rect);
	
	for ( int i = 0; i < graphicsItemList->count(); i++ )
	{
		DuiTextEditGraphicsItem* item = graphicsItemList->at( i );
		if ( item == NULL || !item->isVisible() )
		{
			continue;
		}

		item->paint( painter, rect, doc, posInDocument, format );
	}
}

void DuiTextObject::appendItem2( DuiTextEditGraphicsItem* item )
{
	Q_D(DuiTextObject);

	if ( item->textObjectIndex() == -1 )
	{
		DW_ASSERT( false );
		return;
	}

	item->_setContainerWidget( d->m_edit->realWidget() );
	QString gifImagePath = item->gifImagePath();

	if (!gifImagePath.isEmpty())
	{
		QByteArray gifImageData = item->gifImageData();
		if (GifPool::isAnimationGif(gifImageData))
		{
			d->m_edit->gifPool()->insertKeyImage( gifImagePath, gifImageData );
			d->setAnimationTextObject(gifImagePath, item->textObjectIndex());
		}
		else if (GifPool::isAnimationGif(gifImagePath) )
		{
			d->m_edit->gifPool()->insertKeyImage( gifImagePath, gifImagePath );
			d->setAnimationTextObject(gifImagePath, item->textObjectIndex());
		}
	}

	GraphicsItemList* graphicsItemList	=	NULL;

	if ( ! d->m_graphicsItemListHash.contains( item->textObjectIndex() ) )
	{
		graphicsItemList = new GraphicsItemList;
		d->m_graphicsItemListHash.insert( item->textObjectIndex(), graphicsItemList );
	}
	else
	{
		graphicsItemList = d->m_graphicsItemListHash.value( item->textObjectIndex() );
	}

	graphicsItemList->append( item );
}

void DuiTextObject::appendItem( DuiTextEditGraphicsItem* item )
{
	Q_D(DuiTextObject);

	if ( item->textObjectIndex() == -1 )
	{
		DW_ASSERT( false );
		return;
	}

	item->_setContainerWidget( d->m_edit->realWidget() );
	QString gifImagePath = item->gifImagePath();
	if ( !gifImagePath.isNull() || GifPool::isAnimationGif(gifImagePath) )
	{
		d->m_edit->gifPool()->insertKeyImage( gifImagePath, gifImagePath );
		d->setAnimationTextObject(gifImagePath, item->textObjectIndex());
	}

	GraphicsItemList* graphicsItemList	=	NULL;

	if ( ! d->m_graphicsItemListHash.contains( item->textObjectIndex() ) )
	{
		graphicsItemList = new GraphicsItemList;
		d->m_graphicsItemListHash.insert( item->textObjectIndex(), graphicsItemList );
	}
	else
	{
		graphicsItemList = d->m_graphicsItemListHash.value( item->textObjectIndex() );
	}

	graphicsItemList->append( item );
}

void DuiTextObject::commonEvent( QMouseEvent* event, quint32 textObjectIndex )
{
	PerfLogF(F_DUI);
	Q_UNUSED( event );
	Q_UNUSED( textObjectIndex );

	Q_D( DuiTextObject );

	GraphicsItemList* graphicsItemList = d->m_graphicsItemListHash.value( textObjectIndex );
	if ( graphicsItemList == NULL )
	{
		return ;
	}

	QRectF rect = d->m_objectHash.value( textObjectIndex );
	if ( rect.isNull() )
	{
		return;
	}

	const QPointF mousePos = d_func()->m_edit->mapToContents( event->pos() );

	for ( int i = 0; i < graphicsItemList->count(); i ++ )
	{
		DuiTextEditGraphicsItem* item =  graphicsItemList->at( i );
		if ( item == NULL )
		{
			continue;
		}

		const QPointF pos = item->absolutePos( rect );
		const QRectF itemRect( pos,  item->size() );
		if ( itemRect.isNull() )
		{
			continue;
		}
		

		//qDebug() << "TextObjectIndex=" << textObjectIndex << "DuiTextObject::commonEvent" <<  "event type=" << event->type() << "found Item Rect =" << itemRect << event->pos() << rect;

		switch ( event->type() )
		{
		case QEvent::MouseButtonPress:
			if ( item->acceptedMouseButtons() != Qt::NoButton 
				&& ((item->acceptedMouseButtons() & event->button()) != 0)
				&& itemRect.contains( mousePos ) 
				)
			{
				item->mousePressEvent( static_cast<QMouseEvent *>(event), textObjectIndex );
				item->d_func()->setPressStatus(true);
                emit mousePress(static_cast<QMouseEvent*>(event), item->objectName(), textObjectIndex);
			}
			
			break;
		case  QEvent::MouseButtonRelease:
			if ( item->acceptedMouseButtons() != Qt::NoButton  
				&& ((item->acceptedMouseButtons() & event->button()) != 0)
				&& itemRect.contains( mousePos ) 
				)
			{
				item->mouseReleaseEvent( static_cast<QMouseEvent *>(event), textObjectIndex );
				DW_ASSERT( item->objectName().length() );
				
				if (item->d_func()->pressStatus())
				{
					emit clicked( item->objectName(), textObjectIndex );
					item->d_func()->setPressStatus(false);
				}
			}
			
			break;
		case QEvent::MouseMove:
			if ( itemRect.contains( mousePos ) )
			{
				item->mouseMoveEvent( static_cast<QMouseEvent *>(event), textObjectIndex );
				item->_dispatchHoverEvent( static_cast<QMouseEvent *>(event), true, textObjectIndex );	
			}
			else
			{
				item->_dispatchHoverEvent( static_cast<QMouseEvent *>(event), false, textObjectIndex );
			}
			
			break;
		}
	}

	

}

void DuiTextObject::removeItem(int nTextObjectIndex)
{
    Q_D( DuiTextObject );
    GraphicsItemList* p_list = d->m_graphicsItemListHash.value( nTextObjectIndex, NULL );
    if(p_list == NULL)
    {
        return;
    }
    qDeleteAll(*p_list);
    p_list->clear();
    d->m_graphicsItemListHash.remove(nTextObjectIndex);
    delete p_list;
}

void DuiTextObject::removeItem(int nTextObjectIndex, const QString& strObjName)
{
    Q_D( DuiTextObject );
    GraphicsItemList* p_list = d->m_graphicsItemListHash.value( nTextObjectIndex, NULL );
    if(p_list == NULL)
    {
        return;
    }
    for(GraphicsItemList::iterator it = p_list->begin(); it != p_list->end(); )
    {
        DuiTextEditGraphicsItem* p_item = (*it);
        if(p_item != NULL && p_item->objectName() == strObjName)
        {
            it = p_list->erase(it);
        }
        else ++it;
    }
    if(p_list->isEmpty())
    {
        d->m_graphicsItemListHash.remove(nTextObjectIndex);
        delete p_list;
    }
}


DuiTextEditGraphicsItem* DuiTextObject::findChild( int textObjectIndex, const QString &aName ) const
{
	Q_D( const DuiTextObject );
	GraphicsItemList* graphicsItemList = d->m_graphicsItemListHash.value( textObjectIndex , NULL );
	if ( graphicsItemList == NULL )
	{
		return NULL;
	}

	for ( int i = 0; i < graphicsItemList->count(); i++ )
	{
		DuiTextEditGraphicsItem* item = graphicsItemList->at( i );
		if ( item && aName == item->objectName() )
		{
			return item;
		}
	}

	return NULL;
}

bool DuiTextObject::updateGifImage( quint32 textObjectIndex, quint32 &animationItemCount )
{
	PerfLogF(F_DUI);

	Q_D(DuiTextObject);

	GraphicsItemList* graphicsItemList = d->m_graphicsItemListHash.value( textObjectIndex , NULL );
	if ( graphicsItemList == NULL )
	{
		return false;
	}

	int changes = 0;
	animationItemCount = 0;
	foreach(DuiTextEditGraphicsItem* item, *graphicsItemList)
	{
		if ( item == NULL || item->gifImagePath().isEmpty() )
		{
			continue;
		}
		
		++animationItemCount;
		bool gifUpdated	= false;
		QPixmap pixmap = d->m_edit->gifPool()->updateGif(item->gifImagePath(), gifUpdated);
		if (pixmap.isNull())
		{
			continue;
		}

		DuiTextEditGraphicsGifState gifState;
		gifState.setChangedPixmap( pixmap );
		item->animationChanged( &gifState );

		changes++;
	}

	return changes > 0;
 }

bool DuiTextObject::updateGifImage( quint32 textObjectIndex )
{
    quint32 animationItemCount;
	return updateGifImage(textObjectIndex, animationItemCount);
}

void DuiTextObject::removeAllItem()
{
	d_func()->clear();
}

void DuiTextObject::on_textEditCleared()
{
	removeAllItem();
}


