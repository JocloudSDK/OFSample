#include "componentsdkversion.h"

namespace
{
	unsigned int KMajorVersion = 5;
	unsigned int KMinorVersion = 54;
	unsigned int KPatchVersion = 0;	//8.54.0.0°æ±¾
}

unsigned int componentSdkVersion()
{
	return (KMajorVersion << 16) + (KMinorVersion << 8) + KPatchVersion;
};

unsigned int majorVersion()
{
	return KMajorVersion;
};

unsigned int minorVersion()
{
	return KMinorVersion;
};

unsigned int patchVersion()
{
	return KPatchVersion;
};

std::wstring componentSdkVersionString()
{
	TCHAR buf[MAX_PATH] = {0};
	_stprintf_s(buf, MAX_PATH-1, L"%u", componentSdkVersion());

	return std::wstring(buf);
};

std::wstring componentSdkVersionStringHex()
{
	TCHAR buf[MAX_PATH] = {0};
	_stprintf_s(buf, MAX_PATH-1, L"0x%x", componentSdkVersion());

	return std::wstring(buf);
};
