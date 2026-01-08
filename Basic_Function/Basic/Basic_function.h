#ifndef __BASIC_API__
#define __BASIC_API__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h> // for uint64_t
#include <time.h>     // for srand, time


#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    /* The path symbol */
    // #define PATH_SEP    '\\'
    /* Autostore dir setting */
    #define DUMP_DIR   "Dump_store"
    #define DUMP_FIR   "last_dump.dump"
    #define LAST_DUMP  "Dump_store\\last_dump.dump"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    // #define PATH_SEP    '/'
    #define DUMP_DIR   "Dump_store"
    #define DUMP_FIR   "last_dump.dump"
    #define LAST_DUMP  "Dump_store/last_dump.dump"
#endif

/* 
 * File Metadata Structure 
 * Stores content and encryption details for a file node.
 */
typedef struct FileMeta 
{
    uint8_t aes_key[32];      // Derived key (runtime only, not saved to disk)
    uint8_t iv[16];           // Initialization Vector
    uint8_t encrypted;        // 0 = Plaintext, 1 = Encrypted
    size_t encrypted_size;    // Size of the encrypted content (with padding)
    uint32_t original_size;   // Original file size (plaintext size)
    char* content;            // Content buffer (stores plaintext or ciphertext)

} FileMeta;

typedef enum NodeType
{
    NODE_FILE,
    NODE_DIR
} NodeType;

/* 
 * File System Node Structure 
 * Represents a file or directory in the tree.
 */
typedef struct Node
{
    char name[512];
    NodeType type;
    struct Node *parent;   // Father node
    struct Node *child;    // The child node
    struct Node *sibling;  // Your next brother node
    struct FileMeta* file; // Valid only if type == NODE_FILE
} Node;

/* 
 * File System Statistics 
 */
typedef struct FsStats 
{
    size_t partition_size;   // Total bytes
    size_t block_size;       // Bytes per block
    size_t total_blocks;
    size_t used_blocks;
    size_t file_blocks;
    size_t total_inodes;
    size_t used_inodes;
} FsStats;

/* 
 * Disk Node Structure (For Dump/Load)
 * Header format for storing nodes in the dump file.
 */
typedef struct DiskNode 
{
    uint8_t  type;          // 0 = DIR, 1 = FILE
    uint8_t  encrypt;       // 0 or 1
    uint32_t size;          // Actual content size on disk (Encrypted size if encrypt=1)
    uint32_t original_size; // Original file size (0 for DIR)
    uint8_t  iv[16];        // IV for encryption (0 if not encrypted)
    uint16_t path_len;      // Length of the path string (no '\0')
} DiskNode;

/* Global variables */
extern Node *g_root;        // Root directory
extern Node *g_cwd;         // Current working directory
extern size_t g_block_size;
extern size_t g_total_blocks;
extern size_t g_total_inodes;

/** Function Prototypes **/

/* Basic Functions */
void file_sys_init(void);

int file_sys_mkdir(const char *name);
int file_sys_ls(const Node *nowdir);
int file_sys_cd(const char *path);
int file_sys_rmdir(const char *path);
int file_sys_rm(const char *name);
int file_sys_touch(const char *name);
int file_sys_put(const char* file_path, const char* dst_path);
int file_sys_get(const char *file_name, const char *password, uint8_t** out_buffer, size_t* out_size);
int file_sys_cat(const char *file_name, const char *password);
int file_sys_load(const char* dump_file);
void file_sys_state(void);

/* Helper Functions (Internal but exposed for tools) */
void file_sys_dump_dfs(Node* node, const char* parent_path, FILE *fp);
void file_sys_rule_display(const Node* current_dir);
Node* file_sys_in_dir(Node* dir_path, const char* name, int want_file);
Node* file_sys_resolve(const char* path, int want_file);
const char* file_sys_get_basename(const char* path);

/* Encryption Functions (AES wrappers) */
// Note: You need to implement these in Aes_function.c based on previous lessons
int file_sys_encrypt_content(FileMeta* file_meta, const char *password);
int file_sys_decrypt_content(FileMeta* file_meta, const char *password);

/* Dump Function */
void file_sys_ensure_dump_dir(void);

/* Status Display */
// Note: file_sys_traverse is static in .c, wrapped by file_sys_get_stats
void file_sys_get_stats(FsStats *st);

#endif
