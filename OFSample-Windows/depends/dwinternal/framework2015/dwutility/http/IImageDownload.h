
#include <string>

namespace cache
{
	struct IImageCache;
}

namespace imagedownload
{
	// 下载任务数据
	struct DownloadTask
	{
		std::wstring strURL;			// 下载文件地址(默认电信下载地址)
		std::wstring strWTURL;			// 下载文件的网通地址
		bool bIsDoubleDown;				// 是否需要网通和电信同时下载
		int nRedownTime;				// 失败后重新下载次数
		int nLastFailTime;				// 上次下载失败的时间
		std::wstring strDownloadID;		// 下载后的回调ID

		DownloadTask()
		{
			bIsDoubleDown = true;
			nRedownTime = 12;
			nLastFailTime = 0;
			strDownloadID = _T("");	
		}

		DownloadTask& operator =(DownloadTask& dtTask)
		{
			if (this != &dtTask)
			{
				strURL = dtTask.strURL;
				strWTURL = dtTask.strWTURL;
				bIsDoubleDown = dtTask.bIsDoubleDown;
				nRedownTime = dtTask.nRedownTime;
				nLastFailTime = dtTask.nLastFailTime;
				strDownloadID = dtTask.strDownloadID;
			}

			return *this;
		}
	};

	// 图片下载回调接口
	struct IImageDownloadWatchBase
	{
		// 图片下载回调
		// strImageFilePath 图片文件下载后的本地路径
		// strImageStream	图片文件的二进制数据
		// dtTask			图片下载任务的输入参数,里面的所有参数除了nRedownTime外，都会原样返回
		// bSuccessed		图片下载是否成功
		virtual void OnDownloadImageBack(const std::wstring& strImageFilePath, const std::string& strImageStream, const DownloadTask& dtTask, bool bSuccessed) = 0;	
	};

	// 下载图片接口
	struct IImageDownload
	{
		virtual void Release() = 0;
		// 添加下载任务
		virtual void AddDownloadImageTask(const DownloadTask& dtTask) = 0;
		// 删除任务
		virtual void DeleteDownloadImageTask(const std::wstring& strTaskID) = 0;
		// 下载聊天消息的图片文件
		virtual void DownloadImageInYYMsg(const std::wstring& strMsg) = 0;
		// 检查聊天消息里是否有图片文件
		virtual bool HasImageInYYMsg(const std::wstring& strMsg) = 0;
		// 设置图片缓存
		virtual void SetImageCache(cache::IImageCache* pImageCache) = 0;
		// 添加回调
		virtual void watch(IImageDownloadWatchBase* iInterface) = 0;
		// 删除回调
		virtual void revoke(IImageDownloadWatchBase* iInterface) = 0;
	};
}

