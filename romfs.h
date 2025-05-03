#ifndef P_ROMFS_H
#define P_ROMFS_H
#include <stdint.h>
int p_romfs_read(const char *name, uint8_t **data, uint32_t *size);
#endif
