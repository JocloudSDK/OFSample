#pragma once
#include "duifw/duiitemdrawers.h"

//------------------------------------------------------------------------------
class DuiNinePatchPixmapItemPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiNinePatchPixmapItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(int subIdx READ subIdx WRITE setSubIdx);
public:
    explicit DuiNinePatchPixmapItem(QGraphicsItem *parent = 0);
    explicit DuiNinePatchPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = 0);
    virtual ~DuiNinePatchPixmapItem();

    QPixmap pixmap() const;
    void setPixmap(const QString &p);
    void setPixmap(const QPixmap &p);

    Qt::TransformationMode transformationMode() const;
    void setTransformationMode(Qt::TransformationMode mode);

    void setSize(const QSizeF &s);

    QPointF offset() const;
    void setOffset(const QPointF &offset);
    inline void setOffset(qreal x, qreal y);

    int subIdx() const;
    void setSubIdx(int idx);

    virtual QRectF boundingRect() const;

protected:
    DuiNinePatchPixmapItem(DuiNinePatchPixmapItemPrivate &dd, QGraphicsItem *parent, QGraphicsScene *scene);

    void _updateSize();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Q_DISABLE_COPY(DuiNinePatchPixmapItem)
    Q_DECLARE_PRIVATE_D(m_ddptr, DuiNinePatchPixmapItem);

    DuiNinePatchPixmapItemPrivate *m_ddptr;
};// end of DuiNinePatchPixmapItem

inline void DuiNinePatchPixmapItem::setOffset(qreal ax, qreal ay)
{ setOffset(QPointF(ax, ay)); }