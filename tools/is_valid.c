// gcc -o is_valid is_valid.c

#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int is_valid_address(void *addr) {
    long page_size = sysconf(_SC_PAGESIZE);  // Get the system's page size
    void *result = mmap(addr, page_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    
    if (result == MAP_FAILED) {
        if (errno == EEXIST) {
            printf("Address %p is already in use!\n", addr);
        } else {
            printf("Error with mmap: %s\n", strerror(errno));
        }
        return 0;  // Address is invalid
    }
    
    // If we reach here, the address is valid; unmap it immediately
    munmap(result, page_size);
    return 1;  // Address is valid
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_memory_address>\n", argv[0]);
        return 1;
    }

    // Convert command-line argument from hex string to unsigned long
    unsigned long address = strtoul(argv[1], NULL, 16);
    if (address == 0 && (errno == EINVAL || errno == ERANGE)) {
        fprintf(stderr, "Invalid address format: %s\n", argv[1]);
        return 1;
    }

    void *desired_address = (void *)address;  // Cast to a void pointer

    // Check if the desired address is valid
    if (is_valid_address(desired_address)) {
        printf("Address %p is available for use!\n", desired_address);
    } else {
        printf("Address %p is not available for use!\n", desired_address);
    }

    return 0;
}
