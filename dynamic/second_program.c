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

	globalStruct = (MasterStruct *)shmat(shmid, NULL, 0);
	if (globalStruct == (void *)-1) {
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}

	// set offsets so pointers can be accessed
	size_t *custom1_member_offset;
	size_t *custom2_member_offset;
	size_t *custom3_member_offset;
	size_t *custom4_member_row_offset;

	globalStruct->short_member = (short *)((char *)globalStruct + globalStruct->short_member_offset);

	// Set up pointers for custom1_member
	globalStruct->custom1_member = (Custom1_Struct *)((char *)globalStruct + globalStruct->custom1_member_offset);
	custom1_member_offset = (size_t *)((char *)globalStruct + globalStruct->custom1_member_offset);
	for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
		globalStruct->custom1_member[i].C1_buffer = (short *)((char *)globalStruct + custom1_member_offset[i]);
	}

	printf("set up custom1_member\n");

	// Set up pointers for custom2_member
	globalStruct->custom2_member = (Custom2_Struct *)((char *)globalStruct + globalStruct->custom2_member_offset);
	custom2_member_offset = (size_t *)((char *)globalStruct + globalStruct->custom2_member_offset);
	for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
		globalStruct->custom2_member[i].C2_buffer = (IQ_complex *)((char *)globalStruct + custom2_member_offset[i]);
	}

	printf("set up custom2_member\n");

	// Set up pointers for custom3_member
	globalStruct->custom3_member = (Custom3_Struct *)((char *)globalStruct + globalStruct->custom3_member_offset);
	custom3_member_offset = (size_t *)((char *)globalStruct + globalStruct->custom3_member_offset);
	for (int i = 0; i < ENV_C3_BUF_CNT; i++) {
		globalStruct->custom3_member[i].C3_buffer = (float **)((char *)globalStruct + custom3_member_offset[i]);
		for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
			globalStruct->custom3_member[i].C3_buffer[j] = (float *)((char *)globalStruct + custom3_member_offset[i] + j * ENV_C3_RX_PER_CHAN * sizeof(float));
		}
	}

	printf("set up custom3_member\n");

	// Set up pointers for custom4_member
	globalStruct->custom4_member = (Custom4_Struct **)((char *)globalStruct + globalStruct->custom4_member_row_offset);
	custom4_member_row_offset = (size_t *)((char *)globalStruct + globalStruct->custom4_member_row_offset);
	for (int i = 0; i < ENV_C4_CHAN_CNT + 2; i++) {
		globalStruct->custom4_member[i] = (Custom4_Struct *)((char *)globalStruct + custom4_member_row_offset[i]);
		for (int j = 0; j < ENV_C4_BUF_CNT; j++) {
			globalStruct->custom4_member[i][j].C4_buffer = (short *)((char *)globalStruct + custom4_member_row_offset[i * ENV_C4_BUF_CNT * j]);
		}
	}

	printf("set up custom4_member\n");

}

int main() {	
	setEnvars();
	getEnvars();
	attachToMasterStruct();
	printMasterStruct(globalStruct);
	return 0;
}
