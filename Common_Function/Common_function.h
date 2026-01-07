#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

#include "../Basic_Function/list/Basic_function.h"


#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

    #include <direct.h>
    /*The path symbol */
    // #define PATH_SEP   '\\'
     /*Autostore dir setting */
    #define DUMP_DIR   "Dump_store"
    #define LAST_DUMP  "Dump_store\\last_dump.dump"

#else

    #include <sys/stat.h>
    #include <sys/types.h>
    // #define PATH_SEP   '/'
    #define DUMP_DIR   "Dump_store"
    #define LAST_DUMP  "Dump_store/last_dump.txt"

#endif


typedef struct DiskNode 
{
    uint8_t  type;       // 0 = DIR, 1 = FILE
    uint8_t  encrypt;    // 0 or 1
    uint32_t size;       // file size, 0 for DIR
    uint16_t path_len;   // bytes of path (no '\0')

} DiskNode;

// void Com_free_subtree(Node* now_dir);

void Com_ensure_dump_dir(void);

#endif