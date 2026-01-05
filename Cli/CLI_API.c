#include "CLi_API.h"

static void command_loop(void);

int cli(void) // 這邊要修 內容
{
    while (1)
    {
        char option_buffer[256]; // The max you car read char.
        int option = 0;

        char partition_option_buffer[256];
        uint64_t partition_size = 0;

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
        case 1:
                
            file_sys_load("test.img");

            break;

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
            printf("Make new partition successful!\n");

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
    // char specfy_name [20] = {'\0'};
    char line[MAX_CMD] = {'\0'};

    while (1)
    {
        printf("TONY_File_system > ");

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

        /*改函式指標*/
        if (strcmp(cmd, "exit") == 0)
        {
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
                printf("mkdir: sucess.");
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
        
        else if (strcmp(cmd, "dump") == 0)
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
            char *fs_file  = strtok(NULL, " "); // 第 2 個參數
            char *password = strtok(NULL, " "); // 第 3 個參數

            if (!arg || !fs_file)
            {
                printf("usage: put <host_path> <fs_file> <password>\n");
                continue;
            }

            if (file_sys_put(arg, fs_file, password) != 0)
            {
                printf("put: failed\n");
            }
        }
        else if (strcmp(cmd, "get") == 0)
        {
            char *host_path = strtok(NULL, " "); // 第 2 個參數
            char *password  = strtok(NULL, " "); // 第 3 個參數

            if (!arg || !host_path)
            {
                printf("usage: get <fs_file> <host_path> <password>\n");
                continue;
            }

            if (file_sys_get(arg, host_path, password) != 0)
            {
                printf("get: failed\n");
            }
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
        else if (strcmp(cmd, "help") == 0)
        {
            printf("\n==== Commands ====\n");
            printf("mkdir <name>                    - create directory\n");
            printf("rmdir <name>                    - remove directory\n");
            printf("ls                              - list entries\n");
            printf("cd <path>                       - change directory\n");
            printf("touch <name>                    - create empty file\n");
            printf("rm <name>                       - remove file\n");
            printf("put <host_path> <fs_file> <pw>  - upload & encrypt file\n");
            printf("get <fs_file> <host_path> <pw>  - decrypt & download file\n");
            printf("cat <fs_file> <pw>              - decrypt & show content\n");
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