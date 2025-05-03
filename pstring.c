#include "pstring.h"
#include <stdarg.h>

static volatile char *const vga=(char*)0xB8000;
static uint16_t pos;

void p_memcpy(void *d,const void *s,uint32_t n){ while(n--) ((char*)d)[n]=((char*)s)[n]; }
void p_memset(void *d,int v,uint32_t n){ while(n--) ((char*)d)[n]=v; }
int  p_strcmp(const char *a,const char *b){ while(*a&&(*a==*b)){a++;b++;} return (unsigned char)*a-(unsigned char)*b; }
void p_strncpy(char *d,const char *s,uint32_t n){ while(n-- && (*d++=*s++)); }

static void putc(char c){
    vga[pos++]=c;
    vga[pos++]=0x07;
    if(pos>=80*25*2) pos=0;
}
void p_printf(const char *fmt,...){
    va_list ap; va_start(ap,fmt);
    for(const char *p=fmt;*p;++p){
        if(*p!='%'){ putc(*p); continue; }
        switch(*++p){
            case 's': { char *s=va_arg(ap,char*); while(*s) putc(*s++);} break;
            default: putc('%'); putc(*p); break;
        }
    }
    va_end(ap);
}
