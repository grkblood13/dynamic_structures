// gcc -o first_program first_program.c test_functions.c -lrt

#include "struct.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SHM_KEY 1234

volatile MasterStruct *globalStruct = NULL;  // Define global pointer
int shmid;

volatile MasterStruct *initializeMasterStruct() {

	size_t shm_size = sizeof(MasterStruct);

	// Create shared memory segment
	shmid = shmget(SHM_KEY, shm_size, IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	// Attach shared memory segment to the process's address space
	globalStruct = shmat(shmid, NULL, 0);
	if (globalStruct == (void *)-1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	return globalStruct;
}

void zeroizeMasterStruct(volatile MasterStruct *ptr) {

	if (ptr == NULL) {
		fprintf(stderr, "MasterStruct is NULL\n");
		return;
	}

	// Zeroize int_member and double_member
	ptr->int_member = 0;
	ptr->double_member = 0.0;

	// Zeroize short_member
	memset((void *)ptr->short_member, 0, ENV_SIZE * sizeof(short));

	// Zeroize Custom1_Struct
	for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
		ptr->custom1_member[i].C1_timestamp = 0.0;
		memset((void *)ptr->custom1_member[i].C1_buffer, 0, ENV_SIZE * 2 * sizeof(short));
	}

	// Zeroize Custom2_Struct
	for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
		ptr->custom2_member[i].C2_timestamp = 0.0;
		memset((void *)ptr->custom2_member[i].C2_buffer, 0, ENV_SIZE * sizeof(IQ_complex));
	}

	// Zeroize Custom3_Struct
	for (int i = 0; i < ENV_C3_BUF_CNT; i++) {
		ptr->custom3_member[i].C3_timestamp = 0.0;
		for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
			memset((void *)ptr->custom3_member[i].C3_buffer[j], 0, ENV_C3_CHAN_SIZE * sizeof(float));
		}
	}

	// Zeroize Custom4_Struct
	for (int i = 0; i < ENV_C4_CHAN_CNT + 2; i++) {
		for (int j = 0; j < ENV_C4_BUF_CNT; j++) {
			ptr->custom4_member[i][j].C4_timestamp = 0.0;
			memset((void *)ptr->custom4_member[i][j].C4_buffer, 0, ENV_SIZE * sizeof(short));
		}
	}

	// Zeroize Custom5_Struct
	memset((void *)ptr->custom5_member, 0, sizeof(ptr->custom5_member));
}

// Detach the shared memory
void detachMasterStruct(volatile MasterStruct *ptr) {
	if (ptr == NULL) {
		return; // No need to detach if already NULL
	}
	shmdt((void *)ptr);
	globalStruct = NULL;
}

// Mark the shared memory for deletion
void destroyMasterStruct(int id) {
	if (id == -1) {
		return;
	}
	shmctl(id, IPC_RMID, NULL);
}

void handle_signal(int signal) {
	if (signal == SIGINT) {
		printf("\nDestroy memory.\n");    
		destroyMasterStruct(shmid);
	} else if (signal == SIGTERM) {
		printf("\nDestroy memory.\n");    
		destroyMasterStruct(shmid);
	}
	exit(0);
}

int main() {

	// Register the signal handler
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	printf("\nInitialize MasterStruct:\n");
	globalStruct = initializeMasterStruct();

	// Set test values
	printf("\nSet Test Values\n");
	setTestValues(globalStruct);

	// Print the values
	printMasterStruct(globalStruct);

	// Zeroize the data in MasterStruct
	printf("\nZeroize MasterStruct\n");
	zeroizeMasterStruct(globalStruct);

	// Print the zeroized values
	printf("\nZeroized values:\n");
	printMasterStruct(globalStruct);

	// Set test values again
	printf("\nSet Test Values again\n");
	setTestValues(globalStruct);

	// Print the values again
	printf("\nValues after resetting:\n");
	printMasterStruct(globalStruct);

	printf("\nDetach from memory:\n");
	detachMasterStruct(globalStruct);
	printMasterStruct(globalStruct);

	while (1) {
		sleep(1);
	}

	return 0;
}
