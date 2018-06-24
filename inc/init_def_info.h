
#ifndef _SL_INIT_DEF_INFO_H_
#define _SL_INIT_DEF_INFO_H_

#include "sl_com.h"

typedef struct __def_info_t
{
    int disk_num;   //磁盘个数
    int rthr_num;   //处理线程个数
    int wthr_num;   //写盘线程个数
    int sthr_num;   //信号处理线程个数
    int rcpu_id;   //处理线程的cpuid
    int wcpu_id;   //写盘线程的cpuid
    int scpu_id;   //信号处理线程的cpuid
    int seg_type;   //文件分割方式0-时间,1-大小
    int node_num;   //队列节点个数
    int64_t node_size;  //队列节点大小
    int64_t stime;  //文件分割的间隔时间
    int64_t ssize;  //文件分割大小
    char ctrl_file[PATH_MAX];   //管道文件路径
    char log_file[PATH_MAX];    //日志文件路径
    char **path;    //磁盘挂载路径
}def_info_t;

extern def_info_t  *def_info;

int init_def_info();
int print_data();
#endif


