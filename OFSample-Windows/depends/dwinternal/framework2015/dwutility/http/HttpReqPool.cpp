#include "stable.h"
#include "HttpReqPool.h"

namespace DwUtility
{
	namespace http
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// IHttpReqSink
		bool CHttpReqPool::OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam)
		{
			HttpTaskData* pReq = (HttpTaskData*)pParam;
			assert(pReq != NULL);
			assert(nStatus == 200 || nStatus == 206);

			// 是否被中断
			if (this->IsCmdBreak(pReq->id))
			{
				return false;
			}

			if (nStatus == 200)
			{
				pReq->totalLen = nTotalLength;
				pReq->recvLen  = 0;
			}
			else
			{
				if (nStatus == 206 && pReq->totalLen == -1)//断点续传续传 未接收数
				{
					pReq->totalLen = nTotalLength + pReq->recvLen;//加上曾经下载过的
				}

				if (pReq->totalLen != nTotalLength+pReq->recvLen)
				{
					return false;
				}
			}

			// notify
			return m_pSink->OnHttpReqReceiveHeader(pReq->id, pReq->totalLen, nStatus);
		}

		bool CHttpReqPool::OnHttpReqReceive(const void* buf, int len, void* pParam)
		{
			HttpTaskData* pReq = (HttpTaskData*)pParam;
			assert(pReq != NULL);
			pReq->recvLen += len;
			// notify
			m_pSink->OnHttpReqReceive(pReq->id, buf, len);

			// 是否被中断
			if (this->IsCmdBreak(pReq->id))
			{
				return false;
			}

			return true;
		}

		void CHttpReqPool::OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam)
		{
			HttpTaskData* pReq = (HttpTaskData*)pParam;
			assert(pReq != NULL);
			if (bSucceeded)
			{
				pReq->status = STATUS_COMPLETED;
			}
			else
			{
				// 正常的用户中断也会调用到这里的
				if (this->TakeCmdStatus(pReq, STATUS_PAUSE))
				{
					return;
				}
	            
				// 错误
				pReq->status = STATUS_ERROR;
				++pReq->error;
			}

			// notify
			bool bRet = m_pSink->OnHttpReqCompleted(pReq->id, bSucceeded, pReq->error);
			if (!bRet)
			{
				pReq->totalLen = 0;
				pReq->recvLen = 0;
				if (bSucceeded)
				{
					pReq->status = STATUS_ERROR;
					++pReq->error;
				}
				
				// 放到stop队列中
				DwAutoCSLocker locker(&m_listLock);
				m_stops.push_back(pReq);
			}

			m_pSink->OnHttpReqStatus(pReq->id, pReq->status);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		//
		CHttpReqPool::CHttpReqPool(IHttpTaskSink* pSink)
			: m_pSink(pSink)
			, m_id(0)
		{

		}

		CHttpReqPool::~CHttpReqPool()
		{

		}

		int CHttpReqPool::AddReq(LPCTSTR url, LPCTSTR headers, const void* postdata, int postdataLen)
		{
			// 构造请求包
			HttpTaskData* pData = new HttpTaskData;

			pData->url = url;
			pData->recvLen = 0;
			pData->totalLen = -1;

			if (headers != NULL)
			{
				pData->headers = FilterHeaders(headers, pData);
			}

			if (postdata != NULL && postdataLen > 0)
			{
				pData->postdata.assign((const char*)postdata, postdataLen);
			}

			pData->id = ++m_id;
			
			pData->error = 0;
			pData->status = STATUS_STOP;

			// 保存到map
			{
				DwAutoCSLocker locker(&m_reqsLock);
				m_reqs[m_id] = pData;
			}

			// 保存到队列中
			{
				DwAutoCSLocker locker(&m_listLock);
				m_stops.push_back(pData);
			}

			return m_id;
		}

		void CHttpReqPool::ClearReq()
		{
			m_readys.clear();
			m_stops.clear();
			m_cmds.clear();
			for (REQ_MAP::const_iterator it = m_reqs.begin(); it != m_reqs.end(); ++it)
			{
				delete it->second;
			}
			m_reqs.clear();
		}

		bool CHttpReqPool::QueryReq(int id, HttpTaskData& data)
		{
			DwAutoCSLocker locker(&m_reqsLock);

			REQ_MAP::const_iterator it = m_reqs.find(id);
			if (it != m_reqs.end())
			{
				data = *(it->second);
				return true;
			}
			return false;
		}

		void CHttpReqPool::ChgStatusStart(int id)
		{
			bool bFind = false;

			// 如果在stop队列移动到ready队列中
			{
				HttpTaskData* pReq;
				DwAutoCSLocker locker(&m_listLock);

				for (REQ_LIST::iterator it = m_stops.begin(); it != m_stops.end(); ++it)
				{
					pReq = *it;
					if (pReq->id == id)
					{
						// 状态切换到ready
						pReq->status = STATUS_READY;
						m_readys.push_back(*it);
						m_stops.erase(it);
						bFind = true;
						break;
					}
				}
			}

			// notify
			if (bFind)
			{
				m_pSink->OnHttpReqStatus(id, STATUS_READY);
			}
		}

		void CHttpReqPool::ChgStatusStop(int id)
		{
			bool bFind = false;

			// 如果在ready队列中移动到stop队列
			{
				HttpTaskData* pReq;
				DwAutoCSLocker locker(&m_listLock);

				for (REQ_LIST::iterator it = m_readys.begin(); it != m_readys.end(); ++it)
				{
					pReq = *it;
					if (pReq->id == id)
					{
						// 切换状态到stop
						pReq->status = STATUS_STOP;
						m_stops.push_back(*it);
						m_readys.erase(it);
						bFind = true;
						break;
					}
				}
			}

			// notify
			if (bFind)
			{
				m_pSink->OnHttpReqStatus(id, STATUS_STOP);
				return;
			}

			// 否则只是设置状态
			{
				DwAutoCSLocker locker(&m_cmdsLock);
				m_cmds[id] = STATUS_STOP;
			}
		}

		void CHttpReqPool::ChgStatusSuspend(int id)
		{
			// 设置状态
			DwAutoCSLocker locker(&m_cmdsLock);
			m_cmds[id] = STATUS_PAUSE;
		}

		void CHttpReqPool::ChgStatusResume(int id)
		{
			// 设置状态
			DwAutoCSLocker locker(&m_cmdsLock);
			m_cmds[id] = STATUS_WORKING;
		}

		bool CHttpReqPool::BeginHttpRequest(HttpTaskData** pHttpReq)
		{
			assert(pHttpReq != NULL);
			HttpTaskData*& req = *pHttpReq;
			if (req != NULL)
			{
				// 需要在这里处理resume命令
				assert(req->status == STATUS_PAUSE);

				// 暂停状态可以接受的命令是resume、stop
				return this->TakeCmdStatus(req, STATUS_WORKING);
			}
			else
			{
				bool bFind = false;

				// 从队列中取一个
				{
					DwAutoCSLocker locker(&m_listLock);
					if (!m_readys.empty())
					{
						req = m_readys.front();
						m_readys.pop_front();
						req->status = STATUS_WORKING;
						bFind = true;
					}
				}

				// notify
				if (bFind)
				{
					m_pSink->OnHttpReqStatus(req->id, req->status);
					return true;
				}
			}
			return false;
		}

		void CHttpReqPool::EndHttpRequest(HttpTaskData** pHttpReq)
		{
			assert(pHttpReq != NULL);
			HttpTaskData*& req = *pHttpReq;
			if (req != NULL)
			{
				// 只有在暂停的情况下才需要保存上次处理的请求
				if (req->status != STATUS_PAUSE)
				{
					req = NULL;
				}
			}
		}

		int CHttpReqPool::GetReadyReqCnt()
		{
			// 正在排队的请求个数
			DwAutoCSLocker locker(&m_listLock);
			return (int)m_readys.size();
		}

		bool CHttpReqPool::IsCmdBreak(int id)
		{
			HttpTaskStatus status;

			{
				DwAutoCSLocker locker(&m_cmdsLock);
				CMD_MAP::const_iterator it = m_cmds.find(id);
				if (it != m_cmds.end())
				{
					status = it->second;
				}
				else
				{
					return false;
				}
			}

			if (status == STATUS_PAUSE || status == STATUS_STOP)
			{
				return true;
			}

			return false;
		}

		bool CHttpReqPool::TakeCmdStatus(HttpTaskData* pReq, HttpTaskStatus status)
		{
			HttpTaskStatus tmp;

			{
				DwAutoCSLocker locker(&m_cmdsLock);
				CMD_MAP::iterator it = m_cmds.find(pReq->id);
				if (it != m_cmds.end())
				{
					tmp = it->second;
					m_cmds.erase(it);
				}
				else
				{
					return false;
				}
			}

			if (tmp == STATUS_STOP || tmp == status)
			{
				pReq->status = tmp;
				if (status == STATUS_STOP)
				{
					// 插入队列
					DwAutoCSLocker locker(&m_listLock);
					m_stops.push_back(pReq);
				}
				m_pSink->OnHttpReqStatus(pReq->id, pReq->status);
				return true;
			}

			return false;
		}

		std::wstring CHttpReqPool::FilterHeaders(LPCTSTR headers, HttpTaskData* pData)
		{
			std::wstring filterheaders = headers;
			std::wstring range_bytes = L"YYHttpBeginPos:bytes=";
			std::wstring end_range = L"-\r\n";
			std::size_t startpos = filterheaders.find(range_bytes.c_str());
				
			if (startpos != filterheaders.npos)
			{
				std::size_t numpos = filterheaders.find(end_range, startpos);

				if (numpos != filterheaders.npos)
				{		
					std::wstring strlen = filterheaders.substr(startpos + range_bytes.length(), numpos - startpos - range_bytes.length());
					pData->recvLen = _wtoi(strlen.c_str());
					filterheaders.erase(startpos, numpos + end_range.length());
				}
			}

			return filterheaders;
		}
	}
}
