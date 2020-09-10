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
	TASK_ERROR_NOIMPL,				//����İ�װ����ģʽָ��TASK_INSTALL_NONEʱ�����ش˴���.
	TASK_ERROR_SERVICE,				//��û���ã����ù�.
	TASK_ERROR_SERVICE_BREAK, //�����е�������ֹʱ�����ش˴�����.һ���ǿ���ipc�������ipc�Ͽ�����ipc���������������£��ų��ִ������
	TASK_ERROR_SERVICE_BREAK_WHILE_PEEDING, //����ȴ�������ʱ�����ش˴���һ���ǿ���ipc�������ipc�Ͽ�����ipc���������������£��ų��ִ������
	TASK_ERROR_SERVICE_BREAK_DLLNOFOUND,//dwupdate.dll����ʧ�ܣ����߻�ȡ"createUpdateTask"������ַʧ��.
	TASK_ERROR_DOWNLOAD_URL		= 1001,//���ص�ַ��Ч.
	TASK_ERROR_DOWNLOAD_CONNECT,			//��http�Ựʧ�ܣ���������http������ʧ�ܣ�����http head����ʧ���ҷ��ص�http�����벻��400��500֮�䣬���糣������502,�������Ҫ����ӡ����־.
	TASK_ERROR_DOWNLOAD_NOFOUND,			//http head����ʧ���ҷ��ص�http��������400��500֮��,һ�������http 404����.
	TASK_ERROR_DOWNLOAD_BREAK,				//http��������������ֹ.
	TASK_ERROR_DWONLOAD_WRITEILE,			//�������ر����ļ�����д�ļ�������ʧ��.
	TASK_ERROR_DOWNLOAD_FILEINFO,			//��ȡ���ضϵ��������ļ���Ϣ���������������.
	TASK_ERROR_DOWNLOAD_READ,					//����http�ļ����ݳ���(InternetReadFile).
	TASK_ERROR_DOWNLOAD_REQUEST,			//get����head����ʧ��.һ����dns������.����http����ʱ.
	TASK_ERROR_DOWNLOAD_NOSPACE,			//�������ص��ļ�ʱ�����̿ռ�С��3�����ذ���С.
	TASK_ERROR_DOWNLOAD_NOHEADER,		//ʹ��head�����ȡ���������ļ��Ĵ�С.
	TASK_ERROR_VERIFYING		= 2001,				//md5����ʧ��.
	TASK_ERROR_INSTALLING		= 3001,				//��װʧ��.
	TASK_ERROR_INSTALL_CREATEFOLDER,			//��װʱ������Ŀ��CACHEĿ¼ʧ��.cacheĿ¼����˵��%appdata%\duowan\yy\yycomstore\2052\com.yy.gamenotify\66311.cache
	TASK_ERROR_INSTALL_BACKUP,					//����Ŀ��Ŀ¼ʧ��.
	TASK_ERROR_INSTALL_CUSTOMSTEP,			//ִ���Զ��岽��ʧ��.��Щ�����װ��ɺ󣬻�Ӱ�װĿ¼����Microsoft.VC90.CRT�����Ŀ¼������ִ��������������Ϣ.
	TASK_ERROR_INSTALL_COMPLETE,				//��װʱ����CACHEĿ¼���Ƶ�Ŀ��Ŀ¼ʧ��.�Ƚ�˵yycomstore\2052\com.yy.gamenotify\66311.cache��yycomstore\2052\com.yy.gamenotify\66311.
	TASK_ERROR_INSTALL_SRCNOTEXIST,			//��װʱ�����صİ�װ���Ҳ�������������.
	TASK_ERROR_INSTALL_NOSPACE,					//��װʱ�����̿ռ�С��3����װ����С.
	TASK_ERROR_INSTALL_BSPATCHBEGIN = 3100, // 31xxΪ������װ��������xxΪ������װ����ϸ�����룬��ϸ��鿴depends\dwinternal\bpatch\include\typdefs.h.
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

// ���ز���
typedef struct _DOWNLOAD_TASK_PARAMS {
	_DOWNLOAD_TASK_PARAMS() : priorty(TASK_PRIORITY_NORMAL)
		, timeoutSeconds(0)
		, verifyMode(TASK_VERIFY_NONE) {
	}

	UPDATE_TASK_PRIORITY priorty;//���ȼ�
	UINT timeoutSeconds;//��ʱ���룩
	std::wstring remoteFilePath;//URL
	UPDATE_TASK_VERIFY_MODE verifyMode;
	std::string verifyMd5;
} DOWNLOAD_TASK_PARAMS;

// ��װ·��
//**** installPath  ****
// ��װ����
//**** installParam  ****
// �Զ��岽��
//**** customStep  ****
// 1������ָ��Ŀ¼�µ�ĳЩ�ļ�
//    PRECOPY|SRCPATH>DESTPATH|
//            SRCPATH��DESTPATH������Ŀ¼�����ԡ�/����\����β����Ҳ�������ļ���
//    ʾ����PRECOPY|C:\Program Files\duowan\yy-3.9\Microsoft.VC90.CRT\>C:\temp\Microsoft.VC90.CRT\|
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