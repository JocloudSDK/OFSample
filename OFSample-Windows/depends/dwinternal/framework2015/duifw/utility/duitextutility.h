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
	
	// ���˵�Ӱ�쵥����ʾ���ַ�.
	static QString singleLineTextFilter( const QString& text );

	// ����Ӱ���Ű���ַ�.
	static QString multiLineTextFilter( const QString& text );

	//unicode()С��128���ַ���byte��Ϊ1�����ڵ���128����Ϊ2
	static bool fixupStringByBytes(QString& addText, int remainBytes, int pos = -1);
	static int measureStringByBytes(QString& text);

	//���ذ�ȫ����̬�ַ���, QStringList һ��������̬�ַ���
	enum {State_Normal=0, State_Hover, State_PressDown, State_Disble, State_CheckOrFocus, State_Max};
	static QStringList resolvingStates(const QString& states, 
		const QString& stateNormal="", 
		const QString& stateHover="", 
		const QString& statePressDown="", 
		const QString& stateDisable="", 
		const QString& stateCheckOrFocus="");
};