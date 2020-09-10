#pragma once
#include "dwglobal.h"
#include "duiwidget.h"

class DuiSpinBoxPrivate;
class QSpinBoxEx;
class DW_DUIFW_EXPORT DuiSpinBox : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSpinBox )
	Q_DECLARE_PRIVATE( DuiSpinBox )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSpinBox )

	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
	Q_PROPERTY(QString cleanText READ cleanText)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
	Q_PROPERTY(int value READ value WRITE setValue)
	Q_PROPERTY(QString upButtonImage READ upButtonImage WRITE setUpButtonImage)
	Q_PROPERTY(QString downButtonImage READ downButtonImage WRITE setDownButtonImage)

public:
	explicit DuiSpinBox( QGraphicsItem *parent = NULL );
	virtual ~DuiSpinBox();

	QString text() const;

	int value() const;
	void setValue(int val);

	QString prefix() const;
	void setPrefix(const QString &prefix);

	QString suffix() const;
	void setSuffix(const QString &suffix);

	QString cleanText() const;

	int singleStep() const;
	void setSingleStep(int val);

	int minimum() const;
	void setMinimum(int min);

	int maximum() const;
	void setMaximum(int max);

	void setRange(int min, int max);

	QString upButtonImage() const;
	void setUpButtonImage(const QString& pushButtonImage);

	QString downButtonImage() const;
	void setDownButtonImage(const QString& pushButtonImage);
	
	QSpinBoxEx* realWidget(void);

public slots:
	void stepUp();
	void stepDown();

signals:
	void valueChanged(double);
	void valueChanged(const QString &);

protected:
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);
	virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

	virtual void resizeEvent( QGraphicsSceneResizeEvent * event );
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};
