#pragma once

// RQ3305, 增加静默和提示复合型通知方式
typedef enum
{
	NOTIFY_REMIND = 0,	// 兼容4.17之前版本
	NOTIFY_SILENT = 1,	// 兼容4.17之前版本
	NOTIFY_COMPLEX = 2,	// 复合静默和提示通知
} NOTIFY_MODE;

class YYVersionUpdaterPrivate;
class YYVersionUpdater
{
public:
	YYVersionUpdater(int argc, char *argv[]);
	~YYVersionUpdater(void);

	bool isSlienceUpdate() const;
	bool isDownloadCompleted() const;

	/**
	  * @brief 启动升级
	  * @param launcherPath 升级器名字，相对于当前exe目录
	  * @param shellName 应用exe名字，空则为当前exe。鉴于exe有可能被用户改名，提供此选项。
	  */
	bool launchUpdate(const wchar_t *launcherPath, const wchar_t *shellName = NULL, const wchar_t *arguments = NULL);

private:
	YYVersionUpdaterPrivate *d_ptr;
};
