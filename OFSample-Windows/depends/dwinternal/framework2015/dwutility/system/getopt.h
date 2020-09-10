#pragma once

#if 0
#include <map>
#include <vector>
#include <string>

#include "dwglobal.h"

namespace DwUtility
{
	namespace system
	{
		class DW_DWUTILITY_EXPORT DwGetopt
		{
		public:
			DwGetopt()
				:   m_parsed(false)
			{
			}

			virtual ~DwGetopt()
			{
			}

			//! ע��һ����������
			virtual void Register(const std::wstring &name);

			//!
			virtual int Parse(int argc, wchar_t *argv[], std::wstring *error_item = NULL);

			//!
			virtual int Parse(const std::vector<std::wstring> &args, std::wstring *error_item);

			//! ����Ƿ��ṩ��ָ������
			virtual bool IsGiven(const std::wstring &name) const;

			//! ��ȡһ�����������Ŀ��ֵ(ֻ������need_valueΪtrue����, ͬʱָ��������Ѿ�����)
			/*! \return ���û���ҵ��򷵻�NULL
			 */
			virtual const std::wstring& Get(const std::wstring &name) const;

		private:
			struct Item
			{
				std::wstring   name;
				std::wstring   value;
				bool        found;
			};
			
			std::map<std::wstring, Item>    m_items;
			bool         m_parsed;
		};
	}
}

#endif