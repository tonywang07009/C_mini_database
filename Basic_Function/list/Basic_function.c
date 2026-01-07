#include "Basic_function.h"

/*The extern golobal varable*/
Node *g_root = NULL;
Node *g_cwd = NULL;

size_t g_block_size   = 1024;
size_t g_total_blocks = 2000;
size_t g_total_inodes = 221;

/*
    1. 系統壓力測試
    2. clean code  設計
    3. makefile 加
    4. 檢查每個code 是否有符合老師要求
    5. 完成 基本功能 readme 跟 common function (整理共同函式 抽成小工具)
    6. 寫維基
*/

/* file system initize. */
void file_sys_init(void)
{
    char path_display_buffer[1024] = {0};

    g_root = (Node *)malloc(sizeof(Node)); // The root node create.
    if (g_root == NULL)
    {
        printf("The root create fail \n");
        return;
    }
    strcpy(g_root->name, "/"); // Copy the "/" to g_root->name space.
    g_root->type = NODE_DIR;
    g_root->parent = NULL;
    g_root->child = NULL;
    g_root->sibling = NULL;
    g_cwd = g_root; /*Pointer the g_cwd to g_root
                      The g_cwd is use for pointer now directory.
                    */
}

/* create the directory */
int file_sys_mkdir(const char *dir_name)
{
    if (!dir_name || !*dir_name)
    {
        printf("Invalid directory dir_name.\n");
        return -1;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        printf("The mkdir create fail.\n");
        return -1;
    }

    // init_newnode
    strncpy(new_node->name, dir_name, (sizeof(new_node->name) - 1)); // The dir_name dupilcation
    new_node->name[sizeof(new_node->name) - 1] = '\0';               // The end symbol
    new_node->type = NODE_DIR;
    new_node->parent = g_cwd;
    new_node->child = NULL;
    new_node->sibling = NULL;

    // The first connect the child chain
    if (g_cwd->child == NULL)
    {
        g_cwd->child = new_node;
    }
    else
    {
        // The parallelization (平行化) the child sibling chain
        Node *current_dir = g_cwd->child;

        /* Find the empry silbing algtrom*/
        while (current_dir->sibling != NULL)
        {
            current_dir = current_dir->sibling;
        }
        current_dir->sibling = new_node; // now dir next build the new one
    }
    return 0;
}

/*Display this layer include directory*/
int file_sys_ls(const Node *now_dir)
{
    if (now_dir == NULL)
    {
        printf("fail_dir.\n");
        return -1;
    }

    Node *current_dir = now_dir->child;

    /*print the layer all child node*/
    while (current_dir != NULL)
    {
        printf("%s ", current_dir->name);
        current_dir = current_dir->sibling;
    }
    printf("\n");
    return 0;
}

/* Into the directory*/
int file_sys_cd(const char *cd_dir)
{
    /*back to last layer*/
    if (strcmp(cd_dir, "..") == 0) // strcmp is for compare the string is similar.
    {
        if (g_cwd->parent)
        {
            g_cwd = g_cwd->parent;
        }
        return 0;
    }

    /*back to root layer*/
    if (strcmp(cd_dir, "/") == 0)
    {
        g_cwd = g_root;
        return 0;
    }

    /* cd "name" */
    Node *current_dir = g_cwd->child;
    while (current_dir != NULL)
    {
        if (current_dir->type == NODE_DIR && strcmp(current_dir->name, cd_dir) == 0)
        {
            g_cwd = current_dir;
            return 0;
        }
        current_dir = current_dir->sibling;
    }

    return -1;
}

/*Removed the directory*/
int file_sys_rmdir(const char *rm_dir)
{

    Node *prev_dir = NULL; // this is for find the target node last.

    Node *current_dir = g_cwd->child;
    while (current_dir != NULL)
    {
        if (current_dir->type == NODE_DIR && strcmp(current_dir->name, rm_dir) == 0)
        {
            break;
        }

        /* The target find */
        prev_dir = current_dir;
        current_dir = current_dir->sibling;
    }

    if (current_dir == NULL)
    {
        printf("rmdir: no such directory: %s\n", rm_dir);
        return -1;
    }
    printf("found target %s\n", current_dir->name);

    /*The "Fool-proof" mechanism*/
    if (current_dir->child != NULL)
    {
        char check_del[8] = {0};
        printf("Directory not empty, delete anyway? \n into : (yes or no) ");

        if (scanf("%7s", check_del) != 1)
        {
            printf("Input error \n");
            return -1;
        }

        if (strcmp(check_del, "no") == 0)
        {
            return 0; // cancel del.
        }
        else if (strcmp(check_del, "yes") == 0) // finsh
        {
            /*call void free_subtree(Node* now_dir);*/
            /*will finshed in afternoon */
        }
        else
        {
            printf("Undefine parameter ");
            return -1;
        }
    }

    /*The root situation to update linklist*/
    if (prev_dir == NULL)
    {
        g_cwd->child = current_dir->sibling;
    }

    /*The genral situation to update linklist*/
    else
    {
        prev_dir->sibling = current_dir->sibling; // Update postion.
    }

    free(current_dir);
    return 0;
}

/*The remove file */
int file_sys_rm(const char *file_name)
{
    Node *prev_file = NULL;
    Node *current_file = g_cwd->child;

    while (current_file != NULL)
    {
        if (current_file->type == NODE_FILE && strcmp(current_file->name, file_name) == 0)
        {
            break; // the find del file
        }
        prev_file = current_file;
        current_file = current_file->sibling;
    }

    if (current_file == NULL)
    {
        printf("rm: no such file: %s\n", file_name);
        return -1;
    }

    printf("found target file %s\n", current_file->file);
    if (current_file->file != NULL)
    {
        if (current_file->file->content != NULL)
        {
            free(current_file->file->content);
        }
        free(current_file->file);
    }

    if (prev_file == NULL)
    {
        g_cwd->child = current_file->sibling; // Update the child postion
    }
    else
    {
        prev_file->sibling = current_file->sibling; // Update postion.
    }

    free(current_file);
    printf("free sussful");

    return 0;
}

/*The create file*/
int file_sys_touch(const char *name)
{

    if (!name || !*name) // Name pointer address *name -> content
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
    strncpy(new_node->name, name, (sizeof(new_node->name) - 1)); /*The name dupilcation
                                                                   The size of -1 is for end symbol S*/

    new_node->name[sizeof(new_node->name) - 1] = '\0'; // Add The end symbol
    new_node->type = NODE_FILE;
    new_node->parent = g_cwd;
    new_node->child = NULL;
    new_node->sibling = NULL;

    /* Create the file*/
    if (new_node->type == NODE_FILE)
    {
        FileMeta *file = (FileMeta *)malloc(sizeof(FileMeta));
        if (file == NULL)
        {
            printf("file node create fail.\n");
            return -1;
        }
        new_node->file = file;
        new_node->file->content = NULL;
        new_node->file->Encrypt = 0;
        new_node->file->size = 0;
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

/*Create the recoding the file system tree*/
void file_sys_dump_dfs(Node *node, const char *parent_path, FILE *fp)
{
    if (!node)
    {
        return;
    }

    char path[2048]; // The store path role

    /*put in the file condition */
    if (node == g_root)
    {
        strcpy(path, "/"); // for root
    }
    else // build the abs file role
    {
        snprintf(path, sizeof(path), "%s/%s", parent_path, node->name);
        // The display file path name
        /*
            The "snprintf" function is out the format file to you specify area
            and this function requestion the buffer sizeof
            -----
            path : you specft area
            sizeof(path) : the area total size.

        */
    }

    /*The condtion for */
       DiskNode header;

       header.type    = (node->type == NODE_DIR? 0:1); // 0-> Dir 1->File
       header.encrypt = (node->type == NODE_FILE && 
                         node->file? node->file->Encrypt : 0);

       header.size    = (node->type == NODE_FILE && 
                         node->file ? node->file->size : 0); 
       header.path_len = (uint16_t)strlen(path);

       /*written in header sturct in the dump file*/
       fwrite(&header,sizeof(header),1,fp);

       /*written path , path len is from your file define
         so the fwrite will accoding the path len writen the real role long */
       fwrite(path,1,header.path_len,fp);

       if(node->type == NODE_FILE &&
          node->file && node->file->size >0)
       {
         fwrite(node->file->content,1,node->file->size,fp);
       }


    /*The recursion the relation role */
    file_sys_dump_dfs(node->child, path, fp);
    file_sys_dump_dfs(node->sibling, parent_path, fp);
}

/*The recover file system content*/
int file_sys_load(const char *dump_file)
{

    FILE *fp = fopen(dump_file, "rb"); // The reading file content
    if (fp == NULL)
    {
        printf("Sys load : miss dump file %s\n", dump_file);
        return -1;
    }
    DiskNode header;
    while (fread(&header,sizeof(header),1,fp)==1)
    // envery time only reading the one element
    // sizeof(header) is the element size
    {
        char path[2048] = {0};
        if(header.path_len >=sizeof(path))
        {
            printf("load error: The Disknode path_len over path buffer\n");
            return -1;
        }

        if(fread(path,1,header.path_len,fp)!=header.path_len)
        // The 1 mean is reading one time reading all path_len element amount
        // The size is header.path_len byte
        {
            printf("load error: The Disknode path_len unequal the pathbufferlen\n");
            return -1;
        }
        
        path[header.path_len] = '\0'; //The add end symbol

        if (header.type ==0)
        {
            if (strcmp(path, "/") == 0)
            {
                continue; // root already exists
            }

            /*duplication path content to tmp buffer*/
            char tmp[1024];
            strncpy(tmp, path, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            /*The '/' inspection handle 
              root handle*/
            char *tmpPtr = tmp;
            if (*tmpPtr == '/')
            {
                tmpPtr++;
            }

            /* The recodeing g_cwd origin position  */
            Node *saved_cwd = g_cwd;
            g_cwd = g_root;

            /* Cutting the world token */
            char *token = strtok(tmpPtr, "/");
            while (token != NULL)
            {
                /*Now node is g_root , so can reading the all file content*/
                Node *current = g_cwd->child;
                Node *found = NULL;

                /*check the root have child
                  find the same node type
                */

                while (current != NULL)
                {
                    if (current->type == NODE_DIR &&
                        strcmp(current->name, token) == 0)
                    {
                        found = current; // find the same dirteoryname
                        break;
                    }

                    current = current->sibling;
                }

                /* The s safe mechanism*/
                if (found == NULL)
                {

                    // is create the new dir from token
                    if (file_sys_mkdir(token) != 0)
                    {
                        printf("load: mkdir fail on %s\n", token);
                        break;
                    }

                    current = g_cwd->child; // The repoint to g_root child
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

            g_cwd = saved_cwd; // repointer to original pointer dirteory
            
        }
        else
        {
            if (strcmp(path, "/") == 0)
            {
                continue; // root already exists so we need go next round
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
            /*find the last time appear sepcy char.
              The search directory is right to left
              ---
              tmpPtr is pointer to mine space
              '/' is mine target char

            */
            char *filename = NULL;

            if (last_slash!=NULL)
            {
                *last_slash = '\0';        // find the target add the endsymbol
                filename = last_slash + 1; // To pointer the first file name.
            }

            /*handle file node*/
            else
            {
                filename = tmpPtr; // The tmpPtr repointer to origin tmp array
                tmpPtr = NULL;
            }
            
            if (tmpPtr!=NULL && *tmpPtr != '\0')
            {
                char *token = strtok(tmpPtr, "/"); //Token 
                while (token != NULL)
                {
                    
                    Node *current = g_cwd->child;
                    Node *found = NULL;

                    /*The dirtery find*/
                    while (current != NULL)
                    {
                        if (current->type == NODE_DIR &&
                            strcmp(current->name, token) == 0)
                        {
                            found = current;
                            break;
                        }

                        current = current->sibling; 
                    }

                    if (found==NULL)
                    {
                        // no find do the mkdir new one
                        if (file_sys_mkdir(token) != 0)
                        {
                            printf("load: mkdir fail on %s\n", token);
                            break;
                        }

                        current = g_cwd->child; // repointer to childnode
                        while (current->sibling != NULL)
                        {
                            current = current->sibling;
                        }

                        found = current;

                    }
                    // Prepare the next token cut
                    g_cwd = found;
                    token = strtok(NULL, "/");
                }
            }
            /*create the dumpfile*/
            if(file_sys_touch(filename)!=0)
            {
                printf("load: touch fail on %s \n" ,filename);
                g_cwd = saved_cwd;
                return -1;
            }

            Node* cur = g_cwd->child;
            Node* file_node = NULL;

            /*The find saem filename*/
            while (cur)
            {
                if (cur->type == NODE_FILE 
                    && strcmp(cur->name, filename) == 0)
                {
                    file_node = cur;
                    break;
                }
                cur = cur->sibling;
            }

            if (!file_node || !file_node->file) 
            {
                printf("load: internal error, file node not found\n");
                g_cwd = saved_cwd;
                return -1;
            }

            /* Allocate　and read file content */
            if(header.size > 0)
            {
                file_node->file->content = (char*)malloc(header.size);
                if(!file_node->file->content)
                {
                    printf("load : content malloc fail \n");
                    g_cwd = saved_cwd;

                    return -1;
                }

                if (fread(file_node->file->content, 1, header.size, fp) != header.size) 
                {
                    printf("load: read file content fail\n");
                    free(file_node->file->content);
                    file_node->file->content = NULL;
                    g_cwd = saved_cwd;

                    return -1;
                }

            }

            /*into the data to file space*/
            file_node->file->size = header.size;
            file_node->file->Encrypt = header.encrypt;
            g_cwd = saved_cwd;
        }
    }

    fclose(fp);
    return 0;
}
    
    

/*The Product the key*/
uint8_t derive_key(const char *password)
{
    if (!password || !*password)
    {
        return 0x5A; // default key
    }

    uint8_t key = 0;
    while (*password)
    {
        key ^= (uint8_t)(*password);
        password++;
    }

    return key ? key : 0x5A; // avoid key = 0
}

/*The Encrepty Decrtpty key*/
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

/*The put file function*/
int file_sys_put(const char* file_name, const char* dst_path)
{
    if (!file_name || !*file_name || !dst_path || !*dst_path)
    {
        printf("Invalid file_name or filename.\n");
        return -1;
    }

    /*step 1 Pointer the now current*/
    Node *target = NULL;
    Node *current = g_cwd->child;

    /*Find the target path*/
    while (current != NULL)
    {
        if (current->type == NODE_FILE && strcmp(current->name, dst_path) == 0)
        {
            target = current;
            break;
        }
        current = current->sibling;
    }

    // if not found, create new file
    if (target == NULL)
    {
        if (file_sys_touch(dst_path) != 0)
        {
            printf("put: failed to create file %s\n", dst_path);
            return -1;
        }

         /*Find the target path again*/
        current = g_cwd->child;
        while (current != NULL)
        {
            if (current->type == NODE_FILE && strcmp(current->name, dst_path) == 0)
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

    // Step 2: read file content from file_name
    FILE *fp = fopen(file_name, "rb");

    if (fp==NULL)
    {
        printf("put: cannot open file %s\n", file_name);
        return -1;
    }

    /* get the file size*/
    /* RD */
    fseek(fp, 0, SEEK_END);
    
    long file_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    if (file_size <= 0)
    {
        printf("put: file is empty or error\n");
        fclose(fp);
        return -1;
    }

    /* Free old file content*/
    if (target->file->content != NULL)
    {
        free(target->file->content);
    }

    /* Allow the new file buffer  for duplication*/
    target->file->content = (char *)malloc(file_size);
    if (!target->file->content)
    {
        printf("put: memory allocation failed\n");
        fclose(fp);
        return -1;
    }

    /* Read content join the file content*/
    size_t read_size = fread(target->file->content, 1, file_size, fp);
    fclose(fp);

    if (read_size != file_size)
    {
        printf("put: read file error\n");
        free(target->file->content);
        target->file->content = NULL;
        return -1;
    }

    /* Join the file size content */
    target->file->size = file_size;

    printf("Do you want to setting the passworld? 0) No ; 1) YES \n");
    int set_passworld = 0;
    if(scanf("%d",&set_passworld)!=1)
    {
        printf("input error \n");
        return -1;
    }  

    /*The clear buffer */
    int change  = 0;
    while ((change = getchar()) != '\n' && change != -1){};
    /* getchar = get the char for buffer
       and drop this , until do the \n and change == -1
       Ctrl+Z == -1
    */

    if (set_passworld == 1)
    {
        char password[64] = {0};
        printf ("please setting the passworld \n");
        
        if(scanf("%63s",password)!=1)
        {
            printf("input error\n");
            return -1;
        }
        /*Step 3: encrypt if password provided*/
        if (password!=NULL && password[0]!='\0')
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
    }
    else
    {
            target->file->Encrypt = 0;
            target->file->key = 0;
    }

    printf("put: success, file size = %d bytes\n", target->file->size);
    return 0;
}


/*The ge file function*/
int file_sys_get(const char *file_name, const char *password , uint8_t** out_buffer, size_t* out_size)
{
    if (!file_name || !*file_name || !out_buffer || !out_size)
    {
        printf("get: invalid filename or path\n");
        return -1;
    }

    // Step 1: find file node
    Node *target = Fs_resolve(file_name,1);

    if(!target)
    {
        printf("get: Path not found \n");
        return -1;
    }

    if (!target->file || !target->file->content || target->file->size <= 0)
    {
        printf("get: File not found or empty\n");
        return -1;
    }

    // Step 2: check password if Encrypt
    if (target->file->Encrypt)
    {
        if (!password || !*password)
        {
            printf("get: File is Encrypt, password required\n");
            return -1;
        }

        uint8_t input_key = derive_key(password);
        if (input_key != target->file->key)
        {
            printf("get: Wrong password\n");
            return -1;
        }
    }


    /*
        The open file or build the file stream to operation

        path : is your open and build role
        wb : open to the bit mode
        w: write  b: is check the different os compatibility.

        FILE* fp pointer to pointer the file postion
    */

    // Step 3: duplication the Decrypt content
    uint8_t *Decrypt_buffer = malloc(target->file->size);
    
    if (Decrypt_buffer==NULL) 
    {
        printf("get: the Decrypt content memory alloc failed\n");
        return -1;
    }

    memcpy(Decrypt_buffer, target->file->content, target->file->size); //Duplication

    if (target->file->Encrypt) 
    {
        xor_buffer(Decrypt_buffer, target->file->size, target->file->key);
    }

    *out_buffer = Decrypt_buffer;
    *out_size  = target->file->size;
     return 0;
}

/*The display file content*/
int file_sys_cat(const char *file_name, const char *password)
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

    // Step 2: check password if decrypt
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
    fwrite(target->file->content, 1, target->file->size, stdout); // stdout : moniter print
    printf("\n=== End of file ===\n");

    // encrypt back
    if (target->file->Encrypt)
    {
        xor_buffer(target->file->content, target->file->size, target->file->key);
    }

    return 0;
}

void file_sys_state(void)
{
    Node *current = g_cwd->child;

    printf("File Name \t Type\t Encrypt \n"); // \t -> Horzion syymbol

    while (current != NULL)
    {
        /*switch mod*/
        if (current->type == NODE_DIR)
        {
            printf("%s\t DIR\t-\t\n", current->name);
        }
        else if (current->type == NODE_FILE && current->file != NULL)
        {
            printf("%s\tFILE \t Size :%d\t Encrypt %s\n",
                   current->name,
                   current->file->size,
                   current->file->Encrypt ? "yes" : "no");
        }

        current = current->sibling;
    }
}

void file_rule_display(const Node* current_dir)
{
    Node* stack[1024] = {0};
    int depth = 0;
    
    while (current_dir)
    {
        /*Every depth block recodeing the dir address*/
        stack[depth++] =current_dir;
        current_dir = current_dir->parent;  // back
    }

    for(int i = (depth-1) ; i>=0 ; i--)
    {
        printf("%s",stack[i]->name);

        if(i>0 && strcmp(stack[i]->name,"/")!=0)
        {
            printf("/");
        }
    }

}

/*The Auto create file common tool*/
void Com_ensure_dump_dir(void)
{
    #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        _mkdir(DUMP_DIR); // Fail if the dirtery is create 
    #else
         mkdir(DUMP_DIR,0755); // Return -1 if the dirtery is create 
    #endif
}

/*get tool */
Node* find_in_dir(Node* dir_path ,const char* name, int want_file)
{
    Node* cur = dir_path->child;
    while (cur) 
    {
        if (strcmp(cur->name, name) == 0 &&
            ((want_file && cur->type == NODE_FILE) ||
            (!want_file && cur->type == NODE_DIR))) 
            {
                return cur;
            }
        cur = cur->sibling;
    }
    return NULL;
}

Node* Fs_resolve(const char* path, int want_file)
{
    if (!path ||!*path) 
    {
        return NULL;
    }

    // 1. from the root dir
    Node* cur = NULL;
    char buf[512];
    // 2. dupliaction path
    if (path[0] == '/') 
    {
        cur = g_root; // Absolute path
    } 
    else 
    {
        cur = g_cwd; // Relative path (YOU NEED TO DEFINE THIS GLOBAL)
    }

    strncpy(buf, path, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    // 3. splite "/"
    char* token = strtok(buf, "/");
    while (token) 
    {
        /* The find token algthrom */
        char* next = strtok(NULL, "/"); // watch the next arg is appaer? 
        int last = (next == NULL);
        /* The file get cheack*/
        int need_file = last ? want_file : 0;
        printf("token = '%s'\n", token);

        cur = find_in_dir(cur, token, need_file);
        if (!cur) 
        {
            return NULL; // Fail if don get any element.
        }
        token = next;

    }
    return cur;
}

const char* get_basename(const char* path) 
{
    const char *p = strrchr(path, '/');
    return p? p + 1 : path;
}

static void fs_traverse(Node *node, FsStats *status)
{
    if (!node) return;

    status->used_inodes++;

    if (node->type == NODE_FILE && node->file) 
    {
        size_t size   = (size_t)node->file->size;
        size_t blocks = (size + status->block_size - 1) / status->block_size;

        status->file_blocks += blocks;
        status->used_blocks += blocks;
    }

    fs_traverse(node->child,  status);
    fs_traverse(node->sibling, status);
}

void fs_get_stats(FsStats *status)
{
    memset(status, 0, sizeof(*status));

    status->block_size    = g_block_size;
    status->total_blocks  = g_total_blocks;
    status->partition_size = status->block_size * status->total_blocks;
    status->total_inodes  = g_total_inodes;

    fs_traverse(g_root, status);
}