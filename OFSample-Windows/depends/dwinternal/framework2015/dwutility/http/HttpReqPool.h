#ifndef __HttpReqPool_h__
#define __HttpReqPool_h__

#pragma once


/*
 * ������������״̬����
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

			// ����һ������
			// ���� ��ʼ״̬  
			int AddReq(LPCTSTR url, LPCTSTR headers, const void* postdata, int postdataLen);

			// ֱ���������ж�����������б�
			void ClearReq();

			// ��ѯ
			bool QueryReq(int id, HttpTaskData& data);

			// ״̬����
			void ChgStatusStart(int id);
			void ChgStatusStop(int id);
			void ChgStatusSuspend(int id);
			void ChgStatusResume(int id);

			// ������Ҫ�ķ���
			bool BeginHttpRequest(HttpTaskData** pHttpReq);
			void EndHttpRequest(HttpTaskData** pHttpReq);
			int  GetReadyReqCnt();

		private:
			bool IsCmdBreak(int id);
			bool TakeCmdStatus(HttpTaskData* pReq, HttpTaskStatus status);
			std::wstring FilterHeaders(LPCTSTR headers, HttpTaskData* pData);

		private:
			// ���е��������
			typedef std::map<int, HttpTaskData*>   REQ_MAP;
			REQ_MAP                 m_reqs;
			DwCSLock		m_reqsLock;

			// ���ض���
			typedef std::list<HttpTaskData*>       REQ_LIST;
			REQ_LIST                m_readys;
			REQ_LIST                m_stops;

			DwCSLock		m_listLock;

			// ������У�����suspend,resume,stop��������
			typedef std::map<int, HttpTaskStatus>  CMD_MAP;
			CMD_MAP                 m_cmds;

			DwCSLock		m_cmdsLock;

			// ����������id
			int     m_id;

			// sink
			IHttpTaskSink*  m_pSink;
		};
	}
}

#endif // __HttpReqPool_h__
