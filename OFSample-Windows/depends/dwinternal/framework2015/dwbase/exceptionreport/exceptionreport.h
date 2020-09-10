/****************************************************************************
Author: houjinxin
Email : houjinxin@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#ifdef _DEBUG
    bool DWBASE_EXPORTS dwAssertReportOnce(bool exp, const char *file, const char *where, int line, const char *expression, const char *what);
#else
    bool DWBASE_EXPORTS dwInitAssertReport(UINT usrId = 0,LPCSTR yyVersion = NULL);//release�汾ʹ����� ��ʼ��
    void DWBASE_EXPORTS dwUninitAssertReport();//��yy�˳�ʱ��δ�ϴ���������������д�ļ����´�����ʱ���Ƚ��ļ���δ�ϱ������ϴ���������
    bool DWBASE_EXPORTS dwAssertReport(bool result, const char *file, const char *where, int line, const char *expression, const char *what);
#endif

#ifdef _DEBUG
	#define DW_ASSERT_REPORT(exp) \
		do { \
			if (dwAssertReportOnce((exp), __FILE__, __FUNCTION__, __LINE__, #exp, "")) \
			DebugBreak(); \
		} while(0)

	#define DW_ASSERT_REPORT_EX(exp,what) \
		do { \
			if (dwAssertReportOnce((bool(exp)), __FILE__, __FUNCTION__, __LINE__, #exp, what)) \
			DebugBreak(); \
		} while(0)
#else
	#define DW_ASSERT_REPORT(exp)\
		do { \
			dwAssertReport((exp), __FILE__, __FUNCTION__, __LINE__, #exp, ""); \
		} while(0)
	#define DW_ASSERT_REPORT_EX(exp,what)\
		do {\
			dwAssertReport((exp),__FILE__,__FUNCTION__,__LINE__, #exp, what);\
		}while(0)
#endif