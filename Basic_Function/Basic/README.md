# File system basic funciton 
----
## 1. Introduction
 - This file function is for build the File system can used basic instruction.

 - This file include all advanced C final project request function.


## 2. API Overview

| Function             | parameter  | return   |          Role                                       |
|------------------------------|---------------------------------------------------------------------|
| `file_sys_init()`   |   `void `   |  `void`  |init the file system need pararmeter and create the root supervisor. |
| `file_sys_mkdir()`   |   `const char* dir_name `   |  `int`  | use the linked list to create new file directory. |
| `file_sys_ls()`   |   `const Node* nowdir `   |  `int`  | display this layer include all directory. |
| `file_sys_cd()`   |   `const char *cd_path `   |  `int`  | use the linked list to create new file directory. |
| `file_sys_rmdir()`   |   `const char* rm_dir `   |  `int`  | remove target directory. . |
| `file_sys_rm()`   |   `const char* file_name `   |  `int`  | remove the file. |
| `file_sys_touch()`   | `const char* file_name `   |  `int`  | create the  new file in directory.|
| `file_sys_load()`   |   `const char* dump_file `   |  `int`  | load the dumpfile or create the dumpfile |
| `file_sys_put()`   |   `const char* file_name, const char* dst_path `   |  `int`  | put in the specify file in dir. |
| `file_sys_get()`   |   `const char *file_name, const char *password , uint8_t** out_buffer, size_t* out_size`   |  `int`  | obtian the specify file from directory. |



---

