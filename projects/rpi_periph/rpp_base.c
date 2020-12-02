#define _POSIX_C_SOURCE (200809L)

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <liber_tools.h>
#include "../rpi_periph/rpp_base.h"

#define RPP_MEMDEV  "/dev/mem"
#define RPP_MEMPROT (PROT_READ | PROT_WRITE)

#define BCM2835_PERI_BASE (0x20000000)
#define BCM2835_GPIO_BASE (0x00200000)

#define GPIO_BASE (BCM2835_PERI_BASE + BCM2835_GPIO_BASE)

#define BLOCK_SIZE BXI_KB(4)

static i32 mem_fd = 0;
volatile u32 * gpio = NULL;

bool rpp_init(void) {
    void * gpio_ptr;

    mem_fd = open(RPP_MEMDEV, O_RDWR | O_SYNC | O_CLOEXEC);
    if (mem_fd < 0) {
        ert_err(RPP_MEMDEV" open failed");
        return false;
    }

    gpio_ptr = mmap(0, BLOCK_SIZE, RPP_MEMPROT, MAP_SHARED, mem_fd, GPIO_BASE);
    if (gpio_ptr == MAP_FAILED) {
        ert_err(RPP_MEMDEV" memory mapped failed");
        return false;
    }

    gpio = (u32*)gpio_ptr;
    return true;
}

void rpp_free(void) {
    if (close(mem_fd) != 0)
        ert_err(RPP_MEMDEV" close failed");
}
