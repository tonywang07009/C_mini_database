#ifndef __CLI__
#define __CLI__

#include <stdio.h> //fgets
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>  // for into the uint64_t

#define MAX_CMD 512

#include "../Basic_Function/Basic/Basic_function.h"

/*for dump dir table*/
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #include<windows.h>
#else
   #include <dirent.h>  
#endif



/*function list*/
int cli(void);
void cli_expection_handle(int cli_result);
void cli_list_dump_files(void);
#endif