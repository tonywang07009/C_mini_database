#ifndef __CLI__
#define __CLI__

#include <stdio.h> //fgets
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>  // for into the uint64_t

#define MAX_CMD 512


#include "../Basic_Function/list/list_API.h"

/*function list*/
int cli(void);
void cli_expection_handle(int cli_result);

#endif