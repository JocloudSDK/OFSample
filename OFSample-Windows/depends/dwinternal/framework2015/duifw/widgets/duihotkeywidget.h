/****************************************************************************
Author: AaronWang
Email : wangqiang@chinaduo.com
Mobile: 15915985665
Remark:
****************************************************************************/
#pragma once

#include "duifw/duiwidget.h"
#include "dwglobal.h"

class DuiHotkeyWidgetPrivate;

enum ModifierKeyValue
{
	ModifierControl = 0x1,	//Control
	ModifierMenu	= 0x2,	//Alt
	ModifierShift	= 0x4	//Shift
};

class DW_DUIFW_EXPORT DuiHotkeyWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiHotkeyWidget)
	Q_DECLARE_PRIVATE(DuiHotkeyWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiHotkeyWidget)

	Q_PROPERTY( QString backgroundImage READ backgroundImage WRITE setBackgroundImage )
public:
	explicit DuiHotkeyWidget(QGraphicsItem *parent = NULL);
	~DuiHotkeyWidget();

	void setEnableHotKey( bool isEnable );
	void setHotkey(quint32 modifierKey, quint32 virtualKey);
	void getHotkey(quint32 &modifierKey, quint32 &virtualKey);
	void setHotKey(const QString& key );
	QString getHotkey() const;

	void setBackgroundImage( const QString &image );
	QString backgroundImage( ) const;

	void setFocus();

	static QString getVirtualKeyName(quint32 keyCode);
	static QString getModifierKeyName(quint32 keyCode);

signals:
	void hotkeyChanged(quint32 modifierKey, quint32 virtualKey);
	void focusOut();
protected:
	QString _getModifierKeyName( quint32 keyCode );

	bool eventFilter(QObject *object, QEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
private:
	 quint32 modifierToVirtualKey( quint32 modifierKey );
	 quint32 virtualKeyToModifier( quint32 virtualKey );
};