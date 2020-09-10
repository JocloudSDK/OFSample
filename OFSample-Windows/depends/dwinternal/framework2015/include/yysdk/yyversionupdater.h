#pragma once

// RQ3305, ���Ӿ�Ĭ����ʾ������֪ͨ��ʽ
typedef enum
{
	NOTIFY_REMIND = 0,	// ����4.17֮ǰ�汾
	NOTIFY_SILENT = 1,	// ����4.17֮ǰ�汾
	NOTIFY_COMPLEX = 2,	// ���Ͼ�Ĭ����ʾ֪ͨ
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
	  * @brief ��������
	  * @param launcherPath ���������֣�����ڵ�ǰexeĿ¼
	  * @param shellName Ӧ��exe���֣�����Ϊ��ǰexe������exe�п��ܱ��û��������ṩ��ѡ�
	  */
	bool launchUpdate(const wchar_t *launcherPath, const wchar_t *shellName = NULL, const wchar_t *arguments = NULL);

private:
	YYVersionUpdaterPrivate *d_ptr;
};
