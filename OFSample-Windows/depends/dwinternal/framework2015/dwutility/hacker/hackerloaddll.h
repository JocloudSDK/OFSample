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
			static std::wstring m_targetFileExtName;//�ļ���չ��.
			static std::set<std::wstring> m_findNameContainList;//�����ж�ϵͳ���ص��ļ��Ƿ�����Ҫ�滻���ļ�.
			static std::wstring m_hackerFilePath;//ָ�������ļ���·��.
		};
	}
}