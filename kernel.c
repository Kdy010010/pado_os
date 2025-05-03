#include "pstring.h"
#include "pit.h"
#include "task.h"
#include "romfs.h"
#include "ipc.h"
#include "idt.h"

extern void p_gdt_flush(void);

void p_kstart(void)
{
    p_gdt_flush();
    p_idt_init();
    p_task_init();
    p_romfs_init();
    p_ipc_init();
    p_pit_init(100);

    p_task_create("/init/hello2.elf");
    p_task_create("/init/hello.elf");
    p_task_switch();
    for(;;);
}
