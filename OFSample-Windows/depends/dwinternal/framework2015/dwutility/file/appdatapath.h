#pragma once
#include "dwglobal.h"
#include <string>

/*
AppDataĿ¼������ԣ�

�����������汾��A��B��C��A����ǰ����ʹ�õ�YY�汾��B�����һ��Ŀ¼����汾��
C����ڶ���Ŀ¼����汾����ô�����汾��ǰ������������£�

A����д�����ھ��е�yy4.0Ŀ¼�н���
B���£�yy)�ɣ�yy4.0���ļ�˭���ھͶ�д˭��ͬʱ���ڣ��Ͷ�д���ļ�
C���£�yy)�ɣ�yy4.0���ļ�˭���ھͶ�˭��ͬʱ���ڣ������ļ���д�������ļ���ͬʱ�Ѿ��ļ��ƶ�����Ŀ¼

��B����ʱ��A��Bͬʱ���ڵ��������ʱ������û���κΰ汾�����£�duowan\yy��Ŀ¼������
��д���ھ�Ŀ¼��duowan\yy4.0���н��У�

��ʱ����Ҫ������A�汾������B�汾����һ������ɺ������û�ֻ��ʹ��B�汾�ˣ���ͷ����˺���һ����

C�汾����ʱ��B�汾ͬʱ���ڣ��״�����ʱ����Ŀ¼��ͬʱ������ȡ���ļ��ƶ�����Ŀ¼������B�汾
�ڶ�ȡʱ�����Ŀ¼�ж�ȡ��ͬʱ����Ŀ¼��д�롣

��B�汾ȫ������C�汾ʱ�����Կ���ʹ�����ɾ��yy4.0Ŀ¼�ˡ�

�������A��B��C�汾ͬʱ���ڣ���ʹ��A�汾�����������п��ܻᶪʧ��

������ڴ������ļ�����ģ�������Ŀ¼������Ϊһ��Ŀ¼�д��ڶ���ļ�������£�����
дһ���ļ��ʹ�����Ŀ¼���������ļ�����Ŀ¼��ȴ�����ڣ������Ļ��ͻᵼ�����ݶ�ʧ��

����������
�������,������������B�汾������ͬʱҲ���������B�汾��C�汾����ʱҲһͬ���������C�汾��

����������ݿ���û��YYҪ����ô�ϸ�����C�汾�����Ժ�ֻ��Ҫ����һ�����������Ŀ¼�ж�д���ݼ��ɡ�

*/

//! Appdata�ļ���·��ͳһ��װ
/*! 
 * \file appdatapath.h
 * \	%appdata%Ŀ¼����󣬸�ҵ�����Լ������ļ���, %appdata%\\duwan\\yy\\�²������ⴴ��Ŀ¼ 

 ��%appdata%
    ����duowan
        ����yy
			����business					ҵ�������ļ�����ҵ�������Ŀ¼�½����ѵ������ļ�Ŀ¼��
            ����cache						����Ŀ¼������ҵ��
			��   ����download				�����ļ���
			��   ����temp					��ʱ�ļ���
            ����channel						Ƶ��ҵ�����
            ����imgroup						IMҵ�����
			��   ����%UID%					IM�û�Ŀ¼
			����log							��־������ҵ��
            ����mainframe					�������ء���½������
			��   ����account				�˺�Ŀ¼
			����openplatform				����ƽ̨���
			��   ����addonsex				���Ŀ¼
		    ����setup						��װĿ¼
			��   ����%MD5(appRootPath)%		�밲װ·���йص�Ŀ¼
		    ����update						�������
			����yycomstore					�����

 * \	20121121
 */

// �����װ��B�汾��ֻȡ��·������������Ŀ¼

// C�汾��Ҫ����Ŀ¼
// �汾C��
//   �����Ŀ¼�д����ļ��Ͷ�д��Ŀ¼�е��ļ���
//   �����Ŀ¼�е��ļ����ڣ��ͰѾ�Ŀ¼�е��ļ��Ƶ���Ŀ¼�У���д��Ŀ¼�е��ļ���
//   �¾�Ŀ¼�е��ļ��������ڣ�������Ŀ¼�ж�д��
//   ��֮ȡ���Ķ�����Ŀ¼�е�·����

// �޸�λ�ñ�ǣ�
/**-- AppData����: ���ļ�/Ŀ¼���ھͶ�д���ļ�/Ŀ¼ --**/

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
		// �÷�������ʹ�ã���ҵ�����Լ������ļ���, %appdata%\\duwan\\yy\\�²������ⴴ��Ŀ¼
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