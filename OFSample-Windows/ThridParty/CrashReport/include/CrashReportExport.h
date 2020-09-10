/** 
@file
@brief CrashReportͷ�ļ�
@version 2011-7-10 bobdeng
*/


#pragma once

extern "C"
{
	//��ʼ��Bugreport
	_declspec(dllexport) BOOL InitBugReport(LPCTSTR lpszProductId, LPCTSTR lpszProductVer, LPCTSTR lpszProductName);

	//�����û���ʶ��YY���룩
	_declspec(dllexport) void SetUserId(const wchar_t *lpszUserId);

	//���ð汾��Ϣ
	_declspec(dllexport) void SetProductVer(LPCTSTR lpszProductVer);

	//���ò�Ʒ���ƺ�id ��lpszProductId����ʶ���Ʒ��
	_declspec(dllexport) void SetProductNameAndId(LPCTSTR lpszProductName, LPCTSTR lpszProductId);

	//���ð汾��ϸ������Ϣ
	_declspec(dllexport) void SetProductVerDetail(LPCTSTR lpszVerDetail);

	//���ñ�����������·��
	_declspec(dllexport) void SetRestartExePath(LPCTSTR lpszPath);

	//����Զ����ϱ��ļ�
	_declspec(dllexport) void AddCustomReportFile(LPCTSTR lpszPath);

	//�����Զ�������,����Ƶ��ID���������ϱ���Ϣ�У�
	_declspec(dllexport) void SetCustomProperty(const wchar_t* lpszKey, const wchar_t* lpszValue);
	_declspec(dllexport) BOOL GetCustomProperty(const wchar_t* lpszKey, wchar_t* lpszValue, int cch);

	//�Ƿ��������ʾ�ϱ�����
	_declspec(dllexport) void SetShowReportUI(BOOL bShow);

	//֧�ֶ�̬�����İ汾
	_declspec(dllexport) void AddDynaReleaseVer(LPCTSTR lpszDynaVer);
	_declspec(dllexport) void RemoveDynaReleaseVer(LPCTSTR lpszDynaVer);

	_declspec(dllexport) BOOL SetCustomData(int index, const void* lpData, int cchData);

    _declspec(dllexport) void SetCatchPureCall(BOOL setting);

    // ��ȡͨ��InitBugReport��SetProductNameAndId���õ�lpszProductId
    _declspec(dllexport) LPCSTR GetProductId();

    // ��ȡͨ��InitBugReport��SetProductNameAndId���õ�lpszProductVer
    _declspec(dllexport) LPCSTR GetProductVer();

    // ��ȡͨ��InitBugReport��SetProductNameAndId���õ�lpszProductName
    _declspec(dllexport) LPCSTR GetProductName();

}
