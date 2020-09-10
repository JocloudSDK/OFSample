
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

class QTreeViewEx : public QTreeView
{
	Q_OBJECT
public:
	explicit QTreeViewEx(QWidget *parent = NULL);
	~QTreeViewEx(void);

signals :
	void contextMenu( const QModelIndex & index );
	void clicked();
	void currentIndexChanged(const QModelIndex & index);

protected:
	virtual bool event(QEvent *ev);
	virtual void contextMenuEvent( QContextMenuEvent * event );
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};
