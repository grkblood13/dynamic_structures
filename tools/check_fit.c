#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to check if the structure fits within the provided memory range
int will_fit_at_address(unsigned long start_address, size_t struct_size, unsigned long next_address) {
	unsigned long end_address = start_address + struct_size;  // Calculate the end address for the structure

	if (end_address > next_address) {
	printf("Structure of size %zu bytes will NOT fit at address 0x%lx; it overlaps with next structure at 0x%lx.\n",
	struct_size, start_address, next_address);
	return 0;  // Will not fit
	}

	printf("Structure of size %zu bytes will fit at address 0x%lx.\n", struct_size, start_address);
	return 1;  // Will fit
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <start_address> <next_address> <struct_size>\n", argv[0]);
		return 1;
	}

	// Parse command-line arguments as hexadecimal values
	unsigned long start_address = strtoul(argv[1], NULL, 16);  // Start address
	unsigned long next_address = strtoul(argv[2], NULL, 16);   // Address of next structure

	// Parse the struct size as size_t, which is the correct type returned by calculateStructSize()
	size_t struct_size = strtoull(argv[3], NULL, 10);          // Size of the structure in bytes

	// Validate addresses and struct size
	if (start_address == 0 || next_address == 0 || struct_size == 0) {
		fprintf(stderr, "Invalid input: addresses and struct size must be valid numbers.\n");
		return 1;
	}

	// Check if the structure will fit at the given start address without overlapping the next address
	will_fit_at_address(start_address, struct_size, next_address);

	return 0;
}
