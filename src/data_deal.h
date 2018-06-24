/*************************************************************************
	> File Name: data_deal.h
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 10时41分59秒
 ************************************************************************/

#ifndef _DATA_DEAL_H
#define _DATA_DEAL_H

/* 数据处理模块 */

#include "sl_com.h"
#include "disk_info.h"
#define BUFF 250
typedef struct node
{        
    char buff[4096];
}data_node_t;

typedef struct __rthr_info_t
{        
    int thr_id;
    int cpu_id;
    int min_disk_id;
    int disk_num;
    node_info_t **buffer;
    que_t * data_que;
}rthr_info_t;
    
extern rthr_info_t *rthr_info;
    
int start_deal_data();
int init_deal_pth_info();
void print_deal_pth_info();
# endif
