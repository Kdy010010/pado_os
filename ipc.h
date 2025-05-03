#ifndef P_IPC_H
#define P_IPC_H
#include <stdint.h>

#define P_IPC_NAME_LEN 32
#define P_IPC_MSG_SIZE 256
#define P_IPC_MAX 32

#define P_IPC_SEND 1
#define P_IPC_RECV 2

typedef struct{
    char p_from[P_IPC_NAME_LEN];
    char p_to[P_IPC_NAME_LEN];
    char p_msg[P_IPC_MSG_SIZE];
    uint8_t p_used;
} p_ipc_msg_t;

int p_ipc_send(const char *from, const char *to, const char *msg);
int p_ipc_recv(const char *me, char *out_from, char *out_msg);

#endif
