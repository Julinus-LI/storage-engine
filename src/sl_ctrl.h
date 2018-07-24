#ifndef __SL_CTRL_H__
#define __SL_CTRL_H__

typedef void (*bc_func)(int argc, char *argv[]);

typedef struct __ctrlor_key_t
{
    char    key[32];
    char    bak[128];
    bc_func ctrl_func;
    struct __ctrlor_key_t *next;
}ctrlor_key_t;

void ctrlor_add(char *key, char *bak, bc_func func);
int ctrl_init(char *fifo_file);
#endif
