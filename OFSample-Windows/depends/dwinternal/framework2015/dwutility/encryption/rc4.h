#include "dwglobal.h"
#pragma  once

/*
 * Encrypt some data using the supplied key..
 * The input and output buffers may be the same buffer.
 * Since RC4 is a stream cypher, this function is used
 * for both encryption and decryption.
 */

namespace DwUtility
{
    namespace encryption
    {
        void DW_DWUTILITY_EXPORT rc4_crypt(const char *inbuf, char *outbuf, size_t buflen, const char *key, size_t keylen);
    }
}
