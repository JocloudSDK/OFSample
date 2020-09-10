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
	  * @brief 启动自我修复，loadSdk收到RET_ENTRY_FILE_NOTFOUND，可调用此函数尝试修复
	  * @param launcherPath 升级器名字，相对于当前exe目录
	  * @param shellName 应用exe名字，空则为当前exe。鉴于exe有可能被用户改名，提供此选项。
	  */
	int selfRepair(const wchar_t *launcherPath);

private:
	YYVersionLoaderPrivate *d_ptr;
};
