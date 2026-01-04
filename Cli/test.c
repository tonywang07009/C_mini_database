#include "CLi_API.h"

int main (void)
{
    // char name_buffer[512]={'\0'};
    // int mkdir_result = 0;

    file_sys_init ();
    int cli_result = cli();
    cli_expection_handle (cli_result);
    
    

    return 0;
}