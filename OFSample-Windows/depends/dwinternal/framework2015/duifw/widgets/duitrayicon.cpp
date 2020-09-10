#include "stable.h"

#include "duiapplication.h"
#include "duitrayicon.h"
#include "duifw/duiwinapi.h"
#include "duitheme.h"
#include "dwbase/perflog.h"

const int	KTrayIconId = 1;
const int	KTrayIconMsg = (WM_APP + 10);
const char	KEmptyIconPath[] = ":/theme/duifw/icon_trayicon_empty.ico";

#ifndef MSGFLT_ALLOW
#define MSGFLT_ALLOW 1
#endif //MSGFLT_ALLOW

#if _MSC_VER <= 1500 
typedef struct _NOTIFYICONIDENTIFIER {
	DWORD cbSize;
	HWND  hWnd;
	UINT  uID;
	GUID  guidItem;
} NOTIFYICONIDENTIFIER, *PNOTIFYICONIDENTIFIER;
#endif

typedef HRESULT (WINAPI *PtrShell_NotifyIconGetRect)(const NOTIFYICONIDENTIFIER* identifier, RECT* iconLocation);

struct DUITRAYICON_REQUEST
{
    DWORD dwMessage;
    NOTIFYICONDATAW notifyIconData;
};

class DuiTrayIconPrivate
{
	Q_DISABLE_COPY(DuiTrayIconPrivate)
public:
	DuiTrayIconPrivate();
	~DuiTrayIconPrivate();

    bool m_hasPendingRequest;
	bool m_isVisible;
	bool m_isHover;
	HICON	m_icon;
	HICON	m_emptyIcon;
	QTimer	m_flashIconTimer;
	QTimer	m_checkMouseHoverTimer;
	quint32 m_recreateTaskbarMsg;
	bool	m_mouseIn;
	bool	m_isMouseHover;
	QPoint	m_lastMousePoint;
	bool	m_isInFlashNull;
	bool	m_isLBtnClick;

    QList<DUITRAYICON_REQUEST> m_trayiconRequests;
};

DuiTrayIconPrivate::DuiTrayIconPrivate()
: m_icon(NULL)
, m_emptyIcon(NULL)
, m_isVisible(false)
, m_isHover(false)
, m_mouseIn(false)
, m_isInFlashNull(false)
, m_isLBtnClick(false)
, m_isMouseHover(false)
, m_hasPendingRequest(false)
{

}

DuiTrayIconPrivate::~DuiTrayIconPrivate()
{
	if(!m_trayiconRequests.isEmpty())
	{
		LogDev(LOG::KDuiFw) << "Info:~DuiTrayIconPrivate m_trayiconRequests size=" << m_trayiconRequests.size();
		while(!m_trayiconRequests.isEmpty())
		{
			DUITRAYICON_REQUEST request = m_trayiconRequests.takeFirst();
			if(request.notifyIconData.hIcon)
			{
				DestroyIcon(request.notifyIconData.hIcon);
			}
		}
	}
}

DuiTrayIcon::DuiTrayIcon(QWidget* parent /* = NULL */)
: d_ptr( new DuiTrayIconPrivate )
{
	Q_UNUSED(parent);
	Q_D(DuiTrayIcon);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(&d->m_flashIconTimer, SIGNAL(timeout()), SLOT(onFlashIconTimeout()));
	connect(&d->m_checkMouseHoverTimer, SIGNAL(timeout()), SLOT(onCheckMouseTimeout()));
	d->m_recreateTaskbarMsg = ::RegisterWindowMessage(L"TaskbarCreated");
    d->m_emptyIcon = QIcon( DuiTheme::getImage( KEmptyIconPath )).pixmap(16, 16).toWinHICON();

	DuiWinAPI::customizeUIPIPolicy(winId(), d->m_recreateTaskbarMsg, MSGFLT_ALLOW);
}

DuiTrayIcon::~DuiTrayIcon()
{
	_destroyIcon();
	_destroyEmptyIcon();
	DW_SAFE_DELETE( d_ptr );
}

void DuiTrayIcon::show()
{
	_addTrayIcon();
}

void DuiTrayIcon::hide()
{
	_removeTrayIcon();
}

void DuiTrayIcon::setIcon( const QIcon & icon )
{
	Q_D(DuiTrayIcon);

	DW_ASSERT(d->m_isVisible);

	_destroyIcon();

	if( !icon.isNull() )
	{
		d->m_icon = icon.pixmap(16, 16).toWinHICON();
	}
	_updateIcon();
}

void DuiTrayIcon::setToolTip( const QString & tip )
{
	_updateText(tip);
}

void DuiTrayIcon::setIconAndToolTip(const QIcon & icon, const QString & tip)
{
	Q_D(DuiTrayIcon);
	DW_ASSERT(d->m_isVisible);

	_destroyIcon();
	if (!icon.isNull())
		d->m_icon = icon.pixmap(16, 16).toWinHICON();

	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);
	ncd.uFlags = NIF_ICON | NIF_TIP;

	ncd.hIcon = d->m_isInFlashNull ? d->m_emptyIcon : d->m_icon;
	int size = sizeof(ncd.szTip) / sizeof(TCHAR);
	wcsncpy_s(ncd.szTip, size, reinterpret_cast<const WCHAR*>(tip.utf16()), size - 1);

	_callShellNotifyIcon(NIM_MODIFY, &ncd);
	//防止日志太多，异常才打印~
	if(d->m_emptyIcon == NULL || d->m_icon == NULL)
	{
		LogFinal(LOG::KDuiFw)<< "m_emptyIcon = "<< d->m_emptyIcon << ", m_icon = " << d->m_icon;
	}
}

void DuiTrayIcon::beginFlashIcon()
{
	Q_D(DuiTrayIcon);
	DW_ASSERT(d->m_isVisible);

	d->m_isInFlashNull = false;
	d->m_flashIconTimer.start(500);
}

void DuiTrayIcon::stopFlashIcon()
{
	Q_D(DuiTrayIcon);
	d->m_flashIconTimer.stop();
	d->m_isInFlashNull = false;
	_updateIcon();
}

bool DuiTrayIcon::isFlashingIcon()
{
	Q_D(DuiTrayIcon);
	return d->m_flashIconTimer.isActive();
}

void DuiTrayIcon::_addTrayIcon()
{
	PERFLOG;
	Q_D(DuiTrayIcon);
	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);

	ncd.uFlags = NIF_ICON | NIF_MESSAGE;
    ncd.hIcon = d->m_icon ? d->m_icon : d->m_emptyIcon;
	ncd.uCallbackMessage = KTrayIconMsg;
    _callShellNotifyIcon(NIM_ADD, &ncd);

    d->m_isVisible = true;
	if (ncd.hWnd == NULL || ncd.hIcon == NULL)
	{
		LogFinal(LOG::KDuiFw) << "icon hWnd or hiocn is NULL!" <<"ncd.hWnd:"<<ncd.hWnd<<",ncd.hIcon:"<<ncd.hIcon;
	}
}

void DuiTrayIcon::_removeTrayIcon()
{
	Q_D(DuiTrayIcon);
	DW_ASSERT(d->m_isVisible);

	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);
	_callShellNotifyIcon(NIM_DELETE, &ncd);

	d->m_isVisible = false;
}

void DuiTrayIcon::_updateIcon()
{
	Q_D(DuiTrayIcon);
	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);

	ncd.uFlags = NIF_ICON;
	ncd.hIcon = d->m_isInFlashNull ? d->m_emptyIcon : d->m_icon;
	_callShellNotifyIcon(NIM_MODIFY, &ncd);
}

void DuiTrayIcon::_updateText( const QString &tip )
{
	DW_ASSERT(d_func()->m_isVisible);

	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);

	ncd.uFlags = NIF_TIP;

	int size = sizeof(ncd.szTip) / sizeof(TCHAR);
	wcsncpy_s(ncd.szTip, size, reinterpret_cast<const WCHAR*>(tip.utf16()), size - 1);

	_callShellNotifyIcon(NIM_MODIFY, &ncd);
}

void DuiTrayIcon::onFlashIconTimeout()
{
	Q_D(DuiTrayIcon);
	d->m_isInFlashNull = !d->m_isInFlashNull;
	_updateIcon();
}

void DuiTrayIcon::onCheckMouseTimeout()
{
	Q_D(DuiTrayIcon);
	if (d->m_mouseIn)
	{
		if (QCursor::pos() != d->m_lastMousePoint)
		{
			d->m_mouseIn = false;
		}
	}
	else
	{
		d->m_checkMouseHoverTimer.stop();
		if (d->m_isMouseHover)
		{
			emit hoverLeave();
			d->m_isMouseHover = false;
		}
	}
}

void DuiTrayIcon::onRetryAddTimeout()
{
    _executeNextNotifyIconRequest();
}

void DuiTrayIcon::_destroyIcon()
{
	Q_D(DuiTrayIcon);
	if (NULL != d->m_icon)
	{
		::DestroyIcon(d->m_icon);
		d->m_icon = NULL;
	}
}

void DuiTrayIcon::_destroyEmptyIcon()
{
	Q_D(DuiTrayIcon);
	if(NULL != d->m_emptyIcon)
	{
		::DestroyIcon(d->m_emptyIcon);
		d->m_emptyIcon = NULL;
	}
}

bool DuiTrayIcon::winEvent( MSG * message, long * result )
{
	Q_D(DuiTrayIcon);
	Q_UNUSED(result);
	if (KTrayIconMsg == message->message)
	{
		switch(LOWORD(message->lParam))
		{
		case NIN_BALLOONUSERCLICK: // 有些系统禁用了弹出气泡，导致点托盘图标发出的是NIN_BALLOONUSERCLICK，而不是WM_LBUTTONDOWN
		case WM_LBUTTONDOWN:
			d->m_isLBtnClick = true;
			break;
		case WM_LBUTTONUP:
			if(d->m_isLBtnClick)
			{
				emit lButtonClicked();
				d->m_isLBtnClick = false;
			}			
			break;
		case WM_RBUTTONUP:
		case WM_CONTEXTMENU:
			emit rButtonClicked();
			break;
		case WM_MOUSEMOVE:
			_procMouseMove();
			break;
		default:
			break;
		}

		*result = 0;
		return false;
	}

	if (d->m_recreateTaskbarMsg == message->message)
	{
		removeIconWhenExitSystem();
		_addTrayIcon();
		return false;
	}
	return QWidget::winEvent(message, result);
}

void DuiTrayIcon::_procMouseMove()
{
	Q_D(DuiTrayIcon);
	//@bug by huqiuyun for https://yypm.com:8443/browse/YYXMII-13215
	d->m_lastMousePoint = QCursor::pos();
	//@bug end

	if (!d->m_mouseIn)
	{
		//鼠标从托盘图标外移入
		d->m_mouseIn = true;
		if (!d->m_isMouseHover)
		{
			d->m_isMouseHover = true;
			emit hoverEnter();
		}
		//m_lastMousePoint = QCursor::pos();
		if ( !d->m_checkMouseHoverTimer.isActive())
		{
			d->m_checkMouseHoverTimer.start(50);
		}
	}
}

void DuiTrayIcon::_getInitializedNCD( NOTIFYICONDATAW &ncd )
{
	ncd.cbSize = sizeof(NOTIFYICONDATAW);
	ncd.hWnd = winId();
	ncd.uID = KTrayIconId;
	// 修复开机启动偶尔不显示托盘图标问题
	// http://stackoverflow.com/questions/12954796/shell-notifyicon-an-attempt-was-made-to-reference-a-token-that-does-not-exist
	const static GUID c_trayicon_id = { 0x16ec64fd, 0xa964, 0x4323, { 0xac, 0x3e, 0x6d, 0x6b, 0xd5, 0xc3, 0xe8, 0x38 } };
	ncd.guidItem = c_trayicon_id;
}

QRect DuiTrayIcon::getTrayiconRect()
{
	Q_D(DuiTrayIcon);
	DW_ASSERT(d->m_isVisible);

	// WIN7 API获得任务栏位置
	static PtrShell_NotifyIconGetRect Shell_NotifyIconGetRect =
	(PtrShell_NotifyIconGetRect)QLibrary::resolve(QLatin1String("shell32"), "Shell_NotifyIconGetRect");
	if (Shell_NotifyIconGetRect) 
	{
		NOTIFYICONIDENTIFIER nid = {0};
		nid.cbSize = sizeof(nid);
		nid.hWnd = winId();
		nid.uID = KTrayIconId;

		RECT rect;
		HRESULT hr = Shell_NotifyIconGetRect(&nid, &rect);
		if (SUCCEEDED(hr)) 
		{
			if(hr != S_FALSE)
			{
				return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			}
		}
	}

	// WIN7以前的版本获取任务栏位置
	QRect ret;

	TBBUTTON buttonData;
	DWORD processID = 0;
	HWND trayHandle = FindWindow(L"Shell_TrayWnd", NULL);

	//find the toolbar used in the notification area
	if (trayHandle) {
		trayHandle = FindWindowEx(trayHandle, NULL, L"TrayNotifyWnd", NULL);
		if (trayHandle) {
			HWND hwnd = FindWindowEx(trayHandle, NULL, L"SysPager", NULL);
			if (hwnd) {
				hwnd = FindWindowEx(hwnd, NULL, L"ToolbarWindow32", NULL);
				if (hwnd)
					trayHandle = hwnd;
			}
		}
	}

	if (!trayHandle) {
		//if there's no taskbar, it could be WINE in linux, uses mouse position instead of tray
		return QRect(QCursor::pos(), QSize(16, 16));
	}

	GetWindowThreadProcessId(trayHandle, &processID);
	if (processID <= 0)
		return ret;

	HANDLE trayProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ, 0, processID);
	if (!trayProcess)
		return ret;

	int buttonCount = SendMessage(trayHandle, TB_BUTTONCOUNT, 0, 0);
	LPVOID data = VirtualAllocEx(trayProcess, NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);

	if ( buttonCount < 1 || !data ) {
		CloseHandle(trayProcess);
		return ret;
	}

	//search for our icon among all toolbar buttons
	for (int toolbarButton = 0; toolbarButton  < buttonCount; ++toolbarButton ) {
		SIZE_T numBytes = 0;
		DWORD appData[2] = { 0, 0 };
		SendMessage(trayHandle, TB_GETBUTTON, toolbarButton , (LPARAM)data);

		if (!ReadProcessMemory(trayProcess, data, &buttonData, sizeof(TBBUTTON), &numBytes))
			continue;

		if (!ReadProcessMemory(trayProcess, (LPVOID) buttonData.dwData, appData, sizeof(appData), &numBytes))
			continue;

		int currentIconId = appData[1];
		HWND currentIconHandle = (HWND) appData[0];
		bool isHidden = buttonData.fsState & TBSTATE_HIDDEN;

		if (currentIconHandle == winId() &&
			currentIconId == KTrayIconId && !isHidden) {
				SendMessage(trayHandle, TB_GETITEMRECT, toolbarButton , (LPARAM)data);
				RECT iconRect = {0, 0};
				if(ReadProcessMemory(trayProcess, data, &iconRect, sizeof(RECT), &numBytes)) {
					MapWindowPoints(trayHandle, NULL, (LPPOINT)&iconRect, 2);
					QRect geometry(iconRect.left + 1, iconRect.top + 1,
						iconRect.right - iconRect.left - 2,
						iconRect.bottom - iconRect.top - 2);
					if (geometry.isValid())
						ret = geometry;
					break;
				}
		}
	}
	VirtualFreeEx(trayProcess, data, 0, MEM_RELEASE);
	CloseHandle(trayProcess);
	return ret;
}

void DuiTrayIcon::removeIconWhenExitSystem()
{
	Q_D(DuiTrayIcon);
	DW_ASSERT(d->m_isVisible);

	NOTIFYICONDATAW ncd = {0};
	_getInitializedNCD(ncd);
	::Shell_NotifyIconW(NIM_DELETE, &ncd);
	d->m_isVisible = false;
}

void DuiTrayIcon::_callShellNotifyIcon(DWORD dwMessage, PNOTIFYICONDATAW lpData)
{
    Q_D(DuiTrayIcon);

	int nSize = d->m_trayiconRequests.size();
	LogDev(LOG::KDuiFw) << "Info:m_trayiconRequests size=" << nSize;
	if(nSize >= 10)
	{
		LogFinal(LOG::KDuiFw) << "Warning:retry requests more than ten obj";
		DUITRAYICON_REQUEST request = d->m_trayiconRequests.takeLast();
		if(request.notifyIconData.hIcon)
		{
			DestroyIcon(request.notifyIconData.hIcon);
		}
	}
    DUITRAYICON_REQUEST request;
    request.dwMessage = dwMessage;
    request.notifyIconData = *lpData;
    if(request.notifyIconData.hIcon)
    {
        request.notifyIconData.hIcon = DuplicateIcon(NULL, request.notifyIconData.hIcon);
    }

    if(dwMessage == NIM_ADD)
    {
        d->m_trayiconRequests.prepend(request);
    }
    else
    {
        d->m_trayiconRequests.append(request);
    }

    _executeNextNotifyIconRequest();
}

void DuiTrayIcon::_executeNextNotifyIconRequest()
{
    Q_D(DuiTrayIcon);

    if(d->m_hasPendingRequest)
    {
        return;
    }

    if( !d->m_trayiconRequests.isEmpty() )
    {
        d->m_hasPendingRequest = true;

        const DUITRAYICON_REQUEST &firstRequest = d->m_trayiconRequests.first();
        DuiWinAPI::Shell_NotifyIcon_async(firstRequest.dwMessage, (PNOTIFYICONDATAW)&firstRequest.notifyIconData, this, "onNotifyIconFinished");
    }
}

void DuiTrayIcon::onNotifyIconFinished( bool succeed )
{
    Q_D(DuiTrayIcon);

    d->m_hasPendingRequest = false;

    if(succeed)
    {
        if(!d->m_trayiconRequests.isEmpty())
        {
            DUITRAYICON_REQUEST request = d->m_trayiconRequests.takeFirst();
            if(request.notifyIconData.hIcon)
            {
                DestroyIcon(request.notifyIconData.hIcon);
            }
            _executeNextNotifyIconRequest();
        }
    }
    else
    {
        LogFinal(LOG::KDuiFw) << "Add/Modify tray icon failed";
        QTimer::singleShot(500, this, SLOT(onRetryAddTimeout()));
    }
}
