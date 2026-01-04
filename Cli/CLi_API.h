#ifndef __CLI__
#define __CLI__

#include <stdio.h> //fgets
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>  // for into the uint64_t

#include "../Basic_Function/list/list_API.h"

/*function list*/
int cli(void);
void cli_expection_handle(int cli_result);

#endif