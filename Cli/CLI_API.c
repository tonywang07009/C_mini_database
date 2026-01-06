#include "CLi_API.h"

#include "../Common_Function/Common_function.h"

static void command_loop(void);

int cli(void) // 這邊要修 內容
{
    while (1)
    {
        char option_buffer[256]; // The max you car read char
        int option = 0;

        char partition_option_buffer[256];
        uint64_t partition_size = 0;

        
        Com_ensure_dump_dir(); // The init dump store file

        /*Step 1 The basic function choose*/
        printf("\n  ==== WELCOME TO TONY_File_system ===== \n");
        printf("Choose the options please. \n");
        printf("Options : \n 1) lodas from file\n 2) create new partition in memory \n");
        printf("your option : ");
        if (fgets(option_buffer, sizeof(option_buffer), stdin) == NULL)
        {
            printf("your input is fail \n");
            return -1;
        }

        if (sscanf(option_buffer, "%d", &option) != 1)
        {
            printf("Invalid input.\n");
            continue; // 回到 while(1) 重新問
        }

        switch (option)

        {
        // case 1: // writen the autoload dump and then into file
                
        //     // if 

        //     // break;

        case 2: /*Step 2 Specify the partition size */

            printf("\n Input size of a new partition : ");
            if (!fgets(partition_option_buffer, sizeof(partition_option_buffer), stdin)) // duplicatipn The
            {
                printf("Input error.\n");
                continue;
            }
            /*RD*/
            if (sscanf(partition_option_buffer, "%" SCNu64, &partition_size) != 1)
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
        printf("TONY_File_system > "); // The specy name modefie
        file_rule_display(g_cwd);
        
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("Input error.\n");
            break;
        }

        // Del \n
        line[strcspn(line, "\n")] = '\0'; // del '\n';

        // jump the space
        if (line[0] == '\0')
        {
            continue;
        }

        // The reading strtok // RD*
        char *cmd = strtok(line, " "); // line find the first
        char *arg = strtok(NULL, " "); // arg RD

        /* The auto restore function*/
        if (strcmp(cmd, "exit") == 0)
        {
            FILE* fp = fopen(LAST_DUMP,"w");
            if(fp != NULL)
            {
                file_dump_dfs(g_root,"",fp);
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
            FILE *fp = fopen(arg,"w");
            if (fp == NULL)
            {
                    printf("dump: cannot open %s\n", arg);
                    continue;
            }
            
            file_dump_dfs(g_root,"",fp);
            fclose(fp); // close file
        }

        else if (strcmp(cmd, "put") == 0)
        {     
            if (!arg)
            {
                printf("usage: put <filename> <dis_path> \n");
                continue;
            }
            const char* file_path = arg;      // For user specify
            char *arg2 = strtok(NULL, " ");  // For user specify
            char *arg3 = strtok(NULL, " ");  // For user specify

            /*
                arg = 
                fs_file = your file name 
                arg 3 = your 
            */
            const char* dis_path = arg2? arg2:file_path;

            if (file_sys_put(arg, dis_path) != 0)
            {
                printf("put: failed\n");
            }
        }

        else if (strcmp(cmd, "get") == 0)
        {
            uint8_t* get_content = NULL;
            size_t file_size = 0 ;
            char *password  = strtok(NULL, " "); // 第 3 個參數

            if (arg == NULL) 
            {
                printf("usage: get <fs_file> <password>\n");
                continue;
            }

            if (file_sys_get(arg,password,&get_content,&file_size ) != 0)
            {
                printf("get: failed\n");
                continue;
            }

            /*The out put file*/
            FILE* fp = fopen(arg,"wb");
            if(fp==NULL)
            {
                printf("get: cannot write to %s\n", arg);
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

            printf("get: success, written %zu bytes to %s\n", written, arg);
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
            printf("get <fs_file>  <password>  - decrypt & download file\n");
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
        printf("Please choose next step 1)\n call again cli 2) end \n");
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