#include "duicombobox.h"
#include "duiwidget_p.h"
#include "duilineedit.h"
#include "duipushbutton.h"
#include "duiframeitem.h"
#include "duicomboboxlistwindow.h"
#include "utility/duimakecombineenum.h"
#include "dwbase/log.h"

enum ComCombineIndex
{
	DuiLineEdit_Index,
	DuiComboBoxList_Index,

	ComCombineMax,
};

class DuiComboBoxPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiComboBox)

public:
	DuiComboBoxPrivate();
	~DuiComboBoxPrivate();

	void init();

	void relayout(const QRectF &rect);

	DuiLineEdit*			m_edit;
	DuiFrameItem*			m_comboBoxFrame;
	DuiPushButton*			m_dropDownButton;
	DuiComboBoxListWindow*	m_comboBoxWindow;
	int						m_comboxWindowHeight;

	Dui::DuiWidgetState		m_hoverState;
	MakeCombineEnum<Dui::DuiWidgetState, Dui::InvalidState, ComCombineMax, enumOperatorAssign> m_focusState;

	QString					m_hoverEnterStyleId;
	QString					m_hoverLeaveStyleId;

	QMargins				m_margins;
	bool					m_isFroceDropUp;
};

DuiComboBoxPrivate::DuiComboBoxPrivate()
: m_edit( NULL )
, m_comboBoxFrame( NULL )
, m_dropDownButton( NULL )
, m_comboBoxWindow( NULL )
, m_comboxWindowHeight( 196 )
, m_hoverState(Dui::HoverOut)
, m_margins(1,1,1,1)
, m_isFroceDropUp(false)
{
}

DuiComboBoxPrivate::~DuiComboBoxPrivate()
{
}

void DuiComboBoxPrivate::init()
{
	Q_Q(DuiComboBox);

	m_comboBoxFrame = new DuiFrameItem(q);
	m_comboBoxFrame->setParent(q);
	m_comboBoxFrame->setObjectName("comboBoxFrame");
	m_comboBoxFrame->setColorizePolicy(DuiWidget::UnColorableFlag);
	m_comboBoxFrame->setFrameType(DuiFrameItemDrawer::NinePieces);

	m_edit = new DuiLineEdit(q);
	m_edit->setParent(q);
	m_edit->setObjectName("comboBoxEdit");
	m_edit->setFrame(false);
	m_edit->setMargins(QMargins(2, 0, 2, 0));

	QObject::connect(m_edit, SIGNAL(textEdited(const QString&)), q, SLOT(onEditTextChanged(const QString&)));
	QObject::connect(m_edit, SIGNAL(leftButtonClicked()), q, SLOT(onEditLeftButtonClicked()));
	QObject::connect(m_edit, SIGNAL(focusIn()), q, SLOT(onEditFocusIn()));
	QObject::connect(m_edit, SIGNAL(focusOut()), q, SLOT(onEditFocusOut()));

	m_dropDownButton = new DuiPushButton(q);
	m_dropDownButton->setParent(q);
	m_dropDownButton->setObjectName("comboBoxDropDownButton");
	m_dropDownButton->setBackgroundThemeColorType(DuiTheme::MoreDarkColor);
	m_dropDownButton->setButtonStyle(DuiPushButton::ButtonCheckable);
	m_dropDownButton->setFocusPolicy(Qt::ClickFocus);
	QObject::connect(m_dropDownButton, SIGNAL(clicked()), q, SLOT(onToggleDropDownList()));

	m_comboBoxWindow = new DuiComboBoxListWindow(q->graphicsView(), q);
	m_comboBoxWindow->setObjectName("comboxWindow");//给测试提供支持
	QObject::connect(m_comboBoxWindow, SIGNAL(focusOut()), q, SLOT(onComboxlistFocusOut()));
	QObject::connect(m_comboBoxWindow, SIGNAL(focusIn()), q, SLOT(onComboxlistFocusIn()));
	QObject::connect(m_comboBoxWindow, SIGNAL(comboxlist_itemClicked(int)), q, SLOT(onDropDownListClicked(int)));
	QObject::connect(m_comboBoxWindow, SIGNAL(selectedChanged(int)), q, SLOT(onDropDownListClicked(int)));

	q->setEditable(false);
	q->setAcceptHoverEvents(true);
	q->setStyleId("comboBox");
}

void DuiComboBoxPrivate::relayout(const QRectF &rect)
{
	m_comboBoxFrame->setGeometry(rect);

	const QRectF editRect(rect.adjusted(m_margins.left(), m_margins.top(), 
		-m_dropDownButton->width() - m_margins.right(), -m_margins.bottom()));
	m_edit->setGeometry(editRect);

	m_dropDownButton->setPos(editRect.topRight());
}

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiComboBox)
DuiComboBox::DuiComboBox(QGraphicsItem* parent/* = NULL*/)
:DuiWidget(*(new DuiComboBoxPrivate), parent)
{
	Q_D(DuiComboBox);
	d->init();
}

DuiComboBox::DuiComboBox( DuiComboBoxPrivate &dd, QGraphicsItem* parent )
: DuiWidget(dd, parent)
{
	Q_D(DuiComboBox);
	d->init();
}

DuiComboBox::~DuiComboBox()
{
	DW_SAFE_DELETE(d_func()->m_comboBoxWindow);
}

void DuiComboBox::onToggleDropDownList()
{
	Q_D( DuiComboBox );

	emit aboutToDropDown();

	if(d->m_dropDownButton->checked())
	{
		QPoint scenePos = mapToScene( QPointF(0, 0) ).toPoint();	
		QPoint globalPoint = graphicsView()->mapToGlobal( scenePos );

		d->m_comboBoxWindow->showPopup(QRect(globalPoint, size().toSize()), d->m_comboxWindowHeight, isForceDropUp());
	}
	else
	{
		d->m_comboBoxWindow->hide();
	}
}

void DuiComboBox::onEditLeftButtonClicked()
{
	Q_D( DuiComboBox );
	if (d->m_edit->isReadOnly() /*&& !d->m_dropDownButton->checked()*/)
	{
		d->m_dropDownButton->setChecked(true);
		onToggleDropDownList();
	}
}

void DuiComboBox::onEditFocusIn()
{
	DuiWidget::duiUpdateAccessibility(this,DuiWidget::AccFocus);
	Q_D(DuiComboBox);
	d->m_focusState.setEnum(DuiLineEdit_Index, Dui::FocusIn);
	d->m_hoverState = Dui::HoverOut;

	updateFocusHoverState();
}

void DuiComboBox::onEditFocusOut()
{
	Q_D(DuiComboBox);
	d->m_focusState.setEnum(DuiLineEdit_Index, Dui::FocusOut);

    emit focusOut();
	updateFocusHoverState();
}

QVariant DuiComboBox::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case QGraphicsItem::ItemScenePositionHasChanged:
		Q_D(DuiComboBox);
		if (d->m_comboBoxWindow->isVisible())
		{
			QPoint scenePos = mapToScene(QPointF(0, this->height())).toPoint();
			QPoint globalPoint = graphicsView()->mapToGlobal(scenePos);

			d->m_comboBoxWindow->setGeometry(QRect(globalPoint, d->m_comboBoxWindow->geometry().size()));
		}
		break;
	}

	return DuiWidget::itemChange(change, value);
}

void DuiComboBox::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);

	Q_D(DuiComboBox);
	d->relayout(QRectF(QPointF(), event->newSize()));
}

void DuiComboBox::addItemPrototype( DuiAbstractViewItem *prototype )
{
	d_func()->m_comboBoxWindow->addPrototype( prototype );
}

void DuiComboBox::setModel( QStandardItemModel *itemModel )
{
	Q_D(DuiComboBox);
	DW_ASSERT(itemModel);

	if ( d->m_comboBoxWindow->model() != itemModel )
	{
		if ( d->m_comboBoxWindow->model() )
		{
			disconnect(d->m_comboBoxWindow->model(), SIGNAL(rowsAboutToBeRemoved( const QModelIndex &, int, int)), this, SLOT(onCurrentItemRemoved( const QModelIndex &, int, int )));
		}
		
		if (itemModel)
		{
			d->m_comboBoxWindow->setModel( itemModel );
			connect(d->m_comboBoxWindow->model(), SIGNAL(rowsAboutToBeRemoved( const QModelIndex &, int, int)), this, SLOT(onCurrentItemRemoved( const QModelIndex &, int, int )));
		}
	}
}

void DuiComboBox::onDropDownListClicked( int index )
{
	emit activated( index );

	Q_D(DuiComboBox);

	if ( d->m_comboBoxWindow->indexBeforeChange() != currentIndex() )
	{
		setSelectedItem(currentIndex());

		//Tips : Just Clear Focus When widget == comboBox.edit || widget == comboBox.listview
		//Prefix Condition, edit will gain focus when setValue
		QWidget* widget = qApp->focusWidget();

		if ( widget && (widget == d->m_comboBoxWindow))
		{
			widget->clearFocus();
		}

		emit currentIndexChanged( index );
	}

	d->m_edit->setFocus(true);
	if (!d_func()->m_edit->isReadOnly())
	{
		d->m_edit->selectAll();	// 选择列表后让文字全选，方便用户重新编辑
	}
	DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccFocus);
}

int DuiComboBox::currentIndex() const
{
	return d_func()->m_comboBoxWindow->selectedIndex();
}

void DuiComboBox::setCurrentIndex( int index )
{
	if ( index < 0 || index >= count() )
		return ;

	Q_D(DuiComboBox);

	d->m_comboBoxWindow->setSelectedIndex( index );
	setSelectedItem(index);
}

QString DuiComboBox::currentText() const
{
	if (isEditable())
	{
		return d_func()->m_edit->text();
	}

	if ( model() == NULL ) 
		return QString("");

	if ( currentIndex() == -1)
	{
		return QString("");
	}	

	return model()->item( currentIndex() )->text();
}


bool DuiComboBox::isEditable() const
{
	if ( d_func()->m_edit->isReadOnly() )
		return false;
	else 
		return true;
}

void DuiComboBox::setEditable( bool editable )
{
	Q_D(DuiComboBox);

	d->m_edit->setReadOnly( !editable );
	d->m_edit->setDisableStatus( false );
}

QStandardItemModel * DuiComboBox::model() const
{
	return d_func()->m_comboBoxWindow->model();
}

int DuiComboBox::maxdropDownListHeight()
{
	return d_func()->m_comboxWindowHeight;
}

void DuiComboBox::setMaxdropDownListHeight( int max )
{
	d_func()->m_comboxWindowHeight = max;
}

void DuiComboBox::onEditTextChanged( const QString & text )
{
	Q_D(DuiComboBox);
	d->m_comboBoxWindow->setSelectedIndex(-1);
	emit editTextChanged( text );
}

int DuiComboBox::hitTestGlobal( const QPoint &pos )
{
	Q_D(DuiComboBox);
	QPoint scenePos = graphicsView()->mapFromGlobal(pos);
	QPointF p = mapFromScene(scenePos);
	if(d->m_dropDownButton->geometry().contains(p))
	{
		return ComboBoxDropdownButton;
	}
	else if(d->m_edit->geometry().contains(p))
	{
		return ComboBoxEditField;	
	}

	return ComboBoxNone;
}

void DuiComboBox::setData(const QList<QVariant>& dataList )
{
	if ( dataList.count() == 0 )
	{
		return;
	}

	QStandardItemModel *model = new QStandardItemModel();

	for ( int i = 0; i < dataList.count(); i++ )
	{
		const QMap<QString, QVariant> &m = dataList.at(i).toMap();
		QString text = m.value( "text" ).toString();
		QString icon = m.value( "icon" ).toString();

		QStandardItem* item = NULL;

		if ( icon.isEmpty() )
		{
			item = new QStandardItem( text );
		}
		else
		{
			item = new QStandardItem( QIcon( icon ), text );
		}

		model->appendRow( item );
	}

	setModel( model );
}

QList<QVariant> DuiComboBox::data() const
{
	DW_ASSERT_X(KDuiIsEditorMode || false, __FUNCTION__, "this method should not be invoke.");
	return QList<QVariant>();
}

DuiLineEdit* DuiComboBox::lineEdit() const
{
	return d_func()->m_edit;
}

int DuiComboBox::count() const
{
	if ( model() == NULL )
		return 0;

	return model()->rowCount();
}

void DuiComboBox::setSelectedItem( QStandardItem* item )
{
	if ( item != NULL )
	{
		d_func()->m_edit->setText( item->text() );
		d_func()->m_edit->home( false );
	}
}

void DuiComboBox::setSelectedItem( int index )
{
	if ( index < 0 || index >= count() )
		return ;

	Q_D(DuiComboBox);

	QStandardItemModel *model = d->m_comboBoxWindow->model();
	if ( model != NULL )
	{
		QStandardItem* item = model->item( index );
		setSelectedItem( item );
	}
}

void DuiComboBox::onComboxlistFocusIn()
{
	Q_D(DuiComboBox);
	d->m_focusState.setEnum(DuiComboBoxList_Index, Dui::FocusIn);
	//d->m_hoverState = Dui::HoverIn;

	updateFocusHoverState();
}

void DuiComboBox::onComboxlistFocusOut()
{
	Q_D(DuiComboBox);

	if ( d->m_dropDownButton->checked() )
	{
		d->m_dropDownButton->setChecked( false );
	}

	d->m_focusState.setEnum(DuiComboBoxList_Index, Dui::FocusOut);

	// bug18715-咨询了伟彦，之前让它失去焦点是为了资料设置页选择完后不再显示边框
	//		但这会导致不能再方向键选择，中断了用户操作，不符合了通用的combobox逻辑
	//		先暂时屏蔽此行
	// d->m_focusState.setEnum(DuiLineEdit_Index, Dui::FocusOut);
    emit focusOut();
	updateFocusHoverState();
}

void DuiComboBox::doColorize( QRgb color )
{
	Q_UNUSED(color);
	d_func()->m_comboBoxWindow->doColorize( color );
}

void DuiComboBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	Q_D(DuiComboBox);
	d->m_hoverState = Dui::HoverIn;

	updateFocusHoverState();
}

void DuiComboBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	Q_D(DuiComboBox);
	d->m_hoverState = Dui::HoverOut;

	updateFocusHoverState();
}

void DuiComboBox::setEnabled( bool enabled )
{
	DuiWidget::setEnabled( enabled );
	d_func()->m_comboBoxFrame->setEnable(enabled);
	d_func()->m_edit->setDisableStatus(!enabled);
	d_func()->m_dropDownButton->setEnabled(enabled);
}

void DuiComboBox::setPlaceholderText( const QString & text )
{
	Q_D( DuiComboBox );
	d->m_edit->setPlaceholderText(text);
}

void DuiComboBox::setDisplayMode( DisplayMode mode )
{
	Q_D(DuiComboBox);
	DuiListWidget* listWidget = d->m_comboBoxWindow->listWidget();
	if ( listWidget )
	{
		listWidget->setDisplayMode( (DuiListWidget::DisplayMode)mode );
	}
}

int     DuiComboBox::pushButtonWidth()const
{
	Q_D(const DuiComboBox);
	return d->m_dropDownButton->width();
}

void    DuiComboBox::setPushButtonWidth(int width)
{
	Q_D(DuiComboBox);
	const QSizeF sz(width, d->m_dropDownButton->height());
	d->m_dropDownButton->resize(sz);

	d->relayout(rect());
}

int     DuiComboBox::pushButtonHeight()const
{
	Q_D(const DuiComboBox);
	return d->m_dropDownButton->height();
}

void    DuiComboBox::setPushButtonHeight(int height)
{
	Q_D(DuiComboBox);
	const QSizeF sz(d->m_dropDownButton->width(), height);
	d->m_dropDownButton->resize(sz);

	setMaximumHeight(height);
	resize(width(), height);
}

void    DuiComboBox::setPushButtonImage(const QString& pushButtonImage)
{
	Q_D(DuiComboBox);
    d->m_dropDownButton->setImagePath(pushButtonImage);
}

QString DuiComboBox::pushButtonImage()const
{
	Q_D(const DuiComboBox);
	return d->m_dropDownButton->property("imagePath").toString();
}

void    DuiComboBox::setComboBoxImage(const QString& comboBoxImage)
{
	Q_D(DuiComboBox);
    d->m_comboBoxFrame->setImage(comboBoxImage);
}

QString DuiComboBox::comboBoxImage()const
{
	Q_D(const DuiComboBox);
	return d->m_comboBoxFrame->imagePath();
}

QString DuiComboBox::hoverEnterStyleId() const
{
	Q_D(const DuiComboBox);

	return d->m_hoverEnterStyleId;
}

void DuiComboBox::setHoverEnterStyleId(const QString& hoverEnterStyleId )
{
	Q_D(DuiComboBox);
	d->m_hoverEnterStyleId = hoverEnterStyleId;
}

QString DuiComboBox::hoverLeaveStyleId() const
{
	Q_D( const DuiComboBox);
	return d->m_hoverLeaveStyleId;
}

void DuiComboBox::setHoverLeaveStyleId( const QString& hoverLeaveStyleId )
{
	Q_D(DuiComboBox);
	d->m_hoverLeaveStyleId = hoverLeaveStyleId;
}

void DuiComboBox::updateFocusHoverState()
{
	Q_D(DuiComboBox);

	if (d->m_hoverState == Dui::HoverIn || d->m_focusState.isAnyEnum(Dui::FocusIn))
	{
		if( !d->m_hoverEnterStyleId.isEmpty() )
		{
			setStyleId( d->m_hoverEnterStyleId );
		}
	}
	else
	{
		if( !d->m_hoverLeaveStyleId.isEmpty() )
		{
			setStyleId( d->m_hoverLeaveStyleId );
		}
	}

	if (d->m_hoverState == Dui::HoverIn)
	{
		d->m_comboBoxFrame->setHovered(true);
		d->m_dropDownButton->setHovered(true);
	}
	else 
	{
		d->m_comboBoxFrame->setHovered(false);
		d->m_dropDownButton->setHovered(false);
	}

	if (d->m_focusState.isAnyEnum(Dui::FocusIn))
	{
		d->m_comboBoxFrame->setChecked(true);
		d->m_dropDownButton->setChecked(true);
	}
	else
	{
		d->m_comboBoxFrame->setChecked(false);
		d->m_dropDownButton->setChecked(false);
		d->m_dropDownButton->setPressed(false);	
	}

	update();
}


void DuiComboBox::keyPressEvent( QKeyEvent *event )
{
	Q_D( DuiComboBox );

	switch(event->key())
	{
	case Qt::Key_Up:
		if (d->m_dropDownButton->checked())
		{
			onToggleDropDownList();
		}
		d->m_comboBoxWindow->selectTo(DuiComboBoxListWindow::PreviouItem);
		break;
	case Qt::Key_Down:
		if (d->m_dropDownButton->checked())
		{
			onToggleDropDownList();
		}
		d->m_comboBoxWindow->selectTo(DuiComboBoxListWindow::NextItem);
		break;
	default:
		DuiWidget::keyPressEvent(event);
		break;
	}
}

void DuiComboBox::setItemHeight(int h)
{
    Q_D(DuiComboBox);
    d->m_comboBoxWindow->setItemHeight(h);
}

int DuiComboBox::itemHeight() const
{
    Q_D(const DuiComboBox);
    return d->m_comboBoxWindow->itemHeight();
}

void DuiComboBox::setDropDownButtonAnimation(bool b)
{
    Q_D(DuiComboBox);
    d->m_dropDownButton->setAnimateInOut(b);
}

bool DuiComboBox::dropDownButtonAnimation()const
{
    Q_D(const DuiComboBox);
    return d->m_dropDownButton->isAnimateInOut();
}

void DuiComboBox::setTextColor( const QColor &textColor )
{
    Q_D(const DuiComboBox);
    QLineEdit *pEdit = d->m_edit->realWidget();
    
    QPalette pal = pEdit->palette();
    pal.setColor( QPalette::Text, textColor );
    pEdit->setPalette(pal);
}

QColor DuiComboBox::textColor( void ) const
{
    Q_D(const DuiComboBox);
    QLineEdit *pEdit = d->m_edit->realWidget();
    QPalette pal = pEdit->palette();
    return pal.text().color();
}

void DuiComboBox::setFont(const QFont &font)
{
    Q_D(const DuiComboBox);
    d->m_edit->realWidget()->setFont( font );
}

QFont DuiComboBox::font() const
{
    Q_D(const DuiComboBox);
    return d->m_edit->realWidget()->font();
}

void DuiComboBox::setFontSize( int fontSize )
{
    QFont ft  = font();
    ft.setPixelSize(fontSize);
    setFont(ft);
}

int DuiComboBox::fontSize() const
{
    return font().pixelSize();
}

QMargins DuiComboBox::margins() const
{
	Q_D(const DuiComboBox);
	return d->m_margins;
}

void DuiComboBox::setMargins(const QMargins& m)
{
	Q_D(DuiComboBox);
	d->m_margins = m;
	d->relayout(rect());
}

void DuiComboBox::onCurrentItemRemoved( const QModelIndex & parent, int start, int end )
{
	Q_D(DuiComboBox);
	QString text =  d->m_edit->text();
	for(int i = start; i <= end; ++i)
	{
		if (text ==  d->m_comboBoxWindow->model()->item(start)->text())
		{
			d->m_comboBoxWindow->setSelectedIndex(-1);
			break;
		}
	}	
}

QString	DuiComboBox::validator() const
{
	Q_D(const DuiComboBox);
	return d->m_edit->validator();
}

void DuiComboBox::setValidator(const QString& v)
{
	Q_D(const DuiComboBox);
	d->m_edit->setValidator(v);
}

int DuiComboBox::maxLength() const
{
	Q_D(const DuiComboBox);
	return d->m_edit->maxLength();
}

void DuiComboBox::setMaxLength(int n)
{
	Q_D(const DuiComboBox);
	d->m_edit->setMaxLength(n);
}

void DuiComboBox::setComboxListWindowBkImage(const QString &frameImage)
{
	Q_D(DuiComboBox);
	d->m_comboBoxWindow->setBackgroundImage(frameImage);
}

QString DuiComboBox::comboxListWindowBkImage() const
{
	return "";
}

QGraphicsView* DuiComboBox::getComboxListWindow()
{
	Q_D(DuiComboBox);
	return d->m_comboBoxWindow;
}

void DuiComboBox::setForceDropUp(bool force)
{
	Q_D(DuiComboBox);
	d->m_isFroceDropUp = force;
}

bool DuiComboBox::isForceDropUp() const
{
	return d_func()->m_isFroceDropUp;
}

void DuiComboBox::beginInsertRows()
{
	Q_D(DuiComboBox);
	d->m_comboBoxWindow->beginInsertRows();
}
void DuiComboBox::endInsertRows(int start, int end)
{
	Q_D(DuiComboBox);
	d->m_comboBoxWindow->endInsertRows( start, end );
}