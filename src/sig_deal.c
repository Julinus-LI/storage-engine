/*************************************************************************
	> File Name: sig_deal.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 10时32分31秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <aio.h>
#include "sl_log.h"
#include "def_info.h"
#include "disk_info.h"
#include"sig_deal.h"
 
//flag: 0-屏蔽信号 1-解除屏蔽

int block_allsig(int flag)
{
    sigset_t  signal_set;

    if (sigemptyset(&signal_set) < 0) 
    {
        return -1;

    }
    if (sigfillset(&signal_set) < 0) 
    {
        return -1;

    }
    if (pthread_sigmask(flag, &signal_set, NULL) != 0) 
    {
        return -1;

    }
    return 0;
}

/*
 * *     信号处理:
 * *             1. 判断数据是否写入成功
 * *             2. 成功则将node_info写入到空闲队列中
 * *             3. 失败则要重新写入数据
 * *             
 * */
void sig_write(int signo, siginfo_t *info, void *ctext)
{
    disk_info_t *d_info = NULL;
    
    if (signo == SIG_RETURN)
    {
        d_info = (disk_info_t*)info->si_ptr;
         //如果错误为0说明已经存入成功
         if (aio_error(d_info->my_aiocb) == 0) 
         {
             //写入成功,但是需要判断是否完全写入
             while(aio_write(d_info->my_aiocb) < 0) 
             {
                 usleep(10);
                 continue;

             }

         }
         else 
         {
             //该磁盘中繁忙节点已经写入数据了，把该节点刚回空闲节点中
             while(!write_inval(d_info->fbuff, node_info_t, d_info->node_info)) 
             {
                 continue;

             }
             //设置磁盘没有正在写的标志
             d_info->w_flag = 1;

         }

     }
     else 
     {
         while(aio_write(d_info->my_aiocb) < 0) 
         {

             usleep(10);
             continue;

         }

     }
    
     return ;
 }

int make_sig_action()
{
    struct sigaction sig_act;
    //清空信号集和
    if (sigemptyset(&sig_act.sa_mask) < 0) 
    {
        return -1;

    }
    sig_act.sa_flags = SA_SIGINFO;
    sig_act.sa_sigaction = sig_write;

    if (sigaction(SIG_RETURN, &sig_act, NULL)) 
    {
        return -1;

    }
    block_allsig(UNMASK_SIG);

    return 0;
}

