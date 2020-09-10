#include "stable.h"

#pragma warning(push)
#pragma warning(disable : 4091)
#include <shlobj.h>
#pragma warning(pop)

#include "dwbase/exceptionreport.h"
#include "duiwinapi.h"
#include "duiapplication.h"

typedef HRESULT (WINAPI *funcDwmExtendFrameIntoClientArea)(HWND, const MARGINS*);
typedef HRESULT (WINAPI *funcDwmEnableBlurBehindWindow)(HWND, const DWM_BLURBEHIND*);
typedef HRESULT (WINAPI *funcDwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
typedef HRESULT (WINAPI *funcDwmIsCompositionEnabled)(BOOL *p);
typedef HRESULT (WINAPI *funcSHGetPropertyStoreForWindow)(HWND hwnd, REFIID riid, void **ppv);
typedef HRESULT (WINAPI *funcInitPropVariantFromStringVector)(PCWSTR *prgsz, ULONG cElems, PROPVARIANT *ppropvar);

#define INITGUID
#include <propkey.h>
DEFINE_PROPERTYKEY(dwPKEY_AppUserModel_ID, 0x9F4C2855, 0x9F79, 0x4B39, 0xA8, 0xD0, 0xE1, 0xD4, 0x2D, 0xE1, 0xD5, 0xF3, 5);
#undef INITGUID

static HMODULE g_hDwmapiLib = NULL;

static void *_getDwmapiProcAddress(const char *functionName)
{
	if( !g_hDwmapiLib )
	{
		g_hDwmapiLib = ::LoadLibrary(L"dwmapi.dll");
	}

	return ::GetProcAddress(g_hDwmapiLib, functionName);
}

HRESULT DuiWinAPI::ExtendIntoClientAll(HWND hwnd)
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return S_FALSE;
	}

	// Negative margins have special meaning to DwmExtendFrameIntoClientArea.
	// Negative margins create the "sheet of glass" effect, where the client area
	// is rendered as a solid surface with no window border.
	MARGINS margins = {-1};
	HRESULT hr = S_OK;
	funcDwmExtendFrameIntoClientArea pFunc = (funcDwmExtendFrameIntoClientArea)_getDwmapiProcAddress("DwmExtendFrameIntoClientArea");
	// Extend the frame across the entire window.
	hr = pFunc(hwnd, &margins);

	return hr;
}

HRESULT DuiWinAPI::UpdateBlurEffectRegion(HWND hwnd, HRGN hRgn)
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return S_FALSE;
	}

	DWM_BLURBEHIND blurBehind = {0};
	blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	if(hRgn == NULL)
	{
		blurBehind.fEnable = FALSE;
	}
	else
	{
		blurBehind.fEnable = TRUE;
	}
	blurBehind.hRgnBlur = hRgn;

	HRESULT hr = S_OK;
	funcDwmEnableBlurBehindWindow pFunc = (funcDwmEnableBlurBehindWindow)_getDwmapiProcAddress("DwmEnableBlurBehindWindow");
	hr = pFunc(hwnd, &blurBehind);

	return hr;
}

HRESULT DuiWinAPI::DisableNCRendering(HWND hwnd)
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return S_FALSE;
	}
	
	//disable non-client area rendering on window
	DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;

	HRESULT hr = S_OK;
	funcDwmSetWindowAttribute pFunc = (funcDwmSetWindowAttribute)_getDwmapiProcAddress("DwmSetWindowAttribute");
	hr = pFunc(hwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));

	return hr;
}

HRESULT DuiWinAPI::AllowNCPaint(HWND hwnd)
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return S_FALSE;
	}

	HRESULT hr = S_OK;
	funcDwmSetWindowAttribute pFunc = (funcDwmSetWindowAttribute)_getDwmapiProcAddress("DwmSetWindowAttribute");

	BOOL enabled = TRUE;
	hr = pFunc(hwnd, DWMWA_ALLOW_NCPAINT, &enabled, sizeof(enabled));	

	return hr;
}

bool DuiWinAPI::IsCompositionEnabled()
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return false;
	}

	funcDwmIsCompositionEnabled pFunc = (funcDwmIsCompositionEnabled)_getDwmapiProcAddress("DwmIsCompositionEnabled");

	BOOL enabled = TRUE;
	HRESULT hr = pFunc(&enabled);

	if(hr == S_OK)
	{
		return enabled;
	}

	return false;
}

bool DuiWinAPI::ShowWindow(HWND hWnd, int nCmdShow)
{
	return ::ShowWindow(hWnd, nCmdShow);
}

bool DuiWinAPI::SystemParametersInfo( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni )
{
	return ::SystemParametersInfo( uiAction, uiParam, pvParam, fWinIni );
}

bool DuiWinAPI::getCurrentWindowWorkRect( HWND hwnd, RECT& workRect )
{
	MONITORINFO mi = {0};
	mi.cbSize = sizeof(mi);
	HMONITOR hMonitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	BOOL ret = GetMonitorInfo(hMonitor, &mi);
	if (ret == TRUE)
	{
		workRect = mi.rcWork;
	}

	return ret;
}

bool DuiWinAPI::SetWindowRgn( HWND hWnd, HRGN hRgn, bool redrawNow)
{
	return ::SetWindowRgn(hWnd, hRgn, redrawNow);
}

LONG DuiWinAPI::SetWindowLong( HWND hWnd, int nIndex, LONG dwNewLong )
{
	return ::SetWindowLong(hWnd, nIndex, dwNewLong);
}

BOOL DuiWinAPI::setWindowPos(HWND hWnd, HWND hWndlnsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	return ::SetWindowPos(hWnd, hWndlnsertAfter, X, Y, cx, cy, uFlags);
}

BOOL DuiWinAPI::SetLayeredWindowAttributes( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags )
{
	return ::SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
}

DWORD DuiWinAPI::ProcessIdFromWindow(HWND hWnd)
{
	DWORD dwPId = 0;
	::GetWindowThreadProcessId(hWnd, &dwPId);

	return dwPId;
}

namespace
{
	BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
	{
		QList<WId> &list = *((QList<WId>*)lParam);
		list.push_back(hwnd);

		return TRUE;
	}
}

QList<WId> DuiWinAPI::GetClildWindow(HWND hWnd)
{
	QList<WId> list;
	::EnumChildWindows(hWnd, EnumChildProc, ((LPARAM)(&list)));

	return list;
}

bool DuiWinAPI::CreateProcess(const QString &exeName, const QString &cmdLine, quint32 createFlags, quint32& pId)
{
	if (cmdLine.count() > 4096)
	{
		DW_ASSERT_X(false, __FUNCTION__, "cmdline is too long.");
		return false;
	}

	TCHAR buf[4096] = {0};;
	wcscpy_s(buf, _countof(buf), reinterpret_cast<LPCTSTR>(cmdLine.utf16()));

	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pInfo = {0};
	BOOL b = ::CreateProcess(
		exeName.isEmpty() ? NULL : reinterpret_cast<LPCTSTR>(exeName.utf16()),
		buf, 
		NULL, NULL, NULL, 
		createFlags, 
		NULL, NULL, 
		&si, 
		&pInfo);

	DW_ASSERT_X(b, __FUNCTION__, QString("create process %1 with argments %2 failed.").arg(exeName, cmdLine).toLatin1().constData());

	::CloseHandle(pInfo.hProcess);
	::CloseHandle(pInfo.hThread);
	pId = pInfo.dwProcessId;
	return (b == TRUE);
}

bool DuiWinAPI::IsKeyPressed(int vKey)
{
	const unsigned short KEYPRESSMASK = 0x8000;

	SHORT nVal = ::GetAsyncKeyState(vKey);

	/*
	* The return value specifies the status of the specified virtual key, as follows: 
	* If the high-order bit is 1, the key is down; otherwise, it is up. 
	* If the low-order bit is 1, the key is toggled. 
	* A key, such as the CAPS LOCK key, is toggled if it is turned on. 
	* The key is off and untoggled if the low-order bit is 0. 
	* A toggle key’s indicator light (if any) on the keyboard will be on when the key is toggled, and off when the key is untoggled. 
	*/
	if((nVal & KEYPRESSMASK) == KEYPRESSMASK)
	{
		return true;
	}

	return false;
}

DW_DUIFW_EXPORT int DuiWinAPI::GlobalAddAtom( const QString &atomName )
{
	return static_cast<int>(::GlobalAddAtom(reinterpret_cast<LPCTSTR>(atomName.utf16()) ));
}

DW_DUIFW_EXPORT void DuiWinAPI::GlobalDeleteAtom( int atom )
{
	::GlobalDeleteAtom( static_cast<ATOM>(atom) );
}

DW_DUIFW_EXPORT bool DuiWinAPI::RegisterHotKey( HWND hWnd, int id, UINT modifierKey, UINT virtualKey )
{
	return ::RegisterHotKey( hWnd, id, modifierKey, virtualKey );
}

DW_DUIFW_EXPORT void DuiWinAPI::UnregisterHotKey( HWND hWnd, int id )
{
	::UnregisterHotKey( hWnd, id );
}

DW_DUIFW_EXPORT QString DuiWinAPI::GetSpecialFolderPath( SpecialFolderType type )
{
	int csidl = 0;
	switch(type)
	{
	case Folder_Person:
		csidl = CSIDL_PERSONAL;
		break;
	case Folder_UserAppdata:
		csidl = CSIDL_APPDATA;
		break;
	case Folder_MyMusic:
		csidl = CSIDL_MYMUSIC;
		break;
	default:
		qCritical() << "Unsupportted type." << type;
#ifdef _DEBUG
		DebugBreak();
#endif //_DEBUG
		return QString();
	}
	wchar_t buffer[MAX_PATH * 2] = {0};
	::SHGetSpecialFolderPath(NULL, buffer, csidl, TRUE);
	
	QDir dir(QStringFromWCharArray(buffer));
	return  dir.absolutePath() + "/";
}

DW_DUIFW_EXPORT QRect DuiWinAPI::getTaskbarRect()
{
	HWND hWndTaskbar = ::FindWindow( L"Shell_TrayWnd", NULL );

	QRect rectTaskbar;
	if (NULL != hWndTaskbar)
	{
		APPBARDATA barData = {0};
		barData.cbSize = sizeof(APPBARDATA);
		barData.hWnd = hWndTaskbar;
		BOOL result = (BOOL)SHAppBarMessage( ABM_GETTASKBARPOS, &barData ); 
		
		if (result)
		{
			rectTaskbar.setRect(barData.rc.left, barData.rc.top, barData.rc.right - barData.rc.left, barData.rc.bottom - barData.rc.top);
		}
	}
	return rectTaskbar;
}

DW_DUIFW_EXPORT bool DuiWinAPI::isMutexExist(const QString &mutexName, HANDLE &hMutex)
{
    hMutex = ::CreateMutex(NULL, TRUE, reinterpret_cast<LPCTSTR>(mutexName.utf16()));
	if (NULL == hMutex)
	{
		return true;
	}
	
    DWORD dwError = ::GetLastError();
    if (ERROR_ALREADY_EXISTS == dwError)
    {   
		::CloseHandle(hMutex);
        hMutex = NULL;
        return true;
    }
	
    return false;
}

DW_DUIFW_EXPORT bool DuiWinAPI::isMutexExist( const QString &mutexName )
{
	HANDLE hMutex = ::CreateMutex( NULL, FALSE, reinterpret_cast<LPCTSTR>(mutexName.utf16()) );

	DWORD dwError = ::GetLastError();
	if (ERROR_ALREADY_EXISTS == dwError || ERROR_ACCESS_DENIED == dwError)
	{
        if (hMutex)
        {
            ::CloseHandle(hMutex);
        }
		return true;
	}
    if (hMutex)
    {
        ::CloseHandle(hMutex);
    }

	return false;
}

DW_DUIFW_EXPORT UINT32 DuiWinAPI::createMutex( const QString &mutexName )
{
	HANDLE hMutex = ::CreateMutex( NULL, TRUE, reinterpret_cast<LPCTSTR>(mutexName.utf16()) );
	return reinterpret_cast<UINT32>(hMutex);
}

DW_DUIFW_EXPORT void DuiWinAPI::releaseMutex( UINT32 mutex )
{
	::CloseHandle(reinterpret_cast<HANDLE>(mutex));
}

DW_DUIFW_EXPORT void DuiWinAPI::setForegroundWindow( HWND hWnd )
{
	::SetForegroundWindow(hWnd);
}

DW_DUIFW_EXPORT QRect DuiWinAPI::getWorkArea()
{
	RECT rectWorkArea = {0};
	QRect workArea;
	if(!SystemParametersInfo(SPI_GETWORKAREA,sizeof(RECT),&rectWorkArea,0))
	{
		rectWorkArea.right = ::GetSystemMetrics(SM_CXSCREEN);
		rectWorkArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	}
	workArea.setLeft(rectWorkArea.left);
	workArea.setTop(rectWorkArea.top);
	workArea.setRight(rectWorkArea.right);
	workArea.setBottom(rectWorkArea.bottom);
	return workArea;
}

DW_DUIFW_EXPORT HWND DuiWinAPI::windowFromPoint( const QPoint& point )
{
	POINT pt;
	pt.x = point.x();
	pt.y = point.y();
	return ::WindowFromPoint(pt);
}

DW_DUIFW_EXPORT HWND DuiWinAPI::getDesktopWindow()
{
	return ::GetDesktopWindow();
}

DW_DUIFW_EXPORT QList<UINT32> DuiWinAPI::getWindowList( LPCWSTR className, LPCWSTR windowName, UINT32 ignoreId )
{
	QList<UINT32> windowList;
	HWND hWnd = NULL;
	while(hWnd = ::FindWindowExW(NULL, hWnd, className, windowName))
	{
		if (hWnd != (HWND)ignoreId)
		{
			windowList.push_back((UINT32)hWnd);
		}	
	}
	return windowList;
}

DW_DUIFW_EXPORT void DuiWinAPI::broadcastWindowMessage( const QList<UINT32> &windowList, UINT32 message, UINT32 wParam, UINT32 lParam )
{
	foreach(UINT32 windowId, windowList)
	{
		::PostMessage((HWND)windowId, message, (WPARAM)wParam, (LPARAM)lParam);
	}
}

DW_DUIFW_EXPORT bool DuiWinAPI::openFolderWithSelectFile( const QString &fileAbsolutePath )
{
	QFileInfo fileInfo(fileAbsolutePath);
	QString realParameter;
	if (fileInfo.exists())
	{
		realParameter = QString(" /select,/n,\"%1\"").arg(QDir::toNativeSeparators(fileAbsolutePath));
	}
	else
	{
		realParameter = QString("\"%1\"").arg(QDir::toNativeSeparators(fileInfo.dir().path()));
	}

	SHELLEXECUTEINFO exeInfo = {0};
	exeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	exeInfo.fMask = SEE_MASK_NOZONECHECKS | SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC;
	exeInfo.hwnd = NULL;
	exeInfo.lpVerb = L"open";
	exeInfo.lpFile = L"explorer.exe";
	exeInfo.lpParameters = reinterpret_cast<LPCTSTR>(realParameter.utf16());
	exeInfo.nShow = SW_SHOWDEFAULT;
	return ShellExecuteEx(&exeInfo);
}

DW_DUIFW_EXPORT void DuiWinAPI::sendMessage( UINT32 windowId, UINT32 message, UINT32 wParam, UINT32 lParam )
{
	::SendMessage((HWND)windowId, message, (WPARAM)wParam, (LPARAM)lParam);
}

DW_DUIFW_EXPORT void DuiWinAPI::addWindowStyle( UINT32 windowId, UINT32 addStyle )
{
	LONG style = ::GetWindowLong((HWND)windowId, GWL_STYLE);
	style |= addStyle;
	::SetWindowLong((HWND)windowId, GWL_STYLE, style);
}

DW_DUIFW_EXPORT void DuiWinAPI::removeWindowStyle( UINT32 windowId, UINT32 removeStyle )
{
	LONG style = ::GetWindowLong((HWND)windowId, GWL_STYLE);
	removeStyle = style & removeStyle;
	style ^= removeStyle;
	::SetWindowLong((HWND)windowId, GWL_STYLE, style);
}

DW_DUIFW_EXPORT UINT32 DuiWinAPI::mapVirtualKey( quint32 code, quint32 mapType )
{
	return ::MapVirtualKey(code, mapType);
}

DW_DUIFW_EXPORT QString DuiWinAPI::getKeyNameText( quint32 code )
{
	wchar_t buffer[256] = {0};
	QString strKey;
	int ret = ::GetKeyNameText(code, buffer, 256);
	if(ret == 0) {
		strKey = ("Unknown");	
	} else {
		strKey = QStringFromWCharArray(buffer);
	}

	return strKey;
}

DW_DUIFW_EXPORT HWND DuiWinAPI::getForegroundWindow()
{
	return ::GetForegroundWindow();
}

DW_DUIFW_EXPORT UINT32 DuiWinAPI::immAssociateContext( UINT32 windowId, UINT32 imcId )
{
	return (UINT32)::ImmAssociateContext((HWND)windowId, (HIMC)imcId);
}

DW_DUIFW_EXPORT DWORD DuiWinAPI::openFile( const QString &fileAbsolutePath )
{
	QString realFile = QDir::toNativeSeparators(fileAbsolutePath);
	SHELLEXECUTEINFO exeInfo = {0};
	exeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	exeInfo.fMask = SEE_MASK_NOZONECHECKS | SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC;
	exeInfo.hwnd = NULL;
	exeInfo.lpVerb = L"open";
	exeInfo.lpFile = reinterpret_cast<LPCTSTR>(realFile.utf16());
	exeInfo.nShow = SW_SHOWDEFAULT;
	if(ShellExecuteEx(&exeInfo))
	{
		return ERROR_SUCCESS;
	}
	else
	{
		return GetLastError();
	}
}

DW_DUIFW_EXPORT bool DuiWinAPI::isCapsLockOn()
{
	SHORT state = GetKeyState(VK_CAPITAL);
	return state & 0x1;
}

BOOL CALLBACK EnumThreadWndProc( HWND hwnd, LPARAM lParam )
{
	Q_UNUSED(lParam);
	wchar_t buffer[MAX_PATH] = {0};
	::GetClassName(hwnd, buffer, MAX_PATH-1);
	QString className = QStringFromWCharArray(buffer);
	if (className.startsWith("#32770", Qt::CaseInsensitive))
	{
		EndDialog(hwnd, 0);
	}
	return TRUE;
}

DW_DUIFW_EXPORT void DuiWinAPI::closeAllSystemDialog()
{
	::EnumThreadWindows(::GetCurrentThreadId(), EnumThreadWndProc, NULL );
}

DW_DUIFW_EXPORT void DuiWinAPI::customizeUIPIPolicy( HWND hwnd, quint32 message, DWORD dwFlag )
{
	typedef BOOL (WINAPI *PtrChangeWindowMessageFilter)(UINT message, DWORD dwFlag);
	typedef BOOL (WINAPI *PtrChangeWindowMessageFilterEx)(HWND hWnd, UINT message, DWORD action, void* pChangeFilterStruct);

	PtrChangeWindowMessageFilterEx pChangeWindowMessageFilterEx =
		(PtrChangeWindowMessageFilterEx)QLibrary::resolve(QLatin1String("user32"), "ChangeWindowMessageFilterEx");

	// win7
	if (pChangeWindowMessageFilterEx != NULL) 
	{
		// Call the safer ChangeWindowMessageFilterEx API if available
		pChangeWindowMessageFilterEx(hwnd, message, dwFlag, 0);
	} 
	else 
	{
		// vista
		PtrChangeWindowMessageFilter pChangeWindowMessageFilter =
			(PtrChangeWindowMessageFilter)QLibrary::resolve(QLatin1String("user32"), "ChangeWindowMessageFilter");

		if (pChangeWindowMessageFilter) 
		{
			// Call the deprecated ChangeWindowMessageFilter API otherwise
			pChangeWindowMessageFilter(message, dwFlag);
		}
	}
}

bool DuiWinAPI::IsMaximized( HWND hWnd )
{
	return ::IsZoomed(hWnd); 
}

bool DuiWinAPI::IsMinimized( HWND hWnd )
{
	return ::IsIconic(hWnd);
}

class DwAsyncTask_shellNotifyIcon : public IAsyncTaskBase
{
public:
	DwAsyncTask_shellNotifyIcon(DWORD dwMessage, PNOTIFYICONDATAW lpData, int retryTimes, QObject *receiver, const char *member)
		: m_message(dwMessage)
		, m_notifyData(*lpData)
		, m_retryTimes(retryTimes)
        , m_callbackReceiver(receiver)
        , m_callbackMember(member)
        , m_succeed(false)
	{

	}

	virtual void run()
	{
        m_succeed = applyNotifyIcon();

        if(!m_succeed)
        {
            LogFinal(LOG::KDuiFw) <<"Tray icon failed, message: " << m_message << " icon: " << m_notifyData.hIcon << " tip: " << m_notifyData.szTip;
        }
	}

    virtual void finished()
    {
        if(m_callbackReceiver && !m_callbackMember.isEmpty())
        {
            QMetaObject::invokeMethod(m_callbackReceiver, m_callbackMember, Q_ARG(bool, m_succeed));
        }
    }

private:
	bool applyNotifyIcon()
	{
		int delay = 500;
		DWORD dwError = 0;
		for(int i = 0; i < m_retryTimes; i++)
		{
			if(Shell_NotifyIconW(m_message, &m_notifyData))
			{	
				//防止日志刷太多，异常才打印
				if(m_notifyData.cbSize <= 0 || m_notifyData.hWnd == NULL || m_notifyData.hIcon == NULL)
				{
					LogFinal(LOG::KDuiFw)<<"cbSize = "<<m_notifyData.cbSize <<", hWnd = "<< m_notifyData.hWnd
						<<", uId = "<< m_notifyData.uID <<", uFlags =" << m_notifyData.uFlags <<", uCallbackMessage = "
						<< m_notifyData.uCallbackMessage <<", hIcon = " << m_notifyData.hIcon <<", dwState = " <<m_notifyData.dwState
						<< ", dwInfoFlags = "<<m_notifyData.dwInfoFlags << ", tip = " << m_notifyData.szTip;
				}
				return true;
			}

			dwError = GetLastError();
			LogFinal(LOG::KDuiFw) << "set icon error: tick = " << ::GetTickCount();

			//retry later
			Sleep(delay);
		}

		LogFinal(LOG::KDuiFw) << "Fail to set tray icon error = " << dwError 
			<< L", retryTime = " << m_retryTimes
			<< L", message = " << m_message
			<<L", cbSize = "<<m_notifyData.cbSize
			<< L", hIcon = " << m_notifyData.hIcon
			<<L", hWnd = "<<m_notifyData.hWnd
			<<L", uID = "<<m_notifyData.uID
			<< L", tip = " << m_notifyData.szTip;				

		return false;
	}

private:
	int m_retryTimes;
	DWORD m_message;
	NOTIFYICONDATAW m_notifyData;
    QPointer<QObject> m_callbackReceiver;
    QByteArray m_callbackMember;
    bool m_succeed;
};

bool DuiWinAPI::Shell_NotifyIcon( DWORD dwMessage, PNOTIFYICONDATAW lpData )
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
	{
		return ::Shell_NotifyIconW(dwMessage, lpData);
	}

	DuiApplication *duiApp = qobject_cast<DuiApplication*>(qApp);
	duiApp->appendThreadTask(new DwAsyncTask_shellNotifyIcon(dwMessage, lpData, 10, NULL, NULL));
	return true;
}

bool DuiWinAPI::Shell_NotifyIcon_async( DWORD dwMessage, PNOTIFYICONDATAW lpData, QObject *receiver, const char *member)
{
    if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
    {
        bool succeed = ::Shell_NotifyIconW(dwMessage, lpData);
        if(receiver && member)
        {
            QMetaObject::invokeMethod(receiver, member, Qt::QueuedConnection, Q_ARG(bool, succeed));
        }

        return succeed;
    }

    DuiApplication *duiApp = qobject_cast<DuiApplication*>(qApp);
    duiApp->appendThreadTask(new DwAsyncTask_shellNotifyIcon(dwMessage, lpData, 1, receiver, member));
    return true;
}

HRESULT dwSHGetPropertyStoreForWindow(HWND hwnd,
									  REFIID riid,
									  void **ppv)
{
	funcSHGetPropertyStoreForWindow func = (funcSHGetPropertyStoreForWindow)GetProcAddress(GetModuleHandle(L"Shell32.dll"), "SHGetPropertyStoreForWindow");
	if(func)
	{
		return func(hwnd, riid, ppv);
	}
	return E_FAIL;
}

HRESULT dwInitPropVariantFromStringVector(
	PCWSTR *prgsz,
	ULONG cElems,
	PROPVARIANT *ppropvar
	)
{
	funcInitPropVariantFromStringVector func = (funcInitPropVariantFromStringVector)GetProcAddress(GetModuleHandle(L"propsys.dll"), "InitPropVariantFromStringVector");
	if(func)
	{
		return func(prgsz, cElems, ppropvar);
	}
	return E_FAIL;
}

HRESULT dwIPropertyStoreSetValue(IPropertyStore *pps,
								REFPROPERTYKEY pkey, PCWSTR pszValue)
{
	PROPVARIANT var;
	HRESULT hr = dwInitPropVariantFromStringVector(&pszValue, 1, &var);
	if (SUCCEEDED(hr))
	{
		hr = pps->SetValue(pkey, var);
		PropVariantClear(&var);
	}
	return hr;
}

bool DuiWinAPI::setWindowAppUserModelId( HWND hwnd, LPCWSTR lpszAppUserModelId )
{
	if(QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
	{
		//windows 7 and later only
		return false;
	}

	IPropertyStore *pps;
	HRESULT hr = dwSHGetPropertyStoreForWindow(hwnd, IID_PPV_ARGS(&pps));
	if (SUCCEEDED(hr)) 
	{
		dwIPropertyStoreSetValue(pps, dwPKEY_AppUserModel_ID, lpszAppUserModelId);
		pps->Release();
		return true;
	}

	return false;
}

void DuiWinAPI::removeDropShadowStyle( HWND hWnd )
{
	DWORD styles = GetClassLong(hWnd, GCL_STYLE);
	styles &= ~CS_DROPSHADOW;
	SetClassLong(hWnd, GCL_STYLE, styles);
}
