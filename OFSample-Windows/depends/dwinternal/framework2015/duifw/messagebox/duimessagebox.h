	/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
/////特殊messagebox处理
/*DuiMessageBox* message = DuiMessageBox::createMessageBoxDialog(NULL, "sdlkfasdf", "laskfasdasdfasdfasdfnfasdfa六七八", "asdf", "", "");
DuiCheckBox *checkBox = new DuiCheckBox(message->getAnchorWidget());
checkBox->setParent(message->getAnchorWidget());
checkBox->setText("alskdflasdflkasdlkfalskdflaksjflkasjflkasdlkfa");
AnchorInfo data;
data.anchorPoint[2] = Qt::AnchorTop;
data.anchorPoint[3] = Qt::AnchorTop;
data.distance[0] = 65;
data.distance[1] = 250;
data.distance[2] = 100;
data.distance[3] = 120;
checkBox->setAnchor(data);
*/
#pragma once

#include <QMessageBox>
#include "dwglobal.h"
#include "duifw/duidialog.h"

//------------------------------------------------------------------------------
class DuiMessageBoxPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiMessageBox : public DuiDialog
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiMessageBox )
	Q_DECLARE_PRIVATE(DuiMessageBox)
	DUI_DECLARE_DYNAMIC_FRAME_CREATE_SELF(DuiMessageBox)

public:
	explicit DuiMessageBox(QWidget *parent = NULL);
	virtual ~DuiMessageBox();

public:
	enum ButtonClicked
	{
		button0Clicked = 0,
		button1Clicked,
		button2Clicked,
		buttonCloseClicked
	};
	Q_ENUMS( ButtonClicked );

	// 原生的MessageBox
	static DuiMessageBox* createMessageBoxDialog(QWidget *parent, 
		const QString &title, 
		const QString& text,
		const QString& button0Text,
		const QString& button1Text,
		const QString& button2Text);	
	
	///  synchronization             
	static QMessageBox::StandardButton information(QWidget *parent,
		const QString &title,
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

	static QMessageBox::StandardButton question(QWidget *parent, 
		const QString &title,
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

	static QMessageBox::StandardButton warning(QWidget *parent,
		const QString &title,
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

	static QMessageBox::StandardButton critical(QWidget *parent,
		const QString &title,
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

	static ButtonClicked information(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1);

	static ButtonClicked question(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1);

	static ButtonClicked warning(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1);

	static ButtonClicked critical(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1);
	
	/// asynchronous                
	static DuiMessageBox* informationAsync(QWidget *parent,
		const QString &title, 
		const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* questionAsync(QWidget *parent,
		const QString &title,
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* warningAsync(QWidget *parent,
		const QString &title, 
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* criticalAsync(QWidget *parent,
		const QString &title,
		const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* informationAsync(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* questionAsync(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* warningAsync(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static DuiMessageBox* criticalAsync(QWidget *parent,
		const QString &title,
		const QString& text,
		const QString& button0Text,
		const QString& button1Text = QString(),
		const QString& button2Text = QString(),
		int defaultButtonNumber = 0,
		int escapeButtonNumber = -1,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	/// do not use any more, it will be deleted later
	static DuiMessageBox* information2(QWidget *parent,
		const QString &title, 
		const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	/// do not use any more, it will be deleted later
	static DuiMessageBox* warning2(QWidget *parent,
		const QString &title, 
		const QString &text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton,
		const QObject* receiver = NULL,
		const char* slot = NULL);

	static void closeAllOf();

	/// show and return immediately
	void showDialog();
	/// show it in WindowModal
	void showMessageDialog();
	/// show it in ApplicationModal and return immediatily
	void showAsynMessageDialog(const QObject* receiver = NULL, const char* slot = NULL);
	

	QMessageBox::StandardButton getClickedStandarButton()const;
	ButtonClicked  getClickedButton()const; 
	void setButtonIDValue(const QList<QMessageBox::StandardButton>& listSelectedButtons);
	void setMessageImagePath(const QString& imagePath);
	void setSize(const QSize& sz);

	DuiWidget* getAnchorWidget();

signals:
	void buttonClicked(ButtonClicked index);

protected slots:
	void on_button0_clicked();
	void on_button1_clicked();
	void on_button2_clicked();

	void on_htmlLinkClicked(const QString& url);
protected:
	virtual void construct();

private:
	virtual void accept();
	virtual void reject();

private:
	QMessageBox::StandardButton getSelectedButton(const QList<QMessageBox::StandardButton>& listStandarButton, int nIndex);

	static DuiMessageBox* _createMessageBoxDialog(QWidget *parent, const QString &title, const QString& text,
		const QString& button0Text,
		const QString& button1Text,
		const QString& button2Text);

	static QString standardButtonToText(QMessageBox::StandardButton standarButton);
	static void	getSelectStandarButton(QMessageBox::StandardButtons standarButtons, QList<QMessageBox::StandardButton>& listButton);
	static QString getTextStandardButtonList(const QList<QMessageBox::StandardButton>& listStandarButton, int nIndex);
	static int	calcTextHeight(const QString &text, int nTextWidth);	
};
