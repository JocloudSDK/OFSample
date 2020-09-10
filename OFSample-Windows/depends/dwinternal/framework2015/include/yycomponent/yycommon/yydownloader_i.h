#pragma once
#include "yycomponent/yycomclsid.h"

#pragma pack(push, 1)

typedef enum  {
	TASK_PRIORITY_FOREGROUND,
	TASK_PRIORITY_HIGH,
	TASK_PRIORITY_NORMAL,
	TASK_PRIORITY_LOW 
} UPDATE_TASK_PRIORITY;

typedef enum {
	TASK_PENDING,
	TASK_DOWNLOADING,
	TASK_VERIFYING,
	TASK_INSTALLING,
	TASK_COMPLEDTED,
} UPDATE_TASK_STAGE;

typedef enum {
	TASK_ERROR_NOERROR			= 0,
	TASK_ERROR_NOIMPL,				//传入的安装参数模式指定TASK_INSTALL_NONE时，返回此错误.
	TASK_ERROR_SERVICE,				//还没有用，不用管.
	TASK_ERROR_SERVICE_BREAK, //下载中的任务被中止时，返回此错误码.一般是开启ipc服务出错，ipc断开或者ipc的其他错误的情况下，才出现此情况。
	TASK_ERROR_SERVICE_BREAK_WHILE_PEEDING, //清理等待的任务时，返回此错误。一般是开启ipc服务出错，ipc断开或者ipc的其他错误的情况下，才出现此情况。
	TASK_ERROR_SERVICE_BREAK_DLLNOFOUND,//dwupdate.dll加载失败，或者获取"createUpdateTask"函数地址失败.
	TASK_ERROR_DOWNLOAD_URL		= 1001,//下载地址无效.
	TASK_ERROR_DOWNLOAD_CONNECT,			//打开http会话失败，或者连接http服务器失败，或者http head请求失败且返回的http错误码不在400到500之间，比如常见的是502,具体错误还要看打印的日志.
	TASK_ERROR_DOWNLOAD_NOFOUND,			//http head请求失败且返回的http错误码在400到500之间,一般情况是http 404错误.
	TASK_ERROR_DOWNLOAD_BREAK,				//http错误下载任务中止.
	TASK_ERROR_DWONLOAD_WRITEILE,			//创建下载保存文件或者写文件过程中失败.
	TASK_ERROR_DOWNLOAD_FILEINFO,			//获取下载断点续传的文件信息的相关数据有问题.
	TASK_ERROR_DOWNLOAD_READ,					//下载http文件数据出错(InternetReadFile).
	TASK_ERROR_DOWNLOAD_REQUEST,			//get或者head请求失败.一般是dns有问题.或者http请求超时.
	TASK_ERROR_DOWNLOAD_NOSPACE,			//保存下载的文件时，磁盘空间小于3倍下载包大小.
	TASK_ERROR_DOWNLOAD_NOHEADER,		//使用head请求获取不到下载文件的大小.
	TASK_ERROR_VERIFYING		= 2001,				//md5检验失败.
	TASK_ERROR_INSTALLING		= 3001,				//安装失败.
	TASK_ERROR_INSTALL_CREATEFOLDER,			//安装时，创建目标CACHE目录失败.cache目录比如说：%appdata%\duowan\yy\yycomstore\2052\com.yy.gamenotify\66311.cache
	TASK_ERROR_INSTALL_BACKUP,					//备份目标目录失败.
	TASK_ERROR_INSTALL_CUSTOMSTEP,			//执行自定义步骤失败.有些组件安装完成后，会从安装目录复制Microsoft.VC90.CRT到组件目录，或者执行其他命令行信息.
	TASK_ERROR_INSTALL_COMPLETE,				//安装时，从CACHE目录复制到目标目录失败.比较说yycomstore\2052\com.yy.gamenotify\66311.cache到yycomstore\2052\com.yy.gamenotify\66311.
	TASK_ERROR_INSTALL_SRCNOTEXIST,			//安装时，下载的安装包找不到，或者损坏了.
	TASK_ERROR_INSTALL_NOSPACE,					//安装时，磁盘空间小于3倍安装包大小.
	TASK_ERROR_INSTALL_BSPATCHBEGIN = 3100, // 31xx为补丁安装错误，其中xx为补丁安装的详细错误码，详细请查看depends\dwinternal\bpatch\include\typdefs.h.
} UPDATE_TASK_ERROR;

typedef enum {
	TASK_VERIFY_NONE,
	TASK_VERIFY_7Z,
	TASK_VERIFY_MD5,
} UPDATE_TASK_VERIFY_MODE;

#define TASK_INSTALL_CLEAN_MODE 0x1
#define TASK_INSTALL_REBOOT_MODE 0x2
typedef enum {
	TASK_INSTALL_NONE					= 0,
	TASK_INSTALL_COPY					= 0x10,
	TASK_INSTALL_CLEAN_AND_COPY			= TASK_INSTALL_COPY + TASK_INSTALL_CLEAN_MODE,
	TASK_INSTALL_COPY_REBOOT_REPLACE	= TASK_INSTALL_COPY + TASK_INSTALL_REBOOT_MODE,
	TASK_INSTALL_UNPACK7Z				= 0x20,
	TASK_INSTALL_CLEAN_AND_UNPACK7Z		= TASK_INSTALL_UNPACK7Z + TASK_INSTALL_CLEAN_MODE,
	TASK_INSTALL_UNPACKZ_REBOOT_REPLACE = TASK_INSTALL_UNPACK7Z + TASK_INSTALL_REBOOT_MODE,
	TASK_INSTALL_SETUP					= 0x40,
	TASK_INSTALL_INCREMENT				= 0x80,
} UPDATE_TASK_INSTALL_MODE;

// 下载参数
typedef struct _DOWNLOAD_TASK_PARAMS {
	_DOWNLOAD_TASK_PARAMS() : priorty(TASK_PRIORITY_NORMAL)
		, timeoutSeconds(0)
		, verifyMode(TASK_VERIFY_NONE) {
	}

	UPDATE_TASK_PRIORITY priorty;//优先级
	UINT timeoutSeconds;//超时（秒）
	std::wstring remoteFilePath;//URL
	UPDATE_TASK_VERIFY_MODE verifyMode;
	std::string verifyMd5;
} DOWNLOAD_TASK_PARAMS;

// 安装路径
//**** installPath  ****
// 安装参数
//**** installParam  ****
// 自定义步骤
//**** customStep  ****
// 1、复制指定目录下的某些文件
//    PRECOPY|SRCPATH>DESTPATH|
//            SRCPATH和DESTPATH可以是目录名（以“/”或“\”结尾），也可以是文件名
//    示例：PRECOPY|C:\Program Files\duowan\yy-3.9\Microsoft.VC90.CRT\>C:\temp\Microsoft.VC90.CRT\|
typedef struct _INSTALL_TASK_PARAMS {
	_INSTALL_TASK_PARAMS() : installMode(TASK_INSTALL_NONE) {
	}

	UPDATE_TASK_INSTALL_MODE installMode;
	std::wstring installPath;
	std::wstring installParam;
	std::wstring customStep;
} INSTALL_TASK_PARAMS;

#pragma pack(pop)

typedef UINT TASKID;

/**
   * @brief YY downloader interface.
   */
DWDEFINE_INTERFACE(IYYDownloader):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void init(const DOWNLOAD_TASK_PARAMS &downParams, const INSTALL_TASK_PARAMS &instParams) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void start() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stop() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void pause() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool queryStatus(UPDATE_TASK_STAGE& stage, UPDATE_TASK_ERROR& error, ULONGLONG& downloaded,	ULONGLONG& length) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual TASKID getTaskId() = 0;

	typedef xsignals::signal<void (UPDATE_TASK_STAGE, UPDATE_TASK_ERROR, TASKID)> _SIG_TASK_ERROR;
	virtual _SIG_TASK_ERROR* getTaskErrorSignal() = 0;

	typedef xsignals::signal<void (UPDATE_TASK_STAGE, TASKID)> _SIG_TASK_COMPLETE;
	virtual _SIG_TASK_COMPLETE* getTaskCompleteSignal() = 0;
};