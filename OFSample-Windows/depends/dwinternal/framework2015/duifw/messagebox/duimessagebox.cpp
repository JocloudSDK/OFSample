#include "stable.h"

#include "dwutility/usystem.h"
#include "platform/windows/duiwinapi.h"

#include "duimessagebox.h"
#include "duilabelwidget.h"
#include "duiimagewidget.h"
#include "duipushbutton.h"
#include "richedit/duitextedit.h"
#include "frame/duidialog_p.h"

typedef QPointer<DuiMessageBox> DuiMessageBoxPtr;
typedef QList<DuiMessageBoxPtr> MessageBoxList;
Q_GLOBAL_STATIC(MessageBoxList, _dui_GetMessageBoxList);

//------------------------------------------------------------------------------
class DuiMessageBoxPrivate : public DuiDialogPrivate
{
	Q_DISABLE_COPY(DuiMessageBoxPrivate)

public:

	enum ButtonIndex
	{
		Invalid = -1,
		DMB_Button0 = 0,
		DMB_Button1 = 1,
		DMB_Button2 = 2,
	};

	DuiMessageBoxPrivate();
	virtual ~DuiMessageBoxPrivate();

	QMessageBox::StandardButton m_standarButton0ID;
	QMessageBox::StandardButton m_standarButton1ID;
	QMessageBox::StandardButton m_standarButton2ID;
	int			   m_clickedButton;
};

DuiMessageBoxPrivate::DuiMessageBoxPrivate()
: m_standarButton0ID(QMessageBox::NoButton)
, m_standarButton1ID(QMessageBox::NoButton)
, m_standarButton2ID(QMessageBox::NoButton)
, m_clickedButton(Invalid)
{
	;
}

DuiMessageBoxPrivate::~DuiMessageBoxPrivate()
{
	;
}

//------------------------------------------------------------------------------
DUI_IMPLEMENT_DYNAMIC_FRAME_CREATE_SELF(DuiMessageBox)
DuiMessageBox::DuiMessageBox( QWidget *parent /*= NULL*/ )
: DuiDialog(*(new DuiMessageBoxPrivate()), parent)
{
	_dui_GetMessageBoxList()->append(this);
}

DuiMessageBox::~DuiMessageBox()
{
	_dui_GetMessageBoxList()->removeOne(this);
}

QString DuiMessageBox::standardButtonToText( QMessageBox::StandardButton standarButton )
{
	QString strText;
	switch(standarButton)
	{
	case QMessageBox::NoButton:
		strText = ""; break;
	case QMessageBox::Ok:
		strText = tr("OK"); break;
	case QMessageBox::Save:
		strText = tr("Save"); break;     
	case QMessageBox::SaveAll:
		strText = tr("Save All"); break;       
	case QMessageBox::Open:
		strText = tr("Open"); break;       
	case QMessageBox::Yes:
		strText = tr("Yes"); break;       
	case QMessageBox::YesToAll:
		strText = tr("Yes To All"); break;  
	case QMessageBox::No:
		strText = tr("No"); break;   
	case QMessageBox::NoToAll:
		strText = tr("No To All"); break;       
	case QMessageBox::Abort:
		strText = tr("Abort"); break;       
	case QMessageBox::Retry:
		strText = tr("Retry"); break;       
	case QMessageBox::Ignore:
		strText = tr("Ignore"); break;       
	case QMessageBox::Close:
		strText = tr("Close"); break;       
	case QMessageBox::Cancel:
		strText = tr("Cancel"); break;       
	case QMessageBox::Discard:
		strText = tr("Discard"); break;       
	case QMessageBox::Help:
		strText = tr("Help"); break;       
	case QMessageBox::Apply:
		strText = tr("Apply"); break;       
	case QMessageBox::Reset:
		strText = tr("Reset"); break;       
	case QMessageBox::RestoreDefaults:
		strText = tr("Restore Default"); break;  	
	default:
		strText = ""; break;
	}
	return strText;
}

DuiMessageBox* DuiMessageBox::_createMessageBoxDialog( QWidget *parent, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text, const QString& button2Text )
{
	DuiMessageBox * pDialog = qobject_cast<DuiMessageBox*>(DuiXmlUI::createFromFile("layout/duifw/messagebox.xml", parent));

	DuiTextEdit* labelContent = pDialog->findChild<DuiTextEdit*>("textContent");
	labelContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	DuiWidget *buttonWidget = pDialog->findChild<DuiWidget*>("buttonWidget");
	QGraphicsLinearLayout *buttonLayout = (QGraphicsLinearLayout*)buttonWidget->layout();
	DuiPushButton*  button0 = pDialog->findChild<DuiPushButton*>("button0");
	DuiPushButton*  button1 = pDialog->findChild<DuiPushButton*>("button1");
	DuiPushButton*  button2 = pDialog->findChild<DuiPushButton*>("button2");

	pDialog->setTitle(title);
	labelContent->setTextColor(qRgb(0, 0, 0));
	labelContent->clear();
	labelContent->insertHtml(text);
	pDialog->connect( labelContent, SIGNAL(htmlLinkClicked(const QString&)), SLOT(on_htmlLinkClicked(const QString&)));

	if (button2Text.isEmpty())
	{
		button2->setVisible(false);
		buttonLayout->removeItem(button2);
	}
	else
	{
		button2->setText(button2Text);
	}

	if (button1Text.isEmpty())
	{
		button1->setVisible(false);
		buttonLayout->removeItem(button1);
	}
	else
	{
		button1->setText(button1Text);
	}

	if (button0Text.isEmpty())
	{
		button0->setVisible(false);
		buttonLayout->removeItem(button0);
	}
	else
	{
		button0->setText(button0Text);
	}

	int nHeight = calcTextHeight(labelContent->toPlainText(), 250);
	QRect gmDialog(0,0,340,160);//= pDialog->realFrameGeometry();	//默认高度
	if (nHeight > 40)		//text默认 40的高度
	{
		gmDialog.setHeight(gmDialog.height() + nHeight - 40);
	}

	QRect targetRect = DuiWinAPI::getWorkArea();
	if (NULL != parent)
	{
		targetRect = parent->geometry();
	}
	gmDialog.moveCenter(targetRect.center());
	pDialog->setRealFrameGeometry(gmDialog);

	return pDialog;
}

DuiMessageBox* DuiMessageBox::createMessageBoxDialog(QWidget *parent,
													 const QString &title, 
													 const QString& text, 
													 const QString&	button0Text, 
													 const QString& button1Text, 
													 const QString& button2Text )
{
	DuiMessageBox* messagebox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	messagebox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");

	return messagebox;
}

void DuiMessageBox::construct()
{
	;
}

void DuiMessageBox::on_button0_clicked()
{
	Q_D(DuiMessageBox);
	d->m_clickedButton = DuiMessageBoxPrivate::DMB_Button0;
	emit buttonClicked(getClickedButton());
	close();
}

void DuiMessageBox::on_button1_clicked()
{
	Q_D(DuiMessageBox);
	d->m_clickedButton = DuiMessageBoxPrivate::DMB_Button1;
	emit buttonClicked(getClickedButton());
	close();
}

void DuiMessageBox::on_button2_clicked()
{
	Q_D(DuiMessageBox);
	d->m_clickedButton = DuiMessageBoxPrivate::DMB_Button2;
	emit buttonClicked(getClickedButton());
	close();
}

void DuiMessageBox::on_htmlLinkClicked( const QString& url )
{
	if(url.startsWith("http://", Qt::CaseInsensitive) || url.startsWith("https://", Qt::CaseInsensitive))
	{
		DwUtility::system::openUrl( url.toStdWString() );
	}
	else
	{
		DW_ASSERT_X(false, __FUNCDNAME__, "The url should start with 'http://' or 'https://'.");
		DwUtility::system::openUrl( ("http://" + url).toStdWString() );
	}
}

DuiMessageBox::ButtonClicked DuiMessageBox::getClickedButton()const
{
	Q_D(const DuiMessageBox);
	ButtonClicked buttonClicked;
	switch(d->m_clickedButton)
	{
	case DuiMessageBoxPrivate::DMB_Button0:
		buttonClicked = button0Clicked; break;
	case DuiMessageBoxPrivate::DMB_Button1:
		buttonClicked = button1Clicked; break;
	case DuiMessageBoxPrivate::DMB_Button2:
		buttonClicked = button2Clicked; break;
	default:
		buttonClicked = buttonCloseClicked; break;
	}
	return buttonClicked;
}

QMessageBox::StandardButton DuiMessageBox::getClickedStandarButton()const
{
	QMessageBox::StandardButton clickedButton;
	Q_D(const DuiMessageBox);

	switch(d->m_clickedButton)
	{
	case DuiMessageBoxPrivate::DMB_Button0:
		clickedButton = d->m_standarButton0ID; break;
	case DuiMessageBoxPrivate::DMB_Button1:
		clickedButton = d->m_standarButton1ID; break;
	case DuiMessageBoxPrivate::DMB_Button2:
		clickedButton = d->m_standarButton2ID; break;
	default:
		clickedButton = QMessageBox::NoButton; break;
	}
	return clickedButton;
}


void DuiMessageBox::getSelectStandarButton( QMessageBox::StandardButtons standarButtons, QList<QMessageBox::StandardButton>& listButton )
{
	QList<QMessageBox::StandardButton>listAllButton;
	QMessageBox::StandardButton i = QMessageBox::Ok;
	while (i < QMessageBox::RestoreDefaults)
	{
		if (i & standarButtons)
		{
			listButton.push_back(i);
		}
		i = (QMessageBox::StandardButton)(i << 1);
	}
}

//### fix me
QString DuiMessageBox::getTextStandardButtonList( const QList<QMessageBox::StandardButton>& listStandarButton, int nIndex )
{
	if (listStandarButton.size() > nIndex)
	{
		return standardButtonToText(listStandarButton.at(nIndex));
	}
	return QString();
}

QMessageBox::StandardButton DuiMessageBox::getSelectedButton( const QList<QMessageBox::StandardButton>& listStandarButton, int nIndex )
{
	if (listStandarButton.size() > nIndex)
	{
		return listStandarButton.at(nIndex);
	}
	else
	{
		return QMessageBox::NoButton;
	}
}

void DuiMessageBox::setButtonIDValue( const QList<QMessageBox::StandardButton>& listSelectedButtons )
{
	Q_D(DuiMessageBox);

	d->m_standarButton0ID = getSelectedButton(listSelectedButtons, 0);
	d->m_standarButton1ID = getSelectedButton(listSelectedButtons, 1);
	d->m_standarButton2ID = getSelectedButton(listSelectedButtons, 2);
}

void DuiMessageBox::setMessageImagePath( const QString& imagePath )
{
	DuiImageWidget* iconImage = findChild<DuiImageWidget*>("icon");
	if (iconImage != NULL)
	{
		iconImage->setImagePath(imagePath);
	}
}

int DuiMessageBox::calcTextHeight(const QString &text, int nTextWidth)
{
	QGraphicsTextItem textItem;
	textItem.setTextWidth(nTextWidth);
	textItem.setPlainText(text);

	int height = textItem.boundingRect().height();
	height += (textItem.document()->lineCount()-1) * 20;
	return height;
}

void DuiMessageBox::accept()
{
	on_button0_clicked();
}

void DuiMessageBox::reject()
{
	close();
}

DuiWidget* DuiMessageBox::getAnchorWidget()
{
	DuiWidget* anchorWidget = findChild<DuiWidget*>("contentAnchor");
	return anchorWidget;
}

void DuiMessageBox::showMessageDialog()
{
	showModal(!DwUtility::system::isPlayingGame());
}

void DuiMessageBox::showDialog()
{
	setWindowModality(Qt::WindowModal);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowPlacement(ParentCenter);

    bool isPlayingGame = DwUtility::system::isPlayingGame();
	show();	
	if (!isPlayingGame)
	{
		activateWindow();
	}
}

void DuiMessageBox::showAsynMessageDialog(const QObject* receiver /*= NULL*/, const char* slot /*= NULL*/)
{
	if (slot && receiver)
	{
		bool bConnected = QObject::connect(this, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
		if (!bConnected)
		{
			DW_ASSERT(false);
		}
	}

	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowPlacement(ParentCenter);

    bool isPlayingGame = DwUtility::system::isPlayingGame();
	show();	
	if (!isPlayingGame)
	{
		activateWindow();
	}
}

void DuiMessageBox::closeAllOf()
{
	if (!_dui_GetMessageBoxList()->isEmpty())
	{
		MessageBoxList messageBoxPointers = *_dui_GetMessageBoxList();
		foreach(DuiMessageBoxPtr messageBoxPointer, messageBoxPointers)
		{
			if (!messageBoxPointer.isNull())
			{
				messageBoxPointer->close();
			}
		}
	}
}

void DuiMessageBox::setSize( const QSize& sz )
{
	QRect rc = realFrameGeometry();
	rc.setWidth(sz.width());
	rc.setHeight(sz.height());
	setRealFrameGeometry(rc);
}

//------------------------------------------------------------------------------
/**
*/
QMessageBox::StandardButton DuiMessageBox::information( QWidget *parent, 
													   const QString &title, 
													   const QString &text, 
													   QMessageBox::StandardButtons buttons /*= Ok*/,
													   QMessageBox::StandardButton defaultButton /*= NoButton*/ )
{
	Q_UNUSED(defaultButton);

	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	QPointer<DuiMessageBox> messageBox  = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");
	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedStandarButton();
	}
	else
	{
		return QMessageBox::NoButton;
	}
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox::ButtonClicked DuiMessageBox::information( QWidget *parent,
														const QString &title, 
														const QString& text, 
														const QString& button0Text, 
														const QString& button1Text /*= QString()*/, 
														const QString& button2Text /*= QString()*/,
														int defaultButtonNumber /*= 0*/, 
														int escapeButtonNumber /*= -1*/ )
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	messageBox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");
	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedButton();
	}
	else
	{
		return DuiMessageBox::buttonCloseClicked;
	}
}

//------------------------------------------------------------------------------
/**
*/
QMessageBox::StandardButton DuiMessageBox::question( QWidget *parent, 
													const QString &title, 
													const QString &text, 
													QMessageBox::StandardButtons buttons /*= Ok*/,
													QMessageBox::StandardButton defaultButton /*= NoButton*/ )
{
	Q_UNUSED(defaultButton);
	
	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_ask.png");

	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedStandarButton();
	}
	else
	{
		return QMessageBox::NoButton;
	}
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox::ButtonClicked DuiMessageBox::question( QWidget *parent, 
													 const QString &title, 
													 const QString& text, 
													 const QString& button0Text, 
													 const QString& button1Text /*= QString()*/, 
													 const QString& button2Text /*= QString()*/, 
													 int defaultButtonNumber /*= 0*/, 
													 int escapeButtonNumber /*= -1*/ )
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	messageBox->setMessageImagePath(":/theme/duifw/hint_ask.png");
	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedButton();
	}
	else
	{
		return DuiMessageBox::buttonCloseClicked;
	}
}

//------------------------------------------------------------------------------
/**
*/
QMessageBox::StandardButton DuiMessageBox::warning( QWidget *parent, 
												   const QString &title, 
												   const QString &text, 
												   QMessageBox::StandardButtons buttons /*= Ok*/,
												   QMessageBox::StandardButton defaultButton /*= NoButton*/ )
{
	Q_UNUSED(defaultButton);
	
	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_orange_exclamation.png");

	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedStandarButton();
	}
	else
	{
		return QMessageBox::NoButton;
	}
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox::ButtonClicked DuiMessageBox::warning( QWidget *parent, 
													const QString &title, 
													const QString& text, 
													const QString& button0Text,
													const QString& button1Text /*= QString()*/,
													const QString& button2Text /*= QString()*/,
													int defaultButtonNumber /*= 0*/,
													int escapeButtonNumber /*= -1*/ )
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	messageBox->setMessageImagePath(":/theme/duifw/hint_orange_exclamation.png");
	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedButton();
	}
	else
	{
		return DuiMessageBox::buttonCloseClicked;
	}
}

//------------------------------------------------------------------------------
/**
*/
QMessageBox::StandardButton DuiMessageBox::critical( QWidget *parent, 
													const QString &title, 
													const QString &text, 
													QMessageBox::StandardButtons buttons /*= Ok*/,
													QMessageBox::StandardButton defaultButton /*= NoButton*/ )
{
	Q_UNUSED(defaultButton);
	
	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_wrong.png");

	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedStandarButton();
	}
	else
	{
		return QMessageBox::NoButton;
	}
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox::ButtonClicked DuiMessageBox::critical( QWidget *parent, 
													 const QString &title, 
													 const QString& text, 
													 const QString& button0Text, 
													 const QString& button1Text /*= QString()*/, 
													 const QString& button2Text /*= QString()*/, 
													 int defaultButtonNumber /*= 0*/, int escapeButtonNumber /*= -1*/ )
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	QPointer<DuiMessageBox> messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	messageBox->setMessageImagePath(":/theme/duifw/hint_wrong.png");
	messageBox->showMessageDialog();

	if (!messageBox.isNull())
	{
		return messageBox->getClickedButton();
	}
	else
	{
		return DuiMessageBox::buttonCloseClicked;
	}
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::informationAsync(QWidget *parent, 
										   const QString &title,
										  const QString &text, 
										  QMessageBox::StandardButtons buttons,
										  QMessageBox::StandardButton defaultButton,
										  const QObject* receiver/* = NULL*/,
										  const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButton);

	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
	wrap async call, will be deleted later
*/
DuiMessageBox* DuiMessageBox::information2(QWidget *parent, 
											   const QString &title,
											   const QString &text, 
											   QMessageBox::StandardButtons buttons,
											   QMessageBox::StandardButton defaultButton,
											   const QObject* receiver/* = NULL*/,
											   const char* slot/* = NULL*/)
{
	return DuiMessageBox::informationAsync(parent, title, text, buttons, defaultButton, receiver, slot);
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::questionAsync(QWidget *parent,
										const QString &title,											 
										const QString &text,
										QMessageBox::StandardButtons buttons/* = QMessageBox::Ok*/,
										QMessageBox::StandardButton defaultButton/* = QMessageBox::NoButton*/,
										const QObject* receiver/* = NULL*/,
										const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButton);

	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_ask.png");
	messageBox->showDialog();
	
	return messageBox;
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::warningAsync( QWidget *parent, 
									   const QString &title, 
									   const QString &text, 
									   QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, 
									   QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/ ,
									   const QObject* receiver/* = NULL*/,
									   const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButton);

	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_orange_exclamation.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
	wrap async call, will be deleted later
*/
DuiMessageBox* DuiMessageBox::warning2( QWidget *parent, 
										   const QString &title, 
										   const QString &text, 
										   QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, 
										   QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/ ,
										   const QObject* receiver/* = NULL*/,
										   const char* slot/* = NULL*/)
{
	return DuiMessageBox::warningAsync(parent, title, text, buttons, defaultButton, receiver, slot);
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::criticalAsync(QWidget *parent,
										const QString &title,
										const QString &text, 
										QMessageBox::StandardButtons buttons/* = QMessageBox::Ok*/,
										QMessageBox::StandardButton defaultButton/* = QMessageBox::NoButton*/,
										const QObject* receiver/* = NULL*/,
										const char* slot /*= NULL*/)
{
	Q_UNUSED(defaultButton);

	QList<QMessageBox::StandardButton> listSelectedButton;
	getSelectStandarButton(buttons, listSelectedButton);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, 
		getTextStandardButtonList(listSelectedButton, 0), 
		getTextStandardButtonList(listSelectedButton, 1), 
		getTextStandardButtonList(listSelectedButton, 2));

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setButtonIDValue(listSelectedButton);
	messageBox->setMessageImagePath(":/theme/duifw/hint_wrong.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::informationAsync(QWidget *parent, 
										   const QString &title,
										   const QString& text,
										   const QString& button0Text,
										   const QString& button1Text/* = QString()*/,
										   const QString& button2Text/* = QString()*/,
										   int defaultButtonNumber/* = 0*/,
										   int escapeButtonNumber/* = -1*/,
										   const QObject* receiver/* = NULL*/,
										   const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);
	
	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::questionAsync(QWidget *parent, 
										const QString &title,
										const QString& text,
										const QString& button0Text,
										const QString& button1Text/* = QString()*/,
										const QString& button2Text/* = QString()*/,
										int defaultButtonNumber/* = 0*/,
										int escapeButtonNumber/* = -1*/,
										const QObject* receiver/* = NULL*/,
										const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setMessageImagePath(":/theme/duifw/hint_blue_exclamation.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::warningAsync(QWidget *parent, 
									   const QString &title,
									   const QString& text,
									   const QString& button0Text,
									   const QString& button1Text/* = QString()*/,
									   const QString& button2Text/* = QString()*/,
									   int defaultButtonNumber/* = 0*/,
									   int escapeButtonNumber/* = -1*/,
									   const QObject* receiver/* = NULL*/,
									   const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setMessageImagePath(":/theme/duifw/hint_orange_exclamation.png");
	messageBox->showDialog();

	return messageBox;
}

//------------------------------------------------------------------------------
/**
*/
DuiMessageBox* DuiMessageBox::criticalAsync(QWidget *parent, 
										const QString &title,
										const QString& text,
										const QString& button0Text,
										const QString& button1Text/* = QString()*/,
										const QString& button2Text/* = QString()*/,
										int defaultButtonNumber/* = 0*/,
										int escapeButtonNumber/* = -1*/,
										const QObject* receiver/* = NULL*/,
										const char* slot/* = NULL*/)
{
	Q_UNUSED(defaultButtonNumber);
	Q_UNUSED(escapeButtonNumber);

	DuiMessageBox* messageBox = _createMessageBoxDialog(parent, title, text, button0Text, button1Text, button2Text);

	if (slot && receiver)
	{
		QObject::connect(messageBox, SIGNAL(buttonClicked(ButtonClicked)), receiver, slot);
	}

	messageBox->setMessageImagePath(":/theme/duifw/hint_wrong.png");
	messageBox->showDialog();

	return messageBox;
}