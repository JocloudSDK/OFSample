
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#ifdef _WINDOWS

#include <windows.h>
#include <winuser.h>
#include <Uxtheme.h>
#include <Dwmapi.h>

#include <QLibrary>

#include "dwglobal.h"
class DuiWinAPI
{
public:
	static DW_DUIFW_EXPORT HRESULT ExtendIntoClientAll(HWND hwnd);
	static DW_DUIFW_EXPORT HRESULT UpdateBlurEffectRegion(HWND hwnd, HRGN hRgn);
	static DW_DUIFW_EXPORT HRESULT DisableNCRendering(HWND hwnd);
	static DW_DUIFW_EXPORT HRESULT AllowNCPaint(HWND hwnd);

	static DW_DUIFW_EXPORT bool IsCompositionEnabled();

	static DW_DUIFW_EXPORT bool ShowWindow(HWND hWnd, int nCmdShow);
	static DW_DUIFW_EXPORT bool IsMaximized(HWND hWnd);
	static DW_DUIFW_EXPORT bool IsMinimized(HWND hWnd);
	static DW_DUIFW_EXPORT bool SystemParametersInfo( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni );

	static DW_DUIFW_EXPORT bool SetWindowRgn( HWND hWnd, HRGN hRgn, bool redrawNow );

	static DW_DUIFW_EXPORT LONG SetWindowLong( 
		HWND hWnd, 
		int nIndex, 
		LONG dwNewLong
		);

	static DW_DUIFW_EXPORT BOOL setWindowPos(
		HWND hWnd,
		HWND hWndlnsertAfter,
		int X,
		int Y,
		int cx,
		int cy,
		UINT uFlags
		);

	static DW_DUIFW_EXPORT BOOL SetLayeredWindowAttributes(
		HWND hwnd,
		COLORREF crKey,
		BYTE bAlpha,
		DWORD dwFlags
		);

	static DW_DUIFW_EXPORT DWORD ProcessIdFromWindow(HWND hWnd);
	static DW_DUIFW_EXPORT QList<WId> GetClildWindow(HWND hWnd);
	static DW_DUIFW_EXPORT bool CreateProcess(const QString &exeName, const QString &cmdLine, quint32 createFlags, quint32& pId);
	static DW_DUIFW_EXPORT bool IsKeyPressed(int vKey);

	static DW_DUIFW_EXPORT int GlobalAddAtom( const QString &atomName );
	static DW_DUIFW_EXPORT void GlobalDeleteAtom( int atom );
	static DW_DUIFW_EXPORT bool RegisterHotKey( HWND hWnd, int id, UINT modifierKey, UINT virtualKey );
	static DW_DUIFW_EXPORT void UnregisterHotKey( HWND hWnd, int id );

	static DW_DUIFW_EXPORT bool Shell_NotifyIcon(DWORD dwMessage, PNOTIFYICONDATAW lpData);
    static DW_DUIFW_EXPORT bool Shell_NotifyIcon_async( DWORD dwMessage, PNOTIFYICONDATAW lpData, QObject *receiver, const char *member);
	enum SpecialFolderType
	{
		Folder_Person = 0,
		Folder_UserAppdata,
		Folder_MyMusic
	};
	static DW_DUIFW_EXPORT QString GetSpecialFolderPath( SpecialFolderType type );

	static DW_DUIFW_EXPORT QRect getTaskbarRect();

    static DW_DUIFW_EXPORT bool isMutexExist(const QString &mutexName, HANDLE &hMutex);

	static DW_DUIFW_EXPORT bool isMutexExist( const QString &mutexName );

	static DW_DUIFW_EXPORT UINT32 createMutex( const QString &mutexName );

	static DW_DUIFW_EXPORT void releaseMutex( UINT32 mutex );

	static DW_DUIFW_EXPORT void setForegroundWindow( HWND hWnd );

	static DW_DUIFW_EXPORT QRect getWorkArea();

	static DW_DUIFW_EXPORT HWND windowFromPoint(const QPoint& point);

	static DW_DUIFW_EXPORT HWND getDesktopWindow();

	static DW_DUIFW_EXPORT QList<UINT32> getWindowList( LPCWSTR className, LPCWSTR windowName, UINT32 ignoreId );

	static DW_DUIFW_EXPORT void broadcastWindowMessage( const QList<UINT32> &windowList, UINT32 message, UINT32 wParam, UINT32 lParam );

	static DW_DUIFW_EXPORT bool openFolderWithSelectFile( const QString &fileAbsolutePath );

	static DW_DUIFW_EXPORT void sendMessage( UINT32 windowId, UINT32 message, UINT32 wParam, UINT32 lParam );

	static DW_DUIFW_EXPORT void addWindowStyle( UINT32 windowId, UINT32 addStyle );

	static DW_DUIFW_EXPORT void removeWindowStyle( UINT32 windowId, UINT32 removeStyle );

	static DW_DUIFW_EXPORT UINT32 mapVirtualKey(quint32 code, quint32 mapType);

	static DW_DUIFW_EXPORT QString getKeyNameText(quint32 code);

	static DW_DUIFW_EXPORT HWND getForegroundWindow();

	static DW_DUIFW_EXPORT UINT32 immAssociateContext( UINT32 windowId, UINT32 imcId );

	static DW_DUIFW_EXPORT DWORD openFile(const QString &fileAbsolutePath );

	static DW_DUIFW_EXPORT bool isCapsLockOn();

	static DW_DUIFW_EXPORT void closeAllSystemDialog();

	// 获取窗口所在屏幕的work area
	static DW_DUIFW_EXPORT bool getCurrentWindowWorkRect( HWND hwnd, RECT& workRect );

	// 定制UIPI（用户界面特权隔离）规则
	static DW_DUIFW_EXPORT void customizeUIPIPolicy(HWND hwnd, quint32 message, DWORD dwFlag);

	static DW_DUIFW_EXPORT bool setWindowAppUserModelId(HWND hwnd, LPCWSTR lpszAppUserModelId);

	static void removeDropShadowStyle(HWND hWnd);
};

#endif