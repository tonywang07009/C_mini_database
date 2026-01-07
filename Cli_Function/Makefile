CC     = gcc
CFLAGS = -Wall -Wextra -std=c11

# --------------------
# OS detection
# --------------------
ifeq ($(OS),Windows_NT)
    # Windows / MinGW
    DETECTED_OS := Windows
else
    # Linux / macOS
    DETECTED_OS := $(shell uname -s)
endif

# Setting os different instruction for del
ifeq ($(DETECTED_OS),Windows)
    RM = del /s /q
    EXE_EXT = .exe
else
    RM = rm -f
    EXE_EXT =
endif

# --------------------
# Sources / Objects
# --------------------
BASIC_FUNCTION = \
    ../Basic_Function/Basic/Basic_function.c \
    ../Basic_Function/Security/Aes_function.c \
    ./CLI_API.c \
    ./main.c

SAFETY_FUNCTION = ../Additional_Function/tiny-AES-c/aes.c

BASIC_FUNCTION_OBJ  = $(BASIC_FUNCTION:.c=.o)
SAFETY_FUNCTION_OBJ = $(SAFETY_FUNCTION:.c=.o)

TARGET = CLI$(EXE_EXT)

# --------------------
# Build
# --------------------

all: $(TARGET)

$(TARGET): $(BASIC_FUNCTION_OBJ) $(SAFETY_FUNCTION_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------
# Clean
# --------------------

# Recursion del
ifeq ($(DETECTED_OS),Windows)
    # Windows / MinGW: %%f for Make, double quotes for path
    # Also delete the target EXE
    CLEAN_CMD = for /r .. %%f in (*.o) do del /q "%%f" & if exist $(TARGET) del /q $(TARGET)
else
    # Linux / macOS
    CLEAN_CMD = find .. -name '*.o' -type f -delete && rm -f $(TARGET)
endif

clean:
	@echo "Cleaning object files and executable recursively..."
	@$(CLEAN_CMD)

.PHONY: all clean
