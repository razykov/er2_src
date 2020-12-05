#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libbixi.h>
#include <liber_tools.h>
#include <libmachine.h>
#include <librpi_peri.h>

static void _my_nsleep(u32 sec, u32 nsec) {
    struct timespec ts;
    ts.tv_sec  = sec;
    ts.tv_nsec = nsec;
    nanosleep(&ts, NULL);
}

static void _my_memerr(u32 size, const char * file, u32 line) {
    fprintf(stderr, "Can't alloc %u at %s:%u\n", size, file, line);
    exit(EXIT_FAILURE);
}

static void * _my_malloc(u32 size, const char * file, u32 line) {
    void * result = calloc(size, 1);
    UNUSED(file);
    UNUSED(line);
    return result;
}

static void _my_free(void * ptr, const char * file, u32 line) {
    UNUSED(file);
    UNUSED(line);
    free(ptr);
}

static void * _my_realloc(void * ptr, u32 size, const char * file, u32 line) {
    void * result = realloc(ptr, size);
    UNUSED(file);
    UNUSED(line);
    return result;
}

static void _bxi_init_memory(void) {
    bxi_malloc_set (_my_malloc );
    bxi_realloc_set(_my_realloc);
    bxi_free_set   (_my_free   );
    bxi_memerr_set (_my_memerr );
}


i32 main(void) {
    er2_machine_t mch;

    _bxi_init_memory();
    bxi_nsleep_set(_my_nsleep);

    ert_inf("machine test started");

    mch = er2_machine_create();

    bxi_msleep(2000);

    er2_mch_set_speed(mch, 100);


    er2_mch_wheels_state(mch, &whls_forth);
    bxi_msleep(1600);
    er2_mch_wheels_state(mch, &whls_stop);
    bxi_msleep(500);

    er2_mch_wheels_state(mch, &whls_lrot);
    bxi_msleep(850);
    er2_mch_wheels_state(mch, &whls_stop);
    bxi_msleep(500);

    er2_mch_wheels_state(mch, &whls_forth);
    bxi_msleep(1600);
    er2_mch_wheels_state(mch, &whls_stop);
    bxi_msleep(500);

    er2_mch_wheels_state(mch, &whls_rrot);
    bxi_msleep(750);
    er2_mch_wheels_state(mch, &whls_stop);
    bxi_msleep(500);

    er2_machine_destroy(mch);
    
    return 0;
}
 
