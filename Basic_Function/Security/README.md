// ...existing code...
# Security Module (AES Functions)

----
## 1. Introduction
This module implements data security features for the in-memory mini file system.  
It provides functions to __derive an AES-256 key from a password__, __generate an IV__, and __encrypt/decrypt file content using AES-CBC__.

----
## 2. Security logic & algorithms

- __Key derivation (file_sys_derive_aes_key)__
  - Converts a __user password__ (variable length) into a __fixed 32-byte key__.
  - Uses a __custom mixing loop (4 rounds) combining XOR__ and __addition to produce 32 bytes__.

- __IV generation (file_sys_generate_iv)__
  - Produces a 16-byte IV using rand() __seeded by time(NULL)__.
  - Ensures identical __plaintext encrypts to different ciphertexts when IV differs+__.

- PKCS#7 padding
  - Pads __plaintext to a 16-byte AES block: padding = 16 - (size % 16)__.
  - Each padding byte equals the padding length.

----
## 3. API overview

| Function | Parameters | Return | Role |
|---|---|---:|---|
| `file_sys_derive_aes_key()` | `const char *password, uint8_t out_key[32]` | `void` | Produce a 32-byte AES key from password. |
| `file_sys_generate_iv()` | `uint8_t iv[16]` | `void` | Generate 16-byte IV (time-seeded rand). |
| `file_sys_encrypt_content()` | `FileMeta *file_meta, const char *password` | `int` | Derive key/IV, pad and AES-CBC encrypt data in-place; update FileMeta (encrypted flag, sizes, iv). |
| `file_sys_decrypt_content()` | `FileMeta *file_meta, const char *password` | `int` | Derive key, AES-CBC decrypt, validate/remove padding, restore original size. |



----
## 4. Dependencies
- __tiny-AES-c (aes.h / aes.c) — AES implementation used__ for block operations.
- __FileMeta struct — holds content pointer, original/encrypted sizes, encrypted flag, and stored IV.


----
## 5. Security notes (important)
- The custom key derivation and IV generation (srand/time + rand) __are NOT cryptographically secure__. __Use a standard KDF (e.g., PBKDF2, Argon2) and a cryptographically secure RNG for production code__.

