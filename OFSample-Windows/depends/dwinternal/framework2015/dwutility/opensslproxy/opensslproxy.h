#pragma once
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/blowfish.h>
#include <openssl/hmac.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
//#include <openssl/ssl.h>

#include <openssl/rc4.h>

#include "dwglobal.h"

namespace DwUtility
{
	class DW_DWUTILITY_EXPORT dwBaseFunc
	{
	public:
		//datareport
		//dwEVP_Digest
		static int	dwUtilityedgt(const void *data, size_t count,
			unsigned char *md, unsigned int *size, const EVP_MD *type, ENGINE *imp);
	#ifndef OPENSSL_NO_MD5
		//dwEVP_md5
		static const EVP_MD *dwUtilityem(void);
	#endif
		//dwAES_set_encrypt_key
		static int dwUtilityasek(const unsigned char *userKey, const int bits,
			AES_KEY *key);
		//dwAES_set_decrypt_key
		static int dwUtilityasdk(const unsigned char *userKey, const int bits,
			AES_KEY *key);
		//dwAES_cbc_encrypt
		static void dwUtilityace(const unsigned char *in, unsigned char *out,
			size_t length, const AES_KEY *key,
			unsigned char *ivec, const int enc);
		//dwRSA_free
		static void	dwUtilityrf (RSA *r);
		//dwRSA_size
		static int	dwUtilityrs(const RSA *);
		/* next 4 return -1 on error */
		//dwRSA_public_encrypt
		static int	dwUtilityrpe(int flen, const unsigned char *from,
			unsigned char *to, RSA *rsa,int padding);
		//dwRSA_private_decrypt
		static int	dwUtilityrpd(int flen, const unsigned char *from,
			unsigned char *to, RSA *rsa,int padding);
		//dwBIO_new
		static BIO *	dwUtilitybn(BIO_METHOD *type);
		//dwBIO_s_mem
		static BIO_METHOD *dwUtilitybsm(void);
		//dwBIO_write
		static int	dwUtilitybw(BIO *b, const void *data, int len);
		//dwPEM_read_bio_RSAPrivateKey
		static RSA* dwUtilityprbr(BIO *bp, RSA **x, pem_password_cb *cb, void *u);
		//dwBIO_free
		static int	dwUtilitybf(BIO *a);
		//dwPEM_read_bio_RSA_PUBKEY
		static RSA* dwUtilityprbrp(BIO *bp, RSA **x, pem_password_cb *cb, void *u);
		//dwCRYPTO_cleanup_all_ex_data
		static void dwUtilityccaed(void);

		//improtocol
		//dwRAND_bytes
		static int  dwUtilityrb(unsigned char *buf,int num);
		//dwRAND_seed
		static void dwUtilityrs(const void *buf,int num);
		/* Deprecated version */
	#ifndef OPENSSL_NO_DEPRECATED
		//dwRSA_generate_key
		static RSA *	dwUtilityrgk(int bits, unsigned long e,void
			(*callback)(int,int,void *),void *cb_arg);
	#endif /* !defined(OPENSSL_NO_DEPRECATED) */
		//dwRSA_check_key
		static int	dwUtilityrck(const RSA *);
		//static void	dwRSA_free (RSA *r);
		//dwBN_bn2bin
		static int	dwUtilitybb(const BIGNUM *a, unsigned char *to);
		//static int	dwRSA_private_decrypt(int flen, const unsigned char *from,
		//	unsigned char *to, RSA *rsa,int padding);
		//dwBF_set_key
		static void dwUtilitybsk(BF_KEY *key, int len, const unsigned char *data);
		//dwBF_cbc_encrypt
		static void dwUtilitybce(const unsigned char *in, unsigned char *out, long length,
			const BF_KEY *schedule, unsigned char *ivec, int enc);

		//netio
		//dwRC4_set_key
		static void dwUtilityrsk(RC4_KEY *key, int len, const unsigned char *data);
		//dwRC4
		static void dwUtilityr(RC4_KEY *key, size_t len, const unsigned char *indata,
			unsigned char *outdata);
		//dwMD5
		static unsigned char *dwUtilitym2(const unsigned char *d, size_t n, unsigned char *md);

		static void dwUtilityepf( EVP_PKEY *pkey );

		static unsigned char *dwUtilityhmac(const EVP_MD *evp_md, const void *key, int key_len,
			const unsigned char *d, size_t n, unsigned char *md,
			unsigned int *md_len );

		static const EVP_MD * dwUtilityesha1();

		static const EVP_MD * dwUtilityesha256();

		static int dwUtilityesf(EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s,
			EVP_PKEY *pkey);

		static int dwUtilityedu( EVP_MD_CTX *ctx,const void *d, size_t cnt );

		//EVP_DigestInit
		static int dwUtilityedi( EVP_MD_CTX *ctx, const EVP_MD *type );

		static int dwUtilityeps( EVP_PKEY *pkey );

		//CRYPTO_free
		static void dwUtilitycf( void* );

		//BIO_new_mem_buf
		static BIO* dwUtilitybnmf( void *buf, int len );

		//EVP_MD_CTX_cleanup
		static int dwUtilityemcc( EVP_MD_CTX *ctx );

		//EVP_VerifyFinal
		static int dwUtilityevf( EVP_MD_CTX *ctx,const unsigned char *sigbuf,
			unsigned int siglen,EVP_PKEY *pkey );

		//X509_get_pubkey
		static EVP_PKEY* dwUtilityxgp(X509 *x);

		//EVP_MD_CTX_init
		static void dwUtilityemci( EVP_MD_CTX *ctx );

		//EVP_DigestFinal
		static int dwUtilityedf( EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s );

		//EVP_MD_size
		static int dwUtilityems( const EVP_MD *md );

		//DW_PEM_read_bio_PrivateKey
		static EVP_PKEY *dwUtilityprbp( BIO *bp, EVP_PKEY **x, pem_password_cb *cb, void *u );

		//PEM_read_bio_PUBKEY
		static EVP_PKEY *dwUtilityprbpk( BIO *bp, EVP_PKEY **x, pem_password_cb *cb, void *u );

		//X509_free
		static void dwUtilityxf(X509 *a);

		//PEM_read_bio_X509
		static X509 *dwUtilityprbx(BIO *bp, X509 **x, pem_password_cb *cb, void *u);

		// int RSA_verify
		static int dwUtilityrv(int type, const unsigned char *m, unsigned int m_length,
			const unsigned char *sigbuf, unsigned int siglen, RSA *rsa);


		//RSA *PEM_read_bio_RSAPublicKey
		static RSA *dwUtilityprbrpk(BIO *bp, RSA **x, pem_password_cb *cb, void *u);
	};
}

#define DW_EVP_Digest DwUtility::dwBaseFunc::dwUtilityedgt
#ifndef OPENSSL_NO_MD5
#define DW_EVP_md5 DwUtility::dwBaseFunc::dwUtilityem
#endif
#define DW_AES_set_encrypt_key DwUtility::dwBaseFunc::dwUtilityasek
#define DW_AES_set_decrypt_key DwUtility::dwBaseFunc::dwUtilityasdk
#define DW_AES_cbc_encrypt DwUtility::dwBaseFunc::dwUtilityace
#define DW_RSA_free DwUtility::dwBaseFunc::dwUtilityrf
#define DW_RSA_size DwUtility::dwBaseFunc::dwUtilityrs
#define DW_RSA_public_encrypt DwUtility::dwBaseFunc::dwUtilityrpe
#define DW_RSA_private_decrypt DwUtility::dwBaseFunc::dwUtilityrpd
#define DW_BIO_new DwUtility::dwBaseFunc::dwUtilitybn
#define DW_BIO_s_mem DwUtility::dwBaseFunc::dwUtilitybsm
#define DW_BIO_write DwUtility::dwBaseFunc::dwUtilitybw
#define DW_PEM_read_bio_RSAPrivateKey DwUtility::dwBaseFunc::dwUtilityprbr
#define DW_BIO_free DwUtility::dwBaseFunc::dwUtilitybf
#define DW_PEM_read_bio_RSA_PUBKEY DwUtility::dwBaseFunc::dwUtilityprbrp
#define DW_CRYPTO_cleanup_all_ex_data DwUtility::dwBaseFunc::dwUtilityccaed
#define DW_RAND_bytes DwUtility::dwBaseFunc::dwUtilityrb
#define DW_RAND_seed DwUtility::dwBaseFunc::dwUtilityrs
#ifndef OPENSSL_NO_DEPRECATED
#define  DW_RSA_generate_key DwUtility::dwBaseFunc::dwUtilityrgk
#endif /* !defined(OPENSSL_NO_DEPRECATED) */
#define DW_RSA_check_key DwUtility::dwBaseFunc::dwUtilityrck
#define DW_BN_bn2bin DwUtility::dwBaseFunc::dwUtilitybb
#define DW_BF_set_key DwUtility::dwBaseFunc::dwUtilitybsk
#define DW_BF_cbc_encrypt DwUtility::dwBaseFunc::dwUtilitybce
#define DW_RC4_set_key DwUtility::dwBaseFunc::dwUtilityrsk
#define DW_RC4 DwUtility::dwBaseFunc::dwUtilityr
#define DW_MD5 DwUtility::dwBaseFunc::dwUtilitym2

#define DW_EVP_PKEY_free  DwUtility::dwBaseFunc::dwUtilityepf
#define DW_HMAC		DwUtility::dwBaseFunc::dwUtilityhmac
#define DW_EVP_SHA1  DwUtility::dwBaseFunc::dwUtilityesha1
#define DW_EVP_SHA256  DwUtility::dwBaseFunc::dwUtilityesha256
#define DW_EVP_SignFinal  DwUtility::dwBaseFunc::dwUtilityesf
#define DW_EVP_DigestUpdate  DwUtility::dwBaseFunc::dwUtilityedu
#define DW_EVP_DigestInit	DwUtility::dwBaseFunc::dwUtilityedi
#define DW_EVP_PKEY_size DwUtility::dwBaseFunc::dwUtilityeps
#define DW_CRYPTO_free		DwUtility::dwBaseFunc::dwUtilitycf
#define DW_BIO_new_mem_buf   DwUtility::dwBaseFunc::dwUtilitybnmf
#define DW_EVP_MD_CTX_cleanup DwUtility::dwBaseFunc::dwUtilityemcc
#define DW_EVP_VerifyFinal DwUtility::dwBaseFunc::dwUtilityevf
#define DW_X509_get_pubkey  DwUtility::dwBaseFunc::dwUtilityxgp
#define DW_EVP_MD_CTX_init  DwUtility::dwBaseFunc::dwUtilityemci
#define DW_EVP_DigestFinal	DwUtility::dwBaseFunc::dwUtilityedf
#define DW_EVP_MD_size  DwUtility::dwBaseFunc::dwUtilityems
#define DW_PEM_read_bio_PrivateKey  DwUtility::dwBaseFunc::dwUtilityprbp
#define DW_PEM_read_bio_PUBKEY	DwUtility::dwBaseFunc::dwUtilityprbpk
#define DW_X509_free		DwUtility::dwBaseFunc::dwUtilityxf
#define DW_PEM_read_bio_X509	DwUtility::dwBaseFunc::dwUtilityprbx
#define DW_RSA_verify DwUtility::dwBaseFunc::dwUtilityrv
#define DW_PEM_read_bio_RSAPublicKey DwUtility::dwBaseFunc::dwUtilityprbrpk
