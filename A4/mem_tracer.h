/**
 * Description: This is a header file of function definitions required for memory tracing. Include this header file in desired file and link object file of mem_tracer.c to trace memory usage.
 * Author names: Brian Qian
 * Author emails: brian.qian@sjsu.edu
 * Last modified date: 4/8/2023
 * Creation date: 3/31/2023
 **/

// Required define directives to convert memory functions to the corresponding mem_tracer functions
#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

void* REALLOC(void* p,int t,char* file,int line);
void* MALLOC(int t,char* file,int line);
void FREE(void* p,char* file,int line);


void PUSH_TRACE(char* p); // put at beginning of each function
void POP_TRACE(); // put at end of each function
void terminationCleanup(); // put after POP_TRACE() in main function
