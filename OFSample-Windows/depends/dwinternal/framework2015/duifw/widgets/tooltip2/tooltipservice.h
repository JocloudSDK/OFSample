#pragma once
#include "duitooltip2.h"

class DuiWidget;
class ToolTipService : public QObject
{
	Q_OBJECT;

public:
	explicit ToolTipService( QObject *parent = NULL );
	~ToolTipService();

	void attachWidget( DuiWidget* targetWidget );
	void detachWidget( DuiWidget* targetWidget ); 

	void setShowTime( quint32 showTime );
	void setHideTime( quint32 hideTime );

	void setHidePolicy( DuiToolTip2::HidePolicy hidePolicy );
	void setShowPolicy( DuiToolTip2::ShowPolicy showPolicy );

	void setAcceptRect( const QRect &rect );

	//用于标识每个tooltip对像
	void setTooltipID( const QString& id );

	void trackShow(bool emitShow);

	void sendCloseToolTipSingal(void);

signals:
	void showToolTip( const QString& id, DuiWidget *targetWidget );
	void hideToolTip( const QString& id, DuiWidget *targetWidget ); 
	void closeToolTip( const QString& id, DuiWidget *targetWidget );

	void enterWidget( DuiWidget* targetWidget );
	void leaveWidget( DuiWidget* targetWidget ); 

protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:

	void _on_checkTimer_timeout();
	void _on_checkHideTimer_timeout();
	void _on_showTimer_timeout();
	void _on_targetWidget_destroyed(QObject* object);

private:

	void	_startShowTooltip(bool emitShow);
	void	_startHideTooltip();

	void	_hideTooltip();

	void	_checkHideTipsAndStopTimer();

	enum	State{Hidden,Showed,TrackHide};

private:
	
	QPointer< DuiWidget >	m_targetWidget;

	quint32		m_checkHoverTime;
	quint32		m_checkHideTime;

	// check the mouse has stopped begin showing the tooltip;
	QTimer		m_checkTimer;

	// check the mouse has moved after showing the tooltip;
	QTimer		m_checkHideTimer;

	// the time for show the tooltip;
	QTimer		m_showTimer;

	QPoint		m_hovePoint;
	QRect		m_acceptableRect;

	DuiToolTip2::HidePolicy		m_hidePolicy;
	DuiToolTip2::ShowPolicy		m_showPolicy;

	State		m_state;
	QString		m_tooltipID;
};

ToolTipService* toolTipInstance();
