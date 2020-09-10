#include "stable.h"

#include <windowsx.h>

#include "duiframestyles.h"
#include "duiframewindow.h"
#include "duifw/duiwidget.h"

#include "platform/windows/duiwinapi.h"

COLORREF duiFrameKeyColor()
{
#if 0
	static COLORREF s_keyCcolor = 0;
	if(s_keyCcolor == 0)
	{
		s_keyCcolor = RGB(40 + qrand() % 24, 0, 40 + qrand() % 24);
	}
	return s_keyCcolor;
#else
	return RGB(64, 0, 64);
#endif
}

QString duiFrameWindowStyleSheet_Transparent()
{
	//by default is :
	//	const char KFrameWindowStyleSheet_Vista[] = "QGraphicsView { border-style: none; background: rgb(64, 0, 64)}";

	return QLatin1String("QGraphicsView { border-style: none; background: transparent}");
}

QString duiFrameWindowStyleSheet_KeyColor()
{
	//by default is :
	//	const char KFrameWindowStyleSheet_Vista[] = "QGraphicsView { border-style: none; background: rgb(64, 0, 64)}";

	COLORREF c = duiFrameKeyColor();
	return QString(QLatin1String("QGraphicsView { border-style: none; background: rgb(%1, %2, %3)}"))
		.arg(GetRValue(c))
		.arg(GetGValue(c))
		.arg(GetBValue(c));
}

static QPoint mapFromGlobal(QWidget *frame, LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	return frame->mapFromGlobal(QPoint(x, y));
}

static LRESULT OnNcHitTest(const QPoint &pt, const QGraphicsView *graphicView, const QMargins &borderMargins, int captionHeight)
{
	if(::IsZoomed(graphicView->winId()))
	{
		return HTCLIENT;
	}
	int cx = graphicView->size().width();
	int cy = graphicView->size().height();

	QRect rectCaption(borderMargins.left(), borderMargins.top(), cx - borderMargins.left() - borderMargins.right(), captionHeight);
	if(rectCaption.contains(pt))
	{
		return HTCAPTION;
	}

	if(borderMargins.left() > 0)
	{
		QRect rectTopLeft(0, 0, borderMargins.left(),  borderMargins.top());
		if(rectTopLeft.contains(pt))
		{
			return HTTOPLEFT;
		}
		QRect rectLeft(0, borderMargins.top(), borderMargins.left(), cy - borderMargins.top() - borderMargins.bottom());
		if(rectLeft.contains(pt))
		{
			return HTLEFT;
		}
	}

	if(borderMargins.right() > 0)
	{
		QRect rectTopRight(cx - borderMargins.right(), 0, borderMargins.right(), borderMargins.top());
		if(rectTopRight.contains(pt))
		{
			return HTTOPRIGHT;
		}
		QRect rectRight(cx - borderMargins.right(), borderMargins.top(), borderMargins.right(), cy - borderMargins.top() - borderMargins.bottom());
		if(rectRight.contains(pt))
		{
			return HTRIGHT;
		}
	}

	if(borderMargins.top() > 0)
	{
		QRect rectTop(borderMargins.left(), 0, cx - borderMargins.left() - borderMargins.right(), borderMargins.top());
		if(rectTop.contains(pt))
		{
			return HTTOP;
		}
	}

	if(borderMargins.bottom() > 0)
	{
		QRect rectBottomLeft(0, cy - borderMargins.bottom(), borderMargins.left(), borderMargins.bottom());
		if(rectBottomLeft.contains(pt))
		{
			return HTBOTTOMLEFT;
		}

		QRect rectBottomRight(cx - borderMargins.right(), cy - borderMargins.bottom(), borderMargins.right(), borderMargins.bottom());

		//increace the bottom right corner
		rectBottomRight.setLeft( rectBottomRight.left() - 10 );
		rectBottomRight.setTop( rectBottomRight.top() - 10 );
		if(rectBottomRight.contains(pt))
		{
			return HTBOTTOMRIGHT;
		}

		QRect rectBottom(borderMargins.left(), cy - borderMargins.bottom(), cx - borderMargins.left() - borderMargins.right(), borderMargins.bottom());
		if(rectBottom.contains(pt))
		{
			return HTBOTTOM;
		}
	}

	return HTCLIENT;
}

static bool OnNcCalcSize(MSG *message)
{
	NCCALCSIZE_PARAMS* pParams = NULL;
	RECT* pRect = NULL;

	BOOL bValue = static_cast<BOOL>(message->wParam);
	if(bValue) pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(message->lParam);
	else pRect = reinterpret_cast<RECT*>(message->lParam);

	if(bValue)
	{
		pRect = &pParams->rgrc[0];
	}

	if(bValue)
	{
		pRect->left = pRect->left + 0;
		pRect->top = pRect->top + 0;
		pRect->right = pRect->right - 0;
		pRect->bottom = pRect->bottom - 0;

		pParams->rgrc[1] = pParams->rgrc[0];
		return true;
	}
	else
	{
		return false;
	}
}

static void HitTestDuiWidget( long& reslut, DuiWidget *& item, const QPoint& pt )
{
	if( item != NULL )
	{
		if ( item->hitTest(pt) == DuiWidget::DragWindow )
		{
			reslut = HTCAPTION;
		}
		else if ( item->hitTest(pt) == DuiWidget::DragWithParentFlag )
		{
			int flag = DuiWidget::DragWithParentFlag;
			do
			{
				item = qobject_cast<DuiWidget*>( item->parentWidget() );
				if ( item != NULL )
				{
					flag = item->hitTest(pt);
					if ( flag == DuiWidget::DragWindow )
					{
						reslut = HTCAPTION;
					}
				}
			} while ( item != NULL &&  flag == DuiWidget::DragWithParentFlag );
		}
	}
}

static void HitTestClient( QGraphicsView* graphicsView, long &result, const QPoint& pt )
{
	if(graphicsView)
	{
		QWidget *childWidget = graphicsView->childAt(pt);
		if( childWidget && childWidget != graphicsView->viewport() )
		{
			result = HTCLIENT;
			return;
		}

		QGraphicsItem *item = graphicsView->scene()->itemAt(pt);
		if ( item != NULL )
		{
			DuiWidget *widget = qobject_cast<DuiWidget*>( item->toGraphicsObject() );
			if( widget != NULL )
			{
				HitTestDuiWidget( result, widget, pt );
			}
			else
			{
				do
				{
					item = item->parentItem();
					if(item)
					{
						widget = qobject_cast<DuiWidget*>( item->toGraphicsObject() );
						if ( widget != NULL )
						{
							HitTestDuiWidget( result, widget, pt );
							break;
						}
					}
				} while ( item != NULL );
			}
		}
	}
}

/*
	Abstract frame style
*/
DuiFrameStyle::DuiFrameStyle( QGraphicsView *frame )
{
	m_resizable = false;
	m_toolWindow = false;
	m_popup = false;
	m_isRoundCorner = true;
	m_frame = frame;
}

DuiFrameStyle::~DuiFrameStyle()
{
}

bool DuiFrameStyle::isResizable() const
{
	return m_resizable;
}

void DuiFrameStyle::setResizable(bool resizable)
{
	m_resizable = resizable;
}

qreal DuiFrameStyle::opacity() const
{
	return 1.0;
}

bool DuiFrameStyle::isToolWindow() const
{
	return m_toolWindow;
}

void DuiFrameStyle::setToolWindow( bool toolWindow )
{
	m_toolWindow = toolWindow;
}

bool DuiFrameStyle::isPopup() const
{
	return m_popup;
}

void DuiFrameStyle::setPopup( bool popup )
{
	m_popup = popup;
}

LONG DuiFrameStyle::calculateCurrentScreenWidth()
{
	MONITORINFO mi = {0};
	mi.cbSize = sizeof(mi);
	HMONITOR hMonitor = ::MonitorFromWindow(m_frame->winId(), MONITOR_DEFAULTTONEAREST);
	BOOL ret = GetMonitorInfo(hMonitor, &mi);
	DW_ASSERT(ret == TRUE);

	return mi.rcMonitor.right - mi.rcMonitor.left;
}

RECT DuiFrameStyle::reviseCurrentWorkArea(const RECT& curWorkArea)
{
	Q_UNUSED(curWorkArea);
	RECT rtPrimaryWorkArea = {0};
	DuiWinAPI::SystemParametersInfo(SPI_GETWORKAREA, 0, &rtPrimaryWorkArea, 0);
	if (curWorkArea.left == rtPrimaryWorkArea.left
		&& curWorkArea.right == rtPrimaryWorkArea.right
		&& curWorkArea.top == rtPrimaryWorkArea.top
		&& curWorkArea.bottom == rtPrimaryWorkArea.bottom)
	{
		return curWorkArea;
	}

	LONG currentScreenWidth = 0;
	LONG primaryWidth = 0;
	currentScreenWidth = calculateCurrentScreenWidth();
	primaryWidth = ::GetSystemMetrics(SM_CXSCREEN);
	RECT revisedWorkArea = curWorkArea;
	// ×ó±ßÊÇÖ÷ÆÁ
	if (curWorkArea.left >= rtPrimaryWorkArea.right)
	{
		revisedWorkArea.left = curWorkArea.left - primaryWidth;
		revisedWorkArea.right = curWorkArea.right - primaryWidth;
	}
	else if (curWorkArea.right <= rtPrimaryWorkArea.left)
	{
		revisedWorkArea.left = curWorkArea.left + currentScreenWidth;
		revisedWorkArea.right = curWorkArea.right + currentScreenWidth;
	}
	else
	{
		DW_ASSERT(false);
	}

	int delta = revisedWorkArea.top;
	revisedWorkArea.top = 0;
	revisedWorkArea.bottom -= delta;

	return revisedWorkArea;
}

bool DuiFrameStyle::isAnimationEnabled() const
{
	return false;
}

void DuiFrameStyle::setRoundCorner( bool enable )
{
	m_isRoundCorner = enable;
}

bool DuiFrameStyle::isRoundCorner() const
{
	return m_isRoundCorner;
}

/*
	Windows XP frame style, frameless and owner draw system buttons
*/
DuiFrameStyle_WinXP::DuiFrameStyle_WinXP( QGraphicsView *frame )
: DuiFrameStyle(frame)
{
}

bool DuiFrameStyle_WinXP::winEvent( MSG *message, long *result )
{
	switch(message->message)
	{
	case WM_GETMINMAXINFO:
		{
			RECT rtWorkArea = {0};
			DuiWinAPI::getCurrentWindowWorkRect(m_frame->winId(), rtWorkArea);
			RECT revisedWorkArea = reviseCurrentWorkArea(rtWorkArea);
			MINMAXINFO* pInfo = (MINMAXINFO*)message->lParam;
			pInfo->ptMaxSize.x = revisedWorkArea.right - revisedWorkArea.left + 4;
			pInfo->ptMaxSize.y = revisedWorkArea.bottom - revisedWorkArea.top + 4;
			pInfo->ptMaxPosition.x = revisedWorkArea.left - 2;
			pInfo->ptMaxPosition.y = revisedWorkArea.top - 2;
			pInfo->ptMaxTrackSize = pInfo->ptMaxSize;

			QSize s = m_frame->minimumSize();
			pInfo->ptMinTrackSize.x = s.width();
			pInfo->ptMinTrackSize.y = s.height();
		}
		return true;

	case WM_NCCALCSIZE:
		*result = OnNcCalcSize(message);
		return true;

	case WM_ERASEBKGND:
		return true;

	case 0xAE:	//WM_NCUAHDRAWCAPTION	(undocument window message)
	case 0xAF:	//WM_NCUAHDRAWFRAME		(undocument window message)
	case WM_NCPAINT:
		*result = 0;
		return true;

	//comment it, to fix drawing problem, something can't be erased when yy window was deactivated.
	//case WM_NCACTIVATE:
	//	{
	//		DWORD dwStyle = GetWindowLong(message->hwnd, GWL_STYLE);
	//		SetWindowLong(message->hwnd, GWL_STYLE, dwStyle & ~WS_VISIBLE);
	//		*result = ::DefWindowProc(message->hwnd, message->message, message->wParam, message->lParam);
	//		SetWindowLong(message->hwnd, GWL_STYLE, dwStyle);
	//		return true;
	//	}

	case WM_NCLBUTTONDBLCLK:
		{
			if(isResizable())
			{
				HWND hWnd = m_frame->winId();
				if(::IsZoomed(hWnd))
				{
					DuiWinAPI::ShowWindow(hWnd, SW_RESTORE);
				}
				else
				{
					DuiWinAPI::ShowWindow(hWnd, SW_MAXIMIZE);
				}
			}
			return true;
		}

	case WM_NCLBUTTONDOWN:
		{
			DuiFrameWindow *frameWin = qobject_cast<DuiFrameWindow*>(m_frame);
			if(frameWin)
			{
				frameWin->filterNcMessage(message);
			}

			if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
			{
				if(::IsZoomed(m_frame->winId()))
				{
					return true;
				}
			}

			break;
		}

	case WM_NCHITTEST:
		{
			QPoint pt = mapFromGlobal(m_frame, message->lParam);

			int borderSize = 0;
			if(isResizable())
			{
				borderSize = 3;
			}
			*result = OnNcHitTest(pt, m_frame, QMargins(borderSize, borderSize, borderSize, borderSize), 0);

			if(*result == HTCLIENT)
			{
				HitTestClient( m_frame, *result, pt );
			}
			return true;
		}

	default:
		break;
	}

	return false;
}

void DuiFrameStyle_WinXP::resizeEvent( QResizeEvent *event )
{
	if(isRoundCorner())
	{
		QRect boundRect(QPoint(0, 0), event->size());
		QRegion rgn = DuiTheme::calcFrameClipRegion( boundRect );
		DuiWinAPI::SetWindowRgn(m_frame->winId(), rgn.handle(), true);
	}
}

void DuiFrameStyle_WinXP::init()
{
	m_frame->setStyleSheet( "QGraphicsView { border-style: none; background: transparent}" );

	if(isPopup())
	{
		m_frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
		m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
		m_frame->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
	}
	else if(isResizable())
	{
		m_frame->setWindowFlags( Qt::Window |  Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint );

		//the WS_THICKFRAME is necessary for winXP, or the window can't be dragged to resize.
		DuiWinAPI::SetWindowLong(m_frame->winId(),
			GWL_STYLE,
			GetWindowLong(m_frame->winId(), GWL_STYLE) | WS_THICKFRAME);
	}
	else
	{
		m_frame->setWindowFlags( Qt::Window |  Qt::FramelessWindowHint | Qt::WindowSystemMenuHint );
	}
}

QRect DuiFrameStyle_WinXP::calcContentRect(const QRect &frameRect) const
{
	return frameRect;
}

/*
	System frame style, has system frame and system buttons
*/
DuiFrameStyle_System::DuiFrameStyle_System( QGraphicsView *frame )
	: DuiFrameStyle( frame )
{
}

void DuiFrameStyle_System::init()
{
	m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
	m_frame->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");

	DuiWinAPI::ExtendIntoClientAll(m_frame->winId());
}

bool DuiFrameStyle_System::winEvent( MSG *message, long *result )
{
	Q_UNUSED(result);

	switch(message->message)
	{
	case WM_ERASEBKGND:
		return true;

	default:
		break;
	}

	return false;
}

void DuiFrameStyle_System::resizeEvent( QResizeEvent *event )
{
	Q_UNUSED(event);
}

QRect DuiFrameStyle_System::calcContentRect( const QRect &frameRect ) const
{
	return frameRect.adjusted(7, 7, -7, -7);
}

/*
	Dialog frame style, frameless, no sizable frame and owner draw system buttons
*/
DuiFrameStyle_Dialog::DuiFrameStyle_Dialog( QGraphicsView *frame )
: DuiFrameStyle(frame)
{
}

void DuiFrameStyle_Dialog::init()
{
	m_frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog /*| Qt::Tool*/);
	m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
	m_frame->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");

	setResizable(false);
}

void DuiFrameStyle_Dialog::resizeEvent( QResizeEvent *event )
{
	Q_UNUSED( event );
}

bool DuiFrameStyle_Dialog::winEvent( MSG *message, long *result )
{
	switch(message->message)
	{
	case WM_NCCALCSIZE:
		//just return, the whole rect would be client area.
		*result = 0;
		return true;

	case WM_WINDOWPOSCHANGING:
		{
			LPWINDOWPOS lp = (LPWINDOWPOS)(message->lParam);
			if(lp->hwnd == m_frame->winId() && lp->flags & 0x8000)
			{
				lp->cx += 1;
				lp->flags |= SWP_NOSENDCHANGING;
			}
			break;
		}

	case WM_ERASEBKGND:
		return true;

	case WM_NCHITTEST:
		{
			QPoint pt = mapFromGlobal(m_frame, message->lParam);

			*result = OnNcHitTest(pt, m_frame, QMargins(0, 0, 0, 0), 0);

			if(*result == HTCLIENT)
			{
				HitTestClient( m_frame, *result, pt );
			}

			return true;
		}
	default:
		break;
	}

	return false;
}

QRect DuiFrameStyle_Dialog::calcContentRect( const QRect &frameRect ) const
{
	return frameRect.adjusted(1, 1, -1, -1);
}

void DuiFrameStyle_Vista::init()
{
	if(isPopup())
	{
		m_frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
		m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
		m_frame->setStyleSheet(duiFrameWindowStyleSheet_Transparent());
	}
	else if(isResizable())
	{
		if(isToolWindow())
		{
			m_frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
			m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
			m_frame->setStyleSheet(duiFrameWindowStyleSheet_Transparent());
		}
		else
		{
			m_frame->setWindowFlags(Qt::Window);
			m_frame->setStyleSheet(duiFrameWindowStyleSheet_KeyColor());
			DuiWinAPI::SetWindowLong(m_frame->winId(),
				GWL_EXSTYLE,
				GetWindowLong(m_frame->winId(), GWL_EXSTYLE) | WS_EX_LAYERED);

			DuiWinAPI::SetLayeredWindowAttributes(m_frame->winId(), duiFrameKeyColor(), 0, LWA_COLORKEY);
		}
	}
	else
	{
		m_frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		m_frame->setStyleSheet(duiFrameWindowStyleSheet_KeyColor());
		DuiWinAPI::SetWindowLong(m_frame->winId(),
			GWL_EXSTYLE,
			GetWindowLong(m_frame->winId(), GWL_EXSTYLE) | WS_EX_LAYERED);

		DuiWinAPI::SetLayeredWindowAttributes(m_frame->winId(), duiFrameKeyColor(), (255 * 70) / 100, LWA_COLORKEY);
	}
}

void DuiFrameStyle_Vista::resizeEvent( QResizeEvent *event )
{
	Q_UNUSED(event);

	//### trun off blur effect since it's not compatible with video and webbrowser controls.

	//QRect rect(QPoint(0, 0), event->size());
	//QRegion rgn(rect.adjusted(2, 2, -2, -2));
	//DuiWinAPI::UpdateBlurEffectRegion(m_frame->winId(), rgn.handle());
}

bool DuiFrameStyle_Vista::winEvent( MSG *message, long *result )
{
	switch(message->message)
	{
	case WM_NCCALCSIZE:
		{
			*result = OnNcCalcSize(message);
			return true;
		}

	case WM_WINDOWPOSCHANGING:
		{
			//## don't need this anymore? not sure, just comments it.
			break;

			if(!isPopup())
			{
				LPWINDOWPOS lp = (LPWINDOWPOS)(message->lParam);
				if(lp->hwnd == m_frame->winId() && lp->flags & 0x8000)
				{
					if(lp->cx % 2 == 0)
					{
						lp->cx += 1;
					}
					else
					{
						lp->cx -= 1;
					}
					lp->flags |= SWP_NOSENDCHANGING;
				}
			}
			break;
		}

	case WM_GETMINMAXINFO:
		{
			RECT rtWorkArea = {0};
			DuiWinAPI::getCurrentWindowWorkRect(m_frame->winId(), rtWorkArea);
			MINMAXINFO* pInfo = (MINMAXINFO*)message->lParam;
			if(isToolWindow())
			{
				RECT revisedWorkArea = reviseCurrentWorkArea(rtWorkArea);
				pInfo->ptMaxSize.x = revisedWorkArea.right - revisedWorkArea.left + 4;
				pInfo->ptMaxSize.y = revisedWorkArea.bottom - revisedWorkArea.top + 4;
				pInfo->ptMaxPosition.x = revisedWorkArea.left - 2;
				pInfo->ptMaxPosition.y = revisedWorkArea.top - 2;
			}
			else
			{
				//## don't need this anymore? not sure, just comments it.
                //a weird bug, sometimes the window maximized can't cover full working area when the system task bar on left/right side
                //rtWorkArea.right += 1;
                //end

				RECT revisedWorkArea = reviseCurrentWorkArea(rtWorkArea);
				pInfo->ptMaxSize.x = revisedWorkArea.right - revisedWorkArea.left;
				pInfo->ptMaxSize.y = revisedWorkArea.bottom - revisedWorkArea.top;
				pInfo->ptMaxPosition.x = 0; //must be zero
				pInfo->ptMaxPosition.y = 0; //must be zero
			}
			pInfo->ptMaxTrackSize = pInfo->ptMaxSize;

			QSize s = m_frame->minimumSize();
			pInfo->ptMinTrackSize.x = s.width();
			pInfo->ptMinTrackSize.y = s.height();
			return true;
		}

	case WM_NCLBUTTONDOWN:
		{
			DuiFrameWindow *frameWin = qobject_cast<DuiFrameWindow*>(m_frame);
			if(frameWin)
			{
				frameWin->filterNcMessage(message);
			}
			break;
		}

	case WM_NCLBUTTONDBLCLK:
		{
			if(isResizable())
			{
				HWND hWnd = m_frame->winId();
				if(::IsZoomed(hWnd))
				{
					DuiWinAPI::ShowWindow(hWnd, SW_RESTORE);
				}
				else
				{
					DuiWinAPI::ShowWindow(hWnd, SW_MAXIMIZE);
				}
				return true;
			}
			break;
		}

	case 0xAE:	//WM_NCUAHDRAWCAPTION	(undocument window message)
	case 0xAF:	//WM_NCUAHDRAWFRAME		(undocument window message)
		*result = 0;
		return true;

	case WM_NCHITTEST:
		{
			if(message->hwnd != m_frame->winId())
			{
				break;
			}

			QPoint pt = mapFromGlobal(m_frame, message->lParam);

			int borderSize = 0;
			if(isResizable())
			{
				borderSize = 3;
			}
			*result = OnNcHitTest(pt, m_frame, QMargins(borderSize, borderSize, borderSize, borderSize), 0);
			if(*result == HTCLIENT)
			{
				HitTestClient(m_frame, *result, pt);
			}

			return true;
		}

	default:
		break;
	}

	return false;
}

QRect DuiFrameStyle_Vista::calcContentRect( const QRect &frameRect ) const
{
	if(::IsZoomed(m_frame->winId()))
	{
		if(isToolWindow())
		{
			return frameRect.adjusted(0, 0, 0, 0);
		}
		else
		{
			int cx = GetSystemMetrics(SM_CXSIZEFRAME);
			int cy = GetSystemMetrics(SM_CYSIZEFRAME);

			return frameRect.adjusted(cx - 1, cy - 1, -cx + 1, -cy + 2);
		}
	}

	return frameRect;
}

DuiFrameStyle_Vista::DuiFrameStyle_Vista( QGraphicsView *frame )
: DuiFrameStyle(frame)
{
}

qreal DuiFrameStyle_Vista::opacity() const
{
	return 1.0;
}

bool DuiFrameStyle_Vista::isAnimationEnabled() const
{
	return !isResizable();
}

DuiFrameStyle_Tooltip::DuiFrameStyle_Tooltip( QGraphicsView *frame )
: DuiFrameStyle(frame)
{
}

void DuiFrameStyle_Tooltip::init()
{
	if(isToolWindow())
	{
		m_frame->setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
	}
	else
	{
		m_frame->setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint );
	}

	m_frame->setAttribute(Qt::WA_TranslucentBackground, true);
	m_frame->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
}

void DuiFrameStyle_Tooltip::resizeEvent( QResizeEvent *event )
{
	Q_UNUSED(event);
}

bool DuiFrameStyle_Tooltip::winEvent( MSG *message, long *result )
{
	Q_UNUSED(message);
	Q_UNUSED(result);
	switch(message->message)
	{
	case WM_NCHITTEST:
		{
			QPoint pt = mapFromGlobal(m_frame, message->lParam);

			int borderSize = 0;
			if(isResizable())
			{
				borderSize = 3;
			}
			*result = OnNcHitTest(pt, m_frame, QMargins(borderSize, borderSize, borderSize, borderSize), 0);

			if(*result == HTCLIENT)
			{
				HitTestClient( m_frame, *result, pt );
			}
			return true;
		}
		break;
	}

	return false;
}

QRect DuiFrameStyle_Tooltip::calcContentRect( const QRect &frameRect ) const
{
	return frameRect;
}


DuiFrameStyle_Child::DuiFrameStyle_Child(QGraphicsView *frame)
:DuiFrameStyle(frame)
{
}

void DuiFrameStyle_Child::init()
{
	m_frame->setWindowFlags( Qt::FramelessWindowHint);
	m_frame->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
}

bool DuiFrameStyle_Child::winEvent( MSG *message, long *result )
{
	switch(message->message)
	{
	case WM_NCCALCSIZE:
		*result = OnNcCalcSize(message);
		return true;

	case WM_ERASEBKGND:
		return true;

	case WM_NCHITTEST:
		{			
			*result = HTCLIENT;
			return true;
		}
	case WM_LBUTTONDOWN:
		{
			int i = 0;
			i++;
		}
		break;
	default:
		break;
	}

	return false;
}

void DuiFrameStyle_Child::resizeEvent( QResizeEvent *event )
{
	Q_UNUSED(event);
}

QRect DuiFrameStyle_Child::calcContentRect( const QRect &frameRect ) const
{
	return frameRect;
}

DuiFrameStyle_Windows7::DuiFrameStyle_Windows7( QGraphicsView *frame )
: DuiFrameStyle_Vista(frame)
{

}

void DuiFrameStyle_Windows7::init()
{
	DuiFrameStyle_Vista::init();
}

void DuiFrameStyle_Windows7::resizeEvent( QResizeEvent *event )
{
	RECT rcWin;
	GetWindowRect(m_frame->winId(), &rcWin);
	HRGN hrgn = CreateRoundRectRgn(0, 0, rcWin.right - rcWin.left + 1, rcWin.bottom - rcWin.top + 1, 5, 5);
	::SetWindowRgn(m_frame->winId(), hrgn, TRUE);
}

bool DuiFrameStyle_Windows7::winEvent( MSG *message, long *result )
{
	switch(message->message)
	{
	case WM_NCPAINT:
		*result = 0;
		return true;

	case WM_NCACTIVATE:
		{
			DWORD dwStyle = GetWindowLong(message->hwnd, GWL_STYLE);
			SetWindowLong(message->hwnd, GWL_STYLE, dwStyle & ~WS_VISIBLE);
			*result = ::DefWindowProc(message->hwnd, message->message, message->wParam, 0);
			SetWindowLong(message->hwnd, GWL_STYLE, dwStyle);
			return true;
		}
	default:
		return DuiFrameStyle_Vista::winEvent(message, result);
	}
}

QRect DuiFrameStyle_Windows7::calcContentRect( const QRect &frameRect ) const
{
	return DuiFrameStyle_Vista::calcContentRect(frameRect);
}
