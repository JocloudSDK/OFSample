
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

extern const char KRootPathProperty[];
extern const char KAnchorParent[];

//for ui editor
extern DW_DUIFW_EXPORT const char KAttributeXmlNode[];
extern DW_DUIFW_EXPORT const char KClassNameDuiFrameWindow[];
extern DW_DUIFW_EXPORT const char KClassNameDuiWidget[];

namespace duifw
{
	enum ApplicationAttribute
	{
		AA_ApplicationName = 1,
		AA_ApplicationSettingTemplate,
		AA_OptimizationFontDatabase = 10000,
		/*
		Pass a QStringList that contains fallback fonts for every script, every string in the QStringList can contain multi fonts, splited by a comma.
		*/
		AA_FontFallback,
		/*
		Pass a QStringList that contains blacklist fonts for every script.
		*/
		AA_FontBlacklist,
	};
}

DW_DUIFW_EXPORT void duiInitApplicationAttribute(duifw::ApplicationAttribute aa, const QVariant &value);
DW_DUIFW_EXPORT QVariant duiGetApplicationAttribute(duifw::ApplicationAttribute aa);

struct QSignalSpyCallbackSet;
DW_DUIFW_EXPORT void duiRegisterSignalCallbacks(const QSignalSpyCallbackSet &callback_set);

DW_DUIFW_EXPORT Qt::DropAction duiExecDrag(QDrag* drag, Qt::DropActions supportsActions = Qt::MoveAction);