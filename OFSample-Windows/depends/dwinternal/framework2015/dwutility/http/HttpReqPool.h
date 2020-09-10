#ifndef __HttpReqPool_h__
#define __HttpReqPool_h__

#pragma once


/*
 * 请求管理，请求的状态管理
 *
 *
 *
 */

#include <string>
#include <map>
#include <list>
#include "locker/dwlocker.h"
#include "httptask.h"
#include "httpreq.h"
#include "wininethttpbase.h"

namespace DwUtility
{
	namespace http
	{
		class CHttpReqPool : public IHttpReqSink
		{
		private:
			// IHttpReqSink
			virtual bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam);
			virtual bool OnHttpReqReceive(const void* buf, int len, void* pParam);
			virtual void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam);

		public:
			// CHttpReqPool
			CHttpReqPool(IHttpTaskSink* pSink);
			~CHttpReqPool();

			// 增加一个请求
			// 处于 初始状态  
			int AddReq(LPCTSTR url, LPCTSTR headers, const void* postdata, int postdataLen);

			// 直接销毁所有对象，清空所有列表
			void ClearReq();

			// 查询
			bool QueryReq(int id, HttpTaskData& data);

			// 状态管理
			void ChgStatusStart(int id);
			void ChgStatusStop(int id);
			void ChgStatusSuspend(int id);
			void ChgStatusResume(int id);

			// 下载需要的方法
			bool BeginHttpRequest(HttpTaskData** pHttpReq);
			void EndHttpRequest(HttpTaskData** pHttpReq);
			int  GetReadyReqCnt();

		private:
			bool IsCmdBreak(int id);
			bool TakeCmdStatus(HttpTaskData* pReq, HttpTaskStatus status);
			std::wstring FilterHeaders(LPCTSTR headers, HttpTaskData* pData);

		private:
			// 所有的任务管理
			typedef std::map<int, HttpTaskData*>   REQ_MAP;
			REQ_MAP                 m_reqs;
			DwCSLock		m_reqsLock;

			// 下载队列
			typedef std::list<HttpTaskData*>       REQ_LIST;
			REQ_LIST                m_readys;
			REQ_LIST                m_stops;

			DwCSLock		m_listLock;

			// 命令队列，处理suspend,resume,stop三种命令
			typedef std::map<int, HttpTaskStatus>  CMD_MAP;
			CMD_MAP                 m_cmds;

			DwCSLock		m_cmdsLock;

			// 递增的任务id
			int     m_id;

			// sink
			IHttpTaskSink*  m_pSink;
		};
	}
}

#endif // __HttpReqPool_h__
