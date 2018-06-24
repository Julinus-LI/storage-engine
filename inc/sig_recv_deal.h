#ifndef __SL_SIG_RECV_PTH_H__
#define __SL_SIG_RECV_PTH_H__
#include <signal.h>

#define MASK_SIG    SIG_BLOCK
#define UNMASK_SIG  SIG_UNBLOCK
#define SIG_RETURN  (SIGRTMIN+4)

typedef struct __sthr_info_t
{
    int thr_id;
    int cpu_id;
}sthr_info_t;

int block_allsig(int flag);
int make_sig_action();

#endif
