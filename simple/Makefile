# Root-level Makefile

# Compiler and flags
CC := gcc
CFLAGS := -lrt

# Directories
DYNAMIC_DIR := dynamic
DYNAMIC_OFFSETS_DIR := dynamic_with_offsets

# Targets for programs in dynamic directory
DYNAMIC_TARGETS := $(DYNAMIC_DIR)/first_program $(DYNAMIC_DIR)/second_program
DYNAMIC_OFFSETS_TARGETS := $(DYNAMIC_OFFSETS_DIR)/first_program $(DYNAMIC_OFFSETS_DIR)/second_program

# All targets
.PHONY: all clean

all: $(DYNAMIC_TARGETS) $(STATIC_TARGETS) \
$(DYNAMIC_OFFSETS_TARGETS) $(STATIC_TARGETS) \

# Rules for dynamic programs
$(DYNAMIC_DIR)/first_program: $(DYNAMIC_DIR)/first_program.c $(DYNAMIC_DIR)/common.c $(DYNAMIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

$(DYNAMIC_DIR)/second_program: $(DYNAMIC_DIR)/second_program.c $(DYNAMIC_DIR)/common.c $(DYNAMIC_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

$(DYNAMIC_OFFSETS_DIR)/first_program: $(DYNAMIC_OFFSETS_DIR)/first_program.c $(DYNAMIC_OFFSETS_DIR)/common.c $(DYNAMIC_OFFSETS_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)

$(DYNAMIC_OFFSETS_DIR)/second_program: $(DYNAMIC_OFFSETS_DIR)/second_program.c $(DYNAMIC_OFFSETS_DIR)/common.c $(DYNAMIC_OFFSETS_DIR)/test_functions.c
	$(CC) -o $@ $^ $(CFLAGS)
	
# Clean all targets
clean:
	rm -f $(DYNAMIC_TARGETS) $(DYNAMIC_OFFSETS_TARGETS)
