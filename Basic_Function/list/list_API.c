#include "list_API.h"

Node *g_root = NULL;   // define for here
Node *g_cwd  = NULL;

void file_sys_init(void)
{
    g_root = (Node*)malloc(sizeof(Node));
    if(g_root == NULL)
    {
        printf("The root create fail \n");
        return ;
    }

    strcpy(g_root->name,"/"); // The file for inside
    g_root->type = NODE_DIR;
    g_root->parent = NULL;
    g_root->child = NULL;
    g_root->sibling = NULL;
    g_cwd = g_root;
}

int file_sys_mkdir(const char* name)
{
      if (!name || !*name) {
        printf("Invalid directory name.\n");
        return -1;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        printf("The mkdir create fail.\n");
        return -1;
    }

    // init_newnode
    strncpy(new_node->name, name, (sizeof(new_node->name) - 1));
    new_node->name[sizeof(new_node->name) - 1] = '\0'; // The end symbol
    new_node->type    = NODE_DIR;
    new_node->parent  = g_cwd;
    new_node->child   = NULL;
    new_node->sibling = NULL;

    // 掛到 g_cwd 的 children/sibling 鏈上
    if (g_cwd->child == NULL) 
    {
        g_cwd->child = new_node;
    } 
    else 
    {
        Node *current = g_cwd->child;
        while (current->sibling != NULL)  //The mkdir like samnode build tree.
        {
            current = current->sibling;
        }
        
        current->sibling = new_node; // now dir next build the new one
    }

    return 0;
}