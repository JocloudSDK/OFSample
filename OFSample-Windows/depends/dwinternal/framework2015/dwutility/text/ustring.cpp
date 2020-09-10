#include <cassert>
#include "ustring.h"

namespace DwUtility
{
	namespace text
	{
		void toWideString(const char* pStr, int len, std::wstring& buf, UINT CodePage)
		{
			if (pStr == NULL)
			{
				assert(NULL);
				return;
			}

			if (len == -1)
			{
				len = strlen(pStr);
			}
			else if (len < 0)
			{
				assert(NULL);
				return;
			}


			// figure out how many wide characters we are going to get 
			int nChars = MultiByteToWideChar(CodePage, 0, pStr, len, NULL, 0);
			if (len == -1)
			{
				--nChars;
			}
				
			if (nChars == 0)
				return ;

			// convert the narrow string to a wide string 
			// nb: slightly naughty to write directly into the string like this
			buf.resize(nChars);
			MultiByteToWideChar(CodePage, 0, pStr, len,
				const_cast<wchar_t*>(buf.c_str()), nChars);
		}
		std::wstring toWideString( const char* pStr , int len,UINT CodePage)
		{
			std::wstring buf ;
			toWideString(pStr, len, buf, CodePage);
			return buf ;
		}

		void toNarrowString(const wchar_t* pStr, int len, std::string& buf, UINT CodePage )
		{
			if (pStr == NULL)
			{
				assert(NULL);
				return;
			}

			if (len == -1)
			{
				len = wcslen(pStr);
			}
			else if (len < 0)
			{
				assert(NULL);
				return;
			}

			// figure out how many narrow characters we are going to get 
			int nChars = WideCharToMultiByte(CodePage, 0,
				pStr, len, NULL, 0, NULL, NULL);
			if (len == -1)
			{
				--nChars;
			}
			if (nChars == 0)
				return;

			// convert the wide string to a narrow string
			// nb: slightly naughty to write directly into the string like this
			buf.resize(nChars);
			WideCharToMultiByte(CodePage, 0, pStr, len,
				const_cast<char*>(buf.c_str()), nChars, NULL, NULL);
		}
		std::string toNarrowString( const wchar_t* pStr , int len, UINT CodePage )
		{
			std::string buf ;
			toNarrowString(pStr, len, buf, CodePage);
			return buf ; 
		}

		void splitWString( const std::wstring &stringOrg, const std::wstring & delimiter, std::vector<std::wstring> &vectorResult )
		{
			size_t last = 0;
			size_t index = stringOrg.find_first_of(delimiter,last);
			while ( index != std::string::npos )
			{
				vectorResult.push_back(stringOrg.substr(last,index-last));
				last = index + 1;
				index = stringOrg.find_first_of(delimiter,last);
			}
			if ( index - last > 0 )
			{
				vectorResult.push_back(stringOrg.substr(last ,index - last));
			}
		}

		std::string getHashString( const char *hashData, int length )
		{
			const char p[] = "0123456789abcdef";

			char out[64] = {0};
			for ( int i = 0; i < length; i++ )
			{
				unsigned char c = hashData[i];
				out[ i * 2 ] = p[c >> 4];
				out[ i * 2 + 1 ] = p[c & 0xf];
			}

			return std::string(out);
		}

		std::wstring Char2String(unsigned char c)
		{
			static wchar_t s_Table[] = {'0', '1', '2', '3', 
				'4', '5', '6', '7',
				'8', '9', 'A', 'B',
				'C', 'D', 'E', 'F'
			};

			std::wstring s;
			s.resize(2);
			s[0] = s_Table[c/16];
			s[1] = s_Table[c%16];

			return s;
		}

		std::wstring BinToHex(const std::string& bin)
		{
			std::wstring result;
			result.reserve(bin.size() * 2);
			for (size_t i = 0; i < bin.size(); ++i)
			{
				unsigned char c = (unsigned char)bin[i];
				result += Char2String(c);
			}
			return result;
		}

		std::string URLEncode2( const std::string& src )
		{
			static    char hex[] = "0123456789ABCDEF";  
			std::string dst;  

			for (size_t i = 0; i < src.size() && src[i] != 0; i++)  
			{  
				unsigned char ch = src[i];  
				if (isalnum(ch) || ch == '.')  
				{  
					dst += ch;  
				}  
				else
				{  
					unsigned char c = static_cast<unsigned char>(src[i]);  
					dst += '%';  
					dst += hex[c >> 4];  
					dst += hex[c % 16];  
				}  
			}  
			return dst;
		}

		std::string URLEncode2( const std::wstring& str )
		{
			//按照要求，必须先转换为utf8，再urlencode
			std::string data;
			toNarrowString(str.c_str(), str.size(), data, CP_UTF8);
			return URLEncode2(data);
		}

		std::wstring URLEncode(const std::wstring& src)
		{
			std::wstring result;
			wchar_t buf[8] = {0};
			wchar_t c;

			for(size_t i = 0; i < src.length(); ++i)
			{
				c = src.at(i);      
				if( iswalnum(c) || c == L'.' )
				{
					result += c;
				}
				else
				{
					wsprintf(buf, L"%%%02X", c);
					result += buf;
				}
			}
			return result;
		}

        bool isUtf8Text(const char *text, size_t size)
        {
            bool isUtf8 = true;
            unsigned char* start = (unsigned char*)text;
            unsigned char* end = start + size;

            while (start < end)
            {
                if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
                {
                    start++;
                }
                else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
                {
                    isUtf8 = false;
                    break;
                }
                else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
                {
                    if (start >= end - 1)
                    {
                        break;
                    }
                    if ((start[1] & (0xC0)) != 0x80)
                    {
                        isUtf8 = false;
                        break;
                    }
                    start += 2;
                } 
                else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
                {
                    if (start >= end - 2)
                    {
                        break;
                    }
                    if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
                    {
                        isUtf8 = false;
                        break;
                    }
                    start += 3;
                } 
                else
                {
                    isUtf8 = false;
                    break;
                }
            }

            return isUtf8;
        }

		const char KBase64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		std::string DW_DWUTILITY_EXPORT toBase64String( const void *data, size_t size )
		{
			const unsigned char *inputData = (const unsigned char*)data;

			/* valid output size ? */
			size_t len2 = 4 * ((size + 2) / 3) + 1;
			std::auto_ptr<char> out(new char[len2]);
			char *p = out.get();

			size_t leven = 3 * (size / 3);
			size_t i = 0;
			for ( i = 0; i < leven; i += 3) 
			{
				*p++ = KBase64Chars[(inputData[0] >> 2) & 0x3F];
				*p++ = KBase64Chars[(((inputData[0] & 3) << 4) + (inputData[1] >> 4)) & 0x3F];
				*p++ = KBase64Chars[(((inputData[1] & 0xf) << 2) + (inputData[2] >> 6)) & 0x3F];
				*p++ = KBase64Chars[inputData[2] & 0x3F];
				inputData += 3;
			}
			/* Pad it if necessary...  */
			if (i < size) 
			{
				unsigned a = inputData[0];
				unsigned b = (i+1 < size) ? inputData[1] : 0;

				*p++ = KBase64Chars[(a >> 2) & 0x3F];
				*p++ = KBase64Chars[(((a & 3) << 4) + (b >> 4)) & 0x3F];
				*p++ = (i+1 < size) ? KBase64Chars[(((b & 0xf) << 2)) & 0x3F] : L'=';
				*p++ = L'=';
			}

			/* append a NULL byte */
			*p = '\0';
			return out.get();
		}

		std::string DW_DWUTILITY_EXPORT StringFormatA(const char *format, ...)
		{
			char output[4096] = {0};

			int ret = 0;
			va_list args;
			va_start(args, format);
			ret = vsprintf_s(output, sizeof(output)/sizeof(char), format, args);
			va_end(args);

			if (ret == -1)
			{	
				const int rate[3] = {2, 10, 2048};
				for (int i = 0; i < 3; ++i)
				{
					std::string output_;
					output_.resize(4096*rate[i]);

					va_list args;
					va_start(args, format);
					ret = vsprintf_s(const_cast<char*>(output_.c_str()), output_.size(), format, args);
					va_end(args);

					if (ret != -1)
					{
						return std::string(output_.c_str());
					}
				}
			}

			return std::string(output);
		}

		void DW_DWUTILITY_EXPORT splitStringA(const std::string &stringOrg, const std::string & delimiter, std::vector<std::string> &vectorResult)
		{		
			size_t last = 0;
			size_t index = stringOrg.find_first_of(delimiter,last);
			while ( index != std::string::npos )
			{
				vectorResult.push_back(stringOrg.substr(last,index-last));
				last = index + 1;
				index = stringOrg.find_first_of(delimiter,last);
			}
			if ( index - last > 0 )
			{
				vectorResult.push_back(stringOrg.substr(last ,index - last));
			}
		}

		std::wstring DW_DWUTILITY_EXPORT StringFormatW(const wchar_t *format, ...)
		{
			wchar_t output[4096] = {0};

			va_list args;
			va_start(args, format);
			int ret =  vswprintf_s(output, sizeof(output)/sizeof(wchar_t), format, args);
			va_end(args);

			if (ret == -1)
			{	
				const int rate[3] = {2, 10, 2048};
				for (int i = 0; i < 3; ++i)
				{
					std::wstring output_;
					output_.resize(4096*rate[i]);

					va_list args;
					va_start(args, format);
					ret =  vswprintf_s(const_cast<wchar_t*>(output_.c_str()), output_.size(), format, args);
					va_end(args);

					if (ret != -1)
					{
						return std::wstring(output_.c_str());
					}
				}
			}

			return std::wstring(output);
		}

	}
}
