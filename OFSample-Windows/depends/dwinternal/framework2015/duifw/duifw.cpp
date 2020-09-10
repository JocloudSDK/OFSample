#include "stable.h"

#include "duifw.h"
#include "duisigslothook.h"

const char KRootPathProperty[] = "rootPath";
const char KAnchorParent[] = "parent";

//ui editor
const char KAttributeXmlNode[] = "__de_xmlnode";
const char KClassNameDuiFrameWindow[] = "DuiFrameWindow";
const char KClassNameDuiWidget[] = "DuiWidget";

QHash<int, QVariant> _dui_applicationAttributes;
void duiInitApplicationAttribute(duifw::ApplicationAttribute aa, const QVariant &value)
{
	_dui_applicationAttributes[aa] = value;
}

QVariant duiGetApplicationAttribute(duifw::ApplicationAttribute aa)
{
	if(_dui_applicationAttributes.contains(aa))
	{
		return _dui_applicationAttributes[aa];
	}

	return QVariant();
}

void duiRegisterSignalCallbacks( const QSignalSpyCallbackSet &callback_set )
{
	_dui_sigslot_callbacks = callback_set;
}

Qt::DropAction duiExecDrag(QDrag* drag, Qt::DropActions supportsActions/* = Qt::MoveAction*/)
{
	Qt::DropAction dropact = Qt::IgnoreAction;

	__try
	{
		drag->exec(supportsActions);
    }
	__except(EXCEPTION_EXECUTE_HANDLER )
	{
		LogFinal(LOG::KDuiFw) << "exception occurs when dragging!";
	}

	return dropact;
}