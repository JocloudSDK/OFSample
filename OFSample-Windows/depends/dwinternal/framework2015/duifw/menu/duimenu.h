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

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������
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

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������
	/*
	�����Ҫ�ڲ˵�����Ӧ�����ﵯ��ģʽ�Ի���������ϵͳ��ģʽ�Ի��򣨴򿪱����ļ�֮��ģ�
	Ӧ���� Qt::QueuedConnection ��������������Ϊ�˵��Ѿ������ˣ����Ƴ�ģʽѭ���󻹻�ص��˵������
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

	// ��ָ�����򵯳��˵�����ָ���˵���Ӧ�����ĳ����
	void			popup2(const QRect &alignRect, AlignCorner alignCorner = AlignAuto, QAction * atAction = 0);

	bool			connectMenuAction(const QString& actionName, QObject* obj, const char* slotName);
	
	using QWidget::removeAction;
	void			removeAction( const QString &actionName );
	QAction			*actionByName( const QString &actionName );
	// �Զ���������Ҫ���뱣��
	QAction			*actionByIndex(const int index);
	
	AlienType		alienMenuType() const;
	void			setAlienMenuType( AlienType alienType );
	
	//���������˲˵��İ�ť����,��ֹ�ٴε����������ε���
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