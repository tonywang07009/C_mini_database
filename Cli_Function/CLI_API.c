#include "CLi_API.h"

static void command_loop(void);

int cli(void)
{
    file_sys_ensure_dump_dir(); // The init dump store file
    while (1)
    {
        char name_only[128];     // specfy the dump file name
        char option_buffer[256]; // The max you car read char
        int dump_choose = 0;
        int option = 0;

        char partition_option_buffer[256]; // specfy your db space capacity
        uint64_t partition_size = 0; 

        /*Step 1 The basic function choose*/
        printf("\n  ==== WELCOME TO File_system Builder ===== \n");
        printf("Choose the options please. \n");
        printf("Options : \n 1) lodas from file\n 2) create new partition in memory \n 0) exit the builder \n");
        printf("your option : ");
        if (fgets(option_buffer, sizeof(option_buffer), stdin) == NULL)
        {
            printf("your input is fail \n");
            return -1;
        }
        printf("\n");

        if (sscanf(option_buffer, "%d", &option) != 1)
        {
            printf("Invalid input.\n");
            continue; // 回到 while(1) 重新問
        }

        switch (option)

        {

        case 0:

            return 1;
            break;

        case 1: /*Dump load*/
            cli_list_dump_files(); // The display now dump
            printf("\n");
            printf(" 1) Choose the dump file name to load : \n 2) auto load last dump :\n");
            printf("your choose : ");
            scanf("%d",&dump_choose);

            if(dump_choose != 0)
            {
                if (!fgets(name_only, sizeof(name_only), stdin)) 
                {
                    printf("No input\n");
                    continue;
                }
                
                name_only[strcspn(name_only, "\n")] = '\0';
                char dump_path[256];
                /*The array size catch*/
                if(dump_choose == 1)
                {
                    snprintf(dump_path, sizeof(dump_path),"%s/%s", DUMP_DIR, name_only);
                        // "%s/%s" == "/"
                }
                else if(dump_choose == 2)
                {
                    snprintf(dump_path, sizeof(dump_path),"%s/%s", DUMP_DIR, DUMP_FIR);
                }
                else
                {
                    printf("Unknow option \n");
                    continue;
                }
                file_sys_init(); // The empty root.

                if(file_sys_load(dump_path)!=0)
                {
                    printf("load fail \n");
                    break;   
                }
                command_loop ();
            }
            
            break; 
        
        case 2: /*Specify the partition size */

            printf("\n Input size of a new partition : ");
            if (!fgets(partition_option_buffer, sizeof(partition_option_buffer), stdin)) // duplicatipn The
            {
                printf("Input error.\n");
                continue;
            }
            
            if (sscanf(partition_option_buffer, "%" SCNu64, &partition_size) != 1) // Unsign long long 
            {
                printf("Invalid input.\n");
                printf("ReSpecify again please\n");
                continue;
            }

            printf(" into the CLI \n");
            command_loop();
            // printf("Make new partition successful!\n");

            break;
            // case 3: // system info display

        default:

            printf("Unknow option !! \n rechoose again please \n");
            continue;
            break;
        }
    }
    return 0;
}

static void command_loop(void) // The init_CLI
{
    
    char line[MAX_CMD] = {0};

    while (1)
    {
        printf("TONY_File_system > : "); // The specy name modefie
        file_sys_rule_display(g_cwd);

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("Input error.\n");
            break;
        }

        // Del \n
        line[strcspn(line, "\n")] = '\0'; // replace the \n to \0

        // jump the space
        if (line[0] == '\0')
        {
            continue;
        }

        // The reading strtok // RD*
        char *cmd = strtok(line, " "); // specfy the function
        char *arg = strtok(NULL, " "); // specfy the function additional argermant (like help "-s") 

        /* The auto restore function*/
        if (strcmp(cmd, "exit") == 0)
        {
            FILE* fp = fopen(LAST_DUMP,"w");
            if(fp != NULL)
            {
                file_sys_dump_dfs(g_root,"",fp);
                fclose(fp);
                printf("Auto store to %s \n",LAST_DUMP);
            }
            else
            {
                printf("Auto store is failed: cannot open %s\n",LAST_DUMP);
            }

            break;
        }

        else if (strcmp(cmd, "mkdir") == 0)
        {
            if (!arg)
            {
                printf("usage: mkdir <name>\n");
                continue;
            }
            else if (file_sys_mkdir(arg) != 0)
            {
                printf("mkdir failed.\n");
            }
            else
            {
                printf("mkdir: sucess.\n");
            }
        }
        else if (strcmp(cmd, "ls") == 0)
        {
            file_sys_ls(g_cwd); // extern varable.
        }

        else if (strcmp(cmd, "cd") == 0)
        {
    
            if (!arg)
            {
                printf("usage: cd <path>\n");
                continue;
            }
            if (file_sys_cd(arg) != 0)
            {
                printf("cd: no such directory: %s\n", arg);
            }
        }

        else if (strcmp(cmd, "rmdir") == 0)
        {
            if (!arg)
            {
                printf("usage: rmdir <name>");
                continue;
            }

            if (file_sys_rmdir(arg) != 0)
            {
                printf("rmdir : no such directory: %s \n", arg);
            }
        }
        else if (strcmp(cmd, "touch") == 0)
        {
            if (!arg)
            {
                printf("usage: touch <name>");
                continue;
            }

            if (file_sys_touch(arg) != 0)
            {
                printf("touch : failed to create : %s \n", arg);
            }
        }

        else if (strcmp(cmd, "rm") == 0)
        {
            if(!arg)
            {
                printf("usage: rm <name>");
                continue;
            }
        
            if (file_sys_rm(arg) != 0)
            {
                printf("rm : failed to del : %s \n", arg);
            }
        }
        
        else if (strcmp(cmd, "dump") == 0) /*The DIY dump*/
        {
            if(!arg)
            {
                printf("usage: dump <file> \n");
                continue;
            }
            /*The specfy  open dump file name*/
            char dump_path[256] = {0};
            snprintf(dump_path,sizeof(dump_path),"%s/%s",DUMP_DIR,arg);

            FILE *fp = fopen(dump_path,"wb");
            if (!fp)
            {
                    printf("dump: cannot open %s\n", dump_path);
                    continue;
            }
            
            file_sys_dump_dfs(g_root,"",fp);
            fclose(fp); // close file
            printf("dump: saved to %s\n", dump_path);
        }

        else if (strcmp(cmd, "put") == 0)
        {     
            if (!arg)
            {
                printf("usage: put <filename> <dis_path> \n");
                continue;
            }
            const char* file_path = arg;      // OS real file : For user specify 
            char *arg2 = strtok(NULL, " ");   // Target dir
           
            /* Get the basename from file_path*/
            const char* basename = strrchr(file_path,'/');

            /*spcal design*/
            #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

                const char *basename2 = strrchr(file_path, '\\');
                if (!basename || (basename2 && basename2 > basename))
                {
                    basename = basename2;
                }

            #endif
                if(basename)
                {
                    basename++;
                }
                else
                {
                    basename = file_path;
                }

            /*The specy dir handle*/
            Node *saved_cwd = g_cwd;
            
            if(arg2)
            {
                if(file_sys_cd(arg2)!=0)
                {

                    printf("put: no such  directory: %s\n", arg2);
                    g_cwd = saved_cwd;
                    continue;

                }
            }
            
            /*The real put in*/
            if (file_sys_put(file_path, basename) != 0)
            {
                printf("put: failed\n");
            }
            
            /*Repoint orign postion*/
            g_cwd = saved_cwd;
        }

        else if (strcmp(cmd, "get") == 0)
        {
            uint8_t* get_content = NULL;
            size_t file_size = 0 ;

            if (arg == NULL) 
            {
                printf("usage: get <fs_file> [host_path] <password>\n");
                continue;
            }
            
            /*The parameter get*/
            char *dir_path =  strtok(NULL, " ");
            char *password  = strtok(NULL, " "); // The number three arg

            if(dir_path && !password)
            {
                password = dir_path; // default setting
                dir_path = NULL;
            }

            const char *fs_path = arg; /*The complite role*/
            
            /* Token splite get the file name rd*/


            if (file_sys_get(fs_path,password,&get_content,&file_size ) != 0)
            {
                printf("get: failed\n");
                continue;
            }

            /* The path switch*/
            const char* out_path = dir_path? dir_path : file_sys_get_basename(fs_path);
            /*The out put file*/
            FILE* fp = fopen(out_path,"wb");
            if(fp==NULL)
            {
                printf("get: cannot write to %s\n", out_path);
                free(get_content);
                continue;
            }
            size_t written = fwrite(get_content,1,file_size,fp);
            fclose(fp);

            if(written != file_size)
            {
                printf("get: write error \n");
                free(get_content);
                continue;
            }

            printf("get: success, written %zu bytes to %s\n", written, out_path);
            free(get_content);

        }
        else if (strcmp(cmd, "cat") == 0)
        {
            char *password = strtok(NULL, " "); // 第 2 個參數

            if (!arg)
            {
                printf("usage: cat <fs_file> <password>\n");
                continue;
            }

            if (file_sys_cat(arg, password) != 0)
            {
                printf("cat: failed\n");
            }
        }
        else if (strcmp(cmd, "status")==0)
        {
            FsStats st;
            file_sys_get_stats(&st);

            size_t free_blocks =  0;
            if(st.total_blocks > st.used_blocks)
            {
                free_blocks =  st.total_blocks - st.used_blocks;
            }
            else
            {
                free_blocks = 0;
            }

            size_t free_space  = free_blocks * st.block_size;
            printf("\n===== The System status ===== \n");
            
            printf("partition size: %zu\n", st.partition_size);
            printf("total inodes: %zu\n",   st.total_inodes);
            printf("used inodes: %zu\n",    st.used_inodes);
            printf("total blocks: %zu\n",   st.total_blocks);
            printf("used blocks: %zu\n",    st.used_blocks);
            printf("files' blocks: %zu\n",  st.file_blocks);
            printf("block size: %zu\n",     st.block_size);
            printf("free space: %zu\n",     free_space);


            printf("\n");
            file_sys_state();
            
        }
        else if (strcmp(cmd, "help") == 0)
        {
            printf("\n==== Commands ====\n");
            printf("mkdir <name>                    - create directory\n");
            printf("rmdir <name>                    - remove directory\n");
            printf("ls                              - list entries\n");
            printf("cd <path>                       - change directory\n");
            printf("touch <name>                    - create empty file\n");
            printf("rm <name>                       - remove file\n");
            printf("put <filename> <dis_path>  - upload & encrypt file\n");
            printf("get <fs_file> [host_path] <password>  - decrypt & download file\n");
            printf("cat <fs_file> <pw>              - decrypt & show content\n");
            printf("status                           - display now directory all include file\n");
            printf("dump <file>                     - dump file system\n");
            printf("exit                            - exit cli\n");
            printf("==================\n");
        }
        else
        {
            printf("Unknown command, type 'help'\n");
        }


    }
}
void cli_expection_handle(int cli_result)
{
    /*The expection handle*/
    if (cli_result != 0)
    {
        char choose_buf[256] = {0};
        int broken_choose = 0;
        printf("Your cli is broken \n");
        printf("Please choose next step \n 1) call again cli 2) end \n");
        printf("your choose : ");
        if (!fgets(choose_buf, sizeof(choose_buf), stdin))
        {
            printf("Input error.\n");
            return;
        }
        if (sscanf(choose_buf, "%d", &broken_choose) != 1)
        {
            printf("Invalid input.\n");
            return;
        }
        /*The expection handle*/
        if (broken_choose == 1)
        {
            cli();
        }

        if (broken_choose == 2)
        {
            printf("BYE \n");
        }
    }
}

/*The display dump flies*/
void cli_list_dump_files(void)
{
    printf("=== Available dump file in %s === \n", DUMP_DIR);

    #if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // windows

        WIN32_FIND_DATAA ffd; //define the find data struct
        char pattern[MAX_PATH]; // The windows path max file
        snprintf(pattern, sizeof(pattern), "%s\\*.dump", DUMP_DIR);

        HANDLE hFind = FindFirstFileA(pattern, &ffd); // auto find the dump file
        if (hFind == INVALID_HANDLE_VALUE) 
        {
            printf("(no dump files)\n");
            return;
         }
         do
         {
            printf("%s\n", ffd.cFileName);
         }
         while (FindNextFile(hFind,&ffd));
         
         FindClose(hFind);
         
           
    #else // linux / Unix
         
         DIR* dir;
         struct dirent* ent;

         dir = opendir(DUMP_DIR);
         if(!dir)
         {
            printf("(cannot open %s)\n" , DUMP_DIR);
            return;
         }
         
         while ((ent = readdir(dir))!=NULL)
         {
            const char* name = ent->d_name;
            size_t len = strlen(name);
            
            if(len > 5 && 
               strcmp(name + len -5, ".dump") ==0)
            {
                printf("%s\n", name);
            }
         }
         closedir(dir);
    #endif

}