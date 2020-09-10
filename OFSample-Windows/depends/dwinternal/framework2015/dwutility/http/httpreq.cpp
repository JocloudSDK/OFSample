#include "stable.h"
#include "httpreq.h"
#include "file/ufile.h"
#include "HttpReqBase.h"
#include "HttpReqSink.h"
#include "HttpReqWrapper.h"
#include "HttpTaskMgr.h"
#include "encryption/md5encrypt.h"
#include "wininethttpbase.h"

namespace DwUtility
{
	namespace http
	{
		IHttpRequest* CreateHttpReq()
		{
			return new CHttpReqBase();
		}

		IHttpRequest* CreateAsyncHttpReq()
		{
			return new CAsyncHttpReq();
		}

		bool SendHttpReq(std::string& ret, LPCTSTR url, LPCTSTR headers, const void* postdata , int postdataLen)
		{
			CHttpReqSink httpsink(&ret);
			CHttpReqBase httpreq;
			httpreq.SetReqSink(&httpsink, &httpreq);
			if (httpreq.InitRequest())
			{
				httpreq.SendRequest(url, headers, postdata, postdataLen);
				httpreq.EndRequest();
			}
			return httpsink.m_flag;
		}

		bool HttpReq2File(LPCTSTR url, const char* md5, LPCTSTR path)
		{
			std::string ret;
			if (!SendHttpReq(ret, url, NULL, NULL, 0))
			{
				return false;
			}

			if (md5 != NULL)
			{
				std::string filemd5;
				DwUtility::encryption::MD5CryptA(ret.c_str(), ret.size(), filemd5);
				if (_stricmp(filemd5.c_str(), md5) != 0)
				{
					return false;
				}
			}

			return DwUtility::ufile::WriteFileData(path, ret);
		}

		IHttpRequestAdv* CreateHttpReqAdv()
		{
			return new CAsyncHttpReqAdv();
		}

		IHttpTaskMgr* CreateHttpTaskMgr(int nMaxThread, IHttpTaskSink* pSink)
		{
			return new CHttpReqMgr(nMaxThread, pSink);
		}
	}
}
