#include <fcntl.h>
#include <unistd.h>

extern int ENV_SIZE;
extern int ENV_C1_BUF_CNT;
extern int ENV_C2_BUF_CNT;
extern int ENV_C3_BUF_CNT;
extern int ENV_C3_RX_PER_CHAN;
extern int ENV_C3_CHAN_SIZE;
extern int ENV_C4_CHAN_CNT;
extern int ENV_C4_BUF_CNT;

#define ENV_C5_MAX_PER_CHAN 3

typedef struct {
    float re;
    float im;
} IQ_complex;

typedef struct {
    double C1_timestamp;
    short *C1_buffer; // [ENV_SIZE * 2]
} Custom1_Struct;

typedef struct {
    double C2_timestamp;
    IQ_complex *C2_buffer; // [ENV_SIZE]
} Custom2_Struct;

typedef struct {
    double C3_timestamp;
    float **C3_buffer; // [ENV_C3_RX_PER_CHAN][ENV_C3_CHAN_SIZE]
} Custom3_Struct;

typedef struct {
	double C4_timestamp;
	short *C4_buffer; // [ENV_SIZE]
} Custom4_Struct;

typedef struct {
    unsigned int freq;
    int type;
    int flag;
    int filler;
} Custom5_Struct;

typedef struct {
    int int_member;
    double double_member;
    
    // Offsets for pointers
    // These were added to handle pointer data
    size_t short_member_offset;
    size_t custom1_member_offset;
    size_t custom2_member_offset;
    size_t custom3_member_offset;
    size_t custom4_member_row_offset;
    size_t custom4_member_col_offset;
     
    // Actual pointers for the program
    short *short_member; // [ENV_SIZE]
    Custom1_Struct *custom1_member; // [ENV_C1_BUF_CNT]
    Custom2_Struct *custom2_member; // [ENV_C2_BUF_CNT]
    Custom3_Struct *custom3_member; // [ENV_C3_BUF_CNT]
	Custom4_Struct **custom4_member; // [ENV_C4_CHAN_CNT+2][ENV_C4_BUF_CNT]
    Custom5_Struct custom5_member[ENV_C5_MAX_PER_CHAN];
} MasterStruct;

extern volatile MasterStruct *globalStruct;  // Declare global pointer

void getEnvars();
void setEnvars();
volatile MasterStruct *initializeMasterStruct();
void setTestValues(volatile MasterStruct *ptr);  // Declare setTestValues
void printMasterStruct(volatile MasterStruct *ptr); // Declare printMasterStruct
void zeroizeMasterStruct(volatile MasterStruct *ptr);
