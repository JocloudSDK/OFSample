
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QObject>
#include "duiwidget_p.h"
#include "duititlebar.h"

//------------------------------------------------------------------------------
class DuiPushButton;
class DuiLabelWidget;

//------------------------------------------------------------------------------
class DuiTitlebarPrivate : public DuiWidgetPrivate
{
	Q_DISABLE_COPY(DuiTitlebarPrivate)

public:
	explicit DuiTitlebarPrivate();
	typedef QList<DuiPushButton*> ButtonList;

private:
	void on_minimizeButton_clicked();
	void on_maximizeButton_clicked();
	void on_restoreButton_clicked();
	void on_closeButton_clicked();
	void init();
	void setMaximized(bool maxzimized);
	void setHasMinimizeButton( bool hasMinimizeButton );
	void setHasMaximizeButton( bool hasMaximizeButton );
	DuiPushButton *titleButton( DuiTitlebar::TitlebarButton button );
	void setWindowId( QWidget *window  );
	void setIcon(const QPixmap &icon);
	void setIcon(const QString &icon);
	void setHeight(int h);

	void setMargins(const QMargins& m);
	void setAlignment(Qt::Alignment align);
	void setSpacing(int s);

	void _initTitleText();
	void _adjustButtonSize();

	void refreshButtonName();

private:
	DuiLabelWidget			*m_windowTitleItem;
	DuiImageWidget			*m_icon;
	DuiWidget						*m_left;
	int						m_height;

	//idx 0 ~ 3 maps to corresponed TitlebarButton object
	//don NOT remove them or change their order
	//elements are in right to left order of titlebar buttons
	ButtonList					m_buttons;

	bool								m_maximized;
	bool								m_hasMinimizeButton;
	bool								m_hasMaximizeButton;

	QPointer<QWidget> m_window;

	QMargins				m_margins;
	Qt::Alignment			m_alignment;
	int						m_spacing;

	Q_DECLARE_PUBLIC(DuiTitlebar);
};
