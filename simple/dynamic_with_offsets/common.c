#include "struct.h"
#include <stdlib.h>

int ENV_SIZE;

volatile SimpleStruct *simpleStruct = NULL;  // Define global pointer

size_t calculateSimpleStructSize() {
	size_t totalSize = sizeof(SimpleStruct);

	// Add sizes of dynamically allocated arrays and buffers
	totalSize += ENV_SIZE * sizeof(short); // short_member_1
	totalSize += ENV_SIZE * sizeof(short); // short_member_2

	return totalSize;
}

int get_env_var_as_int(const char *varname) {
	const char *value = getenv(varname);
	return value ? atoi(value) : 0;
}

void setEnvars() {
	setenv("ENV_SIZE", "2", 1);
}

void getEnvars() {
	ENV_SIZE = get_env_var_as_int("ENV_SIZE");
}
