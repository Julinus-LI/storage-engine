/*************************************************************************
	> File Name: disk_info.h
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 09时42分35秒
 ************************************************************************/

#ifndef _DISK_INFO_H
#define _DISK_INFO_H

/* 磁盘信息结构体 */
#include <stdint.h>
#include "sl_com.h"
#include "sl_que.h"
 
typedef struct __node_info_t
{
    int len;
    char buff[SIZE_M];

}node_info_t;
  
typedef struct __disk_info_t
{
    volatile int    w_flag;
    int             file_fd;
    int             disk_id;
    int             seg_type;//0-time 1-size
    int             is_full;//判断该磁盘是否有空闲节点
    int64_t         time_temp;//指定间隔时间
    int64_t         file_size;//文件指定大小
    int64_t         cur_fsize;//当前文件大小
    int64_t         cur_ftime;//写入起始时间
    que_t           *fbuff;
    que_t           *bbuff;
    node_info_t     *node_info;
    struct aiocb    *my_aiocb;
    char            path[PATH_MAX];

}disk_info_t;
   
extern disk_info_t *disk_info;

// 初始化磁盘信息
int init_disk_info();

// 打印磁盘信息
int print_disk_info();

#endif
