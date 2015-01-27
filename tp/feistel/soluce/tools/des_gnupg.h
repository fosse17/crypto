/****************************************************************************/
/* DES GnuPG                                                                */
/****************************************************************************/

#ifndef DES_GNUPG_H
#define DES_GNUPG_H

#define G10ERR_SELFTEST_FAILED 2
#define G10ERR_WRONG_KEYLEN 3
#define G10ERR_WEAK_KEY 4

#include "tools.h"

/*
 * Encryption/Decryption context of DES
 */
typedef struct _des_ctx
  {
    u32 encrypt_subkeys[32];
    u32 decrypt_subkeys[32];
  } 
des_ctx[1];

/*
 * Encryption/Decryption context of Triple-DES
 */
typedef struct _tripledes_ctx
  {
    u32 encrypt_subkeys[96];
    u32 decrypt_subkeys[96];
  }
tripledes_ctx[1];


void des_key_schedule (const byte *, u32 *);
int des_setkey (struct _des_ctx *, const byte *);
int des_ecb_crypt (struct _des_ctx *, const byte *, byte *, int);
int tripledes_set2keys (struct _tripledes_ctx *, const byte *, const byte *);
int tripledes_set3keys (struct _tripledes_ctx *, const byte *, const byte *, const byte *);
int tripledes_ecb_crypt (struct _tripledes_ctx *, const byte *, byte *, int);
int is_weak_key (const byte *key );
const char *selftest (void);

/*
 * Handy macros for encryption and decryption of data
 */
#define des_ecb_encrypt(ctx, from, to)		des_ecb_crypt(ctx, from, to, 0)
#define des_ecb_decrypt(ctx, from, to)		des_ecb_crypt(ctx, from, to, 1)
#define tripledes_ecb_encrypt(ctx, from, to)	tripledes_ecb_crypt(ctx, from, to, 0)
#define tripledes_ecb_decrypt(ctx, from, to)	tripledes_ecb_crypt(ctx, from, to, 1)

#endif
/****************************************************************************/

