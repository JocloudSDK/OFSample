#pragma once

#include "dwglobal.h"

class DW_DUIFW_EXPORT duitextutils
{
public:
	//Format the nickName/channelName/groupName ext.

	//TODO: will be removed in 5.0. 
	//replace with singleLineTextFilter.
	static void formatName(QString& text);
	static void removeNewLine(QString& text);
	
	// 过滤掉影响单行显示的字符.
	static QString singleLineTextFilter( const QString& text );

	// 过滤影响排版的字符.
	static QString multiLineTextFilter( const QString& text );

	//unicode()小于128的字符的byte算为1，大于等于128的算为2
	static bool fixupStringByBytes(QString& addText, int remainBytes, int pos = -1);
	static int measureStringByBytes(QString& text);

	//返回安全的五态字符串, QStringList 一定会有五态字符串
	enum {State_Normal=0, State_Hover, State_PressDown, State_Disble, State_CheckOrFocus, State_Max};
	static QStringList resolvingStates(const QString& states, 
		const QString& stateNormal="", 
		const QString& stateHover="", 
		const QString& statePressDown="", 
		const QString& stateDisable="", 
		const QString& stateCheckOrFocus="");
};