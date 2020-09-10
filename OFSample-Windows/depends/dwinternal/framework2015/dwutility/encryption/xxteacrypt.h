/*
 * crypto.h - Hearder file
 * Crypt or decrypt given block. XXTEA cipher is used.
 * Based on: 
 * David J. Wheeler and Roger M. Needham (October 1998). "Correction to XTEA".
 * Computer Laboratory, Cambridge University, England.
 * Author: Vlastimil Kosar <ikosar@fit.vutbr.cz> 
 */

#include <stdint.h>

/*
 * Decrypt block by XXTEA.
 * Params:
 *   block - block of encrypted data
 *   len   - length of block
 *   key   - 128b key
 */
void xxtea_decrypt(uint32_t *block, uint32_t len, uint32_t *key);

/*
 * Crypt block by XXTEA.
 * Params:
 *   block - block of input data
 *   len   - length of block
 *   key   - 128b key
 */
void xxtea_encrypt(uint32_t *block, uint32_t len, uint32_t *key);
