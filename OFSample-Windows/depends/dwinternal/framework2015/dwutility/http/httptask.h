#ifndef __HttpTaskMgr_h__
#define __HttpTaskMgr_h__

#include <string>

namespace DwUtility
{
	namespace http
	{
		enum HttpTaskStatus
		{
			STATUS_READY   = 0,   // 排队等待下载
			STATUS_WORKING    ,   // 已经被下载线程调度到了，下载的过程中
			STATUS_PAUSE      ,   // 暂停
			STATUS_STOP       ,   // 停止
			STATUS_COMPLETED  ,   // 完成
			STATUS_ERROR          // 失败
		};

		// 任务信息
		struct HttpTaskData
		{
			std::wstring   url;
			std::wstring   headers;
			std::string    postdata;
			int            id;
			int            totalLen;
			int            recvLen;
			int            error; // 错误次数
			HttpTaskStatus status;
		};

		// http回调
		struct IHttpTaskSink
		{
			virtual void OnHttpReqStatus(int id, HttpTaskStatus status) = 0;
			virtual bool OnHttpReqReceiveHeader(int id, int nTotalLength, int nStatus) = 0;
			virtual bool OnHttpReqReceive(int id, const void* buf, int len) = 0;

			// 任务完成，成功or失败，错误的次数
			// 返回true表示完成，false表示需要重试
			virtual bool OnHttpReqCompleted(int id, bool bSucceeded, int nErrors) = 0;
		};

		// http异步多任务
		struct IHttpTaskMgr
		{
			// 释放
			virtual void Release() = 0;

			// 处理一个http请求
			// url,如：http://crash.yy.duowan.com/crash3_0.php
			// headers,如：Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,有就用post，没有就用get
			// 返回任务号
			virtual int  AddTask(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// 删除任务(暂未实现)
			virtual void RemoveTask(int id) = 0;

			// 开始任务
			virtual void StartTask(int id) = 0;

			// 暂停任务
			virtual void SuspendTask(int id) = 0;

			// 继续任务
			virtual void ResumeTask(int id) = 0;

			// 结束任务
			virtual void StopTask(int id) = 0;

			// 查询任务
			virtual bool QueryTask(int id, HttpTaskData& data) = 0;
		};

		// 创建下载管理器
		// nMaxThread 并发线程的个数
		// pSink      sink
		EXTERN_C DW_DWUTILITY_EXPORT IHttpTaskMgr* CreateHttpTaskMgr(int nMaxThread, IHttpTaskSink* pSink);
	}
}

#endif // __HttpTaskMgr_h__
