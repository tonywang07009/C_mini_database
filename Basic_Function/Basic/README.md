# File system basic funciton 
----
## 1. Introduction

 - This file provides the core file‑system APIs used by the CLI to manage the in‑memory mini file system.

 - It implements all required operations for the __NTUT Advanced C final project__: __directory tree management, file creation, data storage, encryption support__, and dump/load of the whole file system.


-----
## 2. Data structures (overview)

- __Node__
    Represents one node in the file‑system tree. A node can be a directory or a file.

        Each node stores:

        - name: file or directory name (string)

        - type: NODE_DIR or NODE_FILE

        - parent: pointer to parent directory

        - child: first child node (for directories)

        - sibling: next node in the same directory

        - file: pointer to FileMeta (only valid if the node is a file)

- __FileMeta__
    Stores metadata and content for a file:

        Each node stores:
        
        - content: pointer to file data (plaintext or ciphertext)

        - original_size:  plaintext size

        - encrypted_size:  ciphertext size (if encrypted)

        - encrypted:  flag (0 = plaintext, 1 = encrypted)

        - aes_key,iv: runtime key and IV for AES (key is not stored in dump file)

-  __DiskNode__

    Header structure used when writing/reading dump files. It records node type, encryption flag, content size, original size, IV, and path length.

-  __FsStats__

    Collects statistics such as total/used blocks, inode count, and partition size.


----
## 3. API Overview

| Function | Parameters | Return | Role |
|---|---|---:|---|
| `file_sys_init()` | `void` | `void` | Initialize global variables and create the root directory `/`. Must be called once at start. |
| `file_sys_mkdir()` | `const char *dir_name` | `int` | Create a new directory node under current working directory (`g_cwd`). |
| `file_sys_ls()` | `const Node *nowdir` | `int` | List all child nodes (files and directories) in the given directory. |
| `file_sys_cd()` | `const char *cd_path` | `int` | Change current working directory. Supports `..`, `/`, and simple names. |
| `file_sys_rmdir()` | `const char *rm_dir` | `int` | Remove an empty or confirmed-non-empty directory under `g_cwd`. |
| `file_sys_rm()` | `const char *file_name` | `int` | Remove a file node under `g_cwd` and free its `FileMeta` and content buffer. |
| `file_sys_touch()` | `const char *file_name` | `int` | Create an empty file node under `g_cwd` and allocate an empty `FileMeta`. |
| `file_sys_put()` | `const char *file_path, const char *dst_path` | `int` | Read an external OS file into memory, store as a file node (optionally encrypt). |
| `file_sys_get()` | `const char *file_name, const char *password, uint8_t **out_buf, size_t *out_size` | `int` | Read a file node from the mini FS, decrypt if needed, and return a heap buffer to caller. |
| `file_sys_cat()` | `const char *file_name, const char *password` | `int` | Print file content to stdout. Decrypts in a temporary buffer when `encrypted == 1`. |
| `file_sys_load()` | `const char *dump_file` | `int` | Rebuild the whole directory tree and file contents from a dump file on disk. |
| `file_sys_state()` | `void` | `void` | Show basic information of files and directories in the current directory. |
| `file_sys_dump_dfs()` | `Node *node, const char *parent_path, FILE *fp` | `void` | DFS traversal used by dump; write `DiskNode` headers and content to an opened file. |
| `file_sys_rule_display()` | `const Node *current_dir` | `void` | Print the full path of `current_dir` (used in CLI prompt). |
| `file_sys_in_dir()` | `Node *dir_path, const char *name, int want_file` | `Node *` | Search a child by name inside `dir_path`, filtering by type (file/dir). |
| `file_sys_resolve()` | `const char *path, int want_file` | `Node *` | Resolve a relative or absolute path into a `Node *` using `/` splitting. |
| `file_sys_get_basename()` | `const char *path` | `const char *` | Return pointer to the last component of a path (basename). |
| `file_sys_encrypt_content()` | `FileMeta *file_meta, const char *password` | `int` | Derive AES key/IV from password and encrypt the file content in place. |
| `file_sys_decrypt_content()` | `FileMeta *file_meta, const char *password` | `int` | Regenerate key/IV from password and decrypt content in place. |
| `file_sys_ensure_dump_dir()` | `void` | `void` | Create dump directory (`Dump_store`) if it does not exist. |
| `file_sys_get_stats()` | `FsStats *st` | `void` | Traverse the tree and fill `FsStats` (blocks, inodes, partition size, etc.). |

---

