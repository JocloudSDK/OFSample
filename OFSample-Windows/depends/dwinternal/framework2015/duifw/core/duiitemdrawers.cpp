#include "stable.h"

#include "duitheme.h"
#include "duidrawutil.h"
#include "duiitemdrawers.h"
#include "dwbase/perflog.h"

//------------------------------------------------------------------------------
class DuiFrameItemDrawerPrivate
{
	Q_DISABLE_COPY(DuiFrameItemDrawerPrivate)

public:
	DuiFrameItemDrawerPrivate();
	~DuiFrameItemDrawerPrivate();

	QPixmap m_frameImage;
	DuiFrameItemDrawer::FrameType m_type;
};

DuiFrameItemDrawerPrivate::DuiFrameItemDrawerPrivate()
: m_type(DuiFrameItemDrawer::Undefined)
{
	;
}

DuiFrameItemDrawerPrivate::~DuiFrameItemDrawerPrivate()
{
	;
}

//------------------------------------------------------------------------------
DuiFrameItemDrawer::DuiFrameItemDrawer(const QString &frameGraphicName, FrameType type) 
: d_ptr(new DuiFrameItemDrawerPrivate())
{
    Q_D(DuiFrameItemDrawer);
	d->m_type = type;
	d->m_frameImage = DuiTheme::getImage(frameGraphicName);
}

DuiFrameItemDrawer::DuiFrameItemDrawer(const QPixmap &frameGraphic, FrameType type)
: d_ptr(new DuiFrameItemDrawerPrivate())
{
    Q_D(DuiFrameItemDrawer);
	d->m_type = type;
	d->m_frameImage = frameGraphic;
}

DuiFrameItemDrawer::~DuiFrameItemDrawer()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiFrameItemDrawer::paint(QPainter *painter, const QRect &frameRect, const QMargins &frameMargin, int idx/* = 0*/) const
{
	PerfLogF(F_DUI);
	DW_ASSERT(painter);
    Q_D(const DuiFrameItemDrawer);

	if (frameRect.width() <= 0 || frameRect.height() <= 0)
	{
		return;
	}

    bool isNinePatchs = DuiDrawUtil::isNinePatchsPixmap(d->m_frameImage);
    if (isNinePatchs)
    {
        DuiDrawUtil::drawPixmap(painter, frameRect, d->m_frameImage, idx);
    }
    else
    {
        switch(d->m_type)
        {
        case NinePieces:
            easyDraw(painter, frameRect, frameMargin);
            break;
        case OnePiece:
            drawOnePiece(painter, frameRect, frameMargin);
            break;
        case ThreePiecesHorizontal:
            drawThreePiecesHorizontal( painter, frameRect, frameMargin );
            break;
        case ThreePiecesVertical:
            drawThreePiecesVertical(painter, frameRect, frameMargin);
            break;
        case  ThreePiecesVerticalMid:
            drawThreePiecesVerMid(painter, frameRect, frameMargin);
            break;
        default:
            DW_ASSERT_X(false, __FILE__, "type not support");
            break;
        }
    }
}

void DuiFrameItemDrawer::easyDraw( QPainter * painter, const QRect &frameRect, const QMargins & frameMargin ) const
{
	PerfLogF(F_DUI);
	Q_D(const DuiFrameItemDrawer);

	QPixmap image = d->m_frameImage;
	int cxImage = image.width();
	int cyImage = image.height();
	if(cxImage == 0 || cyImage == 0)
	{
		painter->fillRect(frameRect, Qt::transparent);
		return;
	}

	setClipMargins(painter, frameRect, frameMargin);

    DuiTheme::drawNinePiecesBoarder(painter, frameRect, image);
}

void DuiFrameItemDrawer::drawOnePiece(QPainter *painter, const QRect &frameRect, const QMargins &frameMargin) const
{
	PerfLogF(F_DUI);
	Q_D(const DuiFrameItemDrawer);

	setClipMargins(painter, frameRect, frameMargin);

	painter->drawPixmap(frameRect, d->m_frameImage);
}

void DuiFrameItemDrawer::drawThreePiecesVertical(QPainter* painter, const QRect &frameRect, const QMargins &frameMargin ) const
{
	PerfLogF(F_DUI);
	Q_UNUSED(frameMargin);
	Q_D(const DuiFrameItemDrawer);

    DuiDrawUtil::drawThreePiecesVerticalBoarder(painter, frameRect, d->m_frameImage);
}

void DuiFrameItemDrawer::drawThreePiecesHorizontal( QPainter * painter, const QRect &frameRect, const QMargins & frameMargin ) const
{
	PerfLogF(F_DUI);
	Q_UNUSED(frameMargin);
	Q_D(const DuiFrameItemDrawer);

    DuiDrawUtil::drawThreePiecesHorizontalBoarder(painter, frameRect, d->m_frameImage);
}

void DuiFrameItemDrawer::drawThreePiecesVerMid(QPainter * painter, const QRect &frameRect, const QMargins & frameMargin ) const
{
	PerfLogF(F_DUI);
	Q_UNUSED(frameMargin);
	Q_D(const DuiFrameItemDrawer);

	DuiDrawUtil::drawThreePiecesVerticalBoarder(painter, frameRect, d->m_frameImage);
}

void DuiFrameItemDrawer::setClipMargins(QPainter * painter, const QRect &frameRect, const QMargins & frameMargin) const
{
    if( !frameMargin.isNull() )
    {
        QRegion clipRgn(painter->clipRegion());
        clipRgn = clipRgn.subtracted( frameRect.adjusted(frameMargin.left(), frameMargin.top(), -frameMargin.right(), -frameMargin.bottom()) );
        painter->setClipRegion(clipRgn);
    }
}

int DuiFrameItemDrawer::string2Enum(const QString& str)
{
	struct Data 
	{
		const char *name;
		int value;
	};

	static const Data s_data[] = 
	{
		{"OnePiece", OnePiece},
		{"ThreePiecesHorizontal", ThreePiecesHorizontal},
		{"ThreePiecesHorizontalMid", ThreePiecesHorizontalMid},
		{"ThreePiecesVertical", ThreePiecesVertical},
		{"ThreePiecesVerticalMid", ThreePiecesVerticalMid},
		{"NinePieces", NinePieces},
	};

	int value = OnePiece;
	for (int i = 0; i < DW_ARRAY_SIZE(s_data); ++i)
	{
		if (s_data[i].name == str)
		{
			value = s_data[i].value;
			break;
		}
	}

	return value;
}
