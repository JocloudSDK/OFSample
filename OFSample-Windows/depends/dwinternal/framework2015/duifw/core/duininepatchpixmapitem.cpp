#include "stable.h"
#include "duininepatchpixmapitem.h"
#include "duitheme.h"
#include "duidrawutil.h"

class DuiNinePatchPixmapItemPrivate
{
public:
    DuiNinePatchPixmapItemPrivate();
    QPixmap m_pixmap;
    QSizeF m_size;
    QPointF m_offset;
    Qt::TransformationMode m_transformationMode;
    int m_subIdx;
};

DuiNinePatchPixmapItemPrivate::DuiNinePatchPixmapItemPrivate()
: m_subIdx(0)
, m_transformationMode(Qt::FastTransformation)
{
    ;
}

DuiNinePatchPixmapItem::DuiNinePatchPixmapItem(QGraphicsItem *parent/* = 0*/)
: QGraphicsObject(parent)
{
    m_ddptr = new DuiNinePatchPixmapItemPrivate();
}

DuiNinePatchPixmapItem::DuiNinePatchPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent/* = 0*/)
: QGraphicsObject(parent)
{
    m_ddptr = new DuiNinePatchPixmapItemPrivate();
    setPixmap(pixmap);
}

DuiNinePatchPixmapItem::DuiNinePatchPixmapItem(DuiNinePatchPixmapItemPrivate &dd, 
                                               QGraphicsItem *parent, 
                                               QGraphicsScene *scene)
: QGraphicsObject(parent)
{
    DW_UNUSED(scene);
    m_ddptr = &dd;
}

DuiNinePatchPixmapItem::~DuiNinePatchPixmapItem()
{
    DW_SAFE_DELETE(m_ddptr);
}

QPixmap DuiNinePatchPixmapItem::pixmap() const
{
    Q_D(const DuiNinePatchPixmapItem);
    return d->m_pixmap;
}

void DuiNinePatchPixmapItem::setPixmap(const QString &p)
{
    setPixmap(QPixmap(p));
}

Qt::TransformationMode DuiNinePatchPixmapItem::transformationMode() const
{
    Q_D(const DuiNinePatchPixmapItem);
    return d->m_transformationMode;
}

void DuiNinePatchPixmapItem::setTransformationMode(Qt::TransformationMode mode)
{
    Q_D(DuiNinePatchPixmapItem);
    if (mode != d->m_transformationMode) 
    {
        d->m_transformationMode = mode;
        update();
    }
}

void DuiNinePatchPixmapItem::setPixmap(const QPixmap &p)
{
    Q_D(DuiNinePatchPixmapItem);

    d->m_pixmap = p;
    _updateSize();
}

void DuiNinePatchPixmapItem::setSize(const QSizeF &s)
{
    Q_D(DuiNinePatchPixmapItem);
    if (d->m_size == s)
        return;
    prepareGeometryChange();
    d->m_size = s;
    update();
}

QPointF DuiNinePatchPixmapItem::offset() const
{
    Q_D(const DuiNinePatchPixmapItem);
    return d->m_offset;
}

void DuiNinePatchPixmapItem::setOffset(const QPointF &ofs)
{
    Q_D(DuiNinePatchPixmapItem);
    if (d->m_offset == ofs)
        return;
    prepareGeometryChange();
    d->m_offset = ofs;
    update();
}

int DuiNinePatchPixmapItem::subIdx() const
{
    Q_D(const DuiNinePatchPixmapItem);
    return d->m_subIdx;
}

void DuiNinePatchPixmapItem::setSubIdx(int idx)
{
    Q_D(DuiNinePatchPixmapItem);
    d->m_subIdx = idx;
    update();
}

QRectF DuiNinePatchPixmapItem::boundingRect() const
{
    Q_D(const DuiNinePatchPixmapItem);
    return QRectF(d->m_offset, d->m_size);
}

void DuiNinePatchPixmapItem::_updateSize()
{
    Q_D(DuiNinePatchPixmapItem);
    setSize(DuiDrawUtil::calcPixmapSize(d->m_pixmap, d->m_subIdx));
}

void DuiNinePatchPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget/* = 0*/)
{
    Q_D(DuiNinePatchPixmapItem);
    DW_UNUSED(option);
    DW_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform,
        (d->m_transformationMode == Qt::SmoothTransformation));

    bool isNines = DuiDrawUtil::isNinePatchsPixmap(d->m_pixmap);
    if (isNines)
    {
        if (!d->m_pixmap.isNull() && d->m_subIdx >= 0)
        {
            DuiDrawUtil::drawPixmap(painter, boundingRect().toRect(), d->m_pixmap, d->m_subIdx);
        }
    }
    else
    {
        painter->drawPixmap(d->m_offset, d->m_pixmap);
    }
}