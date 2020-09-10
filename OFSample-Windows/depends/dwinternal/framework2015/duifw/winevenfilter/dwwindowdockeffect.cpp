#include "stable.h"
#include "dwwindowdockeffect.h"
#include "duifw/duiframewindow.h"
#include "duifw/duiwinapi.h"

//------------------------------------------------------------------------------
const int KEdgeInterval  = 50;				//����ճ��ʱ�������Ļ�߽����С�������λΪ����
const int KEdgeInfalte   = 10;				//��������ʱ����봰�ڱ߽����С�������λΪ����
const int KEdgeWidthWhenHide = 2;			//���������ڱ�Եʱ�ɼ��Ŀ�Ȼ��߸߶�

//------------------------------------------------------------------------------
class DWWindowDockEffectPrivate
{
	Q_DISABLE_COPY(DWWindowDockEffectPrivate)

public:
	DWWindowDockEffectPrivate();
	~DWWindowDockEffectPrivate();

	DuiFrameWindow* m_window;
	QTimer m_timerCheckMouse;
	DWWindowDockEffect::HideMode m_hideMode;
	DWWindowDockEffect::AreaInfo m_workAreaInfo;
	DWWindowDockEffect::AreaInfo m_screenAreaInfo;
	QRect m_oldWndRect;		//��¼�˴��������ص������Ե֮ǰ��λ�ã�ͣ��λ�ã���
	bool m_isSizeChanged;	//���ڴ�С�Ƿ�ı���
	bool m_bNeedReSizeWindow;//��Ǵ����Ƿ��Ǵӱ�Ե�϶������棬���ڽ����ϵͳδ���á��϶�ʱ��ʾ�������ݡ�ʱ����δ�ָ�ԭ����С�����⡣
	bool m_alwaysTopMost;

	// �����汻����������뿪���淶Χ�Ƿ�ᴥ������
	// �˱�־���ڵ�ʹ�ÿ�ݼ���˫��������YYͼ��������������뿪���淶Χ����������
	bool m_isMouseLeaveTrigger;

	bool m_childBlock;
    bool m_enabled;
};

DWWindowDockEffectPrivate::DWWindowDockEffectPrivate()
: m_window(NULL)
, m_hideMode(DWWindowDockEffect::HM_NONE)
, m_oldWndRect(QRect(0, 0, 0, 0))
, m_isSizeChanged(false)
, m_bNeedReSizeWindow(false)
, m_alwaysTopMost(false)
, m_isMouseLeaveTrigger(true)
, m_childBlock(false)
, m_enabled(true)
{
	;
}

DWWindowDockEffectPrivate::~DWWindowDockEffectPrivate()
{
	m_window = NULL;
}

//------------------------------------------------------------------------------
DWWindowDockEffect::DWWindowDockEffect(DuiFrameWindow *window)
: DWAbstractWinEvenFilter(window)
, d_ptr(new DWWindowDockEffectPrivate())
{
	d_func()->m_window = window;
	d_func()->m_window->installWinEvenFilter(this);

	_updateAreaInfo();
	d_func()->m_timerCheckMouse.setInterval(200);
	connect(&(d_func()->m_timerCheckMouse), SIGNAL(timeout()), this, SLOT(on_timeout()));
	d_func()->m_timerCheckMouse.start();
}

DWWindowDockEffect::~DWWindowDockEffect()
{
	DW_SAFE_DELETE(d_ptr);
}

bool DWWindowDockEffect::winEvent(MSG *message, long *result)
{
	Q_UNUSED(result);
	switch (message->message)
	{
	case WM_MOVING:
		return _handleMoving(message->wParam, (LPRECT)message->lParam);
	case WM_MOVE:
		return _handleMove(LOWORD(message->lParam), HIWORD(message->lParam));
	case WM_NCHITTEST:
		return _handleHitClient(QPoint(LOWORD(message->lParam), HIWORD(message->lParam)));
	case WM_GETMINMAXINFO:
		return _handleMaxMinInfo();
	default:
		break;
	}

	return false;
}

bool DWWindowDockEffect::IsHideToEdge(void)const
{
	Q_D(const DWWindowDockEffect);
	return d->m_hideMode == HM_TOP || d->m_hideMode == HM_LEFT || d->m_hideMode == HM_RIGHT;
}

bool DWWindowDockEffect::IsCloseToEdge(void)const
{
	Q_D(const DWWindowDockEffect);
	return d->m_hideMode == HM_TOP_EDGE || d->m_hideMode == HM_LEFT_EDGE || d->m_hideMode == HM_RIGHT_EDGE;
}

void DWWindowDockEffect::ToShowFromEdge(bool isMouseLeaveTrigger)
{
	Q_D(DWWindowDockEffect);
	if (IsHideToEdge())
	{
		d->m_isMouseLeaveTrigger = isMouseLeaveTrigger;
		_toShowOrHideWindow();
	}
}

void DWWindowDockEffect::ToHideToEdge(void)
{
	if (IsCloseToEdge())
	{
		_toShowOrHideWindow();
	}
}

void DWWindowDockEffect::on_timeout()
{
	Q_D(DWWindowDockEffect);
	if (!::IsWindowVisible(d->m_window->winId()))
	{
		return;
	}

	QPoint curPos = QCursor::pos();

	//��ȡ��ʱ���ڴ�С
	QRect rcWindow = d->m_window->realFrameGeometry();
	//����rcWindow���Դﵽ����뿪���ڱ���һ������Ŵ����¼�
	rcWindow.adjust(-KEdgeInfalte, -KEdgeInfalte, KEdgeInfalte, KEdgeInfalte);

	if(!rcWindow.contains(curPos)) //�������뿪���������
	{
		d->m_timerCheckMouse.stop();

		if (IsCloseToEdge())
		{
			_toShowOrHideWindow();
		}
	}
}

void DWWindowDockEffect::on_showOrHideWindow()
{
	Q_D(DWWindowDockEffect);

	if (IsCloseToEdge())
	{
		if ( !d->m_childBlock )
		{
			_doHide();
		}
	}
	else if (IsHideToEdge())
	{
		_doShow();
	}
}

bool DWWindowDockEffect::_handleMoving(UINT fwSide, LPRECT pRect)
{
	_fixMovingRect(fwSide, pRect); //����pRect
	return false;
}

bool DWWindowDockEffect::_handleMove(int x, int y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_D(DWWindowDockEffect);

	//�˴����µ������ڴ�С��Ϊ�˽����ϵͳδ���á��϶�ʱ��ʾ�������ݡ�ʱ����δ�ָ�ԭ����С�����⡣
	if (!_isDragFullWindows())
	{
		if (d->m_bNeedReSizeWindow)
		{
			QRect rcWindow = d->m_window->realFrameGeometry();

			if (d->m_hideMode == HM_NONE || d->m_hideMode == HM_TOP_EDGE)
			{
				if (rcWindow.height() != d->m_oldWndRect.height())
				{
					rcWindow.setHeight(d->m_oldWndRect.height());
					d->m_window->setRealFrameGeometry(rcWindow);
				}
			}
			else if (d->m_hideMode == HM_LEFT_EDGE || d->m_hideMode == HM_RIGHT_EDGE)
			{
				LONG workHeight = qAbs(d->m_workAreaInfo.bottom - d->m_workAreaInfo.top);
				if (rcWindow.height() != workHeight)
				{
					rcWindow.setHeight(workHeight);
					d->m_window->setRealFrameGeometry(rcWindow);
				}
			}

			d->m_bNeedReSizeWindow = false;
		}	
	}

	return false;
}

bool DWWindowDockEffect::_handleHitClient(QPoint point)
{
	Q_UNUSED(point);
	Q_D(DWWindowDockEffect);

	if (d->m_hideMode != HM_NONE && !d->m_timerCheckMouse.isActive() && d->m_isMouseLeaveTrigger)
	{
		d->m_timerCheckMouse.start();

		if (IsHideToEdge())
		{
			_toShowOrHideWindow();
		}
	}
	return false;
}

bool DWWindowDockEffect::_handleMaxMinInfo()
{
	//�޸ķֱ�����Ҫ����
	_updateAreaInfo();
	return false;
}

DWWindowDockEffect::CloseToBoundary DWWindowDockEffect::_checkColseToBoundary()
{
	Q_D(DWWindowDockEffect);

	CloseToBoundary eBoundary = CTB_None;
	QPoint curPos = QCursor::pos();
	if (curPos.y() - d->m_screenAreaInfo.top <= KEdgeInterval && d->m_workAreaInfo.top == d->m_screenAreaInfo.top)
	{
		eBoundary = CTB_Top;
	}
	else if (curPos.x() - d->m_screenAreaInfo.left <= KEdgeInterval && d->m_workAreaInfo.left == d->m_screenAreaInfo.left)
	{
		eBoundary = CTB_Left;
	}
	else if(d->m_screenAreaInfo.right - curPos.x() <= (KEdgeInterval) && d->m_workAreaInfo.right == d->m_screenAreaInfo.right)
	{
		eBoundary = CTB_Right;
	}

	return eBoundary;
}

void DWWindowDockEffect::_fixMovingRect(UINT fwSide, LPRECT pRect)
{
	Q_D(DWWindowDockEffect);
	Q_UNUSED(fwSide);
	int height = pRect->bottom - pRect->top;
	int width  = pRect->right - pRect->left;

	CloseToBoundary eBoundary = _checkColseToBoundary();
	switch(eBoundary)
	{
	case CTB_Top:
		{
			if (d->m_isSizeChanged)
			{
				height = d->m_oldWndRect.height();
				d->m_bNeedReSizeWindow = true;
			}

			pRect->bottom = d->m_workAreaInfo.top + height;
			pRect->top = d->m_workAreaInfo.top;

			d->m_hideMode = HM_TOP_EDGE;
			break;
		}
	case CTB_Left:
		{
			if(!d->m_isSizeChanged)
			{
				_saveWindowRect(pRect);
			}

			pRect->right = d->m_workAreaInfo.left + width;
			pRect->left = d->m_workAreaInfo.left;
			pRect->top = d->m_workAreaInfo.top;
			pRect->bottom = d->m_workAreaInfo.bottom;

			d->m_isSizeChanged = true;
			d->m_bNeedReSizeWindow = true;
			d->m_hideMode = HM_LEFT_EDGE;
			break;
		}
	case CTB_Right:
		{
			if(!d->m_isSizeChanged)
			{
				_saveWindowRect(pRect);
			}

			pRect->left = d->m_workAreaInfo.right - width;
			pRect->right = d->m_workAreaInfo.right;
			pRect->top = d->m_workAreaInfo.top;
			pRect->bottom = d->m_workAreaInfo.bottom;

			d->m_isSizeChanged = true;
			d->m_bNeedReSizeWindow = true;
			d->m_hideMode = HM_RIGHT_EDGE;
			break;
		}
	case CTB_None:
		{
			if(d->m_isSizeChanged)
			{
				//�������������,���ϳ��������ԭ����С
				pRect->bottom = pRect->top + d->m_oldWndRect.height();
				d->m_isSizeChanged = false;
				d->m_bNeedReSizeWindow = true;
			}

			if(d->m_timerCheckMouse.isActive())
			{
				d->m_timerCheckMouse.stop();
			}

			d->m_hideMode = HM_NONE;
			_setTopMost(false);
			break;
		}
	default:
		DW_ASSERT(false);
		break;
	}
}

void DWWindowDockEffect::_toShowOrHideWindow()
{
	// bug16876: ���������Ƶ��ʱ����ͣ���������Ե�Ŀͻ����޷������Զ�����
	// ���ﵱ�겻֪Ϊ��Ҫ�ӳ�50ms����ѯδ�����ĳ�0�����OK
	QTimer::singleShot(0, this, SLOT(on_showOrHideWindow()));
}

void DWWindowDockEffect::_doHide()
{
	Q_D(DWWindowDockEffect);
    if (!d->m_enabled)
    {
        return;
    }

	QRect rcWindow = d->m_window->realFrameGeometry();

	switch(d->m_hideMode)
	{
	case HM_TOP_EDGE:
		{
			d->m_hideMode = HM_TOP;
			rcWindow.moveBottom(d->m_workAreaInfo.top + KEdgeWidthWhenHide);
		}
		break;
	case HM_LEFT_EDGE:
		{
			d->m_hideMode = HM_LEFT;
			rcWindow.moveRight(d->m_workAreaInfo.left + KEdgeWidthWhenHide);
		}
		break;
	case HM_RIGHT_EDGE:
		{
			d->m_hideMode = HM_RIGHT;
			rcWindow.moveLeft(d->m_workAreaInfo.right - KEdgeWidthWhenHide);
		}
		break;
	default:
		break;
	}

	_setTopMost(true);
	d->m_window->animatedMoveTo(rcWindow, 200);

	// ÿһ�δ������غ󣬻ָ�����ƿ����津������
	QTimer::singleShot(200, this, SLOT(on_doneHide()));

	d->m_childBlock = false;
}

void DWWindowDockEffect::_doShow()
{
	Q_D(DWWindowDockEffect);

	if(d->m_hideMode == HM_NONE)
		return;

	QRect rcWindow = d->m_window->realFrameGeometry();

	switch(d->m_hideMode)
	{
	case HM_TOP:
		{
			d->m_hideMode = HM_TOP_EDGE;
			rcWindow.moveTop(d->m_workAreaInfo.top);
		}
		break;
	case HM_LEFT:
		{
			d->m_hideMode = HM_LEFT_EDGE;
			rcWindow.moveLeft(d->m_workAreaInfo.left);
		}
		break;
	case HM_RIGHT:
		{
			d->m_hideMode = HM_RIGHT_EDGE;
			rcWindow.moveRight(d->m_workAreaInfo.right);
		}
		break;
	default:
		break;
	}

	d->m_window->animatedMoveTo(rcWindow, 200);
}

void DWWindowDockEffect::setAlwaysOnTopState(bool topMost)
{
	Q_D(DWWindowDockEffect);
	d->m_alwaysTopMost = topMost;
}

void DWWindowDockEffect::setChildBlock(bool childBlock )
{
	Q_D(DWWindowDockEffect);
	d->m_childBlock = childBlock;

	// bug16876 fix: ȡ��childBlock��Ҫ���¿�����ʱ�������򴰿ڲ�������
	if (!childBlock && !d->m_timerCheckMouse.isActive())
	{
		d->m_timerCheckMouse.start();
	}
}

void DWWindowDockEffect::_setTopMost(bool bTopMost)
{
	Q_D(DWWindowDockEffect);
	// ע�ⲻҪ������
	if (bTopMost)
	{
		DuiWinAPI::setWindowPos(d->m_window->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	}
	else if(!d->m_alwaysTopMost)
	{
		DuiWinAPI::setWindowPos(d->m_window->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	}
}

bool DWWindowDockEffect::_isDragFullWindows()
{
	BOOL bDrag = FALSE;
	DuiWinAPI::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bDrag, 0);
	return bDrag == TRUE;
}

void DWWindowDockEffect::_saveWindowRect(LPRECT rect)
{
	Q_D(DWWindowDockEffect);
	d->m_oldWndRect.setTop(rect->top);
	d->m_oldWndRect.setHeight(rect->bottom - rect->top);
	d->m_oldWndRect.setLeft(rect->left);
	d->m_oldWndRect.setWidth(rect->right - rect->left);
}

void DWWindowDockEffect::ToShowFromEdgeImmediately()
{
	Q_D(DWWindowDockEffect);
	if (d->m_oldWndRect.isEmpty())
	{
		d->m_oldWndRect = d->m_window->realFrameGeometry();
	}

	QRect rcWindow;
	if (d->m_oldWndRect.left() < 0)
	{
		rcWindow.setLeft(0);
	}
	else if (d->m_oldWndRect.right() > d->m_workAreaInfo.right)
	{
		rcWindow.setLeft(d->m_workAreaInfo.right - d->m_oldWndRect.width());
	}
	else
	{
		rcWindow.setLeft(d->m_oldWndRect.left());
	}

	if (d->m_oldWndRect.top() < 0)
	{
		rcWindow.setTop(0);
	}
	else if (d->m_oldWndRect.bottom() > d->m_workAreaInfo.bottom)
	{
		rcWindow.setTop(d->m_workAreaInfo.bottom - d->m_oldWndRect.height());
	}
	else
	{
		rcWindow.setTop(d->m_oldWndRect.top());
	}

	rcWindow.setWidth(d->m_oldWndRect.width());
	rcWindow.setHeight(d->m_oldWndRect.height());
	d->m_window->setRealFrameGeometry(rcWindow);
}

void DWWindowDockEffect::_updateAreaInfo()
{
	Q_D(DWWindowDockEffect);

	_resetAreaInfo();

	QDesktopWidget desktop;
	int count = desktop.screenCount();
	for (int i = 0; i < count; i++)
	{
		// work area
		QRect workRect = desktop.availableGeometry(i);
		int workAreaLeft = workRect.left();
		if (workAreaLeft < d->m_workAreaInfo.left)
		{
			d->m_workAreaInfo.left = workAreaLeft;
		}

		int workAreaRight = workRect.right();
		if (workAreaRight > d->m_workAreaInfo.right)
		{
			d->m_workAreaInfo.right = workAreaRight;
		}

		// screen area
		QRect screenRect = desktop.screenGeometry(i);
		int screenAreaLeft = screenRect.left();
		if (screenAreaLeft < d->m_screenAreaInfo.left)
		{
			d->m_screenAreaInfo.left = screenAreaLeft;
		}

		int screenAreaRight = screenRect.right();
		if (screenAreaRight > d->m_screenAreaInfo.right)
		{
			d->m_screenAreaInfo.right = screenAreaRight;
		}
	}

	MONITORINFO mi = {0};
	mi.cbSize = sizeof(mi);
	HMONITOR hMonitor = ::MonitorFromWindow(d->m_window->winId(), MONITOR_DEFAULTTONEAREST);
	BOOL ret = GetMonitorInfo(hMonitor, &mi);
	DW_ASSERT(ret == TRUE);

	d->m_screenAreaInfo.top = mi.rcMonitor.top;
	d->m_screenAreaInfo.bottom = mi.rcMonitor.bottom;

	d->m_workAreaInfo.top = mi.rcWork.top;
	d->m_workAreaInfo.bottom = mi.rcWork.bottom;
}

void DWWindowDockEffect::_resetAreaInfo()
{
	Q_D(DWWindowDockEffect);

	d->m_workAreaInfo.reset();
	d->m_screenAreaInfo.reset();
}

void DWWindowDockEffect::on_doneHide()
{
	Q_D(DWWindowDockEffect);
	d->m_isMouseLeaveTrigger = true;
}

void DWWindowDockEffect::setEnabled( bool b )
{
    Q_D(DWWindowDockEffect);
    d->m_enabled = b;
}
bool DWWindowDockEffect::enabled() const
{
    Q_D(const DWWindowDockEffect);
    return d->m_enabled;
}