#include <unistd.h>

#define SHM_KEY 1234

extern int ENV_SIZE;

typedef struct {
	int int_member;
	short *short_member_1; // [ENV_SIZE]
	short *short_member_2; // [ENV_SIZE]
} SimpleStruct;

extern volatile SimpleStruct *simpleStruct;  // Declare global pointer

void getEnvars();
void setEnvars();
volatile SimpleStruct *initializeSimpleStruct();
size_t calculateSimpleStructSize();
void setTestValues(volatile SimpleStruct *ptr);  // Declare setTestValues
void printSimpleStruct(volatile SimpleStruct *ptr); // Declare printSimpleStruct
void zeroizeSimpleStruct(volatile SimpleStruct *ptr);
