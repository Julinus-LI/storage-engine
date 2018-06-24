/*************************************************************************
	> File Name: disk_info.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 09时45分59秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<aio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"sl_log.h"
#include"def_info.h"
#include"disk_info.h"

disk_info_t *disk_info = NULL;
// 初始化磁盘信息
int init_disk_info()
{
    int i = 0, j = 0;
    node_info_t *ptr;

    disk_info = (disk_info_t*)malloc(sizeof(disk_info_t) * def_info->disk_num);
    if (disk_info == NULL) 
    {   
        return -1; 
    }   
    //为每一个磁盘进行初始化

    for (i = 0; i < def_info->disk_num; i++) 
    {   
        disk_info[i].disk_id = i;
        disk_info[i].is_full = 1;
        disk_info[i].seg_type = def_info->seg_type;
        disk_info[i].time_temp = def_info->stime;
        disk_info[i].file_size = def_info->ssize;
        disk_info[i].w_flag = 1; //当前是否可以从队列获取数据
        disk_info[i].my_aiocb = (struct aiocb*)malloc(sizeof(struct aiocb));
        //在开辟my_aiocb时候，要初始化该空间，否则会出现写入出错的问题
        memset(disk_info[i].my_aiocb,'\0',sizeof(struct aiocb));
        //打开磁盘路径上的描述符
        strcpy(disk_info[i].path, def_info->path[i]);
        char path[PATH_MAX];
        sprintf(path,"%s/disk%d.dat",disk_info[i].path,i+1);
        //disk_info[i].file_fd = open(path,O_WRONLY|O_APPEND|O_CREAT,0777);
        disk_info[i].file_fd = open(path,O_WRONLY|O_APPEND|O_CREAT,0644);
        if( disk_info[i].file_fd == -1 ) 
        {
            DBG("打开磁盘文件失败！\n");

        }

        //初始化磁盘的缓冲队列
        ring_init(disk_info[i].fbuff, node_info_t, def_info->node_num);
        ring_init(disk_info[i].bbuff, node_info_t, def_info->node_num);
        if ((ptr = (node_info_t*)malloc(sizeof(node_info_t) * def_info->node_num)) == NULL) 
        {
            return -1;

        }
        for (j = 0; j < def_info->node_num; j++) 
        {
            if (!write_inval(disk_info[i].fbuff, node_info_t, &ptr[j])) 
            {
                return -1;

            }

        }

    }
    return 0;
}

// 打印磁盘信息
int print_disk_info()
{
    int i = 0;
    for(;i<def_info->disk_num;i++)
    {   
        printf("w_flag:[%d]\n", disk_info[i].w_flag);
        printf("is_full:[%d]\n", disk_info[i].is_full);
        printf("file_fd:[%d]\n", disk_info[i].file_fd);
        printf("disk_id:[%d]\n", disk_info[i].disk_id);
        printf("seg_type:[%d]\n", disk_info[i].seg_type);
        printf("time_temp:[%ld]\n", disk_info[i].time_temp);
        printf("file_size:[%ld]\n", disk_info[i].file_size);
        printf("path:[%s]\n", disk_info[i].path);
    }   
    return 0;
}
