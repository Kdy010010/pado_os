#include "task.h"
#include "pstring.h"
#include "romfs.h"
#include "elf.h"
#include <stdlib.h>

static p_task_t p_tasks[P_MAX_TASK];
p_task_t *p_current_task;
static uint32_t next_pid = 1;

static int find_next(int cur)
{
    for(int i=1;i<=P_MAX_TASK;i++){
        int idx=(cur+i)%P_MAX_TASK;
        if(p_tasks[idx].used) return idx;
    }
    return cur;
}

void p_task_init(void)
{
    p_memset(p_tasks,0,sizeof(p_tasks));
    p_tasks[0].used=1;
    p_tasks[0].pid=0;
    p_strncpy(p_tasks[0].name,"idle",31);
    p_current_task=&p_tasks[0];
}

int p_task_create(const char *path)
{
    uint8_t *file;
    uint32_t sz;
    if(p_romfs_read(path,&file,&sz)) return -1;

    int idx=-1;
    for(int i=0;i<P_MAX_TASK;i++) if(!p_tasks[i].used){ idx=i; break; }
    if(idx<0) return -1;

    uint32_t entry;
    if(p_elf_load(file,&entry)) return -1;

    p_task_t *t=&p_tasks[idx];
    t->pid=next_pid++;
    t->eip=entry;
    t->esp=(uint32_t)malloc(P_STACK_SZ)+P_STACK_SZ;
    t->ebp=t->esp;
    t->used=1;
    p_strncpy(t->name,path,31);
    return t->pid;
}

void p_task_switch(void)
{
    int cur=(int)(p_current_task-p_tasks);
    int nxt=find_next(cur);
    if(nxt==cur) return;

    p_task_t *old=p_current_task;
    p_task_t *new=&p_tasks[nxt];
    p_current_task=new;
    __asm__ volatile(
        "cli
	"
        "mov %%esp, %0
	"
        "mov %%ebp, %1
	"
        "mov %2, %%esp
	"
        "mov %3, %%ebp
	"
        "jmp *%4"
        : "=m"(old->esp),"=m"(old->ebp)
        : "m"(new->esp),"m"(new->ebp),"m"(new->eip)
    );
}
