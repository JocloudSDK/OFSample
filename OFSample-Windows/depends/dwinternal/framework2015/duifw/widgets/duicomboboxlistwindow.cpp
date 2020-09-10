#include "stable.h"

#include "duifw/duiframeitem.h"
#include "duiapplication.h"
#include "duicomboboxlistwindow.h"
#include "duitheme.h"
#include "duifw/duigraphicsmaskeffect.h"
#include "duifw/duiwinapi.h"

const int KDropListOffset = 2;
const int KComboBoxItemHeight = 24;

//------------------------------------------------------------------------------
class DuiDefaultComboBoxListItem : public DuiDefaultListItem
{
public:
    DuiDefaultComboBoxListItem()
    {
        init();
    }

    virtual DuiDefaultComboBoxListItem *createItem(QStandardItem *item, QGraphicsItem *parent)
    {
        DuiDefaultComboBoxListItem* listItem = new DuiDefaultComboBoxListItem(item, parent);
        return listItem;
    }

    void init()
    {
        DuiFrameItem *backgroundItem = findChild<DuiFrameItem *>("backgroundframeitem");
        if (backgroundItem)
        {
            // alawys use the blue theme, do not change the theme for combobox
            backgroundItem->setColorizePolicy(DuiWidget::UnColorableFlag);
            backgroundItem->setBaseColor(qRgb(34,  182, 255));
        }

        m_hoverMask = DuiTheme::getImage(":/theme/duifw/item_hovered_mask.png").toImage();
    }

protected:
    DuiDefaultComboBoxListItem(QStandardItem *item, QGraphicsItem *parent)
        : DuiDefaultListItem(item, parent)
    {
        init();
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
    {
        Q_UNUSED(widget);
        if ( option->state.testFlag(QStyle::State_MouseOver))
        {
            // alawys use the blue theme
            painter->fillRect(rect(), qRgb(34,  182, 255));
            painter->drawImage(rect(), m_hoverMask);
        }
    }
    QImage m_hoverMask;
};

//------------------------------------------------------------------------------
DuiComboBoxListWindow::DuiComboBoxListWindow( QWidget* parent, DuiComboBox *combo )
: QGraphicsView( parent ) 
, m_comboBoxList( NULL )
, m_background(NULL)
, m_comboBox( combo )
, m_selectedIndex( -1 )
, m_indexBeforeChange( -1 )
, m_itemHeight(KComboBoxItemHeight)
{
	PERFLOG;
	init();
}

DuiComboBoxListWindow::~DuiComboBoxListWindow( void )
{
}

void DuiComboBoxListWindow::init()
{
    // pop up window
	setWindowFlags( Qt::FramelessWindowHint | Qt::Popup );
    //// translaucnt
    setAttribute(Qt::WA_TranslucentBackground, true);

	m_scene = new QGraphicsScene( this );
	setScene(m_scene);
    
    // add nine patchs background
    m_background = new DuiFrameItem();
    m_background->setImage(":/theme/duifw/combobox_listwindow_background.png");
    m_background->setColorizePolicy(DuiWidget::UnColorableFlag);
    m_scene->addItem(m_background);

    DuiGraphicsMaskEffect *maskEffect = new DuiGraphicsMaskEffect();
    maskEffect->setMask(QPixmap(":/theme/duifw/combobox_listwindow_background_mask.png"));
	m_comboBoxList = new DuiListWidget( );
    m_comboBoxList->setGraphicsEffect(maskEffect);
    m_comboBoxList->addItemPrototype(new DuiDefaultComboBoxListItem());
	m_scene->addItem( m_comboBoxList );
	m_comboBoxList->setParent( m_scene );
	m_comboBoxList->setObjectName( "comboxList" );
	// m_comboBoxList->setBackgroundColor(QColor(255,255,255,255));
	
	connect(m_comboBoxList, SIGNAL(leftButtonClicked(int)), SLOT(on_comboxlist_itemClicked(int)));
	connect(m_comboBoxList, SIGNAL(rightButtonClicked(int)), SLOT(on_comboxlist_itemClicked(int)));

	// BColor(255, 255, 255, 255) + QColor(0, 51, 102, 77) =(premulti)= (178, 193, 209, 255)
	// setStyleSheet("QGraphicsView {border-color:rgba(178, 193, 209, 255); border-width: 1px; border-style:solid;}");
}

void DuiComboBoxListWindow::resizeEvent( QResizeEvent *event)
{
	Q_UNUSED(event);

	QRect rect( pos(), event->size() );
	setSceneRect( rect );
    m_background->setGeometry(rect);
    rect.adjust(2,2,-2,-2);
	m_comboBoxList->setGeometry( rect );

	//修复bug 17314 
	m_comboBoxList->setSelectIndex( selectedIndex(), true );
}

bool DuiComboBoxListWindow::event( QEvent * event )
{
	if ( event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		QPoint globalPos = mapToGlobal( mouseEvent->pos() );
		if ( geometry().contains( globalPos ) == false )
		{
			setAttribute(Qt::WA_NoMouseReplay);

			hide();
			return true;
		}
	}
	else if( event->type() == QEvent::KeyPress )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if ( keyEvent->key() == Qt::Key_Escape )
		{
			hide();
			return true;
		}
	}

	return QGraphicsView::event( event );
}

void DuiComboBoxListWindow::addPrototype( DuiAbstractViewItem *prototype )
{
	return m_comboBoxList->addItemPrototype( prototype );
}

void DuiComboBoxListWindow::setModel( QStandardItemModel *itemModel )
{
	if ( m_comboBoxList->model() )
	{
		disconnect( m_comboBoxList->model(), SIGNAL(modelReset()),this,SLOT(on_modelReset()) );
	}
	if (itemModel)
	{
		connect( itemModel, SIGNAL(modelReset()),this,SLOT(on_modelReset()) );
	}
	return m_comboBoxList->setModel( itemModel );
}

void DuiComboBoxListWindow::hideEvent ( QHideEvent * event )
{
	QGraphicsView::hideEvent(event);
	Q_UNUSED(event);

	emit focusOut();
}

void DuiComboBoxListWindow::showEvent ( QShowEvent * event )
{
	QGraphicsView::showEvent(event);

	emit focusIn();
}

void DuiComboBoxListWindow::showPopup(const QRect &comboRect, int maxHeight, bool isForceDropUp)
{
	if ( m_comboBoxList->model() == NULL )
	{
		return ;
	}

	int itemCount =	m_comboBoxList->model()->rowCount();

	// 直接固定每一项高度? 那propertype的preferHeight就用了。
	int listHeight = itemCount * m_itemHeight;
	if(itemCount ==0 )
	{
		listHeight = 4 * m_itemHeight;
	}
	else if ( m_comboBoxList->displayMode() == DuiListWidget::FishEyeMode )
	{
		// 加上一个大的，两个中等的差。
		// 原来ComboBox已固定item的高度，现在只好也固定高度。
		if ( itemCount == 1 )
		{
			listHeight += 20;
		}
		else if ( itemCount == 2)
		{
			listHeight += 30;
		}
		else if ( itemCount > 2 )
		{
			listHeight += 40;
		}
	}

	listHeight = qMin(listHeight, maxHeight);

	bool dropUp = false;
	if (isForceDropUp)
	{
		dropUp = (comboRect.top() - maxHeight) >= 0;
	}
	else
	{
		dropUp = (comboRect.bottom() + maxHeight) > QApplication::desktop()->availableGeometry().height();
	}

	int listY = comboRect.bottom();
	if ( dropUp )
	{
		listY = comboRect.top() - listHeight;
		listY -= KDropListOffset;
	}
	else
	{
		listY += KDropListOffset;
	}

	QRect dropListRect(comboRect.left(), listY, comboRect.width(), listHeight + KDropListOffset * 2);
	setGeometry( dropListRect );

	DuiWinAPI::removeDropShadowStyle(winId());

	show();

	m_comboBoxList->setSelectIndex( selectedIndex(), true );
}

void DuiComboBoxListWindow::on_comboxlist_itemClicked( int index )
{
	setSelectedIndex( index );
	hide();
	emit comboxlist_itemClicked( index );
}

void DuiComboBoxListWindow::on_modelReset(void)
{
	m_selectedIndex = -1;
	m_indexBeforeChange = -1;
}

int DuiComboBoxListWindow::selectedIndex() const
{
	return m_selectedIndex;
}

void DuiComboBoxListWindow::setSelectedIndex( int index )
{	
	DW_ASSERT(index >= -1);
	m_indexBeforeChange = m_selectedIndex;
	m_selectedIndex = index;
}

QStandardItemModel * DuiComboBoxListWindow::model() const
{
	return m_comboBoxList->model();
}

int DuiComboBoxListWindow::indexBeforeChange() const
{
	return m_indexBeforeChange;
}

void DuiComboBoxListWindow::doColorize( QRgb color )
{
	Q_UNUSED( color );
	//m_comboBoxList->setBackgroundColor( DuiTheme::getColor( DuiTheme::MoreLightColor ) );
	//m_scene->setBackgroundBrush( QBrush( DuiTheme::getColor( DuiTheme::MidDarkColor ) ) );

	foreach (QGraphicsItem* item, items())
	{
		DuiWidget* widget = qobject_cast<DuiWidget*>( item->toGraphicsObject() );
		if (widget == NULL){
			continue;
		}
		widget->colorize( color );
	}
}

void DuiComboBoxListWindow::keyPressEvent( QKeyEvent *event )
{
	switch(event->key())
	{
	case Qt::Key_Up:
		selectTo(PreviouItem);
		break;
	case Qt::Key_Down:
		selectTo(NextItem);
		break;
	case Qt::Key_Home:
		selectTo(FirstItem);
		break;
	case Qt::Key_End:
		selectTo(LastItem);
		break;
		// @add by huqiuyun for "enter key function"
	case Qt::Key_Enter:
	case Qt::Key_Return:
		{
			int index = selectedIndex();
			if( index >= 0 )
			{
				hide();
				emit comboxlist_itemClicked( index );
			}
			break;
		}
	default:
		break;
	}
}

DuiListWidget* DuiComboBoxListWindow::listWidget()
{
	return m_comboBoxList;
}

void DuiComboBoxListWindow::selectTo( QuickSelect to )
{
	if (model() != NULL && model()->rowCount() > 0)
	{
		switch(to)
		{
		case FirstItem:
			setSelectedIndex(0);
			break;
		case LastItem:
			setSelectedIndex(model()->rowCount() - 1);
			break;
		case PreviouItem:
			if (selectedIndex() >= 0)
			{
				setSelectedIndex(selectedIndex() - 1);
			}
			break;
		case NextItem:
			if (selectedIndex() != model()->rowCount() - 1)
			{
				setSelectedIndex(selectedIndex() + 1);
			}
			break;
		default:
			DW_ASSERT(false);
		}
		m_comboBoxList->selectIndex(selectedIndex());

		if (selectedIndex() != indexBeforeChange())
		{
			emit selectedChanged(selectedIndex());
		}
	}
}

void DuiComboBoxListWindow::setItemHeight(int h)
{
    m_itemHeight = qMax(h, KComboBoxItemHeight);
    m_comboBoxList->setFixItemHeight(h);
}

int DuiComboBoxListWindow::itemHeight() const
{
    return m_itemHeight;
}

void DuiComboBoxListWindow::setBackgroundImage(const QString &imagePath)
{
	m_background->setImage(imagePath);
}

void DuiComboBoxListWindow::beginInsertRows()
{
	m_comboBoxList->beginInsertRows();
}
void DuiComboBoxListWindow::endInsertRows(int start, int end)
{
	m_comboBoxList->endInsertRows(start,end);
}