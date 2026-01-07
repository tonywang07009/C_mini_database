#ifndef __AES_API__
#define __AES_API__

#include <stdint.h> 


struct FileMeta;   // forward declaration prevent the cycle include
typedef struct FileMeta FileMeta;

/*The key generate function */
void file_sys_derive_aes_key(const char* password, uint8_t out_key[32]);
void file_sys_generate_iv(uint8_t iv[16]);

/*The de&encrypt function */
int file_sys_encrypt_content(FileMeta *file_meta, const char *password);
int file_sys_decrypt_content(FileMeta *file_meta, const char *password);

#endif