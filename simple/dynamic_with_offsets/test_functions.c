#include "struct.h"
#include <stdio.h>

void setTestValues(volatile SimpleStruct *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SimpleStruct is NULL\n");
        return;
    }

    ptr->int_member = 1;

    for (int i = 0; i < ENV_SIZE; i++) {
        ptr->short_member_1[i] = (short)(i % 256);
    }
    
    for (int i = 0; i < ENV_SIZE; i++) {
        ptr->short_member_2[i] = (short)(i % 256);
    }
}

void printSimpleStruct(volatile SimpleStruct *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SimpleStruct is NULL\n");
        return;
    }

    printf("int_member: %d\n", ptr->int_member);

    // Print short_member_1
    printf("short_member_1: ");
    for (int i = 0; i < ENV_SIZE; i++) {
        printf("%d ", ptr->short_member_1[i]);
    }
    printf("\n");

    // Print short_member_2
    printf("short_member_2: ");
    for (int i = 0; i < ENV_SIZE; i++) {
        printf("%d ", ptr->short_member_2[i]);
    }
    printf("\n");
}
