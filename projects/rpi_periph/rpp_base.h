#ifndef RPI_PRPH_BASE_H
#define RPI_PRPH_BASE_H

#include <libbixi.h>

extern volatile u32 * gpio;

EXPORT_FROM

bool rpp_init(void);

void rpp_free(void);

EXPORT_TO

#endif /* RPI_PRPH_BASE_H */
