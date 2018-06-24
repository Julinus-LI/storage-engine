/*************************************************************************
	> File Name: write_data.h
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 10时13分11秒
 ************************************************************************/

#ifndef _WRITE_DATA_H
#define _WRITE_DATA_H

/* 写线程信息，数据结构 */

#include <stdint.h>
#include "sl_com.h"
#include "sl_que.h"
 
typedef struct __wthr_info_t
{
    int thr_id;
    int cpu_id;
    int disk_num;
    int *disk_id;

}wthr_info_t;
  
extern wthr_info_t *wthr_info;
  
// 初始化写线程信息
int init_write_data_info();
// 打印写信息
void print_write_data();
// 开始写数据
int start_write_data();

#endif
