#pragma once
#include <string>

bool isArugmentExists(int argc, char *argv[], char *option);
std::wstring appPath(void);

typedef int (*FUNC_MAIN_ENTRY)(int argc, char* argv[]);

struct YYVersionInfo;
class YYVersionLoaderPrivate;
class YYVersionLoader
{
public:
	enum
	{
		RET_SUCCESS = 0x00, 
		RET_UPDATER_LOADED = 0x01,
		RET_ENTRY_FILE_NOTFOUND = 0x11, 
		RET_ENTRY_LOAD_FAILED, 
		RET_SELFREPAIR_FAILED = 0x21, 
		RET_INVALID = 0xFF,
	};

	enum /*tagFlags*/
	{
		FLAG_NONE = 0,
		FLAG_NO_SLIENT_UPDATE = 1 << 0,
	};

	YYVersionLoader(int argc, char* argv[]);
	~YYVersionLoader(void);

	bool isSkipUpdate() const;
	std::wstring versionPath() const;
	int loadSdk(const YYVersionInfo *info = NULL);
	int loadEntry(wchar_t *dllName, char *entryName);

	/**
	  * @brief ���������޸���loadSdk�յ�RET_ENTRY_FILE_NOTFOUND���ɵ��ô˺��������޸�
	  * @param launcherPath ���������֣�����ڵ�ǰexeĿ¼
	  * @param shellName Ӧ��exe���֣�����Ϊ��ǰexe������exe�п��ܱ��û��������ṩ��ѡ�
	  */
	int selfRepair(const wchar_t *launcherPath);

private:
	YYVersionLoaderPrivate *d_ptr;
};
