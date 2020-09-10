#include "stable.h"

#include "dwutility/usystem.h"
#include "utility/duitextutility.h"
#include "platform/windows/duiwinapi.h"
#include "customtextobject/duitextobject.h"
#include "duitextcursor.h"
#include "duitexteditcopypaste.h"
#include "richtextbrowser.h"
#include "textimageobject.h"

//////////////////////////////////////////////////////////////////////////
// delay delete the gif from the gif pool. while delete the gif in edit.
const int KDeleteGifDelayTime = 1000;
const int KUpdateDelayTime = 100;

//zoomIcon Sense Area
const int KZoomSenseArea = 30;

class RichTextEditMouseEvent : public QMouseEvent
{
public:
	explicit RichTextEditMouseEvent(const QMouseEvent &ev, const QPoint &offset);
};

RichTextEditMouseEvent::RichTextEditMouseEvent(const QMouseEvent &ev, const QPoint &offset)
	: QMouseEvent(ev)
{
	this->p -= offset;
}

RichTextEdit::RichTextEdit( QWidget *parent )
: QTextBrowser(parent)
, m_isIntellisenseInputing(false)
, m_makeCtrlEnterAsEnter( true )
, m_maxLength( -1 )
, m_maxBytes( -1 )
, m_maxParagraph(-1)
, m_mightDrag(false)
, m_defaultTextCopyPaste(NULL)
, m_isAcceptEnter(true)
, m_defaultIntellisenseInput(this)
, m_bIgnoreContentChange(false)
, m_mightUpdateGif(true)
, m_previousTextObjectIndex(0)
, m_previousTextObjectType(QTextFormat::UserObject)
, m_imageObject(new DuiTextImageObject(this))
{
	init();

	connect( this, SIGNAL( anchorClicked(const QUrl& )), SLOT(on_link_clicked(const QUrl&)));

	connect( &m_delayDeleteTimer, SIGNAL( timeout()), SLOT(on_delayDeleteGif()) );
	connect( &m_delayUpdateTimer, SIGNAL( timeout()), SLOT(on_updateGif()));
	m_delayUpdateTimer.start(KUpdateDelayTime);

	QTextDocument* doc = document();
	DW_ASSERT(doc);

	connect( doc, SIGNAL(contentsChange(int,int,int)),
		SLOT(on_edit_contentsChange(int,int,int)) );

	connect( doc, SIGNAL(contentsChanged()),
		SLOT(on_edit_contentsChanged()) );

	connect( doc, SIGNAL(documentLayoutChanged()),
		SLOT(on_edit_documentLayoutChanged()) );

	connect( doc, SIGNAL(aboutToInsertText(QString&,const QTextCharFormat&, bool&)),
		SLOT(on_edit_aboutToInsertText(QString&,const QTextCharFormat&,bool&)));

	doc->documentLayout()->registerHandler(QTextFormat::ImageObject, m_imageObject);

	QScrollBar *vBar = verticalScrollBar();
	connect( vBar, SIGNAL(valueChanged(int)), SLOT(on_scrollbar_valueChanged(int)) );
    connect( vBar, SIGNAL(destroyed(QObject *)), SLOT(on_vertical_scrollbar_destroy(QObject*)), Qt::QueuedConnection );

	setScrollToLeftOptions(RichTextEdit::HSSKIP_all);
}

RichTextEdit::~RichTextEdit(void)
{
}

DuiTextCursor RichTextEdit::textCursor() const
{
	RichTextEdit *textEdit = const_cast<RichTextEdit*>(this);
	DW_ASSERT(textEdit);

	DuiTextCursor cursor(textEdit, QTextEdit::textCursor());
	
	// use for auto test.
	if ( cursor.isNull() )
	{
		cursor.setTextCursor( QTextCursor(document()));
	}

	DW_ASSERT(!cursor.isNull());
	cursor.setGifPool( const_cast<GifPool*>(&m_gifPool) );
	return cursor;
}

QMimeData* RichTextEdit::createMimeDataFromSelection() const
{
	QMimeData* data = new QMimeData();

	if ( !m_copyPasteHandlerList.isEmpty() )
	{
		DuiTextCursor tc = textCursor();
		QString copyText = tc.selectedText();

		int selectStartPos = tc.selectionStart() + 1;

		if ( tc.hasComplexSelection() )
		{
			// 加table的符号.
			selectStartPos += 1;
		}

		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);

		QString tmp;

		m_copyPasteHandlerList.at(0)->beginCopyData(data);
		m_copyPasteHandlerList.at(0)->beginCopyToHtml(selectStartPos,copyText,tmp);
		m_copyPasteHandlerList.at(0)->beginCopyData(selectStartPos, copyText, stream);

		QString html = "<html><body>";
		html += tmp;
		html += "</body></html>";

		data->setHtml(html);
		data->setData(KTextEditMime,bytes);
	}

	return data;
}

void RichTextEdit::insertFromMimeData( const QMimeData *source )
{
	DW_ASSERT(source);

	if ( m_copyPasteHandlerList.isEmpty() )
	{
		return;
	}

	QByteArray bytes = source->data("MIME/TextEdit");

	DuiTextCursor tc = textCursor();
	tc.beginEditBlock();
	if ( !bytes.isNull() )
	{	
		QDataStream stream(bytes);

		while ( !stream.atEnd() )
		{
			QString imieType;
			stream >> imieType;
			m_copyPasteHandlerList.at(0)->beginPasteData(imieType, tc, stream);
		}
	}
	else
	{
		m_copyPasteHandlerList.at(0)->beginPasteData(source);
	}
	tc.endEditBlock();

	QWidget *parentWidget = this->parentWidget();
	if ( parentWidget )
	{
		parentWidget->activateWindow();
		setFocus();
	}
}

void RichTextEdit::on_delayDeleteGif()
{
	QTextCursor tc = textCursor();
	QTextCharFormat charFormat;
	QString text = toPlainText();

	QHash<QString, int> gifPool;

	for ( int i = 0 ; i < text.length(); i++ )
	{
		if ( text.at( i ) == QChar::ObjectReplacementCharacter )
		{
			tc.setPosition( i + 1 );
			charFormat = tc.charFormat();

			if ( charFormat.isImageFormat() )
			{
				QString imgKey = charFormat.property(QTextCharFormat::ImageName).toString();
				if( !imgKey.isEmpty() )
				{
					gifPool[imgKey]++;
				}
			}
		}
	}

	for (QTextBlock curTextBlock = document()->lastBlock(); 
		curTextBlock.isValid(); 
		curTextBlock = curTextBlock.previous())
	{
		int objectType = curTextBlock.charFormat().objectType();
		if ( objectType > DuiTextObject::objectType )
		{
			DuiTextObject* textObject = static_cast<DuiTextObject*> (document()->documentLayout()->handlerForObject( objectType ));
			if (textObject == NULL)
			{
				continue;
			}
			
			QStringList list = textObject->property(KAnimationTextObject).toStringList();
			foreach (QString item, list) 
			{
				DW_ASSERT(item.length());
				gifPool[item]++;	
			}
		}
	}
		
	m_gifPool.updateGifPool( gifPool );
}

void RichTextEdit::on_edit_contentsChanged()
{
	m_mightUpdateGif = true;
	if ( document()->isEmpty() && m_charFormat.isValid() )
	{
		setCurrentCharFormat( m_charFormat );
	}
}

void RichTextEdit::on_edit_contentsChange( int position, int charsRemoved, int charsAdded )
{
	m_mightUpdateGif = true;
	if ( charsRemoved > 0 )
	{
		m_delayDeleteTimer.start( KDeleteGifDelayTime );
	}

	if ( m_bIgnoreContentChange )
	{
		return;
	}

	if ( !document()->isEmpty() && !m_isIntellisenseInputing )
	{
		beginIntellisenseInput();

		m_InterllisensData.isHandle = false;
		for ( QList<DuiTextAbstractIntellisenseInput*>::iterator it = m_intellisenseInputList.begin() ;
			it != m_intellisenseInputList.end(); ++it )
		{

			if (  charsAdded == 1  || (charsAdded == 0 && charsRemoved == 1) )
			{
				(*it)->intellisenseInputOneChar( m_InterllisensData );
				if ( m_InterllisensData.isHandle )
				{
					break;
				}
			}
			else
			{
				(*it)->intellisenseInputText(position, charsAdded, charsRemoved );
			}
		}
		endIntellisenseInput();
	}
}

void RichTextEdit::on_edit_documentLayoutChanged()
{
	document()->documentLayout()->registerHandler(QTextFormat::ImageObject, m_imageObject);
}

void RichTextEdit::contextMenuEvent( QContextMenuEvent * event )
{
	Q_UNUSED(event);
	event->accept();
	emit contextMenu( event );
}

bool RichTextEdit::canInsertFromMimeData( const QMimeData *source ) const
{
	Q_UNUSED(source);

	if ( m_copyPasteHandlerList.isEmpty() )
	{
		return false;
	}

	QByteArray bytes = source->data(KTextEditMime);
	QDataStream stream(bytes);

	QString imieType;
	QString data;

	while ( !stream.atEnd() )
	{
		stream >> imieType >> data;

		if ( m_copyPasteHandlerList.at(0)->canPasteData(imieType) )
		{
			return true;		
		}
	}

	return  m_copyPasteHandlerList.at(0)->canPasteData(source);
}

void RichTextEdit::scrollToEnd()
{
	m_mightUpdateGif = true;
	QScrollBar *vBar = verticalScrollBar();
	if ( vBar != NULL )
	{
		vBar->setSliderPosition( vBar->maximum() );
	}
	scrollToLeft(HSSKIP_scrolltoend);
}

void RichTextEdit::scrollToTop()
{
	m_mightUpdateGif = true;
	QScrollBar *vBar = verticalScrollBar();
	if ( vBar != NULL )
	{
		vBar->setSliderPosition( vBar->minimum() );
	}
	scrollToLeft(HSSKIP_scrolltotop);
}

void RichTextEdit::on_scrollbar_valueChanged(int)
{
	m_mightUpdateGif = true;
}

void RichTextEdit::on_vertical_scrollbar_destroy(QObject* o)
{
    QScrollBar* vBar = verticalScrollBar();
    if (vBar && qobject_cast<QObject*>(vBar) != o)
    {
        connect( vBar, SIGNAL(valueChanged(int)), SLOT(on_scrollbar_valueChanged(int)) );
        connect( vBar, SIGNAL(destroyed(QObject *)), SLOT(on_vertical_scrollbar_destroy(QObject*)), Qt::QueuedConnection );
    }
}

void RichTextEdit::setAnimationEnabled( bool enable )
{
	m_gifPool.setEnableUpdate( enable );
}

void RichTextEdit::setCharFormat( const QTextCharFormat &format )
{
	if ( m_charFormat == format )
	{
		return ;
	}

	m_charFormat = format;

	QTextCursor cursor = textCursor();

	int pos = cursor.position();
	cursor.select(QTextCursor::Document);

	if ( cursor.hasSelection() )
	{
		cursor.mergeCharFormat(format);
		cursor.clearSelection();
		
		mergeCurrentCharFormat(format);
	}
	else
	{
		setCurrentCharFormat(format);
	}
	cursor.setPosition(pos);
}

QTextCharFormat RichTextEdit::charFormat() const
{
	return m_charFormat;
}

void RichTextEdit::init()
{
	PerfLogF(F_DUI);

	setOpenLinks(false);
	setOpenExternalLinks(true);

	setReadOnly(false);
	//setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextEditorInteraction );

	m_delayDeleteTimer.setSingleShot(true);

	verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
	
	setAcceptRichText(false);

	//set Left-to-Right layout by default
	QTextDocument *doc = document();
	QTextOption textOption = doc->defaultTextOption();
	textOption.setTextDirection(Qt::LeftToRight);
	doc->setDefaultTextOption(textOption);
}

void RichTextEdit::addIntellisenseInput( DuiTextAbstractIntellisenseInput *input )
{
	if ( m_intellisenseInputList.size() != 0 )
	{
		m_intellisenseInputList.last()->setNextChain( input );
	}

	m_intellisenseInputList.push_back(input);
	input->setNextChain( &m_defaultIntellisenseInput );
}

bool RichTextEdit::insertIntellisenseInput(const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input)
{
	if (NULL == input)
	{
		return false;
	}

	for (QList<DuiTextAbstractIntellisenseInput*>::iterator iter = m_intellisenseInputList.begin(); 
		iter != m_intellisenseInputList.end(); 
		++iter)
	{
		if (NULL == *iter)
		{
			continue;
		}

		if (beforeObjectName == (*iter)->objectName())
		{
			input->setNextChain(*iter);
			if (iter != m_intellisenseInputList.begin())
			{
				(*(iter - 1))->setNextChain(input);
			}

			m_intellisenseInputList.insert(iter, input);
			return true;
		}
	}

	return false;
}

void RichTextEdit::intellisensInsertText( const QString &text,DuiTextCursor &tc, const QTextCharFormat &format )
{
    if(!m_intellisenseInputList.isEmpty())
    {
		m_intellisenseInputList.at(0)->intellisenseInputText(text, tc, format );
	}
} 

void RichTextEdit::setIntellisenseInputEnable(bool isEnable)
{
	foreach( DuiTextAbstractIntellisenseInput* input, m_intellisenseInputList)
	{
		input->setEnable(isEnable);
	}
}

//using text cursor foreach
const int KGifImageCountThreshold = 5;
bool RichTextEdit::updateAnimationRect( const QTextBlock& textBlock, const QRect& blockRect)
{
	PerfLogF(F_DUI);
	
	QString blockText = textBlock.text();
	QTextCursor imageTextCursor( document() );
	int offsetY = verticalScrollBar()->sliderPosition();

	int gifImageCount = 0;
	QList<QRect> lstUpdatedRect;
	for (int i = 0; i < blockText.size(); ++i)
	{
		if (blockText.at(i) != QChar::ObjectReplacementCharacter) 
		{
			continue;
		}

		imageTextCursor.setPosition( textBlock.position() + i + 1 );
		QTextCharFormat textCharFormat = imageTextCursor.charFormat();
		if (!textCharFormat.isImageFormat()) 
		{
			continue;
		}

		QString name = textCharFormat.stringProperty(QTextFormat::ImageName);

		bool bUpdated = false;
		QPixmap pixmap = m_gifPool.updateGif(name, bUpdated);
		if (pixmap.isNull())
		{
			continue;
		}
		if (bUpdated)
		{
			document()->addResource( QTextDocument::ImageResource, QUrl(name), pixmap );
		}
	
		QTextLine line = textBlock.layout()->lineForTextPosition(i);
		if (!line.isValid())
		{
			continue;
		}

		if(++gifImageCount > KGifImageCountThreshold)
		{
			//too many images, more than KGifImageCountThreshold, update whole text block.
            continue;
		}
		else
		{
			int left = (int)line.cursorToX(i) + blockRect.x();
			int top = (int)line.y() + blockRect.y() - offsetY;
			int width = pixmap.width();
			int height = (int)line.height();
            lstUpdatedRect.append(QRect(left,top,width,height));
		}
	}
	
    if(gifImageCount > KGifImageCountThreshold)
    {
        //更新整个block，传过来的blockRect好像没有包含block前面的可能的对齐设置，这里暂时从第一行去取对齐设置。
        int xOffset = 0;
        QTextLine line = textBlock.layout()->lineForTextPosition(0);
        if(line.isValid())
        {//获得第一行的偏移量
            xOffset = (int)line.cursorToX(0);
        }
        QRect rcBlock = blockRect.adjusted(xOffset,-offsetY,xOffset,-offsetY);
        viewport()->update(rcBlock);
    }else
    {
        foreach(QRect rc,lstUpdatedRect)
        {
            viewport()->update(rc);
        }
    }

	return gifImageCount > 0;
}

bool RichTextEdit::_isLikeTableStyle()
{
	if (document()->blockCount() < 2)
	{
		return false;
	}

	QTextBlock lastBlock = document()->lastBlock();
	QTextBlock firstBlock = document()->firstBlock().isValid() ? document()->firstBlock().next() : document()->firstBlock();
	DW_ASSERT(lastBlock.isValid());
	DW_ASSERT(firstBlock.isValid());

	bool lastBlockIsTableObject = ( (lastBlock.charFormat().objectType() == QTextFormat::TableCellObject) || (lastBlock.charFormat().objectType() == QTextFormat::TableObject) );
	bool firstBlockIsTableObject = ( (firstBlock.charFormat().objectType() == QTextFormat::TableCellObject) || (firstBlock.charFormat().objectType() == QTextFormat::TableObject) );

	return ( lastBlockIsTableObject && firstBlockIsTableObject );
}


void RichTextEdit::on_updateGif()
{
	PerfLogF(F_DUI);
	if (!m_mightUpdateGif
		|| !m_gifPool.enableUpdate() 
		|| !isVisible()
		|| rect().height() == 0)
	{
		return;
	}

	m_mightUpdateGif = false;
	int topPos = verticalScrollBar()->sliderPosition();
	int bottomPos = height() + topPos;
	bool isBubble = _isLikeTableStyle();

	for (QTextBlock curTextBlock = document()->lastBlock(); 
		curTextBlock.isValid(); 
		curTextBlock = curTextBlock.previous())
	{
		QRect blockRect = document()->documentLayout()->blockBoundingRect( curTextBlock ).toRect();
		if (blockRect.bottom() < topPos) {
			if (isBubble)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else if (blockRect.top() >= bottomPos) {
			continue;
		}

		int objectType = curTextBlock.charFormat().objectType();
		if ( objectType > DuiTextObject::objectType && curTextBlock.charFormat().hasProperty(KTextObjectIndex) 
			|| objectType == QTextFormat::TableCellObject && curTextBlock.charFormat().hasProperty(KTextObjectIndex)
			)
		{
			if ( _updateTextObjectGifimage(curTextBlock) )
			{
				m_mightUpdateGif = true;
			}
		}

		if ( curTextBlock.isVisible() && curTextBlock.blockFormat().hasProperty(KAnimationImage) )
		{
			if (updateAnimationRect( curTextBlock, blockRect ))
			{
				m_mightUpdateGif = true;
			}
		}
	}
}

void RichTextEdit::beginIntellisenseInput()
{
	m_isIntellisenseInputing = true;
}

void RichTextEdit::endIntellisenseInput()
{
	m_isIntellisenseInputing = false;
}

void RichTextEdit::makeCtrlEnterAsEnter( bool makeCtrlEnterAsEnter )
{
	m_makeCtrlEnterAsEnter = makeCtrlEnterAsEnter;
}

void RichTextEdit::keyPressEvent( QKeyEvent *event )
{
	emit keyPress(event);
}

QString RichTextEdit::toText() const
{
	QString text;
	QString translatedText;
	QChar ch;
	QTextDocument* doc = document();
	int endPos = doc->characterCount() - 1;

	text.reserve( endPos );
	InterllisenseInputData data;

	for ( int i = 0 ; i < endPos ; ++i )
	{
		ch = doc->characterAt(i);

		if ( ch == QChar::ObjectReplacementCharacter )
		{
			for ( QList<DuiTextAbstractIntellisenseInput*>::const_iterator it = m_intellisenseInputList.begin(); 
				it != m_intellisenseInputList.end(); ++it )
			{
				translatedText = (*it)->translateToText( i+1, data );
				if ( data.isHandle )
				{
					text += translatedText;
					data.isHandle = false;
					break;
				}
			}
		}
		else
		{
			text += ch; 
		}
	}

	return text;
}

void RichTextEdit::on_link_clicked( const QUrl& url)
{
	PerfLogF(F_DUI);
	QString linkKey = url.toString();
	QChar ch = linkKey.at(linkKey.length()-1);
	if (ch == ':')
	{
		linkKey += "//";
	}

	QTextCursor tc( document() );
	tc.setPosition( exactPositionFromPoint(m_linkPoint) );

	QTextCharFormat format = tc.charFormat();
	
	if ( !format.isValid() )
	{
		return ;
	}

	if ( format.isAnchor() )
	{
		if ( format.hasProperty( KLinkValue ) )
		{
			QString linkValue = format.property( KLinkValue ).toString();
		
			if ( linkKey == DEFAULT_OPEN_FILE_KEY )
			{
				DwUtility::system::openUrl( ("file:///" + linkValue).toStdWString() );
			}
			else if (linkKey == DEFAULT_OPEN_FOLDER_AND_SELECT_FILE_KEY)
			{
				DuiWinAPI::openFolderWithSelectFile(linkValue);
			}
			else
			{
				emit clickedLink(linkKey, linkValue);
			}
		}
		// if use insert html with anchor will emit this signal.
		else
		{
			emit htmlLinkClicked( linkKey );
		}
	}
}

void RichTextEdit::mousePressEvent( QMouseEvent *ev )
{
	emit mousePress(ev);
}

void RichTextEdit::mouseReleaseEvent( QMouseEvent *ev )
{
	emit mouseRelease(ev);
}

void RichTextEdit::removeLinkInfo( const QString &linkKey, const QString &linkValue )
{
	QTextDocument *doc = document();
	QTextCursor tc;
	
	while ( !( tc = doc->find(linkValue, tc.position()) ).isNull() )
	{
		QTextCharFormat format = tc.charFormat();
		
		if ( format.isAnchor() )
		{
			if ( format.anchorHref() == linkKey )
			{
				QString lvalue = format.property(KLinkValue).toString();
				
				if ( lvalue == linkValue )
				{
					tc.setPosition( tc.position() + linkValue.size(), QTextCursor::KeepAnchor );
					tc.setCharFormat( m_charFormat );
					
					break;
				}
			}
		}
	}
}

void RichTextEdit::setMaxParagraph( int nMaxParagraph )
{
	m_maxParagraph = nMaxParagraph;
}

void RichTextEdit::setMaxLength( int nMaxLen )
{
	m_maxLength = nMaxLen;
	m_maxBytes = -1;
}

void RichTextEdit::setMaxBytes( int nMaxBytes )
{
	m_maxBytes = nMaxBytes;
	m_maxLength = -1;
}

void RichTextEdit::mouseDoubleClickEvent( QMouseEvent *ev )
{
	emit mouseDoubleClick(ev);
}

void RichTextEdit::addCopyPasteHandler( DuiTextEditCopyPasteHandler *handler )
{
	handler->setNextChain( m_defaultTextCopyPaste );

	if ( m_copyPasteHandlerList.first() == m_defaultTextCopyPaste )
	{
		m_copyPasteHandlerList.clear();
		m_copyPasteHandlerList.push_back(handler);
		return;
	}

	m_copyPasteHandlerList.last()->setNextChain(handler);
	m_copyPasteHandlerList.push_back(handler);
}

void RichTextEdit::dragEnterEvent( QDragEnterEvent *e )
{
	QTextEdit::dragEnterEvent(e);
}

void RichTextEdit::dragLeaveEvent( QDragLeaveEvent *event )
{
	QTextEdit::dragLeaveEvent(event);
}

void RichTextEdit::dragMoveEvent( QDragMoveEvent * event )
{
	QTextEdit::dragMoveEvent(event);
}

void RichTextEdit::dropEvent( QDropEvent * event )
{
	QTextEdit::dropEvent(event);
}

void RichTextEdit::setReadOnly( bool isReadOnly )
{
	QTextEdit::setReadOnly( isReadOnly );

	if ( isReadOnly )
	{
		setTextInteractionFlags( Qt::TextBrowserInteraction );
	}
	else
	{
		setTextInteractionFlags( Qt::TextEditorInteraction | Qt::LinksAccessibleByMouse );
	}
    m_bIgnoreContentChange = isReadOnly;
}

void RichTextEdit::setAcceptEnter( bool accept )
{
	m_isAcceptEnter = accept;
}

void RichTextEdit::doDrag(QDrag* drag, Qt::DropAction& action)
{
	__try
	{
		if ( m_bIgnoreContentChange )
		{
			action = drag->exec(Qt::CopyAction);
		} 
		else 
		{
			action = drag->exec(Qt::MoveAction);
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		action = Qt::IgnoreAction;
		LogFinal(LOG::KDuiFw) << "exception occurs when dragging!";
	}
}

void RichTextEdit::beginDrag()
{
	QMimeData *data = createMimeDataFromSelection();

	QDrag *drag = new QDrag(this);
	drag->setMimeData(data);

	Qt::DropAction action;

	QTextCursor selectCursor = textCursor();

	doDrag(drag, action);

	if ( action == Qt::MoveAction )
	{
		QTextCursor dropCursor = textCursor();

		if ( selectCursor.hasSelection() )
		{
			if ( dropCursor.position() < selectCursor.selectionStart() ||
				dropCursor.position() > selectCursor.selectionEnd() )
			{
				selectCursor.removeSelectedText();
			}
		}
	}
}

void RichTextEdit::mouseMoveEvent( QMouseEvent *ev )
{
	emit mouseMove(ev);
}

void RichTextEdit::wheelEvent( QWheelEvent * ev )
{
	if ( ev->modifiers() == Qt::ControlModifier )
	{
		ev->accept();
		return ;
	}

	QTextBrowser::wheelEvent(ev);
}

void RichTextEdit::on_edit_aboutToInsertText( QString& addText, const QTextCharFormat &format, bool& fixup )
{
	fixup = false;
	if ( m_maxParagraph != -1 )
	{
		fixup |= _insertTextWithinMaxParagraph( addText, format );
	}

	if ( m_maxLength != -1 )
	{
		fixup |= _insertTextWithinMaxLength( addText, format );
	}
	else if ( m_maxBytes != -1 )
	{
		fixup |= _insertTextWithinMaxBytes( addText, format );
	}
}

bool RichTextEdit::_insertTextWithinMaxParagraph( QString& addText, const QTextCharFormat &format )
{
	Q_UNUSED( format );

	// 这里有点差异，toPlainText()中的换行是\n
	// addText中的换行有两种情况
	//	1.用户输入的是QChar::ParagraphSeparator
	//	2.剪贴板中粘贴的换行是\n
	int curParagraph = toPlainText().count("\n");

	// 为了统一处理，将addText中的换行统一，应该不影响效果
	addText = addText.replace('\n', QChar::ParagraphSeparator);

	int addParagraph = addText.count(QChar::ParagraphSeparator);
	int remainParagraph = m_maxParagraph - curParagraph;

	if (addParagraph < remainParagraph)
	{
		return false;
	}
	else
	{
		// 插入了带换行的内容，截断超出的内容
		int count = 0, idx = -1;
		do 
		{
			idx = addText.indexOf(QChar::ParagraphSeparator, idx + 1);
			++count;
		} while (count < remainParagraph && idx != -1);

		if (idx != -1)
		{
			addText = addText.left(idx);
		}
	}

	return true;
}

bool RichTextEdit::_insertTextWithinMaxLength( QString& addText, const QTextCharFormat &format )
{
	Q_UNUSED( format );
	DW_ASSERT( m_maxBytes == -1 );

	int curSize = toPlainText().size();
	int remainSize = m_maxLength - curSize;
	if ( remainSize > 0 )
	{
		int addSize = addText.size();

		if( remainSize >= addSize )
		{
			return false;
		}

		if ( remainSize < addSize )
		{
			addText = addText.left( remainSize );
		}
	}
	else
	{
		addText.clear();
	}

	return true;
}

bool RichTextEdit::_insertTextWithinMaxBytes( QString& addText, const QTextCharFormat &format )
{
	Q_UNUSED( format );
	DW_ASSERT( m_maxLength == -1 );

	int curBytes = duitextutils::measureStringByBytes(toPlainText());
	int remainBytes = m_maxBytes - curBytes;

	return duitextutils::fixupStringByBytes( addText, remainBytes );
}

void RichTextEdit::_callTextObjectMouseEvent(int textObjectIndex, int textObjectType, QMouseEvent* event) const
{
	if(textObjectIndex > 0 && textObjectType > QTextFormat::UserObject)
	{
		DuiTextObject *item = static_cast<DuiTextObject*>(document()->documentLayout()->handlerForObject(textObjectType));
		if(item)
		{
			item->commonEvent(event, textObjectIndex);
		}
	}
}

void RichTextEdit::_textObjectEvent( QMouseEvent* event )
{
	DuiTextCursor tc =  cursorFromPoint( event->pos() );
	int curTextObjectIndex = tc.charFormat().property( KTextObjectIndex ).toUInt();	
	int curTextObjectType = tc.charFormat().objectType();

	if(curTextObjectIndex != m_previousTextObjectIndex || curTextObjectType != m_previousTextObjectType)
	{
		//call previous text object;
		_callTextObjectMouseEvent(m_previousTextObjectIndex, m_previousTextObjectType, event);
	}

	//call current text object;
	_callTextObjectMouseEvent(curTextObjectIndex, curTextObjectType, event);
	m_previousTextObjectIndex = curTextObjectIndex;
	m_previousTextObjectType = curTextObjectType;
}

void RichTextEdit::setPlaceholderText( const QString & text )
{
	m_placeHolderText = text;
	QWidget *view = viewport();
	DW_ASSERT(view);
	view->update(rect());
}

QString RichTextEdit::placeholderText() const
{
	return m_placeHolderText;
}

void RichTextEdit::focusInEvent( QFocusEvent *ev )
{
	QTextBrowser::focusInEvent(ev);
	
	emit focusIn(ev);

 	QWidget *view = viewport();
 	DW_ASSERT(view);
 	view->update(rect());
}

void RichTextEdit::focusOutEvent( QFocusEvent *ev )
{
	QTextBrowser::focusOutEvent(ev);

	emit focusOut(ev);

 	QWidget *view = viewport();
 	DW_ASSERT(view);
 	view->update(rect());
}

bool RichTextEdit::event(QEvent *ev)
{
	if(ev->type() == QEvent::HoverEnter)
	{
		hoverEnterEvent((QHoverEvent*) ev);
	}
	else if (ev->type() == QEvent::HoverLeave)
	{
		hoverLeaveEvent((QHoverEvent*) ev);
	}
	//  如果tab是用来焦点切换，那么当控件的父亲窗口是QGraphicsView且有scene时，tab用来scene中焦点切换，本身失去焦点
	//  有代理时qt本身解决了，无需此段代码
	else if (tabChangesFocus() && ev->type() == QEvent::KeyPress && (!window()->graphicsProxyWidget()))
	{
		QKeyEvent *k = (QKeyEvent *)ev;
		if ( k->key() == Qt::Key_Tab || k->key() == Qt::Key_Backtab )
		{
			QGraphicsView *view = qobject_cast<QGraphicsView *>(window());
			if (view && view->scene())
			{
				view->setFocus();
				return QApplication::sendEvent(view->scene(), ev);
			}
		}
	}
	return QTextBrowser::event(ev);
}

void RichTextEdit::hoverEnterEvent(QHoverEvent *event)
{
	emit hoverEnter(event);
}

void RichTextEdit::hoverLeaveEvent(QHoverEvent *event)
{	
	_zoomIconHovered(event->pos());
	emit hoverLeave(event);
}

DuiTextCursor RichTextEdit::linkCursor()
{
	return cursorFromPoint( m_linkPoint );
}

void RichTextEdit::resizeEvent( QResizeEvent *ev )
{
	m_mightUpdateGif = true;
	QScrollBar *vBar = verticalScrollBar();
	bool needScrollToEnd = false;

	if ( vBar && vBar->sliderPosition() == vBar->maximum() )
	{
		needScrollToEnd = true;
	}

	QTextBrowser::resizeEvent(ev);

	if ( needScrollToEnd )
	{
		scrollToEnd();
	}
}

int RichTextEdit::metric(PaintDeviceMetric metric) const
{
	if (metric == PdmDpiX || metric == PdmPhysicalDpiX)
	{
		static int dpiXCached = -1;
		if (dpiXCached == -1)
		{
			HDC hdc = GetDC(NULL);
			dpiXCached = GetDeviceCaps(hdc, LOGPIXELSX);
			ReleaseDC(NULL, hdc);
		}
		return dpiXCached;

	}
	else if (metric == PdmDpiY || metric == PdmPhysicalDpiY)
	{
		static int dpiYCached = -1;
		if (dpiYCached == -1)
		{
			HDC hdc = GetDC(NULL);
			dpiYCached = GetDeviceCaps(hdc, LOGPIXELSY);
			ReleaseDC(NULL, hdc);
		}
		return dpiYCached;
	}
	else
	{
		return QWidget::metric(metric);
	}
}

void RichTextEdit::mousePressEventHandle( QMouseEvent *ev )
{
	_zoomIconPressed(ev->pos());

	const QPoint beforeOffset = contentOffset();

	QTextBrowser::mousePressEvent(ev);

	_selectImageOnMousePress(ev->pos());

	if ( ev->buttons() & Qt::LeftButton )
	{
		m_startDragPoint = ev->pos();
	}

	m_mightDrag = false;

	DuiTextCursor tc = textCursor();
	if ( tc.hasSelection() )
	{
		m_mightDrag = true;
	}

	const QPoint afterOffset = contentOffset();

	RichTextEditMouseEvent tev(*ev, afterOffset - beforeOffset);

	m_linkPoint = tev.pos();

	_textObjectEvent( &tev );
}

void RichTextEdit::mouseReleaseEventHandle( QMouseEvent *ev )
{
	QTextBrowser::mouseReleaseEvent(ev);

	_textObjectEvent( ev );

	scrollToLeft(HSSKIP_mouserelease);
}

void RichTextEdit::scrollToLeft(ScrollToLeftOptions options)
{
	PerfLogF(F_DUI);
	if( !( scrollToLeftOptions() & options) )
	{
		return ;
	}
	QScrollBar *hBar = horizontalScrollBar();
	if ( hBar != NULL )
	{
		hBar->setSliderPosition( 0 );
	}
}

void RichTextEdit::setScrollToLeftOptions(ScrollToLeftOptions options)
{
	m_scrollToLeftOption = options;
}

RichTextEdit::ScrollToLeftOptions RichTextEdit::scrollToLeftOptions(void) const
{
	return m_scrollToLeftOption;
}

void RichTextEdit::mouseDoubleClickEventHandle( QMouseEvent *ev )
{
	QTextBrowser::mouseDoubleClickEvent(ev);
}

void RichTextEdit::keyPressEventHandle( QKeyEvent *event )
{
	if ( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter )		
	{
		if (m_bIgnoreContentChange)
		{
			event->ignore();
			return;
		}

		//设置不接受回车
		if (!m_isAcceptEnter)
		{
			event->ignore();
			return;
		}

		Qt::KeyboardModifiers modifier = event->modifiers();

		// set shift enter as enter default.
		if ( ( modifier & Qt::ShiftModifier) == Qt::ShiftModifier )
		{
			event->setModifiers(Qt::NoModifier);
		}
		else if ( ( modifier & Qt::ControlModifier) == Qt::ControlModifier )
		{
			if (m_makeCtrlEnterAsEnter)
			{
				event->accept();
				emit returnPressed();
				return;
			}
			else
			{
				event->setModifiers(Qt::NoModifier);
			}
		}
		else
		{
			if ( !m_makeCtrlEnterAsEnter )
			{
				event->accept();
				emit returnPressed();

				return;
			}
			else
			{
				event->setModifiers(Qt::NoModifier);
			}
		}
	}

	QTextBrowser::keyPressEvent(event);
}

void RichTextEdit::_selectImageOnMousePress( const QPoint &point)
{
	DuiTextCursor tc = textCursor();
	if ( tc.hasSelection() )
	{
		return;
	}

	int pos = _positionFromPoint(point);
	if ( pos == -1 )
	{
		return ;
	}

	QTextCursor cursor( document() );
	cursor.setPosition( pos );	
	
	QTextCharFormat format = cursor.charFormat();	
	if( !format.isImageFormat() )
	{
		return;	
	}

	if ( format.isAnchor() )
	{
		return;
	}

	cursor.setPosition(pos - 1);
	cursor.setPosition(pos, QTextCursor::KeepAnchor);
	setTextCursor(cursor);
}

void RichTextEdit::setDefaultCopyPase( DuiTextEditCopyPasteHandler* copyPasteHandler )
{
	m_defaultTextCopyPaste = copyPasteHandler;
	m_copyPasteHandlerList.push_back(m_defaultTextCopyPaste);
}

DuiTextCursor RichTextEdit::cursorFromPoint( const QPoint &point )
{
	int pos = _positionFromPoint(point);

	if ( pos == -1 )
	{
		QTextCursor		textCursor = QTextBrowser::cursorForPosition(point);
		DuiTextCursor	tcFuzzy;
		tcFuzzy.setTextCursor(textCursor);
		tcFuzzy.setGifPool( &m_gifPool );

		return tcFuzzy;
	}

	DuiTextCursor tcExact = textCursor();
	QTextDocument *doc = document();

	if ( pos >= doc->characterCount() )
	{
		pos = doc->characterCount() - 1;
	}

	tcExact.setPosition( pos );

	return tcExact;
}

int RichTextEdit::_positionFromPoint( const QPoint &point ) const
{
	QPoint pt = point;
	pt.setY( pt.y() + verticalScrollBar()->sliderPosition() );

	QAbstractTextDocumentLayout *layout = document()->documentLayout();
	int index = layout->hitTest(pt, Qt::ExactHit);
	if (index != -1)
	{
		index++;
	}

	return index;
}

bool RichTextEdit::_filterSequenseSpace( QKeyEvent *event )
{
	if ( event->key() == Qt::Key_Space )
	{
		DuiTextCursor tc = textCursor();
		QTextDocument *doc = document();

		const QTextBlock block = doc->findBlock( tc.position() );
		const QTextLayout *layout = block.layout();

		int relativePos = tc.position() - block.position();

		if ( relativePos > 2 )
		{
			QTextLine line = layout->lineForTextPosition(relativePos);

			qreal space_x = line.cursorToX(relativePos);

			int space_width = fontMetrics().width(" ");

			if ( space_x > ( width() - 2 * space_width ) )
			{
				return true;
			}
		}
	}

	return false;
}

void RichTextEdit::mouseMoveEventHandle( QMouseEvent *ev )
{
	QPointer<RichTextEdit> pThis = this;
	if ( m_mightDrag )
	{
		if ( ev->buttons() & Qt::LeftButton )
		{
			if ((ev->pos() - m_startDragPoint).manhattanLength() > QApplication::startDragDistance())
			{
				beginDrag();
				ev->accept();
			}
		}
	}
	else
	{
		QTextBrowser::mouseMoveEvent(ev);
	}

	//如果pThis为空，QDrag exec 发生异常了
	if (pThis)
	{
		_textObjectEvent( ev );
		_zoomIconHovered( ev->pos() );
	}
}

int RichTextEdit::exactPositionFromPoint( const QPoint& point ) const
{
	return _positionFromPoint(point);
}

void RichTextEdit::clear()
{
	QTextBrowser::clear();
	m_gifPool.clear();
	emit textEditCleared();
}

GifPool* RichTextEdit::gifPool()
{
	return &m_gifPool;
}

bool RichTextEdit::_updateTextObjectGifimage( const QTextBlock &curTextBlock )
{
	quint32 updates = 0;
	quint32 animationItemCount = 0;

	QTextBlock::iterator iter = curTextBlock.begin();
	while ( iter != curTextBlock.end() )
	{
		QTextFragment fragmentItem = iter.fragment();

		int objectType = fragmentItem.charFormat().objectType();
	
		if ( objectType > DuiTextObject::objectType && fragmentItem.charFormat().hasProperty(KTextObjectIndex) )
		{
			quint32 ref = 0;
			if(_updateTextObjectGifimage( objectType, fragmentItem.charFormat().property(KTextObjectIndex).toUInt(), ref ))
			{
				updates ++;
			}
			animationItemCount += ref;
		}
		
		++iter;
	}

	if (updates >0)
	{
		viewport()->update();
	}

	return animationItemCount > 0;
}

bool RichTextEdit::_updateTextObjectGifimage( int objectType, quint32 textObjectIndex, quint32 &animationItemCount )
{
	PerfLogF(F_DUI);
	Q_UNUSED( objectType );
	Q_UNUSED( textObjectIndex );
	bool bUpdatedGif = false;
	DuiTextObject* textObject = static_cast<DuiTextObject*> (document()->documentLayout()->handlerForObject( objectType ));
	if (textObject)
	{
		quint32 ref = 0;
		if (textObject->updateGifImage(textObjectIndex, ref))
		{
			bUpdatedGif = true;
		}
		animationItemCount += ref;
	}
	return bUpdatedGif;
}

bool RichTextEdit::getCtrlEnterAsEnter() const
{
	return m_makeCtrlEnterAsEnter;
}

DuiTextAbstractIntellisenseInput* RichTextEdit::intellisenseInputByName( const QString& name )
{
	foreach( DuiTextAbstractIntellisenseInput* input, m_intellisenseInputList )
	{
		if ( input->objectName() == name )
		{
			return input;
		}
	}
	return NULL;
}

QList<DuiTextAbstractIntellisenseInput*> RichTextEdit::intellisenseInput() const
{
	return m_intellisenseInputList;
}

void RichTextEdit::_zoomIconPressed( const QPoint &point )
{
	if ( _isPointInZoom(point) )
	{
		int pressPos = _positionFromPoint(point);
		DuiTextImageObject *imgObj = static_cast<DuiTextImageObject*>(document()->documentLayout()->handlerForObject(QTextFormat::ImageObject));
		DW_ASSERT(imgObj);
		imgObj->setZoomHovered(false);//把放大镜缩小

 		if ( pressPos == -1 )
 		{
 			update(rect());
 			return;
 		}
		
		QRect blockRect = imgObj->objectRect( pressPos-1 ).toRect();
		QRect updateRect(blockRect.right() - KZoomSenseArea, blockRect.bottom() - KZoomSenseArea, KZoomSenseArea, KZoomSenseArea);
		update(updateRect);

		QPoint ptInDoc(point.x() , point.y() + verticalScrollBar()->sliderPosition() );
		emit zoomIconPressed(ptInDoc);
	}
}

bool RichTextEdit::_isPointInZoom( const QPoint &point )
{
	int pos = _positionFromPoint(point);
	if ( pos == -1 )
	{
		return false;
	}

	QTextCursor cursor( document() );	
	cursor.setPosition( pos );

	QTextCharFormat format = cursor.charFormat();	
	if( !format.isImageFormat() )
	{
		return false;	
	}

	if ( format.isAnchor() )
	{
		return false;
	}

	QTextDocument *doc = document();
	DW_ASSERT(doc);
	QPoint ptInDoc(point.x() , point.y() + verticalScrollBar()->sliderPosition() );

	DuiTextImageObject *imageObj = static_cast<DuiTextImageObject*>(doc->documentLayout()->handlerForObject(QTextFormat::ImageObject));
	DW_ASSERT(imageObj);
	QRectF blockRect = imageObj->objectRect( pos-1 );
	QRectF iconRect(blockRect.right() - KZoomSenseArea, blockRect.bottom() - KZoomSenseArea, KZoomSenseArea, KZoomSenseArea);
	if ( blockRect.height() <= KZoomSenseArea )
	{
		iconRect.setY(blockRect.top());
		iconRect.setHeight(blockRect.height());
	}

	if ( iconRect.contains(ptInDoc) )
	{
		return true;
	}

	return false;
}

void RichTextEdit::_zoomIconHovered( const QPoint &point )
{
	int hoverPos = _positionFromPoint(point);

	DuiTextImageObject *imgObj = static_cast<DuiTextImageObject*>(document()->documentLayout()->handlerForObject(QTextFormat::ImageObject));
	DW_ASSERT(imgObj);
	imgObj->setHoveredPos( hoverPos-1 );
	
	if ( _isPointInZoom(point) )
	{
		if ( !imgObj->isZoomHovered() )
		{
			imgObj->setZoomHovered(true);
			update(rect());
		}
	}
	else
	{
		if ( imgObj->isZoomHovered() )
		{	
			imgObj->setZoomHovered(false);
			update(rect());
		}
	}
}

bool RichTextEdit::hasZoomIcon() const
{
	DuiTextImageObject *p = static_cast<DuiTextImageObject*>(document()->documentLayout()->handlerForObject(QTextFormat::ImageObject));
	DW_ASSERT(p);
	return p->hasZoomIcon();
}

void RichTextEdit::setZoomIcon( bool hasZoomIcon )
{
	DuiTextImageObject *p = static_cast<DuiTextImageObject*>(document()->documentLayout()->handlerForObject(QTextFormat::ImageObject));
	DW_ASSERT(p);
	p->setZoomIcon(hasZoomIcon);
}

void RichTextEdit::paintEvent( QPaintEvent *e )
{
	QTextBrowser::paintEvent(e);
	
	//draw placeHoderText
 	if ( QTextBrowser::toPlainText().isEmpty() ) 
 	{	
  		if ((!hasFocus() || m_bIgnoreContentChange) && !m_placeHolderText.isEmpty())
  		{
			QWidget *view = viewport();
			DW_ASSERT(view);
			
			QPainter p(view);
			QFontMetrics fm = fontMetrics();
			int maxLineCount = (view->height() - 6) / fm.lineSpacing();
			QRect rc( 4, 6, view->width() - 4, fm.lineSpacing() * maxLineCount );
			QColor col = palette().text().color();
 			
			col.setAlpha(128);
 			p.setPen(col);
			p.drawText(rc, Qt::TextWordWrap, m_placeHolderText);
 		}	
 	}
}

DuiTextEditCopyPasteHandler* RichTextEdit::copyPasteHandlerByIndex( int index ) const
{
	return m_copyPasteHandlerList.value(index);
}

QList<DuiTextEditCopyPasteHandler*> RichTextEdit::copyPasteHandler() const
{
	return m_copyPasteHandlerList;
}

QPoint RichTextEdit::contentOffset() const
{
	QScrollBar *h = horizontalScrollBar();
	QScrollBar *v = verticalScrollBar();
	const int x = h ? (isRightToLeft() ? h->maximum() - h->value() : h->value()) : 0;
	const int y = v ? v->value() : 0;
	return QPoint(x, y);
}

