#include "stable.h"
#include "rc4.h"

#ifndef u_char
typedef unsigned char u_char;
#endif

typedef struct rc4_state {
	u_char	perm[256];
	u_char	index1;
	u_char	index2;
}rc4_state;

__inline void swap_bytes(u_char *a, u_char *b)
{
	u_char temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * Initialize an RC4 state buffer using the supplied key,
 * which can have arbitrary length.
 */
void
rc4_init(struct rc4_state *const state, const u_char *key, size_t keylen)
{
	u_char j;
	size_t i;

	/* Initialize state with identity permutation */
	for (i = 0; i < 256; i++)
		state->perm[i] = (u_char)i; 
	state->index1 = 0;
	state->index2 = 0;
  
	/* Randomize the permutation using key data */
	for (j = 0, i = 0; i < 256; i++) {
		j += state->perm[i] + key[i % keylen]; 
		swap_bytes(&state->perm[i], &state->perm[j]);
	}
}

/*
 * Encrypt some data using the supplied RC4 state buffer.
 * The input and output buffers may be the same buffer.
 * Since RC4 is a stream cypher, this function is used
 * for both encryption and decryption.
 */

namespace DwUtility
{
	namespace encryption
	{
		void
		rc4_crypt(struct rc4_state *const state,
		const u_char *inbuf, u_char *outbuf, size_t buflen)
		{
			size_t i;
			u_char j;

			for (i = 0; i < buflen; i++) {

				/* Update modification indicies */
				state->index1++;
				state->index2 += state->perm[state->index1];

				/* Modify permutation */
				swap_bytes(&state->perm[state->index1],
					&state->perm[state->index2]);

				/* Encrypt/decrypt next byte */
				j = state->perm[state->index1] + state->perm[state->index2];
				outbuf[i] = inbuf[i] ^ state->perm[j];
			}
		}

		void rc4_crypt(const char *inbuf, char *outbuf, size_t buflen, const char *key, size_t keylen)
		{
			rc4_state state;

			rc4_init(&state, (u_char *)key, keylen);
			rc4_crypt(&state, (const u_char *)inbuf, (u_char *)outbuf, buflen);
		}
	}
}
