/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"
#include "duicombobox.h"
#include "itemviews/duilistwidget.h"

class DuiFrameItem;

class  DuiComboBoxListWindow : public QGraphicsView
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiComboBoxListWindow )

public:
	explicit DuiComboBoxListWindow( QWidget* parent, DuiComboBox *combo );
	~DuiComboBoxListWindow( void );

	void addPrototype( DuiAbstractViewItem *prototype );
	
	QStandardItemModel *model() const;
	void setModel( QStandardItemModel *itemModel );

	void showPopup(const QRect &comboRect, int maxHeight, bool isForceDropUp);

	int selectedIndex() const;
	void setSelectedIndex( int index );

	int indexBeforeChange() const;

	void doColorize( QRgb color );

	DuiListWidget* listWidget();

    void setItemHeight(int h);
    int itemHeight() const ;

	void setBackgroundImage( const QString &imagePath);

	enum QuickSelect
	{
		PreviouItem,
		NextItem,
		FirstItem,
		LastItem
	};

	// 快速选择，内已有边界检查，安全！
	void selectTo(QuickSelect to);
	
	void beginInsertRows();
	void endInsertRows(int start, int end);
signals:
	void focusOut();
	void focusIn();
	void comboxlist_itemClicked( int index );
	void selectedChanged(int index);	// 选择变更，用来通知外面的LabelEdit变化

protected slots:
	void on_comboxlist_itemClicked(int index);
	void on_modelReset(void);

protected:
	virtual void resizeEvent( QResizeEvent *event );
	virtual void hideEvent ( QHideEvent * event );
	virtual void showEvent ( QShowEvent * event );
    //virtual void hoverEvent( QHoverEvent *event );
	virtual bool event ( QEvent * event );

	void keyPressEvent(QKeyEvent *event);

private:
	void init();

private:
	QGraphicsScene	*m_scene;
	DuiComboBox		*m_comboBox;
    DuiFrameItem    *m_background;
	DuiListWidget	*m_comboBoxList;
	int				m_selectedIndex;
	int				m_indexBeforeChange;
    int               m_itemHeight;
};
