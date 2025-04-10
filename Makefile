TARGET   = ft

STRICTFLAGS = -ansi -Wall -Wextra -Werror -Wpedantic -Wconversion -Wshadow -Wuninitialized -Wformat -Wlogical-op -Wimplicit-fallthrough -Wundef -Wmisleading-indentation -Wnull-dereference -Wduplicate-decl-specifier -Wredundant-decls
DEBUGFLAGS  = -g -D_GNU_SOURCE

CC       = gcc
CINCS    = -I. 
CFLAGS   = $(STRICTFLAGS) $(DEBUGFLAGS) $(CINCS)

LINKER   = gcc
LINCS    = -I. -lX11
LFLAGS   = $(STRICTFLAGS) $(DEBUGFLAGS) $(LINCS) 

SOURCES  := $(wildcard ./*.c)
INCLUDES := $(wildcard ./*.h)
OBJECTS  := $(SOURCES:./%.c=./%.o)
rm       = rm -f

./$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "***** LINKING COMPLETE *****"

$(OBJECTS): ./%.o : ./%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "***** COMPILATION COMPLETE *****"

.PHONY: run
run: ./$(TARGET)
	@./$(TARGET)
	@echo "***** EXECUTION COMPLETE *****"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "***** CLEANUP COMPLETE *****"

.PHONY: remove
remove: clean
	@$(rm) ./$(TARGET)
	@echo "***** EXECUTABLE REMOVED *****"

.PHONY: install
install:
	@pkill -x $(TARGET) || true
	@sleep 1
	@cp ./bin/$(TARGET) /usr/local/bin/
	@echo "***** EXECUTABLE INSTALLED *****"  

.PHONY: all
all: ./$(TARGET)

