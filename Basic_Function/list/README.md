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

### `run_multi_client_sessions(const Cli_information *config)`

**Purpose:** Manages the lifecycle of concurrent client simulations.

**Flow:**

1. __Validation:__
   - Checks if thread_count and package_count are positive integers. Returns error 1 if invalid.

2. __Configuration Prep:__
   - Allocates arrays for thread IDs (`pthread_t`) and client arguments (`ClientArg_t`).

   - Distributes global config (`IP, port`) to individual thread configs.

   - Assigns unique `machine_id` to each thread.

3. __Thread Execution:__
   - Loops to call `pthread_create()` for each client.

   -  Passes specific `cfg[i]` to avoid race conditions on argument data.

4. __Aggregation:__
   
   - Loops to `pthread_join()` all threads to ensure completion.

   - Sums up `bad_count` from all threads to report total transmission failures.

5. __Reporting:__ 
    
   - Prints total bad packages or a success message.

---

### `run_server_process(const Cli_information* cfg)`

**Purpose:** Sets up the server environment, including IPC (Inter-Process Communication) and process forking.

**Flow:**

1. __Shared Memory Setup:__
    - Uses `shm_open()` and `ftruncate()` to create a shared memory object (SHM_NAME).
  
    - Maps it to process memory `via mmap()` to store ServerSharedStats_t.

2. __Network Initialization:__
    - Creates a TCP socket (`socket`).
  
    - Sets `SO_REUSEADDR` via `setsockopt()` to allow immediate restart on the same port.
  
    - Binds to `INADDR_ANY` and the specified port.
  
    - Starts listening with a backlog of 500.

3. __Concurrency Setup:__
    - Initializes a Process-Shared Mutex (`PTHREAD_PROCESS_SHARED`) in shared memory to protect the `total_requests` counter.

    - Logs the server start event using `syslog_info()`.

4. __Worker Forking:__

    - Forks `child_process_count (32)` worker processes.

    - __Child:__ Executes run_single_process_server() and exits.

    - __Parent:__ Waits for all children (wait()) to prevent zombie processes.
  
5. __Cleanup:__

    - Unmaps and unlinks shared memory (__shm_unlink__).

    - Closes the main server socket.


--- 

### `cli_show_system_log(void)`

**Purpose:** Displays the most recent activity from the system log file without reading the entire file into memory at once.


**Flow:**
1. Opens system.log in read mode. Handles file open errors gracefully..


2. __Circular Buffer Reading:__
  - Uses a string array __lines[LOG_TAIL_LINES]__ as a circular buffer.

  - Reads the file line-by-line using __fgets__.
  
  - Overwrites old lines cyclically: __idx = count % LOG_TAIL_LINES__.

  - Uses strdup to allocate memory for each line (and frees the old line before overwriting).

3. __Display Logic__:
   - Calculates the __starting index__ to print lines in correct chronological order.

   - Prints the last __N lines__ (up to `LOG_TAIL_LINES`).

4. Frees all allocated strings in the buffer before returning to prevent memory leaks.
