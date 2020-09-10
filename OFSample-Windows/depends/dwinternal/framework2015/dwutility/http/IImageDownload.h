
#include <string>

namespace cache
{
	struct IImageCache;
}

namespace imagedownload
{
	// ������������
	struct DownloadTask
	{
		std::wstring strURL;			// �����ļ���ַ(Ĭ�ϵ������ص�ַ)
		std::wstring strWTURL;			// �����ļ�����ͨ��ַ
		bool bIsDoubleDown;				// �Ƿ���Ҫ��ͨ�͵���ͬʱ����
		int nRedownTime;				// ʧ�ܺ��������ش���
		int nLastFailTime;				// �ϴ�����ʧ�ܵ�ʱ��
		std::wstring strDownloadID;		// ���غ�Ļص�ID

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

	// ͼƬ���ػص��ӿ�
	struct IImageDownloadWatchBase
	{
		// ͼƬ���ػص�
		// strImageFilePath ͼƬ�ļ����غ�ı���·��
		// strImageStream	ͼƬ�ļ��Ķ���������
		// dtTask			ͼƬ����������������,��������в�������nRedownTime�⣬����ԭ������
		// bSuccessed		ͼƬ�����Ƿ�ɹ�
		virtual void OnDownloadImageBack(const std::wstring& strImageFilePath, const std::string& strImageStream, const DownloadTask& dtTask, bool bSuccessed) = 0;	
	};

	// ����ͼƬ�ӿ�
	struct IImageDownload
	{
		virtual void Release() = 0;
		// �����������
		virtual void AddDownloadImageTask(const DownloadTask& dtTask) = 0;
		// ɾ������
		virtual void DeleteDownloadImageTask(const std::wstring& strTaskID) = 0;
		// ����������Ϣ��ͼƬ�ļ�
		virtual void DownloadImageInYYMsg(const std::wstring& strMsg) = 0;
		// ���������Ϣ���Ƿ���ͼƬ�ļ�
		virtual bool HasImageInYYMsg(const std::wstring& strMsg) = 0;
		// ����ͼƬ����
		virtual void SetImageCache(cache::IImageCache* pImageCache) = 0;
		// ��ӻص�
		virtual void watch(IImageDownloadWatchBase* iInterface) = 0;
		// ɾ���ص�
		virtual void revoke(IImageDownloadWatchBase* iInterface) = 0;
	};
}

