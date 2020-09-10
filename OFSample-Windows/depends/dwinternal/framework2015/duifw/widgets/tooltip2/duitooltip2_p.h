#pragma once
#include "duitooltip2.h"

class DuiWidget;
class DuiToolTip2Private : public QObject
{
	Q_DECLARE_PUBLIC(DuiToolTip2);
	Q_OBJECT;

public:
	DuiToolTip2Private();
	virtual ~DuiToolTip2Private();

	void	attatchWidget( DuiWidget* targetWidget );
	void	detatchWidget( DuiWidget* targetWidget );

	void	forceHideTooltipView();

	bool	isVisible() const;
	void	initTooltipService();

protected slots:

	virtual void onTooltipServiceEnterWidget( DuiWidget *targetWidget );
	virtual void onTooltipServiceLeaveWidget( DuiWidget *targetWidget );

	virtual void onTooltipServiceShowToolTip( const QString&, DuiWidget *targetWidget );
	virtual void onTooltipServiceHideTooltip( const QString&, DuiWidget *targetWidget );

	void onTargetWidgetDestroyed(QObject* object);

protected:
	DuiToolTip2						*q_ptr;

	DuiToolTip2::HidePolicy			m_hidePolicy;
	DuiToolTip2::ShowPolicy			m_showPolicy;

	quint32							m_showTime;
	quint32							m_hideTime;

	QString							m_id;

	DuiToolTipStyle2				m_style;
	QList< QPointer<DuiWidget> >	m_targetWidgets;
};


class DuiTextToolTip;
class DuiTextEdit;

class DuiTextToolTipPrivate : public DuiToolTip2Private
{
	Q_DECLARE_PUBLIC(DuiTextToolTip);
public:
	DuiTextToolTipPrivate();
	~DuiTextToolTipPrivate();

public:
	void forceShowToolTip( const QString& text, const QPoint& pos, QWidget* parent );
	void forceShowToolTip( const QString& text, const QRect& textRect, QWidget* parent );
protected:
	virtual void	onTooltipServiceShowToolTip( const QString& id, DuiWidget *targetWidget );

private:
	QSizeF			_textSize(const QString& text);
	void			_initTextView( const QString& text );
	DuiTextEdit*	_initTextEdit(const QString& text);
	void			_forceShowToolTip( const QString& text, const QPoint& pos, const QSize& size, QWidget* parent );

private:
	QRect			m_referenceRect;
	QString			m_text;
	bool			m_isAcceptHtml;	
};


class DuiWidgetToolTip;
class ToolTipService;
class DuiWidgetToolTipPrivate : public DuiToolTip2Private
{
	Q_DECLARE_PUBLIC(DuiWidgetToolTip);

public:
	DuiWidgetToolTipPrivate();
	~DuiWidgetToolTipPrivate();

	void forceShowTooltip(const QPoint& pos, QWidget* parent);

	bool isUnderMouse(void);

	QString getToolTipID(void);

	ToolTipService* getToolTipService(void);

protected:
	virtual void onTooltipServiceShowToolTip( const QString& id, DuiWidget *targetWidget );

private:
	void _showView(const QPoint& pos, QWidget* parent);

private:
	DuiWidget			*m_rootWidget;
	QPointer<DuiWidget>	m_currentWidget;
};


class DuiEmptyToolTip;
class DuiEmptyToolTipPrivate : public DuiToolTip2Private
{
	Q_DECLARE_PUBLIC(DuiEmptyToolTip);

protected:
	virtual void onTooltipServiceEnterWidget( DuiWidget *targetWidget );
	virtual void onTooltipServiceLeaveWidget( DuiWidget *targetWidget );

	virtual void onTooltipServiceShowToolTip( const QString& id, DuiWidget *targetWidget );
	virtual void onTooltipServiceHideTooltip( const QString& id, DuiWidget *targetWidget );
};
