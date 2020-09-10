#include "stable.h"


#include "dwhotkeyimpl.h"

DWHotkeyImpl::DWHotkeyImpl( QWidget* parent /*= 0 */ )
: QWidget(parent)
{

}

DWHotkeyImpl::~DWHotkeyImpl()
{

}

bool DWHotkeyImpl::winEvent( MSG *message, long *result )
{
	Q_UNUSED(result);
	if ( WM_HOTKEY == message->message )
	{
		emit hotkeyActivated( LOWORD( message->lParam ), HIWORD( message->lParam ) );
		return true;
	}

	return false;
}
