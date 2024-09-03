// gcc -o second_program second_program.c test_functions.c -lrt

#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SHM_KEY 1234

volatile MasterStruct *globalStruct = NULL;  // Define global pointer

void attachToMasterStruct() {

	size_t shm_size = sizeof(MasterStruct);
	int shmid = shmget(SHM_KEY, shm_size, 0666);
	if (shmid < 0) {
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}

	globalStruct = (MasterStruct *)shmat(shmid, NULL, 0);
	if (globalStruct == (void *)-1) {
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}

}

int main() {	
	attachToMasterStruct();
	printMasterStruct(globalStruct);
	return 0;
}
