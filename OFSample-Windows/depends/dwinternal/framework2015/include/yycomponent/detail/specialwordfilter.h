#pragma once

#include <tchar.h>
#include <cassert>

namespace specialwordfilter_helper
{
	static const int KCharFilterBufferSize = 65535/8+1;

	static const unsigned char s_unMap[8] = 
	{
		1,
		1<<1,
		1<<2,
		1<<3,
		1<<4,
		1<<5,
		1<<6,
		1<<7
	};

	class SpecialWordFilter
	{
	public:
		SpecialWordFilter() : m_cReplacer(0)
		{
			//Ĭ����Ϊ�����ַ�����printable
			::memset( m_unData, 255, sizeof(m_unData) );

			Init();

			SetReplacer(_T(' '));
		}

		~SpecialWordFilter()
		{
		}

	public:
		void Init()
		{
			static unsigned short s_BreakChars[] = {9, 10, 11, 12, 13, 1608, 1711, 8232, 8233, 8238, 61451, 61452, 61453, 0x0e2a, 0x0e47, 0xfffc};

			for(unsigned short i = 0; i < sizeof(s_BreakChars)/sizeof(s_BreakChars[0]); i++)
			{
				SetUnPrintable(s_BreakChars[i]);
			}
		}

		void SetReplacer( TCHAR tc )
		{
			m_cReplacer = tc;
		}

		bool IsNeedFilter( LPCWSTR str)
		{
			if(str == NULL)
			{
				return false;
			}

			size_t len = wcslen(str);
			if(len == 0)
			{
				return true;
			}

			for(size_t i = 0; i < len; i++)
			{
				if(!IsPrintable(str[i]))
				{
					return true;
				}
			}
			return false;
		}

		void Filter( std::wstring& str )
		{
			if (m_cReplacer == 0)	//ɾ��
			{
				std::wstring tmp;

				tmp.reserve(str.size());

				TCHAR c = 0;

				for (size_t i = 0; i < str.size(); i++)
				{
					c = str[i];
					if (IsPrintable(c))
					{
						tmp.append(1, c);
					}
				}

				str = tmp;
			}
			else
			{
				for (size_t i = 0; i < str.size(); i++)
				{
					TCHAR& c = str[i];
					if (!IsPrintable(c))
					{
						c = m_cReplacer;
					}
				}
			}
		}

	private:
		inline bool IsPrintable(TCHAR c)
		{
#ifdef _DEBUG
			assert(c/8 == (c>>3));
			assert((c%8) ==(c-((c>>3)<<3)));
#endif
			return (m_unData[c>>3] & s_unMap[c-((c>>3)<<3)]) != 0;
		}

		inline void SetPrintable(TCHAR c)
		{
#ifdef _DEBUG
			assert(c/8 == (c>>3));
			assert((c%8) ==(c-((c>>3)<<3)));
#endif
			m_unData[c>>3] |= s_unMap[c-((c>>3)<<3)];
		}

		inline void	SetUnPrintable( wchar_t c )
		{
#ifdef _DEBUG
			assert(c/8 == (c>>3));
			assert((c%8) ==(c-((c>>3)<<3)));
#endif
			unsigned char x = (~s_unMap[c-((c>>3)<<3)]);
			m_unData[c>>3] &= x;
		}

	private:
		//�������65535���ַ���ÿ���ַ�ռһλ��������ַ��ɼ������λΪ1������Ϊ0
		unsigned char	m_unData[KCharFilterBufferSize];	
		wchar_t			m_cReplacer;
	};

	__declspec(selectany) SpecialWordFilter _filter;

	static bool isNeedFilter(LPCWSTR str)
	{
		return _filter.IsNeedFilter(str);
	}

	static void filterString(std::wstring& str)
	{
		//Ϊ�˼��ٿ��������ڵ���filterStringǰ�ȵ���isNeedFilter
		ASSERT(isNeedFilter(str.c_str()));
		_filter.Filter(str);
	}

	static void filterNick(std::wstring& nick)
	{
		filterString(nick);

		if(nick.empty())
		{
			//������ȡ�����ǳ���ʱ���ǿյġ�
			//��ֹ���"������"���������¡�
			//�滻��6�����ַ�
			nick = L"\xFFe7\xFFe7\xFFe7\xFFe7\xFFe7\xFFe7";
		}
	}
};
