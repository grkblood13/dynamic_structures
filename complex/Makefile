# Root-level Makefile

# Compiler and flags
CC := gcc
CFLAGS := -lrt

# Directories
DYNAMIC_DIR := dynamic
STATIC_DIR := static

# Targets for programs in dynamic directory
DYNAMIC_TARGETS := $(DYNAMIC_DIR)/first_program $(DYNAMIC_DIR)/second_program

# Targets for programs in static directory
STATIC_TARGETS := $(STATIC_DIR)/first_program $(STATIC_DIR)/second_program

# All targets
.PHONY: all clean

all: $(DYNAMIC_TARGETS) $(STATIC_TARGETS)

# Rules for dynamic programs
$(DYNAMIC_DIR)/first_program: $(DYNAMIC_DIR)/first_program.c $(DYNAMIC_DIR)/struct.c $(DYNAMIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

$(DYNAMIC_DIR)/second_program: $(DYNAMIC_DIR)/second_program.c $(DYNAMIC_DIR)/struct.c $(DYNAMIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

# Rules for static programs
$(STATIC_DIR)/first_program: $(STATIC_DIR)/first_program.c $(STATIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

$(STATIC_DIR)/second_program: $(STATIC_DIR)/second_program.c $(STATIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

# Clean all targets
clean:
	rm -f $(DYNAMIC_TARGETS) $(STATIC_TARGETS)
