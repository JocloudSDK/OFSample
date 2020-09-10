#include "stable.h"

#include "duisigslothook.h"

QSignalSpyCallbackSet _dui_sigslot_callbacks;

void duiCallSignalBegin(QObject *object, int method_index)
{
	if(_dui_sigslot_callbacks.signal_begin_callback)
	{
		_dui_sigslot_callbacks.signal_begin_callback(object, method_index, NULL);
	}
}

void duiCallSignalEnd(QObject *object, int method_index)
{
	if(_dui_sigslot_callbacks.signal_end_callback)
	{
		_dui_sigslot_callbacks.signal_end_callback(object, method_index);
	}
}