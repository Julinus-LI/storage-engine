#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "sl_log.h"
#include "disk_info.h"
#include "def_info.h"
#include "data_deal.h"


rthr_info_t *rthr_info = NULL; 

int init_deal_pth_info()
{
    int i = 0;
    int disk_id = 0;

    rthr_info = (rthr_info_t*)malloc(sizeof(rthr_info_t) * def_info->rthr_num);
    if (rthr_info == NULL) 
    {
        return -1;
    }

    int area = def_info->disk_num / def_info->rthr_num;
    int left = def_info->disk_num % def_info->rthr_num;

    for (i = 0; i < def_info->rthr_num; i++) 
    {
        rthr_info[i].thr_id = i;
        rthr_info[i].cpu_id = def_info->rcpu_id;
        rthr_info[i].min_disk_id = disk_id;
        rthr_info[i].disk_num = area;
        if (left > 0) 
        {
            disk_id += 1;
            left -= 1;
            rthr_info[i].disk_num += 1;
        }
        disk_id += area;
        rthr_info[i].buffer = (node_info_t**)malloc(sizeof(node_info_t*) * rthr_info[i].disk_num);
        
        //初始线程的获取数据队列
        //data_node_t * ptr = NULL;
        ring_init(rthr_info[i].data_que, data_node_t, 15);
    }
    return 0;
}

void print_deal_pth_info()
{
    int i = 0;
    for(;i<def_info->rthr_num;i++)
    {
        printf("thr_id:[%d]\n", rthr_info[i].thr_id);
        printf("cpu_id:[%d]\n", rthr_info[i].cpu_id);
        printf("disk_num:[%d]\n", rthr_info[i].disk_num);
        int j;
        for(j=0; j<rthr_info[i].disk_num; j++)
        {
            printf("\tdisk_id:[%d]\n", rthr_info[i].min_disk_id+j);
        }
    }
}


/*
    获取数据
    对数据进行处理
    选择一个归自己管理的空闲磁盘队列
    获取磁盘队列一个空闲节点
    向节点中写数据
    将队列节点，放入数据队列
*/
char *get_pdata(rthr_info_t *args)
{
    data_node_t * p = NULL;
    if( can_read(args->data_que) )
    {
        read_outval(args->data_que,data_node_t,p);
        //printf("thread get  data:%s\n",p->buff);
        return p->buff;
    }
    else
    {
        return NULL;
    }
}

int get_wdisk(rthr_info_t *info)
{
    static uint64_t i = 0;
    i++;
    return (info->min_disk_id + (i % info->disk_num));
}

void *thr_run(void *args)
{
    int i = 0;
    int disk_id = 0;
    int disk_rid = 0;
    char *ptr = NULL;
    rthr_info_t *r_info = (rthr_info_t*)args;
    
    for (i = 0; i < r_info->disk_num; i++) 
    {
        //从每一个磁盘空闲队列中获取一个节点，将地址赋值给处理线程的bufffer，通过buffer来操作空间
        //由于该线程总共有disk_num个buffer,所以需要每一个
        while(!read_outval(disk_info[i + r_info->min_disk_id].fbuff, node_info_t, r_info->buffer[i])) 
        {
            ERR("get %d ", disk_id);
            sleep(1);
            continue;
        }
        r_info->buffer[i]->len = 0;
    }

    while(1) 
    {
        //获取一个即将写入数据的磁盘的id
        disk_id = get_wdisk(r_info);
        if( !disk_info[disk_id].is_full )     
        {
            DBG("磁盘没有可用的空闲的空间，正在等待.........");
            sleep(1);
            continue;
        }
        //获取已经获取的磁盘队列的节点下标
        disk_rid = disk_id - r_info->min_disk_id;
        
        //获取数据
        ptr = get_pdata(r_info);
        if( ptr == NULL )
        {
            continue;
        }

        if ((int64_t)(r_info->buffer[disk_rid]->len + strlen(ptr)) > (int64_t)SIZE_M) 
        {
            DBG("线程所管理的磁盘:%d的buffer已经满1M,加入到busy队列",disk_id);
            //如果磁盘的节点数据已经即将写满1M，那就把节点放入数据队列bbuff中供写盘线程获取
            while(!write_inval(disk_info[disk_id].bbuff, node_info_t, r_info->buffer[disk_rid])) 
            {
                DBG("磁盘没有可用的空闲的空间，正在等待.........");
                sleep(1);
                continue;
            }
            //将节点放入数据队列中后，重新获取一个空闲节点准备写数据，并且将节点的写入长度情0
            if(!read_outval(disk_info[disk_id].fbuff, node_info_t, r_info->buffer[disk_rid])) 
            {
                disk_info[disk_id].is_full = 0;
                continue;
            }
            r_info->buffer[disk_rid]->len = 0;
        }
        //将数据拷贝到节点中
        memcpy(r_info->buffer[disk_rid]->buff + r_info->buffer[disk_rid]->len, ptr, strlen(ptr));
        //printf("disk_id:%d 写入数据：%s完成\n",disk_id,r_info->buffer[disk_rid]->buff + r_info->buffer[disk_rid]->len);
        //对节点中数据的长度加上这次写入的数据长度
        r_info->buffer[disk_rid]->len += strlen(ptr);
        
        sleep(1);
    }
    return NULL;
}

//开始创建数据处理线程
int start_deal_data()
{
    int i = 0;
    pthread_t   tid;

    for (i = 0; i < def_info->rthr_num; i++) 
    {
        if (pthread_create(&tid, NULL, thr_run, (void*)&rthr_info[i]) < 0) 
        {
            ERR("start read pthread error!\n");
            return -1;
        }
    }
    return 0;
}
