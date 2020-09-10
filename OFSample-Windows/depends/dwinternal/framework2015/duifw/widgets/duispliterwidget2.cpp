#include "stable.h"

#include "duispliterwidget2.h"
#include "duiwidget_p.h"
#include "duiframeitem.h"
#include "duicheckbox.h"
#include "core/duidrawutil.h"

//////////////////////////////////////////////////////////////////////////
// DuiSpliterPane2Private

class DuiSpliterPane2Private : public DuiWidgetPrivate
{
    Q_DECLARE_PUBLIC( DuiSpliterPane2 );
public:
    DuiSpliterPane2Private()
        : DuiWidgetPrivate()
        , m_priority(PANE_PRIORITY_MIN)
        , m_btnClosePane(NULL)
        , m_buttonTarget(DuiSpliterPane2::BT_LEFT)
    {
    }
    
    ~DuiSpliterPane2Private()
    {
        if(m_btnClosePane) delete m_btnClosePane;
    }
private:
    int m_priority;
    DuiCheckBox   * m_btnClosePane;   //分隔关闭按钮
    DuiSpliterPane2::ButtonTarget m_buttonTarget;
};

//////////////////////////////////////////////////////////////////////////
// DuiSpliterPane2
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpliterPane2)

DuiSpliterPane2::DuiSpliterPane2( QGraphicsItem* parent ):DuiWidget(*new DuiSpliterPane2Private(),parent)
{
}

int DuiSpliterPane2::priority() const
{
    Q_D(const DuiSpliterPane2);
    return d->m_priority;
}

void DuiSpliterPane2::setPriority( int priority )
{
    Q_D(DuiSpliterPane2);
    if(priority < PANE_PRIORITY_MIN || priority > PANE_PRIORITY_MAX) return;
    d->m_priority = priority;
    emit priorityChanged(this);
}

QString DuiSpliterPane2::closePaneImage() const
{
    return QString();
}

void DuiSpliterPane2::setClosePaneImage( const QString & strCloseBtnImage )
{
    Q_D(DuiSpliterPane2);
    if(!d->m_btnClosePane)
    {
        d->m_btnClosePane = new DuiCheckBox(parentItem());
        d->m_btnClosePane->setObjectName(objectName()+"_closeButton");
        d->m_btnClosePane->setCursor(Qt::ArrowCursor);
        connect(d->m_btnClosePane,SIGNAL(stateChanged(bool)),SLOT(onCloseBtnStateChanged(bool)));
    }
    d->m_btnClosePane->setImagePath(strCloseBtnImage);

    QPixmap pixmap(strCloseBtnImage);
    d->m_btnClosePane->setFixSize(DuiDrawUtil::calcPixmapSize(pixmap, 0));
}

DuiCheckBox * DuiSpliterPane2::closePaneButton()
{
    Q_D(DuiSpliterPane2);
    return d->m_btnClosePane;
}

DuiSpliterPane2::ButtonTarget DuiSpliterPane2::buttonTarget() const
{
    Q_D(const DuiSpliterPane2);
    return d->m_buttonTarget;
}

void DuiSpliterPane2::setButtonTarget( ButtonTarget bt )
{
    Q_D(DuiSpliterPane2);
    d->m_buttonTarget=bt;
}

void DuiSpliterPane2::onCloseBtnStateChanged( bool bchecked )
{
    emit closeBtnStateChanged(this,bchecked);
}

bool duiSpliterPane2LessThen(DuiSpliterPane2 * l, DuiSpliterPane2 * r)
{
    return l->priority() < r->priority();
}

void sortSpliterPaneByPriority(QList<DuiSpliterPane2*> &list)
{
    qSort(list.begin(), list.end(), duiSpliterPane2LessThen);
}

//////////////////////////////////////////////////////////////////////////
//
class DuiSpliterWidget2Private : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiSpliterWidget2 );
private:
    Qt::Orientation       m_orintation;       //分隔方向
    DuiFrameItemDrawer *  m_spliterPainter;   //绘制分隔状态
    int                   m_spliterSize;      //分隔条的size
    QString               m_strClosePaneImage;//分隔隐藏按钮使用的图片
    
    typedef QList<DuiSpliterPane2*> SpliterPaneList;
    SpliterPaneList     m_lstPane;            //按显示顺序排序的分隔列表
    SpliterPaneList     m_lstPriority;        //按优先级排序的分隔列表

    QPoint              m_ptDragPrev;         //拖动调整的前一个位置
    int                 m_iDragSep;           //正在拖动哪一根分隔条,-1代表没有拖动状态

    //窗口的SIZE信息
    struct PANESIZE
    {
        qreal actural;
        qreal preferred;
        qreal minimum;
        qreal maximum;
    };
    typedef QVector<PANESIZE> PANESIZELIST;
public:
	DuiSpliterWidget2Private()
		: DuiWidgetPrivate()
		, m_orintation(Qt::Horizontal)
        , m_spliterSize(SPLITER_DEF_SIZE)
        , m_spliterPainter(NULL)
        , m_iDragSep(-1)
	{
	}

    ~DuiSpliterWidget2Private()
    {
        if(m_spliterPainter) DW_SAFE_DELETE(m_spliterPainter);
    }
    
public://methods
    int insertItem( DuiWidget* item, int index = -1  )
    {
        DuiSpliterPane2 * pane = toSpliterPane(item);
        if(!pane)
            return -1;
        
        Q_Q(DuiSpliterWidget2);
        
        //禁止重复插入
        if (m_lstPane.contains(pane))
        {
            DW_ASSERT(!m_lstPane.contains(pane));
            return -1;
        }
        
		if(index <0 )
		{
			index = m_lstPane.size();
		}

        m_lstPane.insert(index, pane);
        m_lstPriority.append(pane);
        sortSpliterPaneByPriority(m_lstPriority);
        pane->setCursor(Qt::ArrowCursor);

        const QRectF rcContainer = q->geometry();

        if (!rcContainer.isEmpty())
        {
            PANESIZELIST lstPaneSize;
            fatchPaneSizeInfo(m_lstPriority, lstPaneSize);

            const int index = m_lstPriority.indexOf(pane);
            lstPaneSize[index].actural = lstPaneSize[index].preferred;

            relayout(rcContainer, lstPaneSize);
        }
        else
        {
            QRectF rcItem(0, 0, rcContainer.width(), rcContainer.height());
            if (m_orintation == Qt::Vertical)
            {
                rcItem.setHeight(pane->preferredHeight());
            }
            else
            {
                rcItem.setWidth(pane->preferredWidth());
            }

            pane->setGeometry(rcItem);
        }

        return index;
    }
    
    void removeItem( DuiWidget* item )
    {
        DuiSpliterPane2 *pane = toSpliterPane(item);
        if(!pane) return;
        
        int index = m_lstPane.indexOf(pane);
        if(index == -1) return;
        
        m_lstPane.removeOne(pane);
        m_lstPriority.removeOne(pane);
        
        Q_Q(DuiSpliterWidget2);
        relayout(q->geometry());
    }

    
public://properties
    void DuiSpliterWidget2Private::setOrientation( Qt::Orientation orientation )
    {
        m_orintation = orientation;
        if(!m_spliterPainter)
        {
            if(m_orintation == Qt::Vertical)
                m_spliterPainter = new DuiFrameItemDrawer(":/theme/duifw/split_vert.png",DuiFrameItemDrawer::ThreePiecesVertical);
            else
                m_spliterPainter = new DuiFrameItemDrawer(":/theme/duifw/split_horz.png",DuiFrameItemDrawer::ThreePiecesHorizontal);
        }
    }

    Qt::Orientation DuiSpliterWidget2Private::orientation() const
    {
        return m_orintation;
    }

    QString spliterImage() const
    {
        return QString();
    }
    
    void setSpliterImage(const QString & strImage)
    {
        if(m_spliterPainter) DW_SAFE_DELETE(m_spliterPainter);
        m_spliterPainter = new DuiFrameItemDrawer(strImage,DuiFrameItemDrawer::NinePieces);
    }
    
public://handle event
    void resizeEvent( QGraphicsSceneResizeEvent *event )
    {
        relayout(QRectF(QPointF(),event->newSize()));
    }
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        if(!m_spliterPainter) return;
        Q_Q(DuiSpliterWidget2);
        QRectF rcSplit(0,0,q->width(),q->height());
        if(m_orintation == Qt::Vertical)
            rcSplit.setHeight(m_spliterSize);
        else
            rcSplit.setWidth(m_spliterSize);

        for(int i=0;i<m_lstPane.size()-1;i++)
        {
            QRectF rc = m_lstPane[i]->geometry();
            if(m_lstPane[i]->isVisible())
            {
                if(m_orintation == Qt::Vertical)
                    rcSplit.translate(0, rc.height());
                else
                    rcSplit.translate(rc.width(),0);
            }
            else
            {
                continue;
            }

            m_spliterPainter->paint(painter,rcSplit.toRect(),QMargins(0,0,0,0));
            
            if(m_orintation == Qt::Vertical)
                rcSplit.translate(0, m_spliterSize);
            else
                rcSplit.translate(m_spliterSize,0);
        }
    }

    void mousePressEvent( QGraphicsSceneMouseEvent *event )
    {
        m_ptDragPrev = event->pos().toPoint();
        if(m_orintation == Qt::Vertical)
        {
            for(int i=0;i<m_lstPane.size();i++)
            {
                if(m_ptDragPrev.y() < m_lstPane[i]->geometry().top())
                {
                    m_iDragSep = i-1;
                    break;
                }
            }
        }else
        {
            for(int i=0;i<m_lstPane.size();i++)
            {
                if(m_ptDragPrev.x() < m_lstPane[i]->geometry().left())
                {
                    m_iDragSep = i-1;
                    break;
                }
            }
        }
    }
    
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
    {
        m_iDragSep = -1;
    }
    
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event )
    {
        if(m_iDragSep == -1) return;
        
        //将列表分裂成两组。
        SpliterPaneList lstPane1,lstPane2;          
        SpliterPaneList lstPriority1,lstPriority2;
        
        for(int i=0;i<m_lstPriority.size();i++)
        {
            if(i <= m_iDragSep)
                lstPane1.push_back(m_lstPane[i]);
            else
                lstPane2.push_back(m_lstPane[i]);
        }
        
        for(int i=0;i<m_lstPriority.size();i++)
        {
            DuiSpliterPane2 *pane = m_lstPriority[i];
            int idx = m_lstPane.indexOf(pane);
            DW_ASSERT(idx!=-1);
            if(idx<=m_iDragSep)
                lstPriority1.push_back(pane);
            else
                lstPriority2.push_back(pane);
        }

        QPoint diffPoint = m_ptDragPrev;
        m_ptDragPrev = event->pos().toPoint();
        diffPoint = m_ptDragPrev - diffPoint;
        qreal diff = m_orintation == Qt::Vertical ? diffPoint.y() : diffPoint.x();

        PANESIZELIST lstPaneSize1,lstPaneSize2;
        fatchPaneSizeInfo(lstPriority1,lstPaneSize1);
        fatchPaneSizeInfo(lstPriority2,lstPaneSize2);

        if(diff > 0)
        {
            qreal maxShrink = 0, maxExtent =0;
            for(int i=0; i< lstPaneSize1.size();i++)
            {
                maxExtent += lstPaneSize1[i].maximum - lstPaneSize1[i].actural;
            }
            for(int i=0;i< lstPaneSize2.size();i++)
            {
                maxShrink += lstPaneSize2[i].actural - lstPaneSize2[i].minimum;
            }
            diff = qMin(diff, qMin(maxShrink,maxExtent));
            if(qFuzzyIsNull(diff)) return;

            //伸长part1
            int idxPrev = lstPriority1.indexOf(lstPane1.back());
            DW_ASSERT(idxPrev != -1);
            PANESIZE & paneSize1 = lstPaneSize1[idxPrev];

            qreal max_digest =paneSize1.maximum - paneSize1.actural;
            if(max_digest>diff)
            {
                paneSize1.actural += diff;
            }else
            {
                paneSize1.actural += max_digest;
                PANESIZE szBackup = paneSize1;
                lstPaneSize1.remove(idxPrev);
                qreal remain = diff - max_digest;
                adjustPanesSize(lstPaneSize1,remain);
                lstPaneSize1.insert(idxPrev,szBackup);
            }

            //压缩part2
            int idxNext = lstPriority2.indexOf(lstPane2.front());
            DW_ASSERT(idxNext != -1);
            PANESIZE & paneSize2 = lstPaneSize2[idxNext];

            max_digest =paneSize2.actural - paneSize2.minimum;
            if(max_digest > diff)
            {
                paneSize2.actural -= diff;
            }else
            {
                paneSize2.actural -= max_digest;
                PANESIZE szBackup=paneSize2;
                lstPaneSize2.remove(idxNext);
                qreal remain = max_digest - diff;//remain < 0
                adjustPanesSize(lstPaneSize2,remain);
                lstPaneSize2.insert(idxNext,szBackup);
            }

        }else
        {
            qreal maxShrink = 0, maxExtent =0;
            for(int i=0; i< lstPaneSize2.size();i++)
            {
                maxExtent += lstPaneSize2[i].maximum - lstPaneSize2[i].actural;
            }
            for(int i=0;i< lstPaneSize1.size();i++)
            {
                maxShrink += lstPaneSize1[i].actural - lstPaneSize1[i].minimum;
            }
            
            diff *= -1;
            diff = qMin(diff, qMin(maxShrink,maxExtent));
            if(qFuzzyIsNull(diff)) return;
            
            //压缩part1
            int idxPrev = lstPriority1.indexOf(lstPane1.back());
            DW_ASSERT(idxPrev != -1);
            PANESIZE & paneSize1 = lstPaneSize1[idxPrev];
            
            qreal max_digest =paneSize1.actural - paneSize1.minimum;
            if(max_digest>diff)
            {
                paneSize1.actural -= diff;
            }else
            {
                paneSize1.actural -= max_digest;
                PANESIZE szBackup = paneSize1;
                lstPaneSize1.remove(idxPrev);
                qreal remain = max_digest - diff; //remain < 0
                adjustPanesSize(lstPaneSize1,remain);
                lstPaneSize1.insert(idxPrev,szBackup);
            }

            //伸长part2
            int idxNext = lstPriority2.indexOf(lstPane2.front());
            DW_ASSERT(idxNext != -1);
            PANESIZE & paneSize2 = lstPaneSize2[idxNext];

            max_digest = paneSize2.maximum - paneSize2.actural;
            if(max_digest > diff)
            {
                paneSize2.actural += diff;
            }else
            {
                paneSize2.actural += max_digest;
                PANESIZE szBackup=paneSize2;
                lstPaneSize2.remove(idxNext);
                qreal remain = diff- max_digest;
                adjustPanesSize(lstPaneSize2,remain);
                lstPaneSize2.insert(idxNext,szBackup);
            }
            diff *= -1;
        }
        //根据新分配的窗口大小重置窗口位置
        qreal offset = resetPanesPostion(lstPane1,lstPriority1,lstPaneSize1,0.0);
        resetPanesPostion(lstPane2,lstPriority2,lstPaneSize2,offset);

        Q_Q(DuiSpliterWidget2);
        q->update();
    }

private:
    DuiSpliterPane2 * toSpliterPane(DuiWidget * item)
    {
        DuiSpliterPane2 * pane = qobject_cast<DuiSpliterPane2*>(item);
        DW_ASSERT(pane);
        return pane;
    }
    
    const DuiSpliterPane2 * toSpliterPane(const DuiWidget * item) const
    {
        const DuiSpliterPane2 * pane = qobject_cast<const DuiSpliterPane2*>(item);
        DW_ASSERT(pane);
        return pane;
    }
    
    int indexOfItem(DuiWidget * item)
    {
        DuiSpliterPane2 * pane = toSpliterPane(item);
        if(!pane) return -1;
        return m_lstPane.indexOf(pane);
    }
    
    DuiSpliterPane2 * itemOf(int iPane)
    {
        if(iPane<0 || iPane>= m_lstPane.size()) return NULL;
        return m_lstPane[iPane];
    }
    
    int itemCount(){return m_lstPane.size();}
    
    void fatchPaneSizeInfo(const SpliterPaneList & lstPane, PANESIZELIST & lstPaneSize)
    {
        lstPaneSize.resize(lstPane.size());//分配空间
        
        for(int i=0;i<lstPane.size();i++)
        {
            if(!lstPane[i]->isVisible())
            {
                lstPaneSize[i].actural = 0;
                lstPaneSize[i].preferred = 0;
                lstPaneSize[i].minimum = 0;
                lstPaneSize[i].maximum = 0;
            }
            else if(m_orintation == Qt::Vertical)
            {
                lstPaneSize[i].actural = lstPane[i]->geometry().height();
                lstPaneSize[i].preferred = lstPane[i]->preferredHeight();
                lstPaneSize[i].minimum = lstPane[i]->minimumHeight();
                lstPaneSize[i].maximum = lstPane[i]->maximumHeight();
            }
            else
            {
                lstPaneSize[i].actural = lstPane[i]->geometry().width();
                lstPaneSize[i].preferred = lstPane[i]->preferredWidth();
                lstPaneSize[i].minimum = lstPane[i]->minimumWidth();
                lstPaneSize[i].maximum = lstPane[i]->maximumWidth();
            }
        }
    }
    
    qreal resetPanesPostion(SpliterPaneList & lstPane, SpliterPaneList & lstPanePriority, PANESIZELIST & lstPaneSize, qreal offset)
    {
        Q_Q(DuiSpliterWidget2);
        QRectF rc = q->rect();
        for(int i=0;i<lstPane.size();i++)
        {
            int idx = lstPanePriority.indexOf(lstPane[i]);
            DW_ASSERT(idx!=-1);
            DuiCheckBox * pBtn = lstPane[i]->closePaneButton();
            if(m_orintation == Qt::Vertical)
            {
                rc.moveTop(offset);
                rc.setHeight(lstPaneSize[idx].actural);
                if(pBtn)
                {
                    QRectF rcBtn=pBtn->geometry();
                    QPointF pt(rc.left()+(rc.width()-rcBtn.width())/2,rc.bottom()+(m_spliterSize-rcBtn.height())/2);
                    rcBtn.moveTopLeft(pt);
                    pBtn->setGeometry(rcBtn);
                }
            }
            else
            {
                rc.moveLeft(offset);
                rc.setWidth(lstPaneSize[idx].actural);
                if(pBtn)
                {
                    QRectF rcBtn=pBtn->geometry();
                    QPointF pt(rc.right()+(m_spliterSize-rcBtn.width())/2,rc.top()+(rc.height()-rcBtn.height())/2);
                    rcBtn.moveTopLeft(pt);
                    pBtn->setGeometry(rcBtn);
                }
            }
            if(lstPane[i]->isVisible()) //窗口不可见时不修改窗口size
            {
                lstPane[i]->setGeometry(rc);
                offset += lstPaneSize[idx].actural + m_spliterSize;
            }
            else
            {
                offset += lstPaneSize[idx].actural;
            }
        }
        return offset;
    }
    
    //调整列表中控件的高度（宽度）
    qreal adjustPanesSize(PANESIZELIST & lstPriority, qreal remain) {
        //step 1: 将残余量按优先级从高到低顺序根据期望size在窗口间分配
        for(int i=lstPriority.size()-1;i>=0;i--)
        {
            PANESIZE & pane = lstPriority[i];
            if(remain > 0)
            {//扩大窗口
                if(pane.preferred < pane.actural) continue;

                qreal max_digest = pane.preferred - pane.actural;
                if(max_digest >= remain)
                {
                    pane.actural+=remain;
                    remain = 0.0;
                    break;
                }else
                {
                    pane.actural += max_digest;
                    remain -= max_digest;
                }
            }else
            {//缩小窗口
                if(pane.preferred > pane.actural) continue;

                qreal max_digest = pane.actural - pane.preferred;
                if(max_digest > -remain)
                {
                    pane.actural += remain;
                    remain = 0.0;
                    break;
                }else
                {
                    pane.actural -= max_digest;
                    remain += max_digest;
                }
            }
        }
        //step 2: 将残余量按优先级从低到高顺序在窗口的极值间分配
        if(!qFuzzyIsNull(remain))
        {
            for(int i=0;i<lstPriority.size();i++)
            {
                PANESIZE & pane = lstPriority[i];
                if(remain > 0)
                {//扩大窗口
                    qreal max_digest = pane.maximum - pane.actural;
                    if(max_digest >= remain)
                    {
                        pane.actural += remain;
                        remain = 0.0;
                        break;
                    }else
                    {
                        pane.actural += max_digest;
                        remain -= max_digest;
                    }
                }else
                {//缩小窗口
                    qreal max_digest = pane.actural - pane.minimum;
                    if(max_digest > -remain)
                    {
                        pane.actural += remain;
                        remain = 0.0;
                        break;
                    }else
                    {
                        pane.actural -= max_digest;
                        remain += max_digest;
                    }
                }
            }
        }
        return remain;
    }
    
    void relayout(QRectF rc, PANESIZELIST lstPaneSize = PANESIZELIST())
    {
        if (lstPaneSize.isEmpty())
        {
            fatchPaneSizeInfo(m_lstPriority, lstPaneSize);
        }
        qreal actural=0.0;
        for(int i=0; i< lstPaneSize.size();i++)
            actural += lstPaneSize[i].actural;

        int nVisiblePanelCount = 0;
        for(int i=0;i<m_lstPriority.size();i++)
        {
            if(m_lstPriority[i]->isVisible())
                ++nVisiblePanelCount;
        }

        qreal remain = ((Qt::Vertical == m_orintation)? rc.height():rc.width()) - actural - (nVisiblePanelCount-1)*m_spliterSize;
        adjustPanesSize(lstPaneSize,remain);
        resetPanesPostion(m_lstPane,m_lstPriority,lstPaneSize,0.0);
    }
    
private://for slot
    void onPanePriotityChanged(DuiSpliterPane2 *pane )
    {
        if (!m_lstPriority.contains(pane))
            return;

        sortSpliterPaneByPriority(m_lstPriority);
        relayout(q_func()->geometry());
    }

    void onPaneVisibleChanged(DuiSpliterPane2 *pane)
    {
        Q_Q(DuiSpliterWidget2);
        if(pane->isVisible())
        {//先备份窗口状态，再把窗口的3个size属性固定，再重排布局，最后恢复窗口的3个size属性。这样可以保证窗口显示的size和关闭时的size一样大
            PANESIZE paneSize;
            QRectF rc = pane->geometry();
            if(m_orintation == Qt::Vertical)
            {
                paneSize.minimum = pane->minimumHeight();
                paneSize.maximum = pane->maximumHeight();
                paneSize.preferred = pane->preferredHeight();
                
                pane->setMinimumHeight(rc.height());
                pane->setMaximumHeight(rc.height());
                pane->setPreferredHeight(rc.height());
            }else
            {
                paneSize.minimum = pane->minimumWidth();
                paneSize.maximum = pane->maximumWidth();
                paneSize.preferred = pane->preferredWidth();

                pane->setMinimumWidth(rc.width());
                pane->setMaximumWidth(rc.width());
                pane->setPreferredWidth(rc.width());
            }
            relayout(q->geometry());
            if(m_orintation == Qt::Vertical)
            {
                pane->setMinimumHeight(paneSize.minimum);
                pane->setMaximumHeight(paneSize.maximum);
                pane->setPreferredHeight(paneSize.preferred);
            }else
            {
                pane->setMinimumWidth(paneSize.minimum);
                pane->setMaximumWidth(paneSize.maximum);
                pane->setPreferredWidth(paneSize.preferred);
            }
        }else
        {
            relayout(q->geometry());
        }

        int idx = indexOfItem(pane);
        DW_ASSERT(idx != -1);

        if(pane->closePaneButton() && pane->buttonTarget() == DuiSpliterPane2::BT_LEFT)
        {
            pane->closePaneButton()->setCheckState(pane->isVisible()?Qt::Unchecked:Qt::Checked);
        }
        if(idx>0)
        {
            DuiSpliterPane2 *panePrev = itemOf(idx-1);
            if(panePrev->closePaneButton() && panePrev->buttonTarget() == DuiSpliterPane2::BT_RIGHT)
            {
                panePrev->closePaneButton()->setCheckState(pane->isVisible()?Qt::Unchecked:Qt::Checked);
            }
        }
    }
};


//////////////////////////////////////////////////////////////////////////
// DuiSpliterWidget2

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpliterWidget2)

DuiSpliterWidget2::DuiSpliterWidget2( QGraphicsItem* parent )
    : DuiWidget(*new DuiSpliterWidget2Private(), parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setDragPolicy( DuiWidget::NotDragWindow );
}

void DuiSpliterWidget2::addItem( DuiWidget* item )
{
    insertItem(item,-1);
}

void DuiSpliterWidget2::removeItem( DuiWidget *item )
{
    Q_D(DuiSpliterWidget2);
    d->removeItem(item);
}

int DuiSpliterWidget2::insertItem( DuiWidget* item,int index /*= -1*/ )
{
    Q_D(DuiSpliterWidget2);
    int idx = d->insertItem(item,index);
    if(-1 == idx) return -1;
    DuiSpliterPane2 *pane = qobject_cast<DuiSpliterPane2 *>(item);
    connect(pane,SIGNAL(priorityChanged(DuiSpliterPane2*)),SLOT(onPanePriotityChanged(DuiSpliterPane2 *)));
    connect(pane,SIGNAL(closeBtnStateChanged(DuiSpliterPane2*,bool)),SLOT(onPaneCloseBtnStateChanged(DuiSpliterPane2*,bool)));
    connect(pane,SIGNAL(visibleChanged()),SLOT(onPaneVisibleChanged()));
    return idx;    
}

void DuiSpliterWidget2::setOrientation( Qt::Orientation orientation )
{
    Q_D(DuiSpliterWidget2);
    d->setOrientation(orientation);
    if(orientation == Qt::Vertical)
        setCursor(Qt::SplitVCursor);
    else
        setCursor(Qt::SplitHCursor);

}

Qt::Orientation DuiSpliterWidget2::orientation() const
{
    Q_D( const DuiSpliterWidget2 );
    return d->orientation();
}

QString DuiSpliterWidget2::spliterImage() const
{
    Q_D(const DuiSpliterWidget2);
    return d->spliterImage();
}

void DuiSpliterWidget2::setSpliterImage( const QString & strSpliterImage )
{
    Q_D(DuiSpliterWidget2);
    d->setSpliterImage(strSpliterImage);
}

int DuiSpliterWidget2::spliterSize() const
{
    Q_D(const DuiSpliterWidget2);
    return d->m_spliterSize;
}

void DuiSpliterWidget2::setSpliterSize( int nSize )
{
    Q_D(DuiSpliterWidget2);
    d->m_spliterSize = nSize;
}

void DuiSpliterWidget2::resizeEvent( QGraphicsSceneResizeEvent *event )
{
    Q_D(DuiSpliterWidget2);
    d->resizeEvent(event);
}

void DuiSpliterWidget2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(DuiSpliterWidget2);
    d->paint(painter,option,widget);
}

void DuiSpliterWidget2::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    Q_D(DuiSpliterWidget2);
    d->mousePressEvent(event);
}

void DuiSpliterWidget2::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    Q_D(DuiSpliterWidget2);
    d->mouseReleaseEvent(event);
}

void DuiSpliterWidget2::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    Q_D(DuiSpliterWidget2);
    d->mouseMoveEvent(event);
}

void DuiSpliterWidget2::onPanePriotityChanged( DuiSpliterPane2 *pane )
{
    Q_D(DuiSpliterWidget2);
    d->onPanePriotityChanged(pane );
}

DuiSpliterPane2 * DuiSpliterWidget2::getPane( int iPane )
{
    Q_D(DuiSpliterWidget2);
    return d->itemOf(iPane);
}

void DuiSpliterWidget2::onPaneCloseBtnStateChanged( DuiSpliterPane2 *pane,bool bchecked )
{
    if(pane->buttonTarget() ==  DuiSpliterPane2::BT_LEFT)
    {
        if(bchecked) pane->hide();
        else pane->show();
    }else
    {
        Q_D(DuiSpliterWidget2);
        int idx = d->indexOfItem(pane);
        DW_ASSERT(idx != -1);
        if(idx < d->itemCount()-1)
        {
            pane = d->itemOf(idx+1);
            if(bchecked) pane->hide();
            else pane->show();
        }
    }
}

void DuiSpliterWidget2::onPaneVisibleChanged()
{
    Q_D(DuiSpliterWidget2);
    DuiSpliterPane2 * pane = qobject_cast<DuiSpliterPane2*>(sender());
    DW_ASSERT(pane);
    d->onPaneVisibleChanged(pane);
}

QVariant DuiSpliterWidget2::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(DuiSpliterWidget2);
    if (change == QGraphicsItem::ItemVisibleHasChanged)
    {
        if (value.toBool())
        {
            d->relayout(geometry());
        }
    }
    
    return DuiWidget::itemChange(change, value);
}
