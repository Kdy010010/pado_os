#include "pit.h"
#include "idt.h"
#include "task.h"

static uint32_t p_tick;

static void p_pit_isr(registers_t *r)
{
    (void)r;
    p_tick++;
    if(!(p_tick % 10))
        p_task_switch();
}

void p_pit_init(uint32_t hz)
{
    uint16_t div = 1193180 / hz;
    __asm__ volatile("outb %0, $0x43"::"a"(0x36));
    __asm__ volatile("outb %0, $0x40"::"a"(div & 0xFF));
    __asm__ volatile("outb %0, $0x40"::"a"(div >> 8));
    p_register_interrupt_handler(32, p_pit_isr);
    p_pic_unmask(0);
}
