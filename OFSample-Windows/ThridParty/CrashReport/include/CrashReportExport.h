/** 
@file
@brief CrashReport头文件
@version 2011-7-10 bobdeng
*/


#pragma once

extern "C"
{
	//初始化Bugreport
	_declspec(dllexport) BOOL InitBugReport(LPCTSTR lpszProductId, LPCTSTR lpszProductVer, LPCTSTR lpszProductName);

	//设置用户标识（YY号码）
	_declspec(dllexport) void SetUserId(const wchar_t *lpszUserId);

	//设置版本信息
	_declspec(dllexport) void SetProductVer(LPCTSTR lpszProductVer);

	//设置产品名称和id （lpszProductId用于识别产品）
	_declspec(dllexport) void SetProductNameAndId(LPCTSTR lpszProductName, LPCTSTR lpszProductId);

	//设置版本详细描叙信息
	_declspec(dllexport) void SetProductVerDetail(LPCTSTR lpszVerDetail);

	//设置崩溃重启程序路径
	_declspec(dllexport) void SetRestartExePath(LPCTSTR lpszPath);

	//添加自定义上报文件
	_declspec(dllexport) void AddCustomReportFile(LPCTSTR lpszPath);

	//设置自定义属性,例如频道ID（包含在上报信息中）
	_declspec(dllexport) void SetCustomProperty(const wchar_t* lpszKey, const wchar_t* lpszValue);
	_declspec(dllexport) BOOL GetCustomProperty(const wchar_t* lpszKey, wchar_t* lpszValue, int cch);

	//是否崩溃后显示上报界面
	_declspec(dllexport) void SetShowReportUI(BOOL bShow);

	//支持动态发布的版本
	_declspec(dllexport) void AddDynaReleaseVer(LPCTSTR lpszDynaVer);
	_declspec(dllexport) void RemoveDynaReleaseVer(LPCTSTR lpszDynaVer);

	_declspec(dllexport) BOOL SetCustomData(int index, const void* lpData, int cchData);

    _declspec(dllexport) void SetCatchPureCall(BOOL setting);

    // 获取通过InitBugReport或SetProductNameAndId设置的lpszProductId
    _declspec(dllexport) LPCSTR GetProductId();

    // 获取通过InitBugReport或SetProductNameAndId设置的lpszProductVer
    _declspec(dllexport) LPCSTR GetProductVer();

    // 获取通过InitBugReport或SetProductNameAndId设置的lpszProductName
    _declspec(dllexport) LPCSTR GetProductName();

}
