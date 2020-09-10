#pragma once

#include "dwglobal.h"

class DuiMenuPrivate;

class DW_DUIFW_EXPORT DuiMenu : public QMenu
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiMenu)
	Q_DECLARE_PRIVATE(DuiMenu)
	
public:
	explicit DuiMenu(QWidget* parent = NULL);
	virtual ~DuiMenu();

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
	static DuiMenu* createPopupMenu();
#endif	//DW_COMPONENT_ENV

	enum AlignCorner
	{
		AlignAuto = 0,
		AlignTopLeft,
		AlignTopRight,
		AlignBottomLeft,
		AlignBottomRight
	};

	enum AlienType
	{
		NormalMenu = 0,
		MainMenu,
		TrayIconMenu
	};

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
	/*
	如果需要在菜单的响应函数里弹出模式对话框，尤其是系统的模式对话框（打开保存文件之类的）
	应该用 Qt::QueuedConnection ，否则会崩溃，因为菜单已经析构了，而推出模式循环后还会回到菜单对象里。
	*/
	bool	createFromFile(const QString& xml, QObject* actionSlotReceiver, Qt::ConnectionType connectionType = Qt::DirectConnection);
    bool	createFromFile(const QString& xml, const QString& rootPath, QObject* actionSlotReceiver, Qt::ConnectionType connectionType = Qt::DirectConnection);
    bool	createFromFile(const QString& xml, const QString& rootPath, QObject* actionSlotReceiver, Qt::ConnectionType connectionType, QTranslator *t);

	//deprecated methods
	QAction*		exec();
	QAction*		exec(const QRect &alignRect, Qt::Alignment alignment);
#endif	//DW_COMPONENT_ENV

	void			popup(QAction * atAction = 0);
	void			popup(const QRect &alignRect, Qt::Alignment alignment, QAction * atAction = 0);

	// 在指定区域弹出菜单，可指定菜单对应区域的某个角
	void			popup2(const QRect &alignRect, AlignCorner alignCorner = AlignAuto, QAction * atAction = 0);

	bool			connectMenuAction(const QString& actionName, QObject* obj, const char* slotName);
	
	using QWidget::removeAction;
	void			removeAction( const QString &actionName );
	QAction			*actionByName( const QString &actionName );
	// 自动化测试需要，请保留
	QAction			*actionByIndex(const int index);
	
	AlienType		alienMenuType() const;
	void			setAlienMenuType( AlienType alienType );
	
	//设置启动此菜单的按钮区域,防止再次点击该区域二次弹出
	void			setSenderRect(const QRectF &senderRect);

private slots:
	void			on_actionTriggered( QAction * action );
	void			on_aboutToHide();

protected:
	void			showEvent(QShowEvent *event);
	bool			event(QEvent * event);

	friend class DuiCustomMenuStyle;
	// custom paint
	void	drawPrimitivePanelMenu(QPainter *p, const QStyleOption *opt, const QWidget *w)const;
	void	drawPrimitiveFrameMenu(QPainter *p, const QStyleOption *opt, const QWidget *w)const;

	// custom element
	void	drawControlMenuItem(QPainter *p, const QStyleOption *opt, const QWidget *w)const;
	void	drawControlMenuScroller(QPainter *p, const QStyleOption *opt, const QWidget *w)const;
	void	drawControlMenuTearoff(QPainter *p, const QStyleOption *opt, const QWidget *w)const;
	void	drawControlMenuEmptyArea(QPainter *p, const QStyleOption *opt, const QWidget *w)const;

private:
	DuiMenuPrivate *d_ptr;
};