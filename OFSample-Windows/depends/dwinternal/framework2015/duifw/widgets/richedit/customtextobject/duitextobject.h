#pragma once

#include <QTextObjectInterface>
#include "duifw/duitextedit.h"

const int  KTextObjectIndex = QTextFormat::ObjectIndex + 1000;
const char KAnimationTextObject[]	=	"AnimationTextObject";

class DuiTextObjectPrivate;
class DuiTextEditGraphicsItem;

class DW_DUIFW_EXPORT DuiTextObject : public QObject, public QTextObjectInterface
{
public:
	Q_OBJECT
	Q_DISABLE_COPY( DuiTextObject )
	Q_DECLARE_PRIVATE( DuiTextObject )
	Q_INTERFACES( QTextObjectInterface )

public:
	enum { objectType = QTextFormat::UserObject + 1 };

	explicit DuiTextObject( DuiTextEdit* editor );

	void appendItem( DuiTextEditGraphicsItem* item );//为了二进制兼容，作废这个接口，新加一个接口
	void appendItem2( DuiTextEditGraphicsItem* item );
    void removeItem(int nTextObjectIndex);
    void removeItem(int nTextObjectIndex, const QString& strObjName);
	void removeAllItem();
	DuiTextEditGraphicsItem* findChild( int textObjectIndex, const QString &aName ) const;

	void commonEvent( QMouseEvent* event, quint32 textObjectIndex );
	bool updateGifImage( quint32 textObjectIndex ); //为了二进制兼容，作废这个接口，新加一个接口
	bool updateGifImage( quint32 textObjectIndex, quint32 &animationItemCount );

signals:
	void clicked( const QString& objectName, int textObjectIndex );
    void mousePress(QMouseEvent *ev, const QString& objectName, int textObjectIndex);

protected slots:
	void on_textEditCleared();

protected:
	DuiTextObject( DuiTextObjectPrivate &dd, DuiTextEdit* editor );
	virtual ~DuiTextObject();

	virtual QSizeF intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format) const;
	virtual void drawObject(QPainter *painter, const QRectF &rect, 
		QTextDocument *doc, int posInDocument, const QTextFormat &format);

protected:
	DuiTextObjectPrivate * d_ptr;
};

