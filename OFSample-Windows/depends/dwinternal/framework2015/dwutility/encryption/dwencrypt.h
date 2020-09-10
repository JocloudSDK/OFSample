#pragma once

#include <string>

#include "dwglobal.h"

namespace DwUtility
{
    namespace encryption
    {
        class DWEncryptPrivate;
        class DW_DWUTILITY_EXPORT DWEncrypt
        {
            friend class DWEncryptPrivate;

        public:
            enum Algorithm
            {
                TEAEncrypt,
                TEADecrypt,
                BlowFishEncrypt,
				XXTEAEncrypt,
				XXTEADecrypt,
				InvalidEncrypt = 0xFFFF
            };

            DWEncrypt( Algorithm algorithm, const std::string &key, int round );
            ~DWEncrypt();

            void addData( const char *data, int length );
            void addData( const std::string &data );
            void reset( void );
            std::string result( void ) const;

        private:
            std::string _TEAEncrypt( const std::string &data, const std::string &key, int round ) const;
            std::string _TEADecrypt( const std::string &data, const std::string &key, int round ) const;
            std::string _BlowFishEncrypt( const std::string &data, const std::string &key, int round ) const;

        protected:
            DWEncryptPrivate	*d_ptr;
        };
    }
}
