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

			//! 注册一个命令行项
			virtual void Register(const std::wstring &name);

			//!
			virtual int Parse(int argc, wchar_t *argv[], std::wstring *error_item = NULL);

			//!
			virtual int Parse(const std::vector<std::wstring> &args, std::wstring *error_item);

			//! 检查是否提供了指定的项
			virtual bool IsGiven(const std::wstring &name) const;

			//! 获取一个解析后的条目的值(只能用于need_value为true的项, 同时指定项必须已经给定)
			/*! \return 如果没有找到则返回NULL
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