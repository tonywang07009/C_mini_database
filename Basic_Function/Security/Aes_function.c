#include "Aes_function.h"
#include "../Basic/Basic_function.h"
#include "../../Additional_Function/tiny-AES-c/aes.h" 
#include <string.h> 
#include <time.h> // for ramdom seed
#include <stdlib.h>

/*The create random key */
void file_sys_derive_aes_key(const char* password, uint8_t out_key[32])
{
    size_t len = strlen(password); // The calculate
    memset(out_key,0,32); // The 32 byte

    for(int round = 0 ; round < 4; round++) /*The random set the key order
                                              like roller washing meaching.  
                              */ 
    {
        for(size_t i =0 ;  i < len; i++)
        {
            out_key[i%32] ^=  (uint8_t)(password[i] + round * 31); // The bit reverse + the position offset
            out_key[(i*7 + round) % 32] += ((uint8_t)password[i]); // The random position + the add the (key_content_value + key_content_size)
        }
    }

}

/*generate iv key*/
void file_sys_generate_iv(uint8_t iv[16])
{
    static int seeded = 0; // The golobal key

    /*The random number for now time*/
    if (!seeded) 
    {
        srand((unsigned)time(NULL)); //Pseudo-random number generator 
                                     // The seed is used the now time
        seeded = 1;
    }

    /*The random key generate*/
    for (int i = 0; i < 16; ++i) // 16 is fit 16 byte
    {
        iv[i] = (uint8_t)(rand() & 0xFF); // rand & 0xFF -> only get the index(0~7) bits 
                                          // byte
    }

}

/**/
int file_sys_encrypt_content(FileMeta* file_meta, const char *password)
{
    /* The unexpect handle*/
    if(!file_meta || !file_meta->content || file_meta->original_size==0)
    {
        printf("Encrypt ERROR : nothing to input (file_meta=%p)\n", (void*)file_meta);
        return -1;
    }

    /* 1 obtian the per-file key + iv*/
    file_sys_derive_aes_key(password , file_meta->aes_key);
    file_sys_generate_iv(file_meta->iv);

    /* 2 Calculate padding size long */
    size_t originl = file_meta->original_size;
    size_t padding = AES_BLOCKLEN - (originl % AES_BLOCKLEN);
    
    if(padding == 0)
    {
        padding = AES_BLOCKLEN;
    }

    size_t encrypt_size = originl + padding;

    uint8_t* buffer = (uint8_t*)malloc(encrypt_size);
    if(!buffer)
    {
        printf("Encrypt ERROR : Encrypt buffer create fail ");
        return -1;
    }

    /*3 Duplication */
    memcpy(buffer,file_meta->content,originl);
    memset((buffer + originl),(uint8_t)padding,padding);

    /*4 Call AES-C API to CBC encrypt */
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx,file_meta->aes_key,file_meta->iv);
    AES_CBC_encrypt_buffer(&ctx,buffer,encrypt_size);

    /*5 Relpace the  origin content */
    free(file_meta->content);
    file_meta->content = (char*)buffer;
    file_meta->encrypted_size = encrypt_size;
    file_meta->encrypted = 1;

    printf(" The Encrypt Sucess\n ");
    return 0;
}

int file_sys_decrypt_content(FileMeta *file_meta, const char *password)
{
     if (!file_meta || !file_meta->content || !file_meta->encrypted) 
     {
        printf("Decrypt ERROR : nothing to input (file_meta=%p)\n", (void*)file_meta);
        return -1;
     }

     /* 1. used the same key deriver to bulid the same key*/
     uint8_t key[32];
     file_sys_derive_aes_key(password,key);

     struct AES_ctx ctx;
     AES_init_ctx_iv(&ctx, key, file_meta->iv);
     AES_CBC_decrypt_buffer(&ctx, (uint8_t *)file_meta->content, file_meta->encrypted_size);

     /* 2. Remove padding*/
     uint8_t padding = ((uint8_t *)file_meta->content)[(file_meta->encrypted_size - 1)];
     if (padding == 0 || padding > AES_BLOCKLEN) 
     {
            printf("Decrypt ERROR : invalid padding\n");
            return -1;
     }   

      size_t  originl = (file_meta->encrypted_size - padding);

      file_meta->original_size  = originl;   
      file_meta->encrypted_size = originl;
      file_meta->encrypted = 0;

      printf(" The Decrypt Sucess\n ");
      return 0;
}