#pragma once
#include <windows.h>
#include <string>
#include <set>

class CHackerDisableDlls
{
private:
	CHackerDisableDlls();

public:	
	virtual ~CHackerDisableDlls();	
	static CHackerDisableDlls *instance();
	void Hook();

protected:
	static HMODULE WINAPI NEW_LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
	static void initDisableDlls();
private:
	static CHackerDisableDlls *m_instance;
	static std::set<std::wstring> m_disabledlls;
	static std::wstring m_apppath;
};