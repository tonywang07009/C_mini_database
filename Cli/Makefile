CC     = gcc
CFLAGS = -Wall -Wextra -std=c11

# The basic function

BASIC_FUNCTION =\
					../Basic_Function/list/Basic_function.c\
					../Cli/CLI_API.c\
					../Cli/test.c
										
BASIC_FUNCTION_OBJ = $(BASIC_FUNCTION:.c=.o)

TARGET = CLI

all: $(TARGET)
$(TARGET): $(BASIC_FUNCTION_OBJ)
	$(CC) $(CFLAGS)	-o $@ $^

clean:
	del *.exe 2>nul || true
	del *.o   2>nul || true
.PHONY: all clean