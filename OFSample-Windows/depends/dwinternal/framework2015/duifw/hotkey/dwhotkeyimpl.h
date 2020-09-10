#pragma once


class DWHotkeyImpl : public QWidget
{
	Q_OBJECT
public:
	explicit DWHotkeyImpl( QWidget* parent = 0 );
	virtual ~DWHotkeyImpl();

signals:
	void hotkeyActivated( UINT32 modifierKey, UINT32 virtualKey );

private:
	bool winEvent( MSG *message, long *result );
};
