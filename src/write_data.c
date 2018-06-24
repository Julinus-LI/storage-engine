/*************************************************************************
	> File Name: write_data.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 10时15分40秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<aio.h>
#include"sl_log.h"
#include"sl_que.h"
#include"def_info.h"
#include"sl_com.h"
#include"disk_info.h"
#include"sig_deal.h"
#include"write_data.h"

wthr_info_t * wthr_info = NULL;

// 初始化写线程信息
int init_write_data_info()
{
    int i = 0, j = 0;
    int disk_id = 0;

    //开辟写线程空间

    wthr_info = (wthr_info_t*)malloc(sizeof(wthr_info_t) * def_info->wthr_num);
    if (wthr_info == NULL) 
    {   
        return -1; 
    }   

    int area = def_info->disk_num / def_info->wthr_num;
    int left = def_info->disk_num % def_info->wthr_num;
    //初始化每个写线程的信息

    for (i = 0; i < def_info->wthr_num; i++) 
    {   
        wthr_info[i].thr_id = i;
        wthr_info[i].cpu_id = def_info->wcpu_id;
        wthr_info[i].disk_num = area;
        if (left > 0) 
        {
            left -= 1;
            wthr_info[i].disk_num = area + 1;

        }
        wthr_info[i].disk_id = (int *)malloc(sizeof(int) * wthr_info[i].disk_num);
        for (j = 0; j < wthr_info[i].disk_num; j++)
        {
            wthr_info[i].disk_id[j] = disk_id;
            disk_id++;

        }
    }
    return 0;
}

// 打印写信息
void print_write_data()
{
    int i = 0;
    for(;i<def_info->wthr_num;i++)
    {
        printf("thr_id:[%d]\n", wthr_info[i].thr_id);
        printf("cpu_id:[%d]\n", wthr_info[i].cpu_id);
        printf("disk_num:[%d]\n", wthr_info[i].disk_num);
        int j;
        for(j=0; j<wthr_info[i].disk_num; j++)
        {
            printf("\tdisk_id:[%d]\n", wthr_info[i].disk_id[j]);

        }

    }
}

void *thw_run(void *args)
{
    wthr_info_t *w_info = NULL;
    int i = 0, disk_id;
    //time_t t;
    
    w_info = (wthr_info_t *)args;
    
    //安装信号，如果异步io写完数据后会触发该回调函数
    make_sig_action();

    while(1) 
    {
        //t = time(NULL);
        //循环查看每一个磁盘中是否有数据可以读
        for (i = 0; i < w_info->disk_num; i++) 
        {
            disk_id = w_info->disk_id[i];
            //当前磁盘正在有数据写入，当前不能继续写入，应该等待正在写入的数据写入完成
            if (disk_info[disk_id].w_flag == 0) 
            {
                DBG("disk:[%d] is writing data,please wait.........");
                sleep(1);
                continue;
            }
            //循环获取每个磁盘数据队列中的数据
            //其实可以使用条件变量使得当没有数据时候可以阻塞此处，当有数据时发出通知
            if (!read_outval(disk_info[disk_id].bbuff,node_info_t, disk_info[disk_id].node_info)) 
            {
                sleep(1);
                continue;
            }
            //如果获取的地址为空，则表示队列出问题了，这时候起始应该程序退出
            if (disk_info[disk_id].node_info == NULL) 
            {
                ERR("get node is null, disk_id is [%d]", disk_id);
                exit(0);
            }

            //获得数据从繁忙队列中,并写入磁盘
            /*char buff[32] = {0};
            strncpy(buff, disk_info[disk_id].node_info->buff, 31);
            DBG("get data:[%s]", buff);
            */

            /*//对文件大小或者文件间隔时间进行判断，若超标则创建新文件
            if (disk_info[disk_id].seg_type == 0) 
            {
                //time
                if ((t - disk_info[disk_id].cur_ftime) > disk_info[disk_id].time_temp) 
                {
                    //change_file(&disk_info[disk_id]);
                }
            }
            else 
            {
                if (disk_info[disk_id].cur_fsize > disk_info[disk_id].file_size) 
                {
                    //change_file(&disk_info[disk_id]);
                }
            }
            */

            //在开辟my_aiocb时候，要初始化该空间，否则会出现写入出错的问题
            disk_info[disk_id].w_flag = 0;
            //指定aio要往哪个文件描述符里边写入数据
            disk_info[disk_id].my_aiocb->aio_fildes = disk_info[disk_id].file_fd;
            //对要写入的数据地址进行赋值
            disk_info[disk_id].my_aiocb->aio_buf = disk_info[disk_id].node_info->buff;
            //要写入的数据长度
            disk_info[disk_id].my_aiocb->aio_nbytes = disk_info[disk_id].node_info->len;
            //从文件哪个位置开始写入数据
            disk_info[disk_id].my_aiocb->aio_offset = 0;// disk_info[disk_id].cur_fsize;
            //使用信号通知进程，系统已经完成数据的写入过程
            disk_info[disk_id].my_aiocb->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
            //使用SIG_RETURN这个自定义信号来通知
            disk_info[disk_id].my_aiocb->aio_sigevent.sigev_signo = SIG_RETURN;
            //使用信号通知进程事件触发的时候，顺便携带一个参数过去
            disk_info[disk_id].my_aiocb->aio_sigevent.sigev_value.sival_ptr = &disk_info[disk_id];

             //printf("disk_info[disk_id].my_aiocb = [%s]\n",(char*)disk_info[disk_id].my_aiocb->aio_buf);
            //开始写入
            while(aio_write(disk_info[disk_id].my_aiocb) < 0) 
            {
                DBG("\t\t\t\taio写入错误");
                sleep(1);
                continue;
            }
            printf("============一次数据已经写完 =======\n");
        }
    }
    return NULL;
}

// 开始写数据
int start_write_data()
{
    pthread_t tid;
    int i = 0;
    for (i = 0; i < def_info->wthr_num; i++) 
    {
        if (pthread_create(&tid, NULL, thw_run, (void *)&wthr_info[i]) < 0) 
        {
            ERR("start write thread error!");
            return -1;
        }

    }
    //pthread_create(&tid, NULL, thw_run, (void *)&wthr_info[0]);

    return 0;
}

