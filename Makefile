TARGET   = ft

STRICTFLAGS = -std=c99 -Wall -Wextra -Werror -Wpedantic
DEBUGFLAGS  = -g -D_GNU_SOURCE

CC       = gcc
CINCS    = -I./inc
CFLAGS   = $(shell pkg-config --cflags wayland-client) $(STRICTFLAGS) $(DEBUGFLAGS) $(CINCS)

LINKER   = gcc
LINCS    = -I./inc
LFLAGS   = $(shell pkg-config --libs wayland-client) $(STRICTFLAGS) $(DEBUGFLAGS) $(LINCS) 

SRCDIR   = src
INCDIR   = inc
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "***** LINKING COMPLETE *****"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(BINDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "***** COMPILATION COMPLETE *****"

.PHONY: run
run: $(BINDIR)/$(TARGET)
	@./$(BINDIR)/$(TARGET)
	@echo "***** EXECUTION COMPLETE *****"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "***** CLEANUP COMPLETE *****"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "***** EXECUTABLE REMOVED *****"

.PHONY: install
install:
	@pkill -x $(TARGET) || true
	@sleep 1
	@cp ./bin/$(TARGET) /usr/local/bin/
	@echo "***** EXECUTABLE INSTALLED *****"  

.PHONY: all
all: $(BINDIR)/$(TARGET)

