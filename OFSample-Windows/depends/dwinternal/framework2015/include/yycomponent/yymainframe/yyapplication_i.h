#pragma once

#ifndef DW_COMPONENT_ENV

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief ��¼�ӿڣ�coCreateComInstance(KSERVICEYYApplication)������ȫ��Ψһ
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
	  * @brief ��ʼ��Application�����ڴ���DuiApplicationʵ������ã�Ĭ���������й���
	  */
	virtual int init() = 0;

	/**
	  * @brief ��ʼ��Application�����ڴ���DuiApplicationʵ�������
	  * @param flags ��ʹ��InitFlag���
	  */
	virtual int initWithFlag(int flags) = 0;

	/**
	  * @brief ��������
	  */
	virtual void restart() = 0;

	/**
	  * @brief �����������½��̲����ϴ���reason
	  * @param reason ���Ӳ���
	  */
	virtual void restartWithReason(int reason) = 0;

	/**
	  * @brief ���ò�Ʒ��������yyrun��ȡ����Ϣʱ����ͬ��Ʒ
	  * @param name ��Ʒ��
	  */
	virtual void setApplicationName(const wchar_t *name) = 0;

	/**
	  * @brief ���ƽ�������ʹ�ô���key��Ϊ����GUID����
	  * @param key ����GUID���ӣ����鴫�������������\0��β
	  * @param limit ����������
	  * @return UINT32 �������ﵽ���޷���0�����򷵻ػ�����HANDLE
	  */
	virtual UINT32 instanceLimit(const char *key, int limit) = 0;

	virtual void exit(int returnCode) = 0;

	virtual bool isRestarted() = 0;

	// ����Ǵ������в�����������
	virtual bool isAutoLoginDisabled() = 0;

	virtual int getSessionToken() = 0;
	virtual void addCrashProperty( const QString &key, const QString &value ) = 0;

	virtual void doRestart(int *reason) = 0;
	virtual bool needRestart() = 0;

	virtual bool loadService(const QString &serviceName) = 0;
	virtual QString currentYYUserAppPath() = 0;

	/**
	  * @brief �����������ǰ���̵�ԭ������һ�������ĳ����Զ���
	  * @return int �ϴ�������ԭ��û�д����򷵻�REASON_Startup
	  */
	virtual int restartReason() = 0;

    /**
      * @brief �Ƿ������˳�
      * @return bool true��ʾ�Ѿ�����exit
      */
    virtual bool isExiting() = 0;
};
#endif // DW_COMPONENT_ENV
