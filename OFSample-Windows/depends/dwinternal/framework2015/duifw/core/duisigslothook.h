#pragma once

#include <private/qobject_p.h>

extern QSignalSpyCallbackSet _dui_sigslot_callbacks;

void duiCallSignalBegin(QObject *object, int method_index);
void duiCallSignalEnd(QObject *object, int method_index);
