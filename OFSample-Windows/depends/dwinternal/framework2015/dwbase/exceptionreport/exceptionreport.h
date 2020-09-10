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
    bool DWBASE_EXPORTS dwInitAssertReport(UINT usrId = 0,LPCSTR yyVersion = NULL);//release版本使用入口 初始化
    void DWBASE_EXPORTS dwUninitAssertReport();//在yy退出时将未上传到服务器的数据写文件，下次启动时首先将文件中未上报数据上传到服务器
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