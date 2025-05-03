#ifndef PSTRING_H
#define PSTRING_H
#include <stdint.h>
void p_memcpy(void *d,const void *s,uint32_t n);
void p_memset(void *d,int v,uint32_t n);
int  p_strcmp(const char *a,const char *b);
void p_strncpy(char *d,const char *s,uint32_t n);
void p_printf(const char *fmt,...);
#endif
