#pragma once
#include <windows.h>
#include <string>
#include <set>


namespace DwUtility
{
	namespace hacker
	{
		class DW_DWUTILITY_EXPORT YYHackerLoadDll
		{
		private:
			YYHackerLoadDll();

		public:	
			virtual ~YYHackerLoadDll();	
			static YYHackerLoadDll *instance();
			void Hook();
			void SetHackerInfo(const std::wstring& targetFileExtName,const std::set<std::wstring>& findNameContainList,
				const std::wstring& hackerFilePath);

		protected:
			static HMODULE WINAPI NEW_LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
			
		private:
			static YYHackerLoadDll *m_instance;
			static std::wstring m_targetFileExtName;//文件扩展名.
			static std::set<std::wstring> m_findNameContainList;//用于判断系统加载的文件是否是想要替换的文件.
			static std::wstring m_hackerFilePath;//指定加载文件的路径.
		};
	}
}