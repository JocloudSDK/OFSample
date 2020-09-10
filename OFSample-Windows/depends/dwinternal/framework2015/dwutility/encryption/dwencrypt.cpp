#include "stable.h"

#include <assert.h>

#include "stdint.h"
#include "xxteacrypt.h"
#include "dwencrypt.h"
#include "blowfish.h"
#include "dwutility.h"

namespace DwUtility
{
	namespace encryption
	{
		std::string _XXTEAEncrypt( const std::string &data, const std::string &key);
		std::string _XXTEADecrypt( const std::string &data, const std::string &key);
		std::string TEAEncryptImpl( const char *data, int length, const std::string &key, int round );
		std::string TEADecryptImpl( const char *data, int length, const std::string &key, int round );

		const int KTEAVGranularitySize = sizeof(unsigned long) * 2;
		const int KTEAKeySize = sizeof(unsigned long) * 4;

		//////////////////////////////////////////////////////////////////////////

		class DWEncryptPrivate
		{
		public:
			DWEncryptPrivate();
			~DWEncryptPrivate();

			void init(DWEncrypt::Algorithm algorithm, const std::string &key, int round);

			DWEncrypt::Algorithm	m_algorithm;
			std::string				m_data;
			std::string				m_key;
			int						m_round;
		};

		DWEncryptPrivate::DWEncryptPrivate()
			: m_round(0)
			, m_algorithm(DWEncrypt::InvalidEncrypt)
		{

		}

		DWEncryptPrivate::~DWEncryptPrivate()
		{

		}

		void DWEncryptPrivate::init( DWEncrypt::Algorithm algorithm, const std::string &key, int round )
		{
			m_algorithm = algorithm;
			m_key = key;
			m_round = round;
		}

		//////////////////////////////////////////////////////////////////////////

		DWEncrypt::DWEncrypt( Algorithm algorithm, const std::string &key, int round )
		: d_ptr(new DwUtility::encryption::DWEncryptPrivate)
		{
			Q_D(DwUtility::encryption::DWEncrypt);
			d->init(algorithm, key, round);
		}

		DWEncrypt::~DWEncrypt()
		{
			DW_SAFE_DELETE(d_ptr);
		}

		void DWEncrypt::addData( const char *data, int length )
		{
			Q_D(DwUtility::encryption::DWEncrypt);
			d->m_data.append(data, length);
		}

		void DWEncrypt::addData( const std::string &data )
		{
			Q_D(DwUtility::encryption::DWEncrypt);
			d->m_data.append(data);
		}

		void DWEncrypt::reset( void )
		{
			Q_D(DWEncrypt);
			d->m_data.clear();
		}

		std::string DWEncrypt::result( void ) const
		{
			Q_D(const DWEncrypt);
			std::string result;
			switch (d->m_algorithm)
			{
			case TEAEncrypt:
				result = _TEAEncrypt(d->m_data, d->m_key, d->m_round);
				break;
			case TEADecrypt:
				result = _TEADecrypt(d->m_data, d->m_key, d->m_round);
				break;
			case BlowFishEncrypt:
				result = _BlowFishEncrypt(d->m_data, d->m_key, d->m_round);
				break;
			case XXTEAEncrypt:
				result = _XXTEAEncrypt(d->m_data, d->m_key);
				break;
			case XXTEADecrypt:
				result = _XXTEADecrypt(d->m_data, d->m_key);
				break;
			default:
				assert(false);
			}
			return result;
		}

		std::string DWEncrypt::_TEAEncrypt( const std::string &data, const std::string &key, int round ) const
		{
			std::string result;
			for ( std::string::size_type pos = 0; pos < data.size(); pos += KTEAVGranularitySize)
			{
				result.append(TEAEncryptImpl(data.c_str() + pos, data.length() - pos, key, round));
			}
			return result;
		}

		std::string DWEncrypt::_TEADecrypt( const std::string &data, const std::string &key, int round ) const
		{
			std::string result;
			for ( std::string::size_type pos = 0; pos < data.size(); pos += KTEAVGranularitySize )
			{
				result.append(TEADecryptImpl(data.c_str() + pos, data.length() - pos, key, round));
			}
			return result;
		}

		std::string TEAEncryptImpl( const char *data, int length, const std::string &key, int round )
		{
			std::string result;
			if (length < KTEAVGranularitySize)
			{
				result.append(data, length);
				return result;
			}
			//只支持16,32,64轮三种
			if ( 16 != round && 32 != round && 64 != round )
			{
				assert(false);
				result.append(data, length);
				return result;
			}

			unsigned long value_Long[2] = {0};
			memcpy(value_Long, data, length < KTEAVGranularitySize ? length : KTEAVGranularitySize);
			unsigned long key_Long[4] = {0};
			memcpy(key_Long, key.data(), key.length() < KTEAKeySize ? key.length() : KTEAKeySize);

			unsigned long y = value_Long[0], z = value_Long[1], sum = 0;
			unsigned long delta = 0x9e3779b9;                 /* a keyey schedule constant */
			unsigned long a = key_Long[0], b = key_Long[1], c = key_Long[2], d = key_Long[3];   /* cache keyey */

			for ( int i = 0; i < round; i++ ) 
			{                        /* basic cycle start */
				sum += delta;
				y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
				z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);/* end cycle */
			}

			value_Long[0]=y;
			value_Long[1]=z;

			result.append((const char *)value_Long, sizeof(value_Long));
			return result;
		}

		std::string TEADecryptImpl( const char *data, int length, const std::string &key, int round )
		{
			std::string result;
			if (length < KTEAVGranularitySize)
			{
				result.append(data, length);
				return result;
			}
			//只支持16,32,64轮三种
			if ( 16 != round && 32 != round && 64 != round )
			{
				assert(false);
				result.append(data, length);
				return result;
			}

			unsigned long value_Long[2] = {0};
			memcpy(value_Long, data, length < KTEAVGranularitySize ? length : KTEAVGranularitySize);
			unsigned long key_Long[4] = {0};
			memcpy(key_Long, key.data(), key.length() < KTEAKeySize ? key.length() : KTEAKeySize);
			
			unsigned long y = value_Long[0], z = value_Long[1], sum = 0xC6EF3720; /* set up */
			unsigned long delta = 0x9e3779b9;                  /* a key_Longey schedule constant */
			unsigned long a = key_Long[0], b = key_Long[1], c = key_Long[2], d = key_Long[3];    /* cache key_Longey */
			if ( 32 == round )
			{
				sum = 0xC6EF3720; /* delta << 5*/
			}
			else if ( 16 == round )
			{
				sum = 0xE3779B90; /* delta << 4*/
			}
			else
			{
				sum = delta * round;
			}

			for(int i = 0; i < round; i++) 
			{                            /* basic cycle start */
				z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
				y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
				sum -= delta;                                /* end cycle */
			}
			value_Long[0] = y;
			value_Long[1] = z;

			result.append((const char *)value_Long, sizeof(value_Long));
			return result;
		}

		std::string DWEncrypt::_BlowFishEncrypt( const std::string &data, const std::string &key, int /*round*/ ) const
		{
			CBlowFish oBlowFish((unsigned char*)key.c_str(), key.size());

			std::string inBuffer = data;

			// buffer长度必须是8的整数倍	
			size_t mod = data.size() % 8;
			if(mod != 0)
			{
				// 不足8的部分，追加'\0'
				inBuffer.append(8 - mod, '\0');
			}

			char* outBuffer = new char[inBuffer.size()];

			oBlowFish.Encrypt((unsigned char*)inBuffer.c_str(), (unsigned char*)outBuffer, inBuffer.size(), CBlowFish::CBC);

			std::string result(outBuffer, inBuffer.size());

			delete[] outBuffer;

			return result;
		}

		class XXTEA_UInt32Array
		{
		public:
			XXTEA_UInt32Array(const std::string &data, bool include_len, size_t minSize = 0) 
				: m_uintArray(NULL)
				, m_uintArraySize(0)
			{
				m_uintArraySize = (data.length() + 3) / 4;
				if(m_uintArraySize < minSize)
				{
					m_uintArraySize = minSize;
				}
				if(include_len)
				{
					m_uintArray = new uint32_t[m_uintArraySize + 1];
					memset(m_uintArray, 0, sizeof(uint32_t) * (m_uintArraySize + 1));
					m_uintArray[m_uintArraySize] = data.length();
					m_uintArraySize += 1;
				}
				else
				{
					m_uintArray = new uint32_t[m_uintArraySize];
					memset(m_uintArray, 0, sizeof(uint32_t) * m_uintArraySize);
				}

				memcpy(m_uintArray, data.data(), data.size());
			}

			~XXTEA_UInt32Array()
			{
				delete m_uintArray;
			}

			uint32_t *uintArray() const
			{
				return m_uintArray;
			}

			size_t uintArraySize() const
			{
				return m_uintArraySize;
			}

			std::string toString(bool include_len) const
			{
				size_t data_len = m_uintArraySize * sizeof(uint32_t);
				if(include_len)
				{
					data_len = m_uintArray[m_uintArraySize - 1];
				}

				return std::string((const char*)m_uintArray, data_len);
			}

		private:
			uint32_t *m_uintArray;
			size_t m_uintArraySize;
		};

		std::string _XXTEAEncrypt(const std::string &data, const std::string &key)
		{
			if(data.length() == 0)
			{
				return std::string();
			}

			XXTEA_UInt32Array v(data, true);
			XXTEA_UInt32Array k(key, false, 4);

			xxtea_encrypt(v.uintArray(), v.uintArraySize(), k.uintArray());

			return v.toString(false);
		}

		std::string _XXTEADecrypt(const std::string &data, const std::string &key)
		{
			if(data.length() == 0)
			{
				return std::string();
			}

			XXTEA_UInt32Array v(data, false);
			XXTEA_UInt32Array k(key, false, 4);

			xxtea_decrypt(v.uintArray(), v.uintArraySize(), k.uintArray());

			return v.toString(true);
		}
	}
}
