#pragma once
#include "dwglobal.h"
#include <string>

/*
AppData目录整理策略：

假设有三个版本：A，B，C。A代表当前正在使用的YY版本，B代表第一期目录整理版本，
C代表第二期目录整理版本，那么三个版本当前的情况分析如下：

A：读写都是在旧有的yy4.0目录中进行
B：新（yy)旧（yy4.0）文件谁存在就读写谁，同时存在，就读写新文件
C：新（yy)旧（yy4.0）文件谁存在就读谁，同时存在，读新文件，写均在新文件，同时把旧文件移动到新目录

在B发布时存A与B同时存在的情况，这时候由于没有任何版本创建新（duowan\yy）目录，所有
读写均在旧目录（duowan\yy4.0）中进行；

这时候需要把所有A版本均升到B版本。这一步骤完成后外网用户只能使用B版本了，这就方便了后面一步；

C版本发布时与B版本同时存在，首次运行时读旧目录，同时把所读取的文件移动到新目录。这样B版本
在读取时会从新目录中读取，同时在新目录中写入。

等B版本全部升到C版本时，可以考虑使用组件删掉yy4.0目录了。

极端情况A，B，C版本同时存在，则使用A版本产生的数据有可能会丢失。

另外对于存在是文件级别的，而不是目录级别。因为一个目录中存在多个文件的情况下，其中
写一个文件就创建了目录，但其他文件在新目录中却不存在，这样的话就会导致数据丢失。

组件的情况：
对于组件,完美的做法是B版本发布的同时也发各组件的B版本，C版本发布时也一同发布组件的C版本；

但组件对数据可能没有YY要求那么严格，所以C版本发布以后只需要更新一次组件，在新目录中读写数据即可。

*/

//! Appdata文件夹路径统一封装
/*! 
 * \file appdatapath.h
 * \	%appdata%目录整理后，各业务有自己各自文件夹, %appdata%\\duwan\\yy\\下不得随意创建目录 

 └%appdata%
    └─duowan
        └─yy
			├─business					业务配置文件（各业务在这个目录下建自已的配置文件目录）
            ├─cache						缓存目录（共享业务）
			│   ├─download				下载文件夹
			│   └─temp					临时文件夹
            ├─channel						频道业务相关
            ├─imgroup						IM业务相关
			│   └─%UID%					IM用户目录
			├─log							日志（共享业务）
            ├─mainframe					主面板相关、登陆面板相关
			│   └─account				账号目录
			├─openplatform				开放平台相关
			│   └─addonsex				插件目录
		    ├─setup						安装目录
			│   └─%MD5(appRootPath)%		与安装路径有关的目录
		    ├─update						升级相关
			└─yycomstore					组件化

 * \	20121121
 */

// 这个封装在B版本中只取新路径，但不创建目录

// C版本中要创建目录
// 版本C：
//   如果新目录中存在文件就读写新目录中的文件；
//   如果旧目录中的文件存在，就把旧目录中的文件移到新目录中，读写新目录中的文件；
//   新旧目录中的文件都不存在，就在新目录中读写；
//   总之取到的都是新目录中的路径；

// 修改位置标记：
/**-- AppData整理: 新文件/目录存在就读写新文件/目录 --**/

namespace DwUtility
{
	class DW_DWUTILITY_EXPORT AppDataPath
	{
	public:
		static std::wstring GetBusinessPath();							// %appdata%\\duowan\\yy\\business\\ 

		static std::wstring GetCachePath();								// %appdata%\\duowan\\yy\\cache\\ 
		static std::wstring GetDownloadPath();							// %appdata%\\duowan\\yy\\cache\\download\\ 
		static std::wstring GetTempPath();								// %appdata%\\duowan\\yy\\cache\\temp\\ 

		static std::wstring GetChannelPath();							// %appdata%\\duowan\\yy\\channel\\ 
		static std::wstring GetChannelUserPath(unsigned long uid);		// %appdata%\\duowan\\yy\\channel\\%uid%\\ 

		static std::wstring GetImGroupPath();							// %appdata%\\duowan\\yy\\imgroup\\ 
		static std::wstring GetImGroupUserPath(unsigned long uid);		// %appdata%\\duowan\\yy\\imgroup\\%uid%\\ 

		static std::wstring GetLogPath();								// %appdata%\\duowan\\yy\\log\\ 

		static std::wstring GetMainframePath();							// %appdata%\\duowan\\yy\\mainframe\\ 

		static std::wstring GetOpenplatformPath();						// %appdata%\\duowan\\yy\\openplatform\\ 
		static std::wstring GetOpenplatformAddonsexPath();				// %appdata%\\duowan\\yy\\openplatform\\addonsex\\ 

		static std::wstring GetSetupPath();								// %appdata%\\duowan\\yy\\setup\\	
		static std::wstring GetSetupMd5Path(LPCWSTR appRootPath);		// %appdata%\\duowan\\yy\\setup\\%MD5(appRootPath)%\\

		static std::wstring GetUpdatePath();							// %appdata%\\duowan\\yy\\update\\ 

		static std::wstring GetYYComstorePath();						// %appdata%\\duowan\\yy\\yycomstore\\ 

		static bool			CreateDirectory(LPCWSTR lpszDir);
		static bool			IsFileExist(LPCWSTR lpszFile);
		static bool			IsDirExist(LPCWSTR lpszDir);
		static std::wstring TransformInstallPath2MD5(LPCWSTR lpszPath);

		static std::wstring applicationName();							// default to yy
		static void setApplicationName(const std::wstring &name);

	private:
		// 该方法慎重使用，各业务有自己各自文件夹, %appdata%\\duwan\\yy\\下不得随意创建目录
		static const std::wstring& GetUserAppDataRoot();

		static std::wstring     _getSubPath(LPCWSTR strSubDir, LPCWSTR strChildDir = NULL);
		static bool				_isFileExist(LPCWSTR lpszFile);
		static bool				_isDirExist(LPCWSTR lpszDir);
		static bool				_createDirectory(LPCWSTR lpszDir);
		static bool				_toWindowPath(std::wstring& output, LPCWSTR lpszFile);

		static std::wstring		_transformInstallPath2MD5(const std::wstring &strPath);
		static std::wstring		_formatPathSpan(LPCTSTR lpszPath);
	};
}