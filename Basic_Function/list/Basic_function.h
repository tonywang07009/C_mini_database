#ifndef __LIST_API__
#define __LIST_API__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h> // for into the uint64_t

#include "../../Common_Function/Common_function.h"
typedef struct FsStats 
{
    size_t partition_size;   // bytes
    size_t block_size;       // bytes
    size_t total_blocks;
    size_t used_blocks;
    size_t file_blocks;
    size_t total_inodes;
    size_t used_inodes;
} FsStats;

typedef enum NodeType
{
    NODE_FILE,
    NODE_DIR

} NodeType;

/*for connet file and cloumn*/
typedef struct Node
{
    char name[512];
    NodeType type;
    struct Node *parent;  // Father node
    struct Node *child;   // The child node
    struct Node *sibling; // your next brother node
    struct FileMeta* file; // The type == node file
    /*This is extent space*/
} Node;

typedef struct FileMeta  //Meta :元
{
    uint32_t size;
    unsigned char Encrypt; // Futer work "the aes add"
    uint8_t key;             // for XOR encryption
    // char password_hash[128];
    char* content; //for dump and load

} FileMeta;

/*function list*/

// Remeber .c inside define
extern Node *g_root; // The extern is golobal var
                     // The queue management
extern Node *g_cwd;  // The child node

// The dispaly memnory space varable
extern size_t g_block_size;    
extern size_t g_total_blocks;  
extern size_t g_total_inodes;  

void file_sys_init(void);

int file_sys_mkdir(const char *name);
int file_sys_ls(const Node *nowdir);
int file_sys_cd(const char *path);
int file_sys_rmdir(const char *path);
int file_sys_rm(const char *name);
int file_sys_touch(const char *name);
int file_sys_put(const char* file_path, const char* dst_path);
int file_sys_get(const char *file_name, const char *password , uint8_t** out_buffer, size_t* out_size);
int file_sys_cat(const char *file_name, const char *password);
int file_sys_load(const char* dump_file);
void file_sys_state(void);
void file_sys_dump_dfs(Node* node , const char* parent_path ,FILE *fp);
void file_rule_display(const Node* current_dir);

// helper function
uint8_t derive_key(const char *password);
void xor_buffer(char *buf, int size, uint8_t key);

/*for get function*/
Node* find_in_dir(Node* dir_path ,const char* name, int want_file);
Node* Fs_resolve(const char* path, int want_file);
const char* get_basename(const char* path) ;


/*status display*/
static void fs_traverse(Node *node, FsStats *st);
void fs_get_stats(FsStats *st);
#endif