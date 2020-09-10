#ifndef DUI_SPLITERWIDGET2_WIDGET__
#define DUI_SPLITERWIDGET2_WIDGET__
#pragma once

#include "duifw/duiwidget.h"

#define PANE_PRIORITY_MIN   0           //保持窗口size优先级最小值
#define PANE_PRIORITY_MAX   100000      //保持窗口size优先级最大值
#define SPLITER_DEF_SIZE    3           //默认分隔条大小
//窗口拉伸时，优先级最低的窗口最先被拉伸，走到它们的最小值（最大值）

class DuiSpliterPane2Private;
class DuiCheckBox;
class DW_DUIFW_EXPORT DuiSpliterPane2 : public DuiWidget
{
public:
    enum ButtonTarget
    {
        BT_LEFT=0,
        BT_TOP=0,
        BT_RIGHT=1,
        BT_BOTTOM=1,
    };
private:
    Q_OBJECT
    Q_ENUMS(ButtonTarget)
    Q_DISABLE_COPY(DuiSpliterPane2)
    Q_DECLARE_PRIVATE( DuiSpliterPane2 )
    DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSpliterPane2 )
    Q_PROPERTY(int priority READ priority WRITE setPriority)
    Q_PROPERTY(QString closePaneImage READ closePaneImage WRITE setClosePaneImage)
    Q_PROPERTY(ButtonTarget buttonTarget READ buttonTarget WRITE setButtonTarget)    
public:
    explicit DuiSpliterPane2( QGraphicsItem* parent );
    
    DuiCheckBox * closePaneButton();
public:
    int priority() const;
    void setPriority(int priority);    

    QString closePaneImage() const;
    void setClosePaneImage(const QString & strCloseBtnImage);
    
    ButtonTarget buttonTarget() const;
    void setButtonTarget(ButtonTarget bt);
private slots:
    void onCloseBtnStateChanged(bool bchecked);
signals:
    void priorityChanged(DuiSpliterPane2 * pPane);
    void closeBtnStateChanged(DuiSpliterPane2 * pPane,bool bchecked);
};

class DuiSpliterWidget2Private;

class DW_DUIFW_EXPORT DuiSpliterWidget2: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiSpliterWidget2)
	Q_DECLARE_PRIVATE( DuiSpliterWidget2 )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSpliterWidget2 )
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(QString spliterImage READ spliterImage WRITE setSpliterImage)
    Q_PROPERTY(int spliterSize READ spliterSize WRITE setSpliterSize)

public:
	explicit DuiSpliterWidget2( QGraphicsItem* parent );

    /*virtual*/ 
    void addItem(DuiWidget* item);
    /*virtual*/ 
    void removeItem(DuiWidget *item);
    /*virtual*/ 
    int  insertItem(DuiWidget* item,int index = -1);
    
    DuiSpliterPane2 * getPane(int iPane);
public:
    void setOrientation( Qt::Orientation orientation );
    Qt::Orientation orientation() const;
    
    QString spliterImage() const;
    void setSpliterImage(const QString & strSpliterImage);
    
    int spliterSize() const;
    void setSpliterSize(int nSize);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void resizeEvent(QGraphicsSceneResizeEvent *event);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    
private slots:
    void onPanePriotityChanged(DuiSpliterPane2 *pane);
    void onPaneCloseBtnStateChanged(DuiSpliterPane2 *pane,bool bchecked);
    void onPaneVisibleChanged();
};

#endif //DUI_SPLITERWIDGET2_WIDGET__
