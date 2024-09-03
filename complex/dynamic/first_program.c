// gcc -o first_program first_program.c struct.c test_functions.c -lrt

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

int shmid;

size_t calculateMasterStructSize() {
	size_t totalSize = sizeof(MasterStruct);

	// Add sizes of dynamically allocated arrays and buffers
	totalSize += ENV_SIZE * sizeof(short); // short_member

	totalSize += ENV_C1_BUF_CNT * sizeof(Custom1_Struct); // custom1_member array
	totalSize += ENV_C1_BUF_CNT * ENV_SIZE * 2 * sizeof(short); // C1_buffers in custom1_member

	totalSize += ENV_C2_BUF_CNT * sizeof(Custom2_Struct); // custom2_member array
	totalSize += ENV_C2_BUF_CNT * ENV_SIZE * sizeof(IQ_complex); // C2_buffers in custom2_member

	totalSize += ENV_C3_BUF_CNT * sizeof(Custom3_Struct); // custom3_member array
	totalSize += ENV_C3_BUF_CNT * ENV_C3_RX_PER_CHAN * ENV_C3_CHAN_SIZE * sizeof(float); // C3_buffer in custom3_member

	totalSize += (ENV_C4_CHAN_CNT + 2) * sizeof(Custom4_Struct *); // Pointers to Custom4_Struct arrays
	totalSize += (ENV_C4_CHAN_CNT + 2) * ENV_C4_BUF_CNT * sizeof(Custom4_Struct); // Custom4_Struct arrays
	totalSize += (ENV_C4_CHAN_CNT + 2) * ENV_C4_BUF_CNT * ENV_SIZE * sizeof(short); // C4_buffers in Custom4_Struct
	return totalSize;
}

volatile MasterStruct *initializeMasterStruct() {

	size_t shm_size = calculateMasterStructSize();

	// Create shared memory segment
	shmid = shmget(SHM_KEY, shm_size, IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	// Attach shared memory segment to the process's address space
	void *shared_mem_ptr = shmat(shmid, NULL, 0);
	if (shared_mem_ptr == (void *)-1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	globalStruct = (MasterStruct *)shared_mem_ptr;

	char *offset = (char *)shared_mem_ptr + sizeof(MasterStruct);
	
	globalStruct->short_member =  (short *)offset;
	offset += ENV_SIZE * sizeof(short);

	globalStruct->custom1_member =  (Custom1_Struct *)offset;
	offset += ENV_C1_BUF_CNT * sizeof(Custom1_Struct);

	for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
		globalStruct->custom1_member[i].C1_buffer =  (short *)offset;
		offset += ENV_SIZE * 2 * sizeof(short);
	}

	globalStruct->custom2_member =  (Custom2_Struct *)offset;
	offset += ENV_C2_BUF_CNT * sizeof(Custom2_Struct);

	for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
		globalStruct->custom2_member[i].C2_buffer =  (IQ_complex *)offset;
		offset += ENV_SIZE * sizeof(IQ_complex);
	}
	
	globalStruct->custom3_member =  (Custom3_Struct *)offset;
	offset += ENV_C3_BUF_CNT * sizeof(Custom3_Struct);

	for (int i = 0; i < ENV_C3_BUF_CNT; i++) {

        globalStruct->custom3_member[i].C3_buffer = (float **)offset;
        offset += ENV_C3_RX_PER_CHAN * sizeof(float *);
        
		for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
			globalStruct->custom3_member[i].C3_buffer[j] =  (float *)offset;
			offset += ENV_C3_CHAN_SIZE * sizeof(float);
		}
	}
	
	globalStruct->custom4_member =  (Custom4_Struct **)offset;
	offset += (ENV_C4_CHAN_CNT + 2) * sizeof(Custom4_Struct *);
	
	for (int i = 0; i < ENV_C4_CHAN_CNT + 2; i++) {
		globalStruct->custom4_member[i] =  (Custom4_Struct *)offset;
		offset += ENV_C4_BUF_CNT * sizeof(Custom4_Struct);
		
		for (int j = 0; j < ENV_C4_BUF_CNT; j++) {
			globalStruct->custom4_member[i][j].C4_buffer =  (short *)offset;
			offset += ENV_SIZE * sizeof(short);
		}
	}
	
   // globalStruct->custom5_member = (Custom5_Struct *)offset;

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
	memset(ptr->short_member, 0, ENV_SIZE * sizeof(short));

	// Zeroize Custom1_Struct
	for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
		ptr->custom1_member[i].C1_timestamp = 0.0;
		memset(ptr->custom1_member[i].C1_buffer, 0, ENV_SIZE * 2 * sizeof(short));
	}

	// Zeroize Custom2_Struct
	for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
		ptr->custom2_member[i].C2_timestamp = 0.0;
		memset(ptr->custom2_member[i].C2_buffer, 0, ENV_SIZE * sizeof(IQ_complex));
	}

	// Zeroize Custom3_Struct
	for (int i = 0; i < ENV_C3_BUF_CNT; i++) {
		ptr->custom3_member[i].C3_timestamp = 0.0;
		for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
			memset(ptr->custom3_member[i].C3_buffer[j], 0, ENV_C3_CHAN_SIZE * sizeof(float));
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

	setEnvars();
	getEnvars();

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
