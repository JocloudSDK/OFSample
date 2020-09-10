
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "duifw/duiwidget.h"

class DuiLineEdit;
class DuiAbstractViewItem;
class DuiComboBoxPrivate;

class DW_DUIFW_EXPORT  DuiComboBox : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiComboBox )
	Q_DECLARE_PRIVATE( DuiComboBox )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiComboBox )

public:
	enum ComboBoxStyle
	{
		Normal,
		ShowOnFocus,
	};
	Q_ENUMS(ComboBoxStyle);
	Q_PROPERTY( bool editable READ isEditable WRITE setEditable  )
	Q_PROPERTY( QList<QVariant> data READ data WRITE setData )
	Q_PROPERTY( int currentIndex READ currentIndex WRITE setCurrentIndex  )
	Q_PROPERTY(int pushButtonWidth READ pushButtonWidth WRITE setPushButtonWidth)
	Q_PROPERTY(int pushButtonHeight READ pushButtonHeight WRITE setPushButtonHeight)
	Q_PROPERTY(QString pushButtonImage READ pushButtonImage WRITE setPushButtonImage)
	Q_PROPERTY(QString comboBoxImage READ comboBoxImage WRITE setComboBoxImage)
	Q_PROPERTY(QString hoverEnterStyleId READ hoverEnterStyleId WRITE setHoverEnterStyleId )
	Q_PROPERTY(QString hoverLeaveStyleId READ hoverLeaveStyleId WRITE setHoverLeaveStyleId )
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight)
    Q_PROPERTY(bool dropDownButtonAnimation READ dropDownButtonAnimation WRITE setDropDownButtonAnimation)
    Q_PROPERTY( QColor textColor WRITE setTextColor READ textColor )
    Q_PROPERTY( QFont font READ font WRITE setFont )
    Q_PROPERTY( int fontSize READ fontSize WRITE setFontSize )
	Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
	Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
	Q_PROPERTY(QString validator READ validator WRITE setValidator)
public:
	enum SubControl
	{
		ComboBoxNone = 0,
		ComboBoxDropdownButton = 1,
		ComboBoxEditField = 2
	};

	enum DisplayMode
	{
		NormalMode = 0,
		FishEyeMode = 1
	};

	explicit DuiComboBox( QGraphicsItem* parent = NULL);
	virtual ~DuiComboBox();

	void addItemPrototype( DuiAbstractViewItem *prototype );

	void setModel( QStandardItemModel *itemModel );
	QStandardItemModel *	model () const;

	int		currentIndex () const;
	void	setCurrentIndex ( int index );

	QString	currentText () const;

	bool	isEditable () const;
	void	setEditable ( bool editable );

	int		maxdropDownListHeight();
	void	setMaxdropDownListHeight( int max );

	int		hitTestGlobal(const QPoint &pos);

	void	setData( const QList<QVariant> &dataList );

	Q_INVOKABLE DuiLineEdit* lineEdit () const;

	int	count () const;

	void setEnabled( bool enabled );

	void setPlaceholderText ( const QString & text);

	void setDisplayMode( DisplayMode mode );

	int     pushButtonWidth()const;
	void    setPushButtonWidth(int width);

	int     pushButtonHeight()const;
	void    setPushButtonHeight(int height);

	QString pushButtonImage()const;
	void    setPushButtonImage(const QString& pushButtonImage);

	QString comboBoxImage()const;
	void    setComboBoxImage(const QString& comboBoxImage);

	QString hoverEnterStyleId() const;
	void setHoverEnterStyleId(const QString& hoverEnterStyleId );

	QString hoverLeaveStyleId() const;
	void setHoverLeaveStyleId( const QString& hoverLeaveStyleId );

    int     itemHeight() const;
    void setItemHeight(int h);

    void setDropDownButtonAnimation(bool b);
    bool dropDownButtonAnimation()const;
    
    void setTextColor( const QColor &textColor );
    QColor textColor( void ) const;
    
    void setFont( const QFont &font );
    QFont font() const;
    
    void setFontSize( int fontSize );
    int fontSize() const;

	QMargins margins() const;
	void setMargins(const QMargins& m);

	QString	validator() const;
	void setValidator(const QString& v);

	int maxLength() const;
	void setMaxLength(int);

	void	setComboxListWindowBkImage( const QString &frameImage );
	QString comboxListWindowBkImage(  ) const;

	QGraphicsView* getComboxListWindow();

	void setForceDropUp( bool force );
	bool isForceDropUp() const;

	void beginInsertRows();
	void endInsertRows(int start, int end);
signals :
	void	aboutToDropDown();
	void	activated ( int index );
	void	currentIndexChanged ( int index );
	void	editTextChanged ( const QString & text );
    void    focusOut();

protected slots:
	void onToggleDropDownList();
	void onDropDownListClicked( int index );
	void onEditTextChanged( const QString & text );
	void onEditLeftButtonClicked();

	void onComboxlistFocusOut();
	void onComboxlistFocusIn();

	void onEditFocusIn();
	void onEditFocusOut();

	void onCurrentItemRemoved( const QModelIndex & parent, int start, int end );

protected:
	DuiComboBox( DuiComboBoxPrivate &dd, QGraphicsItem* parent = NULL);
	void setSelectedItem( int index );

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	virtual void resizeEvent( QGraphicsSceneResizeEvent * event );
	virtual void setSelectedItem( QStandardItem* item );
	virtual void doColorize( QRgb color );

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	virtual void updateFocusHoverState();

	// 按上下键选择
	void keyPressEvent(QKeyEvent *event);

private:
	QList<QVariant> data() const;
};	//DuiCommboBox
