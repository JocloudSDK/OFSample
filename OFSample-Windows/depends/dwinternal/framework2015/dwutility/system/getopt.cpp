#include "stable.h"
#if 0
#include "getopt.h"

namespace DwUtility
{
	namespace system
	{
		void DwGetopt::Register(const std::wstring &name)
		{
			DW_ASSERT(m_items.find(name) == m_items.end());
		    
			Item itm;
			itm.name = name;
			itm.found = false;

			m_items.insert(std::map<std::wstring, Item>::value_type(name, itm));
		}

		int DwGetopt::Parse(const std::vector<std::wstring> &args, std::wstring *error_item)
		{
			std::map<std::wstring, Item>::iterator iter;
			for(size_t i = 0; i < args.size();)
			{
				if((iter = m_items.find(args[i])) == m_items.end())
				{
					i += 2;
					continue;
				}

				if(args.size() - 1 == i)
				{
					if(error_item != NULL)
					{
						*error_item = args[i];
						return -1;
					}
				}
				else
				{
					iter->second.value = args[i + 1];
					iter->second.found = true;
					i += 2;
				}
			}

			m_parsed = true;
			return 0;
		}

		int DwGetopt::Parse(int argc, wchar_t *argv[], std::wstring *error_item)
		{
			DW_ASSERT(argc > 0);
			DW_ASSERT(!m_parsed);

			std::vector<std::wstring> args;
			for(int i = 1; i < argc; ++i)
			{
				args.push_back(std::wstring(argv[i]));
			}
			return Parse(args, error_item);
		}

		bool DwGetopt::IsGiven(const std::wstring &name) const
		{
			DW_ASSERT(m_parsed);

			std::map<std::wstring, Item>::const_iterator citer = m_items.find(name);
			DW_ASSERT(citer != m_items.end());

			return citer->second.found;
		}

		const std::wstring& DwGetopt::Get(const std::wstring &name) const
		{
			DW_ASSERT(m_parsed);

			std::map<std::wstring, Item>::const_iterator citer = m_items.find(name);
			DW_ASSERT(citer != m_items.end());

			return citer->second.value;
		}
	}
}
#endif