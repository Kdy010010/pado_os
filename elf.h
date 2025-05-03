#ifndef P_ELF_H
#define P_ELF_H
#include <stdint.h>
int p_elf_load(uint8_t *image, uint32_t *entry_out);
#endif
