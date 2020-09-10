#include "stable.h"

#include "duifw/duilineedit.h"
#include "duiwidget_p.h"
#include "duihotkeywidget.h"

class DuiHotkeyWidgetPrivate : public DuiWidgetPrivate
{
	Q_DISABLE_COPY(DuiHotkeyWidgetPrivate)
public:
	DuiHotkeyWidgetPrivate()
	: m_textDisplay(NULL)
	, m_modifierKey(0)
	, m_virtualKey(0)
	, m_isEnable(true)
	{

	}
	~DuiHotkeyWidgetPrivate()
	{

	}
	DuiLineEdit *m_textDisplay;
	quint32 m_modifierKey;
	quint32 m_virtualKey;
	QString m_hotKeyString;
	bool	m_isEnable;
};

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiHotkeyWidget)

DuiHotkeyWidget::DuiHotkeyWidget( QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(*(new DuiHotkeyWidgetPrivate()), parent)
{
	Q_D(DuiHotkeyWidget);
	d->m_textDisplay = new DuiLineEdit(this);
	d->m_textDisplay->setEnableMenu(false);
	d->m_textDisplay->setText(tr("Hotkey"));
	d->m_textDisplay->installEventFilter(this);
	
	setDragPolicy(DuiWidget::NotDragWindow);

	connect( d->m_textDisplay, SIGNAL(focusOut()), SIGNAL(focusOut()) );
}

DuiHotkeyWidget::~DuiHotkeyWidget()
{

}

bool DuiHotkeyWidget::eventFilter( QObject *object, QEvent *event )
{
	Q_UNUSED(object);
	Q_D(DuiHotkeyWidget);
	if ( d->m_isEnable)
	{
		if (QEvent::KeyPress == event->type())
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			setHotkey(keyEvent->nativeModifiers(), keyEvent->nativeVirtualKey());
			emit hotkeyChanged(d->m_modifierKey, d->m_virtualKey);
			return true;
		}
	}
	return false;
}

QString DuiHotkeyWidget::getVirtualKeyName( quint32 keyCode )
{
	if ( (keyCode >= '0' && keyCode <= '9')
		|| (keyCode >= 'A' && keyCode <= 'Z') )
	{
		wchar_t name[64] = {0};
		name[0] = keyCode;
		return QString::fromWCharArray(name);
	}
	QString keyName;
	switch(keyCode)
	{
	case 0x8:
		keyName = "Backspace";
		break;
	case 0x14:
		keyName = "Caps Lock";
		break;
	case 0x20:
		keyName = "Space";
		break;
	case 0x21:
		keyName = "PageUp";
		break;
	case 0x22:
		keyName = "PageDown";
		break;
	case 0x23:
		keyName = "End";
		break;
	case 0x24:
		keyName = "Home";
		break;
	case 0x25:
		keyName = "Left";
		break;
	case 0x26:
		keyName = "Up";
		break;
	case 0x27:
		keyName = "Right";
		break;
	case 0x28:
		keyName = "Down";
		break;
	case 0x2d:
		keyName = "Insert";
		break;
	case 0x2e:
		keyName = "Delete";
		break;
	case 0x70:
		keyName = "F1";
		break;
	case 0x71:
		keyName = "F2";
		break;
	case 0x72:
		keyName = "F3";
		break;
	case 0x73:
		keyName = "F4";
		break;
	case 0x74:
		keyName = "F5";
		break;
	case 0x75:
		keyName = "F6";
		break;
	case 0x76:
		keyName = "F7";
		break;
	case 0x77:
		keyName = "F8";
		break;
	case 0x78:
		keyName = "F9";
		break;
	case 0x79:
		keyName = "F10";
		break;
	case 0x7a:
		keyName = "F11";
		break;
	case 0x7b:
		keyName = "F12";
		break;
	case 0x90:
		keyName = "Num Lock";
		break;
	case 0xc0:
		keyName = "`";
		break;
	case 0xbd:
		keyName = "-";
		break;
	case 0xbb:
		keyName = "=";
		break;
	case 0xdb:
		keyName = "[";
		break;
	case 0xdd:
		keyName = "]";
		break;
	case 0xbc:
		keyName = ",";
		break;
	case 0xbe:
		keyName = ".";
		break;
	case 0xbf:
		keyName = "/";
		break;
	case 0xdc:
		keyName = "\\";
		break;
	case 0xba:
		keyName = ";";
		break;
	case 0xde:
		keyName = "'";
		break;
	case 0x60:
		keyName = "Num0";
		break;
	case 0x61:
		keyName = "Num1";
		break;
	case 0x62:
		keyName = "Num2";
		break;
	case 0x63:
		keyName = "Num3";
		break;
	case 0x64:
		keyName = "Num4";
		break;
	case 0x65:
		keyName = "Num5";
		break;
	case 0x66:
		keyName = "Num6";
		break;
	case 0x67:
		keyName = "Num7";
		break;
	case 0x68:
		keyName = "Num8";
		break;
	case 0x69:
		keyName = "Num9";
		break;
	case 0x6a:
		keyName = "Num*";
		break;
	case 0x6b:
		keyName = "Num+";
		break;
	case 0x6c:
		keyName = "";
		break;
	case 0x6d:
		keyName = "Num-";
		break;
	case 0x6e:
		keyName = "Num.";
		break;
	case 0x6f:
		keyName = "Num/";
		break;

	}
	return keyName;
}

QString DuiHotkeyWidget::getModifierKeyName( quint32 keyCode )
{
	QString keyModifier;
	if ((keyCode & 0x1) == 0x1)
	{
		keyModifier = "Shift";
	}
	if ((keyCode & 0x2) == 0x2)
	{
		if (keyModifier.isEmpty())
		{
			keyModifier = "Ctrl";
		}
		else
		{
			keyModifier += " + Ctrl";
		}
		
	}
	if ((keyCode & 0x4) == 0x4)
	{
		if (keyModifier.isEmpty())
		{
			keyModifier = "Alt";
		}
		else
		{
			keyModifier += " + Alt";
		}
	}
	return keyModifier;
}

void DuiHotkeyWidget::setHotkey( quint32 modifierKey, quint32 virtualKey )
{
	Q_D(DuiHotkeyWidget);
	d->m_modifierKey = modifierToVirtualKey(modifierKey);
	//d->m_modifierKey = modifierKey;
	d->m_virtualKey = virtualKey;

	//spacer don't allow
	if (0x20 == d->m_virtualKey)
	{
		d->m_virtualKey = 0;
	}

	if (8 == d->m_virtualKey 
		|| 0x2e == d->m_virtualKey)
	{
		d->m_virtualKey = 0;
		d->m_modifierKey = 0;
	}
	QString keyName = _getModifierKeyName(d->m_modifierKey);
	if (!keyName.isEmpty())
	{
		keyName += " + ";
	}
	QString virtualKeyName = getVirtualKeyName(d->m_virtualKey);
	if (virtualKeyName.isEmpty())
	{
		d->m_virtualKey = 0;
	}
	keyName += virtualKeyName;
	if (keyName.isEmpty())
	{
		keyName = tr("N/A");
	}
	d->m_textDisplay->setText(keyName);
	d->m_hotKeyString = keyName;
}

void DuiHotkeyWidget::getHotkey( quint32 &modifierKey, quint32 &virtualKey )
{
	Q_D(DuiHotkeyWidget);

	modifierKey = virtualKeyToModifier( d->m_modifierKey );
	virtualKey = d->m_virtualKey;
}

QString DuiHotkeyWidget::getHotkey() const
{
	return d_func()->m_hotKeyString;	
}

quint32 DuiHotkeyWidget::virtualKeyToModifier( quint32 virtualKey )
{
	quint32 modifierKey = 0;

	if ( virtualKey & ModifierControl )
	{
		modifierKey |= 0x2;
	}

	if ( virtualKey & ModifierShift )
	{
		modifierKey |= 0x1;
	}

	if ( virtualKey & ModifierMenu )
	{
		modifierKey |= 0x4;
	}
	return modifierKey;
}

quint32 DuiHotkeyWidget::modifierToVirtualKey( quint32 modifierKey )
{
	quint32 virtualKey = 0;
	if (((modifierKey & 0x2) == 0x2) || ((modifierKey & 0x20) == 0x20))
	{
		virtualKey |= ModifierControl;// VK_CONTROL;
	}
	if (((modifierKey & 0x4) == 0x4) || ((modifierKey & 0x40) == 0x40))
	{
		virtualKey |= ModifierMenu;//VK_MENU;
	}
	if (((modifierKey & 0x1) == 0x1) || ((modifierKey & 0x10) == 0x10))
	{
		virtualKey |= ModifierShift;//VK_SHIFT;
	}
	return virtualKey;
}

void DuiHotkeyWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	DuiWidget::resizeEvent( event );

	QRect rect( QPoint(0, 0), event->newSize().toSize() );

	Q_D(DuiHotkeyWidget);
	d->m_textDisplay->setGeometry(rect);
}

void DuiHotkeyWidget::setEnableHotKey( bool isEnable )
{
	d_func()->m_isEnable = isEnable;
}

void DuiHotkeyWidget::setHotKey( const QString& key )
{
	d_func()->m_textDisplay->setText( key );
}

QString DuiHotkeyWidget::_getModifierKeyName( quint32 keyCode )
{
	QString keyModifier;

	if ((keyCode & ModifierControl) == ModifierControl)
	{
		keyModifier = "Ctrl";
	}
	if ((keyCode & ModifierMenu) == ModifierMenu)
	{
		if (keyModifier.isEmpty())
		{
			keyModifier = "Alt";
		}
		else
		{
			keyModifier += " + Alt";
		}
		
	}
	if ((keyCode & ModifierShift) == ModifierShift)
	{
		if (keyModifier.isEmpty())
		{
			keyModifier = "Shift";
		}
		else
		{
			keyModifier += " + Shift";
		}
	}
	return keyModifier;
}

void DuiHotkeyWidget::setBackgroundImage( const QString &image )
{
	Q_D(DuiHotkeyWidget);
	d->m_textDisplay->setFrameImage(image);
}

QString DuiHotkeyWidget::backgroundImage() const
{
	Q_D(const DuiHotkeyWidget);
	return d->m_textDisplay->frameImage();
}

void DuiHotkeyWidget::setFocus()
{
	Q_D(DuiHotkeyWidget);
	d->m_textDisplay->setFocus();
}
