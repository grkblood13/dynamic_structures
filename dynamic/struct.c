#include "struct.h"
#include <stdlib.h>

int ENV_SIZE;
int ENV_C1_BUF_CNT;
int ENV_C2_BUF_CNT;
int ENV_C3_BUF_CNT;
int ENV_C3_RX_PER_CHAN;
int ENV_C3_CHAN_SIZE;
int ENV_C4_CHAN_CNT;
int ENV_C4_BUF_CNT;

volatile MasterStruct *globalStruct = NULL;  // Define global pointer

int get_env_var_as_int(const char *varname) {
    const char *value = getenv(varname);
    return value ? atoi(value) : 0;
}

void setEnvars() {
	setenv("ENV_SIZE", "2", 1);
	setenv("ENV_C1_BUF_CNT", "10", 1);
	setenv("ENV_C2_BUF_CNT", "5", 1);
	setenv("ENV_C3_BUF_CNT", "5", 1);
	setenv("ENV_C3_RX_PER_CHAN", "4", 1);
	setenv("ENV_C3_CHAN_SIZE", "50", 1);
	setenv("ENV_C4_CHAN_CNT", "1", 1);
	setenv("ENV_C4_BUF_CNT", "10", 1);	
}

void getEnvars() {
	ENV_SIZE = get_env_var_as_int("ENV_SIZE");
	ENV_C1_BUF_CNT = get_env_var_as_int("ENV_C1_BUF_CNT");
	ENV_C2_BUF_CNT = get_env_var_as_int("ENV_C2_BUF_CNT");
	ENV_C3_BUF_CNT  = get_env_var_as_int("ENV_C3_BUF_CNT");
	ENV_C3_RX_PER_CHAN = get_env_var_as_int("ENV_C3_RX_PER_CHAN");
	ENV_C3_CHAN_SIZE = get_env_var_as_int("ENV_C3_CHAN_SIZE");
	ENV_C4_CHAN_CNT = get_env_var_as_int("ENV_C4_CHAN_CNT");
	ENV_C4_BUF_CNT = get_env_var_as_int("ENV_C4_BUF_CNT");
}
