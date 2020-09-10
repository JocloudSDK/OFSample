
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

class QLineEditEx : public QLineEdit
{
	Q_OBJECT;
public:
	explicit QLineEditEx(QWidget *parent = NULL);
	~QLineEditEx(void);

public:
    enum KeySequenceType
    {
        enumKST_None  = 0x00,
        enumKST_Cut   = 0x01,
        enumKST_Copy  = 0x02,
        enumKST_Paste = 0x04,
        enumKST_Undo  = 0x08,
        enumKST_Redo  = 0x10,

        enumKST_Text  = enumKST_Cut | enumKST_Copy | enumKST_Paste,
        enukKST_Do    = enumKST_Undo | enumKST_Redo,
        enumKST_All   = enumKST_Cut | enumKST_Copy | enumKST_Paste | enumKST_Undo | enumKST_Redo,
    };

    quint32 disableKeySequence() const;
    void setDisableKeySequence(quint32 type);
    bool isKeySequenceDisable(quint32 type);

	void setInputMethodOffset(const QPoint&);
	const QPoint& inputMethodOffset() const;

public:
	virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const;

protected:
    quint32 m_disKeySequence;
	QPoint m_inputMethodOffset;

signals:
	void focusOut();
	void leftButtonClicked();
	void focusIn();
	void capsLockChanged();
	void focusIn(QFocusEvent* ev);
	void focusOut(QFocusEvent* ev);
	void hoverEnter(QHoverEvent *ev);
	void hoverLeave(QHoverEvent *ev);

protected:
	virtual void contextMenuEvent(QContextMenuEvent *event );
	virtual void keyPressEvent( QKeyEvent *event );
	virtual void mousePressEvent( QMouseEvent *event );
	virtual void focusOutEvent( QFocusEvent *event );
	virtual void focusInEvent( QFocusEvent *event );
	virtual bool event(QEvent *ev);
	virtual void hoverEnterEvent(QHoverEvent *ev);
	virtual void hoverLeaveEvent(QHoverEvent *ev);
};
