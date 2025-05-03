#include "elf.h"
#include "pstring.h"

typedef struct{
    uint8_t  e_ident[16];
    uint16_t e_type,e_machine;
    uint32_t e_version;
    uint32_t e_entry,e_phoff,e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize,e_phentsize,e_phnum;
} __attribute__((packed)) Elf32_Ehdr;

typedef struct{
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} __attribute__((packed)) Elf32_Phdr;

#define PT_LOAD 1

int p_elf_load(uint8_t *img, uint32_t *entry_out)
{
    Elf32_Ehdr *eh=(Elf32_Ehdr*)img;
    if(eh->e_ident[0]!=0x7F || eh->e_ident[1]!='E') return -1;
    Elf32_Phdr *ph=(Elf32_Phdr*)(img+eh->e_phoff);
    for(int i=0;i<eh->e_phnum;i++,ph++){
        if(ph->p_type!=PT_LOAD) continue;
        p_memcpy((void*)ph->p_vaddr,img+ph->p_offset,ph->p_filesz);
        p_memset((void*)(ph->p_vaddr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
    }
    *entry_out=eh->e_entry;
    return 0;
}
