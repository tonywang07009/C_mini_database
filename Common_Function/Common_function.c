#include "Common_Function.h"
#include "../Basic_Function/list/Basic_function.h"

/* The recursion del function*/
// void free_subtree(Node *node) 
// {
//     if (!node) return;

//     // 先處理 child 和兄弟
//     free_subtree(node->child);
//     free_subtree(node->sibling);

//     // 如果是檔案，要先釋放 FileMeta 和 content
//     if (node->type == NODE_FILE && node->file != NULL)
//     {
//         if (node->file->content != NULL)
//         {
//             free(node->file->content);
//         }
//         free(node->file);
//     }

//     free(node);
// }

/*The Auto create file*/
void Com_ensure_dump_dir(void)
{
    #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        _mkdir(DUMP_DIR); // Fail if the dirtery is create 
    #else
         mkdir(DUMP_DIR,0755); // Return -1 if the dirtery is create 
    #endif

}
