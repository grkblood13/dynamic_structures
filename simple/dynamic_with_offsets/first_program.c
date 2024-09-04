// gcc -o first_program first_program.c common.c test_functions.c -lrt
#define _GNU_SOURCE
#include "struct.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

int shmid;

volatile SimpleStruct *initializeSimpleStruct() {

	size_t shm_size = calculateSimpleStructSize();

	// Create shared memory segment
	shmid = shmget(SHM_KEY, shm_size, IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	// Attach shared memory segment to the process's address space
	//void *shared_mem_ptr = shmat(shmid, NULL, 0);
	void *shared_mem_ptr = shmat(shmid, (void *)0x50000000, 0);
	if (shared_mem_ptr == (void *)-1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	simpleStruct = (SimpleStruct *)shared_mem_ptr;

	char *offset = (char *)shared_mem_ptr + sizeof(SimpleStruct);

	simpleStruct->short_member_1_offset = offset - (char *)shared_mem_ptr;
	simpleStruct->short_member_1 =  (short *)((char *)shared_mem_ptr + simpleStruct->short_member_1_offset);
	offset += ENV_SIZE * sizeof(short);

	simpleStruct->short_member_2_offset = offset - (char *)shared_mem_ptr;
	simpleStruct->short_member_2 =  (short *)((char *)shared_mem_ptr + simpleStruct->short_member_2_offset);

	return simpleStruct;
}

void zeroizeSimpleStruct(volatile SimpleStruct *ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "SimpleStruct is NULL\n");
		return;
	}

	// Zeroize int_member and double_member
	ptr->int_member = 0;

	// Zeroize short_member_1
	memset(ptr->short_member_1, 0, ENV_SIZE * sizeof(short));

	// Zeroize short_member_2
	memset(ptr->short_member_1, 0, ENV_SIZE * sizeof(short));

}

// Detach the shared memory
void detachSimpleStruct(volatile SimpleStruct *ptr) {
	if (ptr == NULL) {
		return; // No need to detach if already NULL
	}
	shmdt((void *)ptr);
	simpleStruct = NULL;
}

// Mark the shared memory for deletion
void destroySimpleStruct(int id) {
	if (id == -1) {
		return;
	}
	shmctl(id, IPC_RMID, NULL);
}

void handle_signal(int signal) {
	if (signal == SIGINT) {
		printf("\nDestroy memory.\n");    
		destroySimpleStruct(shmid);
	} else if (signal == SIGTERM) {
		printf("\nDestroy memory.\n");    
		destroySimpleStruct(shmid);
	}
	exit(0);
}

int main() {

	setEnvars();
	getEnvars();

	printf("first_program initializing structure\n");
	simpleStruct = initializeSimpleStruct();

	// Set test values
	printf("first_program setting test values\n");
	setTestValues(simpleStruct);

	// Print the values
	printf("\first_program printing test values:\n");
	printSimpleStruct(simpleStruct);
	printf("\n");
	
	// Run second program
	printf("running second_program\n");
	pid_t pid = 0;
	pid = fork();

	if (pid < 0) {

			perror("fork failed");
			return 1;

	} else if (pid == 0) {

			cpu_set_t set;
			CPU_ZERO(&set);
			CPU_SET(1, &set);
			if (sched_setaffinity(0, sizeof(set), &set) == -1) {
					perror("sched_setaffinity failed");
					exit(1);
			}

			char *program = "./second_program";
			char *args[] = {program, NULL};
			if (execvp(program, args) == -1) {
					perror("execvp failed");
					exit(1);
			}
	} else {
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)) {
					printf("Child exited with status %d\n", WEXITSTATUS(status));
			} else {
					printf("Child process did not terminate normally\n");
			}
	}

	sleep(1);


	// Print the values again
	printf("\nfirst_program printing test values:\n");
	printSimpleStruct(simpleStruct);

	detachSimpleStruct(simpleStruct);
	destroySimpleStruct(shmid);
	
	return 0;
}
