#include "romfs.h"
#include "pstring.h"

extern uint8_t _romfs_start;

int p_romfs_read(const char *name, uint8_t **data, uint32_t *size)
{
    uint8_t *p=&_romfs_start;
    while(1){
        char fname[32];
        p_memcpy(fname,p,32); p+=32;
        uint32_t sz=*(uint32_t*)p; p+=4;
        if(!fname[0]) break;
        if(!p_strcmp(fname,name)){
            *data=p; *size=sz;
            return 0;
        }
        p+=sz;
    }
    return -1;
}
