#include "CLi_API.h"

int cli(void)
{
    while (1)
    {
        char option_buffer[256]; // The max you car read char.
        int option = 0;
    
        char partition_option_buffer[256];
        uint64_t partition_size = 0;    

        /*Step 1 The basic function choose*/
        printf("\n  ==== WELCOME TO TONY_CLI ===== \n");
        printf("Choose the options please. \n");
        printf("Options : \n 1) lodas from file\n 2) create new partition in memory \n");
        printf("your option : ");
        if(fgets(option_buffer ,sizeof(option_buffer),stdin) == NULL)
        {
            printf("your input is fail \n");
            return -1;
        }

        if(sscanf(option_buffer,"%d",&option) != 1)
        {
                printf("Invalid input.\n");
                continue;  // 回到 while(1) 重新問
        }

        switch (option)

        {
        case 1:
            // The file_system_function;
    
            break;
        
        case 2: /*Step 2 Specify the partition size */
            printf ("\n Input size of a new partition : ");
            if(!fgets(partition_option_buffer,sizeof(partition_option_buffer),stdin)) // duplicatipn The  
            {
                printf("Input error.\n");
                continue; 
            }

            if(sscanf(partition_option_buffer,"%" SCNu64 , & partition_size)!= 1 )
            {
                printf("Invalid input.\n");
                printf("ReSpecify again please\n");
                continue;
                
            }

            printf("test : partition_size display = %"PRIu64"\n",partition_size); 
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

void cli_expection_handle(int cli_result)
{
    /*The expection handle*/
    if(cli_result != 0)
    {
        char choose_buf[256] = {0};
        int broken_choose = 0;
        printf("Your cli is broken \n");
        printf("Please choose next step 1) call again cli 2) end \n");
        printf("your choose : ");
        if (!fgets(choose_buf, sizeof(choose_buf), stdin))
        {
            printf("Input error.\n");
            return ;
        }
        if (sscanf(choose_buf, "%d", &broken_choose) != 1)
        {
            printf("Invalid input.\n");
            return ;
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