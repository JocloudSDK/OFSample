#include "stable.h"

#include <openssl/sha.h>
#include "sha.h"

namespace DwUtility
{
	namespace encryption
	{
		std::string SHA(const std::string& pass)
		{
			unsigned char md[SHA_DIGEST_LENGTH];

			SHA1((unsigned char *)pass.data(), pass.size(), md);

			char bbb[SHA_DIGEST_LENGTH * 2 + 1];

			for (int i=0; i<SHA_DIGEST_LENGTH; i++)
				sprintf(&(bbb[i*2]),"%02x",md[i]);

			return std::string(bbb, SHA_DIGEST_LENGTH * 2);
		}
	}
}
