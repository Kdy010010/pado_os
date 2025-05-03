#ifndef P_TASK_H
#define P_TASK_H
#include <stdint.h>

#define P_MAX_TASK 16
#define P_STACK_SZ 4096

typedef struct {
    uint32_t pid;
    uint32_t eip;
    uint32_t esp, ebp;
    uint8_t  used;
    char     name[32];
} p_task_t;

extern p_task_t *p_current_task;
void  p_task_init(void);
int   p_task_create(const char *path);
void  p_task_switch(void);
#endif
