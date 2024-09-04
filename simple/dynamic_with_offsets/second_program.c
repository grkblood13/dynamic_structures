// gcc -o second_program second_program.c common.c test_functions.c -lrt

#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

void attachToSimpleStruct() {

	int shmid = shmget(SHM_KEY, calculateSimpleStructSize(), 0666);
	if (shmid < 0) {
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}

	void *shared_mem_ptr = (SimpleStruct *)shmat(shmid, (void *)0x50000000, 0);
	if (simpleStruct == (void *)-1) {
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}

	simpleStruct = (SimpleStruct *)shared_mem_ptr;
	simpleStruct->short_member_1 =  (short *)((char *)shared_mem_ptr + simpleStruct->short_member_1_offset);
	simpleStruct->short_member_2 =  (short *)((char *)shared_mem_ptr + simpleStruct->short_member_2_offset);
}

int main() {	
	setEnvars();
	getEnvars();
	attachToSimpleStruct();
	printf("second_program printing test values:\n");
	printSimpleStruct(simpleStruct);
	printf("\n");
	/*
	printf("second_program resetting test values\n");
	setTestValues(simpleStruct);
	printf("second_program reprinting test values:\n");
	printSimpleStruct(simpleStruct);
	printf("\n");
	*/
	return 0;
}
