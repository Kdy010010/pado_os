#include "ipc.h"
#include "pstring.h"
#include "task.h"
#include "idt.h"

static p_ipc_msg_t q[P_IPC_MAX];

static int q_send(const char *f,const char *t,const char *m){
    for(int i=0;i<P_IPC_MAX;i++){
        if(!q[i].p_used){
            p_strncpy(q[i].p_from,f,P_IPC_NAME_LEN-1);
            p_strncpy(q[i].p_to,t,P_IPC_NAME_LEN-1);
            p_strncpy(q[i].p_msg,m,P_IPC_MSG_SIZE-1);
            q[i].p_used=1;
            return 0;
        }
    }
    return -1;
}

static int q_recv(const char *me,char *of,char *om){
    for(int i=0;i<P_IPC_MAX;i++){
        if(q[i].p_used && !p_strcmp(q[i].p_to,me)){
            p_strncpy(of,q[i].p_from,P_IPC_NAME_LEN);
            p_strncpy(om,q[i].p_msg,P_IPC_MSG_SIZE);
            q[i].p_used=0;
            return 0;
        }
    }
    return -1;
}

static void p_ipc_isr(registers_t *r){
    uint32_t fn=r->eax;
    const char *a=(const char*)r->ebx;
    const char *b=(const char*)r->ecx;
    const char *c=(const char*)r->edx;
    int ret=-1;
    if(fn==P_IPC_SEND) ret=q_send(a,b,c);
    else if(fn==P_IPC_RECV) ret=q_recv(a,(char*)b,(char*)c);
    r->eax=ret;
}

void p_ipc_init(void){
    p_memset(q,0,sizeof(q));
    p_register_interrupt_handler(90,p_ipc_isr);
}

int p_ipc_send(const char *f,const char *t,const char *m){
    int r; __asm__ volatile("int $0x90":"=a"(r):"a"(P_IPC_SEND),"b"(f),"c"(t),"d"(m));
    return r;
}
int p_ipc_recv(const char *me,char *of,char *om){
    int r; __asm__ volatile("int $0x90":"=a"(r):"a"(P_IPC_RECV),"b"(me),"c"(of),"d"(om));
    return r;
}
