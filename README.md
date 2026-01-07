# C_Simple_Mini_Database

---------
## 1. Project Describe 

This project is implemented in **C** as the final project for the NTUT __*Advanced C*__ course.

- Basic in‑memory file system (directories, files, simple tree structure)

- Optional **AES‑128** encryption for file contents (using tiny-AES-c)

- Manual and automatic dump/load of the whole file system to a dump file

- Simple CLI (mkdir/ls/cd/put/get/cat/status/dump/exit)


-----
## 2. How to used

### 2.1 Add The tiny-AES-c
Library source:

- **[tiny-AES-c](https://github.com/kokke/tiny-AES-c.git)**


__Unix & Linux & Powershell__ 
```
    cd /Additional_Function
    git clone https://github.com/kokke/tiny-AES-c.git
```

### 2.2 Makefile

__Unix & Linux__

```
    make clean VERBOSE=1
    make VERBOSE=1
```
__Windows__

```
    mingw32-make clean VERBOSE=1
    mingw32-make VERBOSE=1
```

### 2.3 Join The cli
__Unix & Linux__
```
    cd /CLI
    ./CLI
```
__Windows__
```
    cd /CLI
    .\CLI.exe
```

----
## 3. Document

- **[CLi_function](./Cli_Function/README.md)**

- **[Basic_function](./Basic_Function/Basic/README.md)**

- **[Security_function](./Basic_Function/Security/README.md)**

----
## License
---
    This project is open-source and available under the MIT License.