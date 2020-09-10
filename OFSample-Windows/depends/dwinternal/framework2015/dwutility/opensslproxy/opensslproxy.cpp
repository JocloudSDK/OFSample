#include "stable.h"

#include "dwutility/dwlocker.h"
#include "opensslproxy.h"

const char rnd_seed[] = "string to make the random number generator think it has entropy";

namespace DwUtility
{
	RSA *g_rsaKey = NULL;
	volatile LONG g_rsaKeyRef = 0;
	DwCSLock g_csRsaKey;

	int dwBaseFunc::dwUtilityedgt( const void *data, size_t count, unsigned char *md, unsigned int *size, const EVP_MD *type, ENGINE *impl )
	{
		return EVP_Digest(data, count, md, size, type, impl);
	}

	#ifndef OPENSSL_NO_MD5
	const EVP_MD * dwBaseFunc::dwUtilityem( void )
	{
		return EVP_md5();
	}
	#endif

	int dwBaseFunc::dwUtilityasek( const unsigned char *userKey, const int bits, AES_KEY *key)
	{
		return AES_set_encrypt_key(userKey, bits, key);
	}

	int dwBaseFunc::dwUtilityasdk( const unsigned char *userKey, const int bits, AES_KEY *key)
	{
		return AES_set_decrypt_key(userKey, bits, key);
	}

	void dwBaseFunc::dwUtilityace( const unsigned char *in, unsigned char *out, size_t length, const AES_KEY *key, unsigned char *ivec, const int enc)
	{
		AES_cbc_encrypt(in, out, length, key, ivec, enc);
	}

	void dwBaseFunc::dwUtilityrf( RSA *r)
	{
		DW_ASSERT(r != NULL);

		PERFLOG;

		DW_AUTO_LOCKER(&g_csRsaKey);

		if(r == g_rsaKey)
		{
			if(--g_rsaKeyRef == 0)
			{
				RSA_free(g_rsaKey);
				g_rsaKey = NULL;
			}
		}
		else
		{
			RSA_free(r);
		}
	}

	int dwBaseFunc::dwUtilityrs( const RSA *r)
	{
		DW_ASSERT(r != NULL);

		return RSA_size(r);
	}

	int dwBaseFunc::dwUtilityrpe( int flen, const unsigned char *from, unsigned char *to, RSA *rsa,int padding)
	{
		return RSA_public_encrypt(flen, from, to, rsa, padding);
	}

	int dwBaseFunc::dwUtilityrpd( int flen, const unsigned char *from, unsigned char *to, RSA *rsa,int padding)
	{
		return RSA_private_decrypt(flen, from, to, rsa, padding);
	}

	BIO * dwBaseFunc::dwUtilitybn( BIO_METHOD *type)
	{
		return BIO_new(type);
	}

	BIO_METHOD * dwBaseFunc::dwUtilitybsm( void )
	{
		return BIO_s_mem();
	}

	int dwBaseFunc::dwUtilitybw( BIO *b, const void *data, int len)
	{
		return BIO_write(b, data, len);
	}

	RSA* dwBaseFunc::dwUtilityprbr( BIO *bp, RSA **x, pem_password_cb *cb, void *u)
	{
		return PEM_read_bio_RSAPrivateKey(bp, x, cb, u);
	}

	int dwBaseFunc::dwUtilitybf( BIO *a)
	{
		return BIO_free(a);
	}

	RSA* dwBaseFunc::dwUtilityprbrp( BIO *bp, RSA **x, pem_password_cb *cb, void *u)
	{
		return PEM_read_bio_RSA_PUBKEY(bp, x, cb, u);
	}

	void dwBaseFunc::dwUtilityccaed( void )
	{
		CRYPTO_cleanup_all_ex_data();
	}

	int dwBaseFunc::dwUtilityrb( unsigned char *buf,int num)
	{
		return RAND_bytes(buf, num);
	}

	void dwBaseFunc::dwUtilityrs( const void *buf,int num)
	{
		RAND_seed(buf, num);
	}

	#ifndef OPENSSL_NO_DEPRECATED
	RSA * dwBaseFunc::dwUtilityrgk( int bits, unsigned long e,void (*callback)(int,int,void *),void *cb_arg)
	{
		PERFLOG;
		DW_AUTO_LOCKER(&g_csRsaKey);

		if(bits == 0 && e == 0 && callback == NULL && cb_arg == NULL)
		{
			if(g_rsaKeyRef == 0)
			{

				DW_RAND_seed(rnd_seed, sizeof rnd_seed); 	

				g_rsaKey =  RSA_generate_key(512, 3, NULL, NULL);
				while(RSA_check_key(g_rsaKey) != 1)
				{
					RSA_free(g_rsaKey);
					g_rsaKey = RSA_generate_key(512, 3, NULL, NULL);
				}
			}
			g_rsaKeyRef++;
			return g_rsaKey;
		}
		else
		{
			return RSA_generate_key(bits, e, callback, cb_arg);
		}
	}
	#endif

	int dwBaseFunc::dwUtilityrck( const RSA * r)
	{
		return 	RSA_check_key(r);
	}

	int dwBaseFunc::dwUtilitybb( const BIGNUM *a, unsigned char *to)
	{
		return BN_bn2bin(a, to);
	}

	void dwBaseFunc::dwUtilitybsk( BF_KEY *key, int len, const unsigned char *data)
	{
		BF_set_key(key, len, data);
	}

	void dwBaseFunc::dwUtilitybce( const unsigned char *in, unsigned char *out, long length, const BF_KEY *schedule, unsigned char *ivec, int enc)
	{
		BF_cbc_encrypt(in, out, length, schedule, ivec, enc);
	}

	void dwBaseFunc::dwUtilityrsk( RC4_KEY *key, int len, const unsigned char *data)
	{
		RC4_set_key(key, len, data);
	}

	void dwBaseFunc::dwUtilityr( RC4_KEY *key, size_t len, const unsigned char *indata, unsigned char *outdata)
	{
		RC4(key, len, indata, outdata);
	}

	unsigned char * dwBaseFunc::dwUtilitym2( const unsigned char *d, size_t n, unsigned char *md)
	{
		return MD5(d, n, md);
	}

	void dwBaseFunc::dwUtilityepf( EVP_PKEY *pkey )
	{
		EVP_PKEY_free( pkey );
	}

	unsigned char* dwBaseFunc::dwUtilityhmac(const EVP_MD *evp_md, const void *key, int key_len,
						  const unsigned char *d, size_t n, unsigned char *md,
						  unsigned int *md_len )
	{
		return HMAC(evp_md, key, key_len, d, n, md, md_len );
	}

	const EVP_MD * dwBaseFunc::dwUtilityesha1()
	{
		return EVP_sha1();
	}

	const EVP_MD * dwBaseFunc::dwUtilityesha256()
	{
		return EVP_sha256();
	}

	int dwBaseFunc::dwUtilityesf(EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s,
					 EVP_PKEY *pkey)
	{
		return EVP_SignFinal( ctx, md, s,  pkey );
	}	

	int dwBaseFunc::dwUtilityedu( EVP_MD_CTX *ctx,const void *d, size_t cnt )
	{
		return	EVP_DigestUpdate( ctx, d, cnt );
	}

	int dwBaseFunc::dwUtilityedi( EVP_MD_CTX *ctx, const EVP_MD *type )
	{
		return EVP_DigestInit( ctx, type );
	}

	int	dwBaseFunc::dwUtilityeps( EVP_PKEY *pkey )	
	{
		return EVP_PKEY_size( pkey );
	}

	void dwBaseFunc::dwUtilitycf( void* addr )
	{
		CRYPTO_free( addr );
	}

	BIO* dwBaseFunc::dwUtilitybnmf( void *buf, int len )
	{
		return BIO_new_mem_buf( buf, len );
	}

	int dwBaseFunc::dwUtilityemcc( EVP_MD_CTX *ctx )
	{
		return EVP_MD_CTX_cleanup( ctx );
	}

	int dwBaseFunc::dwUtilityevf( EVP_MD_CTX *ctx,const unsigned char *sigbuf,
					 unsigned int siglen,EVP_PKEY *pkey )
	{
		return EVP_VerifyFinal( ctx, sigbuf, siglen, pkey );
	}

	EVP_PKEY* dwBaseFunc::dwUtilityxgp(X509 *x)
	{
		return X509_get_pubkey( x );
	}

	void dwBaseFunc::dwUtilityemci( EVP_MD_CTX *ctx )
	{
		return EVP_MD_CTX_init( ctx );
	}

	int dwBaseFunc::dwUtilityedf( EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s )
	{
		return EVP_DigestFinal( ctx, md, s );
	}

	int dwBaseFunc::dwUtilityems( const EVP_MD *md )
	{
		return EVP_MD_size( md );
	}

	EVP_PKEY *dwBaseFunc::dwUtilityprbp( BIO *bp, EVP_PKEY **x, pem_password_cb *cb, void *u )
	{
		return PEM_read_bio_PrivateKey( bp, x, cb, u );
	}

	EVP_PKEY *dwBaseFunc::dwUtilityprbpk( BIO *bp, EVP_PKEY **x, pem_password_cb *cb, void *u )
	{
		return PEM_read_bio_PUBKEY( bp, x, cb, u );
	}

	void dwBaseFunc::dwUtilityxf(X509 *a)
	{
		X509_free(a);
	}

	X509* dwBaseFunc::dwUtilityprbx(BIO *bp, X509 **x, pem_password_cb *cb, void *u)
	{
		return PEM_read_bio_X509(bp, x, cb, u);
	}

	int dwBaseFunc::dwUtilityrv(int type, const unsigned char *m, unsigned int m_length,
			const unsigned char *sigbuf, unsigned int siglen, RSA *rsa)
	{
		return RSA_verify(type, m, m_length, sigbuf, siglen, rsa);
	}

	RSA *dwBaseFunc::dwUtilityprbrpk(BIO *bp, RSA **x, pem_password_cb *cb, void *u)
	{
		return PEM_read_bio_RSAPublicKey(bp, x, cb ,u);
	}
}
