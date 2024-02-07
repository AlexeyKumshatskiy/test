#ifndef _AES_H_
#define _AES_H_


#include <stdint.h>
#include <string.h>


#define AES_BLOCKLEN        16    // Block length in bytes - AES is 128b block only

#define AES_KEY_SIZE_128    16    // 128 bits key size (16 bytes)
#define AES_KEY_SIZE_192    24    // 192 bits key size (24 bytes)
#define AES_KEY_SIZE_256    32    // 256 bits key size (32 bytes)


#define AES_KEY_EXP_SIZE    240   // Max key expansion size


struct AES_ctx
{
  uint8_t RoundKey[AES_KEY_EXP_SIZE];
  uint8_t BytesKeySize;
};



extern int AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key, uint8_t keySize);
extern void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
extern void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);










#endif 
