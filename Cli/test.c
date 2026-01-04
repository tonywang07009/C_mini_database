#include "CLi_API.h"

int main (void)
{
    file_sys_init ();
    int cli_result = cli();
    cli_expection_handle (cli_result);

    return 0;
}