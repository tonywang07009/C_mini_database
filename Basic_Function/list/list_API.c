#include "list_API.h"

Node *g_root = NULL; // define for here
Node *g_cwd = NULL;

/*
    1. 整理共同函式 抽成小工具
    2. clean code  設計
    3. makefile 加

*/

void file_sys_init(void)
{
    g_root = (Node *)malloc(sizeof(Node));
    if (g_root == NULL)
    {
        printf("The root create fail \n");
        return;
    }

    strcpy(g_root->name, "/"); // The file for inside
    g_root->type = NODE_DIR;
    g_root->parent = NULL;
    g_root->child = NULL;
    g_root->sibling = NULL;
    g_cwd = g_root; // now i'm staying node
}


int file_sys_mkdir(const char *name)
{
    if (!name || !*name) // name pointer address *name -> content
    {
        printf("Invalid directory name.\n");
        return -1;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        printf("The mkdir create fail.\n");
        return -1;
    }

    // init_newnode
    strncpy(new_node->name, name, (sizeof(new_node->name) - 1)); // The name dupilcation
    new_node->name[sizeof(new_node->name) - 1] = '\0';           // The end symbol
    new_node->type = NODE_DIR;
    new_node->parent = g_cwd;
    new_node->child = NULL;
    new_node->sibling = NULL;

    // handon the child chain
    if (g_cwd->child == NULL)
    {
        g_cwd->child = new_node;
    }
    else
    {
        Node *current = g_cwd->child;
        while (current->sibling != NULL) // The mkdir like samnode build tree.
        {
            current = current->sibling;
        }

        current->sibling = new_node; // now dir next build the new one
    }
    return 0;
}

int file_sys_ls(Node *nowdir)
{
    if (nowdir == NULL)
    {
        printf("fail_dir.\n");
        return -1;
    }

    Node *current = nowdir->child; // print the layer all child node
    while (current != NULL)
    {
        printf("%s ", current->name);
        current = current->sibling;
    }
    printf("\n");
    return 0;
}


int file_sys_cd(const char *path)
{
    /*cd ".." */
    if (strcmp(path, "..") == 0)
    {
        if (g_cwd->parent)
        {
            g_cwd = g_cwd->parent;
        }
        return 0;
    }

    /* cd "/" */
    if (strcmp(path, "/") == 0) // strcmp 比較字串內容是否相同
    {
        g_cwd = g_root;
        return 0;
    }

    /* cd "name" */
    Node *current = g_cwd->child;
    while (current != NULL)
    {
        if (current->type == NODE_DIR && strcmp(current->name, path) == 0)
        {
            g_cwd = current;
            return 0;
        }
        current = current->sibling;
    }

    return -1; // can find't it
}



int file_sys_rmdir(const char *path)
{
    Node *prev = NULL;
    Node *current = g_cwd->child;
    while (current != NULL)
    {
        if (current->type == NODE_DIR && strcmp(current->name, path) == 0)
        {
            break;
        }

        prev = current;
        current = current->sibling; // nextp
    }

    if (current == NULL)
    {
        printf("rmdir: no such directory: %s\n", path);
        return -1;
    }
    printf("found target %s\n", current->name);

    if (current->child != NULL)
    {
        printf("can't rm : you have child layer .");
    }

    if (prev == NULL)
    {
        g_cwd->child = current->sibling; // update the child postion
    }
    else
    {
        prev->sibling = current->sibling; // update postion.
    }

    free(current);
    return 0;
}

int file_sys_rm(const char *name)
{
    Node *prev = NULL;
    Node *current = g_cwd->child;

    while (current!=NULL)
    {
        if(current->type == NODE_FILE && strcmp(current->name, name) == 0)
        {
            break; // the find del file
        }
        prev = current;
        current = current->sibling;
    }

        if (current == NULL) 
        {
            printf("rm: no such file: %s\n", name);
            return -1;
        }

        printf("found target file %s\n", current->name);
        if(current->file != NULL)
        {
            if(current->file->content != NULL)
            {
                free(current->file->content);
            }
            free(current->file);
        }

        if (prev == NULL)
        {
            g_cwd->child = current->sibling; // update the child postion
        }
        else
        {
            prev->sibling = current->sibling; // update postion.
        }
        
        free(current);
        printf("free sussful");

        return 0;
}



int file_sys_touch(const char *name)
{
    
    if (!name || !*name) // name pointer address *name -> content
    {
        printf("Invalid file name.\n");
        return -1;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        printf("The touch create file fail.\n");
        return -1;
    }

    // init_newnode
    strncpy(new_node->name, name, (sizeof(new_node->name) - 1)); // The name dupilcation
    new_node->name[sizeof(new_node->name) - 1] = '\0';           // The end symbol
    new_node->type = NODE_FILE;
    new_node->parent = g_cwd;
    new_node->child = NULL;
    new_node->sibling = NULL;

    if(new_node->type == NODE_FILE)
    {
        FileMeta* file = (FileMeta*)malloc(sizeof(FileMeta));
        if(file == NULL)
        {
            printf("file node create fail.\n");
            return -1;
        }
        new_node->file = file;
        new_node->file->content =NULL;
        new_node->file->Encrypt= 0;
        new_node->file->size=0;
    }

    // handon the child chain
    if (g_cwd->child == NULL)
    {
        g_cwd->child = new_node;
    }
    else
    {
        Node *current = g_cwd->child;
        while (current->sibling != NULL) // The mkdir like samnode build tree.
        {
            current = current->sibling;
        }

        current->sibling = new_node; // now dir next build the new one
    }
    return 0;
}

void file_dump_dfs(Node* node , const char* parent_path ,FILE *fp)
{
    if(node == NULL)
    {
        return;
    }

    char path[2048]; // The buffer for store dump file

    /*放入 path 空間*/
    if(node == g_root)
    {
        strcpy(path,"/"); // for root
    }
    else //組檔案路徑
    {
        snprintf(path,sizeof(path), "%s/%s",parent_path,node->name);
        // The display file path name
        // Need research snprinft
    }

    if(node->type == NODE_DIR)
    {
        fprintf(fp,"D %s \n",path);
    }
    else if (node->type == NODE_FILE)
    {
        int size =(node->file? node->file->size : 0);
        fprintf(fp,"F %s the file size : %d \n",path,size); //file print
    }
    /* 遞迴 code review*/

    file_dump_dfs(node->child,path,fp);
    file_dump_dfs(node->sibling, parent_path,fp);   // 兄弟節點共用原來的 parent_path

}

int file_sys_load(const char *dump_file)
{
    FILE *fp = fopen(dump_file, "r");
    if (!fp) {
        printf("miss dump file %s\n", dump_file);
        return -1;
    }

    char line[2048];

    while (fgets(line, sizeof(line), fp)) {
        char type = '0';
        char path[1024] = {0};

        if (sscanf(line, " %c %1023s", &type, path) != 2) {
            continue;
        }

        if (type == 'D') {
            if (strcmp(path, "/") == 0) {
                continue;   // root already exists
            }

            char tmp[1024];
            strncpy(tmp, path, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            char *tmpPtr = tmp;
            if (*tmpPtr == '/') 
            {
                tmpPtr++;
            }

            Node *saved_cwd = g_cwd;
            g_cwd = g_root;

            char *token = strtok(tmpPtr, "/");
            while (token != NULL) 
            {
                // find the same name clumn
                Node *current = g_cwd->child;
                Node *found   = NULL;

                while (current != NULL) 
                {
                    if (current->type == NODE_DIR &&
                        strcmp(current->name, token) == 0)
                        {
                            found = current;
                            break;
                        }

                    current = current->sibling;   // 這行別忘記
                }

                if (!found) 
                {
                    // no find do the mkdir new one
                    if (file_sys_mkdir(token) != 0) 
                        {
                            printf("load: mkdir fail on %s\n", token);
                            break;
                        }

                    current = g_cwd->child;

                    while (current->sibling != NULL) 
                    {
                        current = current->sibling;
                    }

                    found = current;
                }
                    // into this clumn
                    g_cwd = found;
                    token = strtok(NULL, "/");
            }

            g_cwd = saved_cwd;
        }
        else if (type == 'F') 
        {
            if (strcmp(path, "/") == 0) {
                continue;   // root already exists
            }

            char tmp[1024];
            strncpy(tmp, path, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            char *tmpPtr = tmp;
            if (*tmpPtr == '/') 
            {
                tmpPtr++;
            }

            Node *saved_cwd = g_cwd;
            g_cwd = g_root;

            char *last_slash = strrchr(tmpPtr, '/');
            char *filename   = NULL;

            
            if (last_slash) 
            {
                *last_slash = '\0';
                filename = last_slash + 1;
            } 
            else 
            {
                filename = tmpPtr;
                tmpPtr = NULL;
            }

            if (tmpPtr && *tmpPtr != '\0') 
            {
                char *token = strtok(tmpPtr, "/");
                while (token != NULL) 
                {
                // find the same name clumn
                    Node *current = g_cwd->child;
                    Node *found   = NULL;

                    while (current != NULL) 
                    {
                        if (current->type == NODE_DIR &&
                            strcmp(current->name, token) == 0)
                            {
                                found = current;
                                break;
                            }

                        current = current->sibling;   // 這行別忘記
                    }

                    if (!found) 
                    {
                        // no find do the mkdir new one
                        if (file_sys_mkdir(token) != 0) 
                            {
                                printf("load: mkdir fail on %s\n", token);
                                break;
                            }

                        current = g_cwd->child;
                        while (current->sibling != NULL) 
                            {
                                current = current->sibling;
                            }

                        found = current;

                    }
                        // into this clumn
                        g_cwd = found;
                        token = strtok(NULL, "/");

                }
            }

            file_sys_touch(filename);
            g_cwd = saved_cwd;
        }
    }

    fclose(fp);
    return 0;
}



uint8_t derive_key(const char *password)
{
    if (!password || !*password)
    {
        return 0x5A;  // default key
    }

    uint8_t key = 0;
    while (*password)
    {
        key ^= (uint8_t)(*password);
        password++;
    }
    
    return key ? key : 0x5A;  // avoid key = 0
}

void xor_buffer(char *buf, int size, uint8_t key)
{
    if (!buf || size <= 0)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        buf[i] ^= key;
    }
}

int file_sys_put(const char *path, const char *file_name, const char *password)
{
    if (!path || !*path || !file_name || !*file_name)
    {
        printf("Invalid path or filename.\n");
        return -1;
    }

    // Step 1: find or create file node
    Node *target = NULL;
    Node *current = g_cwd->child;
    
    while (current != NULL)
    {
        if (current->type == NODE_FILE && strcmp(current->name, file_name) == 0)
        {
            target = current;
            break;
        }
        current = current->sibling;
    }

    // if not found, create new file
    if (target == NULL)
    {
        if (file_sys_touch(file_name) != 0)
        {
            printf("put: failed to create file %s\n", file_name);
            return -1;
        }
        
        // find the new created file
        current = g_cwd->child;
        while (current != NULL)
        {
            if (current->type == NODE_FILE && strcmp(current->name, file_name) == 0)
            {
                target = current;
                break;
            }
            current = current->sibling;
        }
    }

    if (!target || !target->file)
    {
        printf("put: internal error\n");
        return -1;
    }

    // Step 2: read file content from host path
    FILE *fp = fopen(path, "rb");
    if (!fp)
    {
        printf("put: cannot open file %s\n", path);
        return -1;
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size <= 0)
    {
        printf("put: file is empty or error\n");
        fclose(fp);
        return -1;
    }

    // free old content if exists
    if (target->file->content != NULL)
    {
        free(target->file->content);
    }

    // allocate new buffer
    target->file->content = (char*)malloc(file_size);
    if (!target->file->content)
    {
        printf("put: memory allocation failed\n");
        fclose(fp);
        return -1;
    }

    // read content
    size_t read_size = fread(target->file->content, 1, file_size, fp);
    fclose(fp);

    if (read_size != file_size)
    {
        printf("put: read file error\n");
        free(target->file->content);
        target->file->content = NULL;
        return -1;
    }

    target->file->size = file_size;

    // Step 3: encrypt if password provided
    if (password && *password)
    {
        uint8_t key = derive_key(password);
        xor_buffer(target->file->content, target->file->size, key);
        target->file->key = key;
        target->file->Encrypt = 1;
    }
    else
    {
        target->file->Encrypt = 0;
        target->file->key = 0;
    }

    printf("put: success, file size = %d bytes\n", target->file->size);
    return 0;
}

int file_sys_get(const char* file_name, const char* path, const char *password)
{
    if (!file_name || !*file_name || !path || !*path)
    {
        printf("get: invalid filename or path\n");
        return -1;
    }

    // Step 1: find file node
    Node *current = g_cwd->child;
    Node *target = NULL;

    while (current != NULL)
    {
        if (current->type == NODE_FILE && strcmp(current->name, file_name) == 0)
        {
            target = current;
            break;
        }
        current = current->sibling;
    }

    if (!target)
    {
        printf("get: no such file: %s\n", file_name);
        return -1;
    }

    if (!target->file || !target->file->content || target->file->size <= 0)
    {
        printf("get: file is empty\n");
        return -1;
    }

    // Step 2: check password if Encrypt
    if (target->file->Encrypt)
    {
        if (!password || !*password)
        {
            printf("get: file is Encrypt, password required\n");
            return -1;
        }

        uint8_t input_key = derive_key(password);
        if (input_key != target->file->key)
        {
            printf("get: wrong password\n");
            return -1;
        }

        // decrypt temporarily
        xor_buffer(target->file->content, target->file->size, target->file->key);
    }

    // Step 3: write to host path
    FILE *fp = fopen(path, "wb");
    if (!fp)
    {
        printf("get: cannot write to %s\n", path);
        
        // encrypt back if needed
        if (target->file->Encrypt)
        {
            xor_buffer(target->file->content, target->file->size, target->file->key);
        }
        return -1;
    }

    size_t written = fwrite(target->file->content, 1, target->file->size, fp);
    fclose(fp);

    // encrypt back to keep memory secure
    if (target->file->Encrypt)
    {
        xor_buffer(target->file->content, target->file->size, target->file->key);
    }

    if (written != target->file->size)
    {
        printf("get: write error\n");
        return -1;
    }

    printf("get: success, written %zu bytes to %s\n", written, path);
    return 0;
}

int file_sys_cat(const char* file_name, const char *password)
{
    if (!file_name || !*file_name)
    {
        printf("cat: invalid filename\n");
        return -1;
    }

    // Step 1: find file node
    Node *current = g_cwd->child;
    Node *target = NULL;

    while (current != NULL)
    {
        if (current->type == NODE_FILE && strcmp(current->name, file_name) == 0)
        {
            target = current;
            break;
        }
        current = current->sibling;
    }

    if (!target)
    {
        printf("cat: no such file: %s\n", file_name);
        return -1;
    }

    if (!target->file || !target->file->content || target->file->size <= 0)
    {
        printf("cat: file is empty\n");
        return 0;
    }

    // Step 2: check password if Encrypt
    if (target->file->Encrypt)
    {
        if (!password || !*password)
        {
            printf("cat: file is Encrypt, password required\n");
            return -1;
        }

        uint8_t input_key = derive_key(password);
        if (input_key != target->file->key)
        {
            printf("cat: wrong password\n");
            return -1;
        }

        // decrypt temporarily
        xor_buffer(target->file->content, target->file->size, target->file->key);
    }

    // Step 3: print content
    printf("=== Content of %s ===\n", file_name);
    fwrite(target->file->content, 1, target->file->size, stdout);
    printf("\n=== End of file ===\n");

    // encrypt back
    if (target->file->Encrypt)
    {
        xor_buffer(target->file->content, target->file->size, target->file->key);
    }

    return 0;
}


// status 晚上 21:30 做 1/5 要修內容跟寫 read me
