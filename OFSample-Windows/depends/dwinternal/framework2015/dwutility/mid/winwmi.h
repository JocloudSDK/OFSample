#ifndef __winwmi_h__
#define __winwmi_h__

#include <string>
#include <wbemidl.h>

class CWinWmi
{
public:
	CWinWmi();
	~CWinWmi();
	BOOL Init();
	void Uninit();
	std::wstring GetInfo(const std::wstring& name, const std::wstring& param);

	// 开线程取，同步的
	std::string GetInfo2(const wchar_t** wmiList);
	
private:
	IWbemLocator*  m_pLoc;
	IWbemServices* m_pSvc;
};

#endif // __winwmi_h__