#include "Common_Function.h"

/* The recursion del function*/
void free_subtree(Node *node) 
{
    if (!node) return;

    // 先處理 child 和兄弟
    free_subtree(node->child);
    free_subtree(node->sibling);

    // 如果是檔案，要先釋放 FileMeta 和 content
    if (node->type == NODE_FILE && node->file != NULL)
    {
        if (node->file->content != NULL)
        {
            free(node->file->content);
        }
        free(node->file);
    }

    free(node);
}
