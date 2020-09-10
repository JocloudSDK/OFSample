#include "stable.h"
#include "duispinbox.h"

#include "duiwidget_p.h"
#include "duipushbutton.h"
#include "duiframeitem.h"
#include "qtextend/qlineeditex.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpinBox)

const int KSpinButtonWidth = 18;	//°´Å¥¿í¶È

Q_GLOBAL_STATIC_WITH_INITIALIZER(QPalette, defaultTextPalette, 
								{
									x->setColor(QPalette::Base, Qt::transparent);
									x->setColor(QPalette::Background, Qt::transparent);
								})

//////////////////////////////////////////////////////////////////////////
class QSpinBoxEx : public QSpinBox
{
public:
	explicit QSpinBoxEx(QWidget *parent = NULL)
		: QSpinBox(parent)
	{ }
	QLineEdit *lineEdit() const
	{
		return QSpinBox::lineEdit();
	}
};

class DuiSpinBoxPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiSpinBox)

public:
	DuiSpinBoxPrivate();
	~DuiSpinBoxPrivate();

	void init();
	void relayout(const QRectF &rc);

	void setEnable_helper(const bool enabled);

	DuiFrameItem *m_frameItem;
	QString m_spinFrameImage;

	QSpinBoxEx *m_spinBox;

	QString m_upButtonImage;
	DuiPushButton *m_upButton;

	QString m_downButtonImage;
	DuiPushButton *m_downButton;

	int m_buttonWidth;
};

DuiSpinBoxPrivate::DuiSpinBoxPrivate()
: m_spinBox(NULL)
, m_frameItem(NULL)
, m_upButton (NULL)
, m_downButton(NULL)
, m_buttonWidth(KSpinButtonWidth)
, m_upButtonImage(":theme/duifw/spin_button_up.png")
, m_downButtonImage(":theme/duifw/spin_button_down.png")
, m_spinFrameImage(":theme/duifw/edit.png")
{

}

DuiSpinBoxPrivate::~DuiSpinBoxPrivate()
{
}

void DuiSpinBoxPrivate::init()
{
	Q_Q(DuiSpinBox);
	q->setAcceptHoverEvents(true);
	q->setDragPolicy(DuiWidget::NotDragWindow);
	m_frameItem = new DuiFrameItem(m_spinFrameImage, DuiFrameItemDrawer::NinePieces, q);
	m_frameItem->setParent(q);
	m_frameItem->setObjectName("spinBoxFrame");
	m_frameItem->setColorizePolicy(DuiWidget::UnColorableFlag);
	m_frameItem->setImage(m_spinFrameImage);
	m_frameItem->setAcceptHoverEvents(true);
	m_frameItem->installSceneEventFilter(q);

	m_spinBox = new QSpinBoxEx;
	m_spinBox->setFrame(false);
	m_spinBox->setFocusPolicy(Qt::ClickFocus);
	m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	m_spinBox->setPalette(*defaultTextPalette());
	m_spinBox->lineEdit()->setFocusPolicy(Qt::ClickFocus);
	m_spinBox->lineEdit()->setPalette(*defaultTextPalette());
	m_spinBox->setContextMenuPolicy(Qt::NoContextMenu);
	QObject::connect(m_spinBox, SIGNAL(valueChanged(double)), q, SIGNAL(valueChanged(double)));
	QObject::connect(m_spinBox, SIGNAL(valueChanged(const QString &)), q, SIGNAL(valueChanged(const QString &)));

	QGraphicsProxyWidget *spinBoxProxy = new QGraphicsProxyWidget(q);
	spinBoxProxy->setParent(q);
	spinBoxProxy->setWidget(m_spinBox);
	spinBoxProxy->installSceneEventFilter(q);
	spinBoxProxy->setPalette(*defaultTextPalette());

	m_upButton = new DuiPushButton(q);
	m_upButton->setParent(q);
	m_upButton->setObjectName("spinBoxUpButton");
	m_upButton->setImagePath(m_upButtonImage);

	m_downButton = new DuiPushButton(q);
	m_downButton->setParent(q);
	m_downButton->setObjectName("spinBoxDownButton");
	m_downButton->setImagePath(m_downButtonImage);

	QObject::connect(m_upButton, SIGNAL(clicked()), q, SLOT(stepUp()));
	QObject::connect(m_downButton, SIGNAL(clicked()), q, SLOT(stepDown()));
}

void DuiSpinBoxPrivate::relayout(const QRectF &rc)
{
	m_frameItem->setGeometry(rc);

	const QRectF editRect = rc.adjusted(1, 1, -m_buttonWidth-1, -1);
	m_spinBox->graphicsProxyWidget()->setGeometry(editRect);

	QRectF buttonRect(editRect.right(), 0, m_buttonWidth, rc.height() / 2);
	m_upButton->setGeometry(buttonRect);

	buttonRect.moveTop(buttonRect.bottom());
	m_downButton->setGeometry(buttonRect);
}

void DuiSpinBoxPrivate::setEnable_helper(const bool enabled)
{
	m_frameItem->setEnabled(enabled);
	m_spinBox->setEnabled(enabled);
	m_upButton->setEnabled(enabled);
	m_downButton->setEnabled(enabled);
}

//////////////////////////////////////////////////////////////////////////
DuiSpinBox::DuiSpinBox( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*(new DuiSpinBoxPrivate()), parent)
{
	Q_D(DuiSpinBox);
	d->init();
}

DuiSpinBox::~DuiSpinBox()
{

}

QString DuiSpinBox::text() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->text();
}

int DuiSpinBox::value() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->value();
}

void DuiSpinBox::setValue(int value)
{
	Q_D(DuiSpinBox);
	d->m_spinBox->setValue(value);
}

QString DuiSpinBox::prefix() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->prefix();
}

void DuiSpinBox::setPrefix(const QString &prefix)
{
	Q_D(DuiSpinBox);
	return d->m_spinBox->setPrefix(prefix);
}

QString DuiSpinBox::suffix() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->suffix();
}

void DuiSpinBox::setSuffix(const QString &suffix)
{
	Q_D(DuiSpinBox);
	d->m_spinBox->setSuffix(suffix);
}

QString DuiSpinBox::cleanText() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->cleanText();
}

int DuiSpinBox::singleStep() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->singleStep();
}

void DuiSpinBox::setSingleStep(int value)
{
	Q_D(DuiSpinBox);
	d->m_spinBox->setSingleStep(value);
}

int DuiSpinBox::minimum() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->minimum();
}

void DuiSpinBox::setMinimum(int minimum)
{
	Q_D(DuiSpinBox);
	d->m_spinBox->setMinimum(minimum);
}

int DuiSpinBox::maximum() const
{
	Q_D(const DuiSpinBox);
	return d->m_spinBox->maximum();
}

void DuiSpinBox::setMaximum(int maximum)
{
	Q_D(DuiSpinBox);
	return d->m_spinBox->setMaximum(maximum);
}

void DuiSpinBox::setRange(int minimum, int maximum)
{
	Q_D(DuiSpinBox);
	d->m_spinBox->setRange(minimum,maximum);
}

void DuiSpinBox::setUpButtonImage(const QString& upButtonImage)
{
	Q_D(DuiSpinBox);
	d->m_upButtonImage = upButtonImage;
	d->m_upButton->setImagePath(upButtonImage);
}

QString DuiSpinBox::upButtonImage() const
{
	Q_D(const DuiSpinBox);
	return d->m_upButtonImage;
}

void DuiSpinBox::setDownButtonImage(const QString& downButtonImage)
{
	Q_D(DuiSpinBox);
	d->m_downButtonImage = downButtonImage;
	d->m_downButton->setImagePath(downButtonImage);
}

QString DuiSpinBox::downButtonImage() const
{
	Q_D(const DuiSpinBox);
	return d->m_downButtonImage;
}

void DuiSpinBox::stepUp()
{
	Q_D(const DuiSpinBox);
	d->m_spinBox->stepUp();
	d->m_spinBox->setFocus(Qt::MouseFocusReason);
}

void DuiSpinBox::stepDown()
{
	Q_D(const DuiSpinBox);
	d->m_spinBox->stepDown();
	d->m_spinBox->setFocus(Qt::MouseFocusReason);
}

bool DuiSpinBox::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
	Q_D(DuiSpinBox);
	if (watched == d->m_spinBox->graphicsProxyWidget())
	{
		switch (event->type())
		{
		case QEvent::FocusIn:
			d->m_frameItem->setChecked(true);
			break;
		case QEvent::FocusOut:
			d->m_frameItem->setChecked(false);
			break;
		}
	}

	return DuiWidget::sceneEventFilter(watched, event);
}

QVariant DuiSpinBox::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case QGraphicsItem::ItemEnabledHasChanged:
		{
			Q_D(DuiSpinBox);
			d->setEnable_helper(value.toBool());
		}
		break;
	}

	return DuiWidget::itemChange(change, value);
}

void DuiSpinBox::resizeEvent(QGraphicsSceneResizeEvent * event)
{
	Q_D(DuiSpinBox);
	d->relayout(QRectF(QPointF(), event->newSize()));
}

void DuiSpinBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_D(DuiSpinBox);
	d->m_frameItem->setHovered(true);
}

void DuiSpinBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_D(DuiSpinBox);
	d->m_frameItem->setHovered(false);
}


QSpinBoxEx* DuiSpinBox::realWidget(void)
{
	Q_D(DuiSpinBox);
	return d->m_spinBox;
}
