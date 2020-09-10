
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

//------------------------------------------------------------------------------
class DuiTitlebarPrivate;
class DuiPushButton;
class DuiImageWidget;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiTitlebar : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiTitlebar)
	Q_DECLARE_PRIVATE( DuiTitlebar )

public:
	enum titlebarClick
	{
		Close = 0,
		Maximize = 1,
		Restore = 2,
		Minimize = 3,
		User = 4,
	};

	enum TitlebarButton
	{
		Button_Close = Close,
		Button_Maximize = Maximize,
		Button_Restore = Restore,
		Button_Minimize = Minimize,
		Button_User = User,
	};

	Q_ENUMS(titlebarClick)

public:
	explicit DuiTitlebar(QGraphicsItem *parent = NULL);
	void setMaximized(bool maximized);
	QString title() const;
	void setTitle( const QString& title );
	void setHasMinimizeButton( bool hasMinimizeButton );
	void setHasMaximizeButton( bool hasMaximizeButton );
	void addButton( DuiPushButton *item );
	bool takeButton( DuiPushButton *item );
	DuiPushButton *titleButton( DuiTitlebar::TitlebarButton button );
	void setWindowId( QWidget *window );
	void setIcon(const QPixmap &icon);
	void setIcon(const QString &icon);
	int height() const;
	void setHeight(int h);
	DuiImageWidget *icon();
	const DuiImageWidget *icon() const;
	QRect geometryHint(QRect windowRect);
	QRect cutNcArea(QRect windowRect);

	void refreshButtonName();

	void setMargins(const QMargins& m);
	void setAlignment(Qt::Alignment align);
	void setSpacing(int s);

signals:
	void buttonClicked(DuiTitlebar::titlebarClick);

private slots:
	void on_minimizeButton_clicked(void);
	void on_maximizeButton_clicked(void);
	void on_restoreButton_clicked(void);
	void on_closeButton_clicked(void);
	void reLayout(void);

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
	void _reLayout(const QSizeF& newSize);
	QRect buttonsRect();
};
