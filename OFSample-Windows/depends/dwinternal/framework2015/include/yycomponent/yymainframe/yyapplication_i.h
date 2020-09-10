#pragma once

#ifndef DW_COMPONENT_ENV

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief 登录接口，coCreateComInstance(KSERVICEYYApplication)创建，全局唯一
  */
DWDEFINE_INTERFACE(IYYApplication) : public IUnk
{
	typedef enum /*tagInitFlag*/
	{
		FLAG_QT_MEMPOOL = 1 << 0,
		FLAG_HOOK_SLOTS = 1 << 1,
		FLAG_QT_MSG_HANDLER = 1 << 2,
		FLAG_HEAP_INFOMATION = 1 << 3,
		FLAG_CRASH_REPORT = 1 << 4,
		FLAG_BLIND_HELPER = 1 << 5,
		FLAG_UI_AUTOTEST = 1 << 6,
		FLAG_COMPONENT_TEST = 1 << 7,
		FLAG_PERF_LOG = 1 << 8,
		FLAG_MASK = ~0,
	} InitFlag;

	enum /*tagReturnCode*/
	{
		CODE_RESTART = 1000,
	};

	enum
	{
		REASON_Unknown = 0,
		REASON_Startup = 2,
	};

	/**
	  * @brief 初始化Application，请在创建DuiApplication实例后调用，默认启用所有功能
	  */
	virtual int init() = 0;

	/**
	  * @brief 初始化Application，请在创建DuiApplication实例后调用
	  * @param flags 可使用InitFlag组合
	  */
	virtual int initWithFlag(int flags) = 0;

	/**
	  * @brief 重启程序
	  */
	virtual void restart() = 0;

	/**
	  * @brief 重启程序，在新进程参数上带上reason
	  * @param reason 附加参数
	  */
	virtual void restartWithReason(int reason) = 0;

	/**
	  * @brief 设置产品名，用于yyrun等取得信息时区别不同产品
	  * @param name 产品名
	  */
	virtual void setApplicationName(const wchar_t *name) = 0;

	/**
	  * @brief 限制进程数，使用传入key作为计算GUID种子
	  * @param key 计算GUID种子（建议传入程序名），以\0结尾
	  * @param limit 进程数限制
	  * @return UINT32 进程数达到上限返回0，否则返回互斥体HANDLE
	  */
	virtual UINT32 instanceLimit(const char *key, int limit) = 0;

	virtual void exit(int returnCode) = 0;

	virtual bool isRestarted() = 0;

	// 这个是从命令行参数传进来的
	virtual bool isAutoLoginDisabled() = 0;

	virtual int getSessionToken() = 0;
	virtual void addCrashProperty( const QString &key, const QString &value ) = 0;

	virtual void doRestart(int *reason) = 0;
	virtual bool needRestart() = 0;

	virtual bool loadService(const QString &serviceName) = 0;
	virtual QString currentYYUserAppPath() = 0;

	/**
	  * @brief 获得重启到当前进程的原因。由上一次重启的程序自定义
	  * @return int 上次重启的原因，没有传入则返回REASON_Startup
	  */
	virtual int restartReason() = 0;

    /**
      * @brief 是否正在退出
      * @return bool true表示已经调用exit
      */
    virtual bool isExiting() = 0;
};
#endif // DW_COMPONENT_ENV
