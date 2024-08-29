// gcc -o second_program second_program.c struct.c test_functions.c -lrt

#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SHM_KEY 1234

size_t calculateMasterStructSize() {
	size_t totalSize = sizeof(MasterStruct);

	// Add sizes of dynamically allocated arrays and buffers
	totalSize += ENV_SIZE * sizeof(short); // short_member

	totalSize += ENV_C1_BUF_CNT * sizeof(Custom1_Struct); // custom1_member array
	totalSize += ENV_C1_BUF_CNT * ENV_SIZE * 2 * sizeof(short); // C1_buffers in custom1_member

	totalSize += ENV_C2_BUF_CNT * sizeof(Custom2_Struct); // custom2_member array
	totalSize += ENV_C2_BUF_CNT * ENV_SIZE * sizeof(IQ_complex); // C2_buffers in custom2_member

	totalSize += ENV_C3_BUF_CNT * sizeof(Custom3_Struct); // custom3_member array
	totalSize += ENV_C3_BUF_CNT * ENV_C3_RX_PER_CHAN * ENV_C3_CHAN_SIZE * sizeof(float); // C3_buffers in custom3_member

	totalSize += (ENV_C4_CHAN_CNT + 2) * sizeof(Custom4_Struct *); // Pointers to Custom4_Struct arrays
	totalSize += (ENV_C4_CHAN_CNT + 2) * ENV_C4_BUF_CNT * sizeof(Custom4_Struct); // Custom4_Struct arrays
	totalSize += (ENV_C4_CHAN_CNT + 2) * ENV_C4_BUF_CNT * ENV_SIZE * sizeof(short); // C4_buffers in Custom4_Struct
	return totalSize;
}


void attachToMasterStruct() {

	int shmid = shmget(SHM_KEY, calculateMasterStructSize(), 0666);
	if (shmid < 0) {
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}

	void *shared_mem_ptr = (MasterStruct *)shmat(shmid, NULL, 0);
	if (globalStruct == (void *)-1) {
		perror("shmat failed");
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
}

int main() {	
	setEnvars();
	getEnvars();
	attachToMasterStruct();
	printMasterStruct(globalStruct);
	return 0;
}
