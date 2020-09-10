#pragma once
#include "duifw/duiframeitem.h"

//------------------------------------------------------------------------------
class DuiAnimationFrameItemPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiAnimationFrameItem : public DuiFrameItem
{
    Q_OBJECT
    Q_DISABLE_COPY(DuiAnimationFrameItem);
    Q_DECLARE_PRIVATE(DuiAnimationFrameItem);

    Q_PROPERTY( QString imagePath READ imagePath WRITE setImage);
public:
    enum AnimationWhen
    {
        No = 0x00,
        Enter = 0x01,
        Leave = 0x02,
        ALL = 0xff,
    };

    explicit DuiAnimationFrameItem(QGraphicsItem *parent = NULL);
    DuiAnimationFrameItem(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent);
    virtual ~DuiAnimationFrameItem();

    // override the frame item
    void setMargin(const QMargins &margin);

    void setImage(const QString &imageNormal, const QString &imageHover, 
        const QString &imageDown, const QString &imageDisable = "", const QString &imageCheck = "");
    void setImage(const QString &image );

    void setPressed(bool pressed);
    void setEnable(bool enabled);
    void setChecked(bool checked);
    void setHovered(bool hovered);

    void setFrameType(int type);

    // new interface
    void setAnimation(DuiFrameItem::FrameState state, AnimationWhen when);
    AnimationWhen animationOf(DuiFrameItem::FrameState state);
    void setKeepNormal(bool b);
    bool keepNormal() const;

    void setAnimationOff(bool b);
    bool animationOff() const;

protected:
    /// used for subclass
    DuiAnimationFrameItem(DuiAnimationFrameItemPrivate &dd, QGraphicsItem *parent);
    
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void timerEvent(QTimerEvent *event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};
