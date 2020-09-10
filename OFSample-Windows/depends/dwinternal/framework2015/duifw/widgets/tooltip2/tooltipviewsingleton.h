#pragma once

class DuiWidget;
class DuiToolTipStyle2;
class DuiToolTipView;

class ToolTipViewSingleton
{

public:
	ToolTipViewSingleton();
	~ToolTipViewSingleton();

public:

	void	initView();

	DuiWidget*	rootWidget();

	void	setStyle( const DuiToolTipStyle2& style );
	void	setWidget( DuiWidget* widget, bool takeOwnerShip = true );

	void	showToolTipView();
	void	showTooltipView(const QPoint& pos, QWidget* parent);

	void	closeToolTipView();

	/* ����ǿ����ʾ�����صĴ��ڱ�ʶ��
		ȷ�� tooltip ʵ��ֻ�ܹر����� forceshow �� view.
	*/
	void	setViewId(const QString& viewId);
	QString viewId() const;

	void	setReferenceRect( const QRect& rect = QRect() );

	QRect	tooltipViewRect();

	bool	isVisible();

	bool isUnderMouse(void);

private:
	void	_animationMoveView( const QRect& targetRect );
	void	_animationShowView();

private:
	QPointer<DuiToolTipView> m_view;
	QRect	m_referenceRect;
	QString	m_viewId;
};

ToolTipViewSingleton* toolTipViewSingleton();