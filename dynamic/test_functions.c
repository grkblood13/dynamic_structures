#include "struct.h"
#include <time.h>
#include <stdio.h>

void setTestValues(volatile MasterStruct *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "MasterStruct is NULL\n");
        return;
    }

    ptr->int_member = 1;
    ptr->double_member = 1000.0;

    for (int i = 0; i < ENV_SIZE; i++) {
        ptr->short_member[i] = (short)(i % 256);
    }

    // Set test values for Custom1_Struct
    for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
        ptr->custom1_member[i].C1_timestamp = (double)time(NULL) + i;
        for (int j = 0; j < ENV_SIZE * 2; j++) {
            ptr->custom1_member[i].C1_buffer[j] = (short)(j % 512);
        }
    }

    // Set test values for Custom2_Struct
    for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
        ptr->custom2_member[i].C2_timestamp = (double)time(NULL) + i;
        for (int j = 0; j < ENV_SIZE; j++) {
            ptr->custom2_member[i].C2_buffer[j].re = (float)(j % 256) / 256.0f;
            ptr->custom2_member[i].C2_buffer[j].im = (float)(255 - (j % 256)) / 256.0f;
        }
    }

    // Set test values for Custom3_Struct
    for (int i = 0; i < ENV_C3_BUF_CNT; i++) {
        ptr->custom3_member[i].C3_timestamp = (double)time(NULL) + i;
        for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
            for (int k = 0; k < ENV_C3_CHAN_SIZE; k++) {
                ptr->custom3_member[i].C3_buffer[j][k] = (float)(j * k % 256) / 256.0f;
            }
        }
    }

	// Set test values for Custom4_Struct
    for (int i = 0; i < ENV_C4_CHAN_CNT + 2; i++) {
        for (int j = 0; j < ENV_C4_BUF_CNT; j++) {
            ptr->custom4_member[i][j].C4_timestamp = (double)(i + j) * 0.5;
            for (int k = 0; k < ENV_SIZE; k++) {
                ptr->custom4_member[i][j].C4_buffer[k] = (short)(i + j + k);
            }
        }
    }
	
    // Set test values for Custom5_Struct
    for (int i = 0; i < ENV_C5_MAX_PER_CHAN; i++) {
        ptr->custom5_member[i].freq = i * 1000;
        ptr->custom5_member[i].type = i % 2;
        ptr->custom5_member[i].flag = (i % 3 == 0);
        ptr->custom5_member[i].filler = 0;
    }
}

void printMasterStruct(volatile MasterStruct *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "MasterStruct is NULL\n");
        return;
    }

    printf("int_member: %d\n", ptr->int_member);
    printf("double_member: %f\n", ptr->double_member);

    // Print short_member
    printf("short_member: ");
    for (int i = 0; i < ENV_SIZE; i++) {
        printf("%d ", ptr->short_member[i]);
    }
    printf("\n");

    // Print Custom1_Struct values
    for (int i = 0; i < ENV_C1_BUF_CNT; i++) {
        printf("Custom1_Struct[%d].C1_timestamp: %f\n", i, ptr->custom1_member[i].C1_timestamp);
        printf("Custom1_Struct[%d].C1_buffer: ", i);
        for (int j = 0; j < ENV_SIZE * 2; j++) {
            printf("%d ", ptr->custom1_member[i].C1_buffer[j]);
        }
        printf("\n");
    }

    // Print Custom2_Struct values
    for (int i = 0; i < ENV_C2_BUF_CNT; i++) {
        printf("Custom2_Struct[%d].C2_timestamp: %f\n", i, ptr->custom2_member[i].C2_timestamp);
        printf("Custom2_Struct[%d].C2_buffer: ", i);
        for (int j = 0; j < ENV_SIZE; j++) {
            printf("(%f, %f) ", ptr->custom2_member[i].C2_buffer[j].re, ptr->custom2_member[i].C2_buffer[j].im);
        }
        printf("\n");
    }

    // Print Custom3_Struct values
    for (int i = 0; i < ENV_C3_BUF_CNT; i++) {
        printf("Custom3_Struct[%d].C3_timestamp: %f\n", i, ptr->custom3_member[i].C3_timestamp);
        printf("Custom3_Struct[%d].C3_buffer: ", i);
        for (int j = 0; j < ENV_C3_RX_PER_CHAN; j++) {
            for (int k = 0; k < ENV_C3_CHAN_SIZE; k++) {
                printf("%f ", ptr->custom3_member[i].C3_buffer[j][k]);
            }
        }
        printf("\n");
    }
	
	// Print Custom4_Struct values
    for (int i = 0; i < ENV_C4_CHAN_CNT + 2; i++) {
        for (int j = 0; j < ENV_C4_BUF_CNT; j++) {
            printf("custom4_member[%d][%d].C4_timestamp: %f\n", i, j, ptr->custom4_member[i][j].C4_timestamp);
            for (int k = 0; k < ENV_SIZE; k++) {
                printf("custom4_member[%d][%d].C4_buffer[%d]: %d\n", i, j, k, ptr->custom4_member[i][j].C4_buffer[k]);
            }
		}
	}

    // Print Custom5_Struct values
    for (int i = 0; i < ENV_C5_MAX_PER_CHAN; i++) {
        printf("Custom5_Struct[%d].freq: %u\n", i, ptr->custom5_member[i].freq);
        printf("Custom5_Struct[%d].type: %d\n", i, ptr->custom5_member[i].type);
        printf("Custom5_Struct[%d].flag: %d\n", i, ptr->custom5_member[i].flag);
        printf("Custom5_Struct[%d].filler: %d\n", i, ptr->custom5_member[i].filler);
    }
}
