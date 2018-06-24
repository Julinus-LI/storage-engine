/*************************************************************************
	> File Name: sig_deal.h
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 10时31分30秒
 ************************************************************************/

#ifndef _SIG_DEAL_H
#define _SIG_DEAL_H

/* 信号处理模块 */
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
