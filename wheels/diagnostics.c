
#include <stdio.h>
//#include <time.h>
#include <sys/time.h>
#include "diagnostics.h"

// #ifndef __arm__
// #undef CLOCKS_PER_SEC
// #define CLOCKS_PER_SEC	1000000
// #endif

static time_t start_sec;
static long int start_usec;
struct timeval end;


void __init_diagnostics() {
    struct timeval start;
    gettimeofday(&start, NULL);
    
    printf("[Start diagnostics]\n");
    
    start_sec = start.tv_sec;
    start_usec = start.tv_usec;
}

void __system_time_print() {
    gettimeofday(&end, NULL);

    printf("[%lld:%ld] - [%ld:%ld] ", start_sec,
				      start_usec,
				      end.tv_sec,
				      end.tv_usec
 	  );
}

