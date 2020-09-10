#pragma once

class DuiTextEditGraphicsItemPrivate;
class DuiTextEditGraphicsAnimation;
class DuiTextEditGraphicsAbstractState;

class DW_DUIFW_EXPORT DuiTextEditGraphicsItem
{
	Q_DISABLE_COPY( DuiTextEditGraphicsItem )
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsItem )

public:
	DuiTextEditGraphicsItem();	
	virtual ~DuiTextEditGraphicsItem();

	QString	objectName() const;
	void	setObjectName( const QString& objectName );

	void	setTextObjectIndex( int index );
	int		textObjectIndex() const;

	void	setPos ( const QPointF & pos );
	void	setPos ( qreal x, qreal y );
	QPointF pos() const ;

	void	setAcceptHoverEvents( bool enabled );
	bool	acceptHoverEvents() const;

    // 参数为Qt::MouseButton成员，可以合并(|)使用
	void	setAcceptedMouseButtons ( quint32 buttons );
	quint32	acceptedMouseButtons () const;

	QVariant	property( const QString& propertyKey ) const;
	void		setProperty ( const QString& propertyKey, const QVariant & value );

	QSizeF		size() const;
	void		setSize( const QSizeF& size );

	bool	isVisible () const;
	void	setVisible ( bool visible );

	void	update( );

	void	setCursor ( const QCursor& cursor );
    QCursor cursor() const;
    void    setHoverCursorShape( Qt::CursorShape shape );

	void	setAnimationObject( DuiTextEditGraphicsAnimation* animation );
	DuiTextEditGraphicsAnimation* animationObject() const;

	virtual QString gifImagePath() const;
	
	friend class DuiTextObject;

public: //signal
	virtual void animationChanged( DuiTextEditGraphicsAbstractState* state );

protected:
	explicit DuiTextEditGraphicsItem(DuiTextEditGraphicsItemPrivate &dd );
	
	virtual void paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format  ) = 0;
	virtual void mousePressEvent( QMouseEvent *event, quint32 textObjectIndex );
	virtual void mouseMoveEvent( QMouseEvent *event, quint32 textObjectIndex );
	virtual void mouseReleaseEvent(QMouseEvent *event, quint32 textObjectIndex );
	virtual void hoverEnterEvent ( QMouseEvent * event, quint32 textObjectIndex );
	virtual void hoverLeaveEvent ( QMouseEvent * event, quint32 textObjectIndex );

	QPointF absolutePos( const QRectF& rect ) const;

private:
	void _setContainerWidget( QWidget* widget );
	void _dispatchHoverEvent( QMouseEvent* event, bool isMouseMove, quint32 textObjectIndex );

public:
    virtual QByteArray gifImageData() const;

protected:
	DuiTextEditGraphicsItemPrivate * d_ptr;
};