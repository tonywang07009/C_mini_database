#ifndef __LIST_API__
#define __LIST_API__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>  // for into the uint64_t

typedef enum NodeType
{
    NODE_FILE,
    NODE_DIR

}NodeType;

/*for connet file and cloumn*/
typedef struct Node
{
    char name [512];
    NodeType type;
    struct Node* parent; // Father node
    struct Node* child;  // The child node
    struct Node* sibling; //your next brother node
    /*This is extent space*/
}Node;


/*function list*/

// Remeber .c inside define
extern Node* g_root; //The extern is golobal var 
                     //The queue management
extern Node* g_cwd;  // The child node

void file_sys_init(void);
int file_sys_mkdir(const char* name);
#endif