#pragma once

#include <string>

namespace DwUtility
{
    namespace encryption
    {
        std::string DW_DWUTILITY_EXPORT AESEncrypt(std::string const& in,std::string const& key);
        std::string DW_DWUTILITY_EXPORT AESDecrypt(std::string const& in,std::string const& key);
		void DW_DWUTILITY_EXPORT AESDecrypt2(const char* in, int len, std::string const& key, std::string& out);
    }
}
