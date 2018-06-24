/*************************************************************************
	> File Name: main.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月20日 星期三 18时39分25秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include"sl_que.h"
#include"sl_log.h"
#include"def_info.h"
#include"disk_info.h"
#include"sig_deal.h"
#include"write_data.h"
#include"data_deal.h"
#include"capture_package.h"

char log_type[8] = {0};

// daemon_flags 为 0 表示进入前台模式运行
// daemon_flags 为 1 表示进入后台运行模式（守护进程模式)

int daemon_flags = 0;

//获取运行程序的参数

static int get_opt(int argc, char *argv[])
{            
    int opt; 

    while((opt = getopt(argc, argv, "di:")) != -1) {
        switch(opt) {
            case 'd':
            daemon_flags = 1;
            break;
            case 'i':
            printf("-----param is i,[%s]\n", optarg);
            break;
            default:
            return -1;

        }    

    }        
    return 0;
}            

//守护进程的写法

static int daemon_instance()
{
    pid_t  pid;
    int i = 3;

    if ((pid = fork()) < 0) {
        exit(0);
    }else if (pid > 0) {
        exit(0);
    } 
    if (setsid() < 0) {
        exit(-1);
    } 
    if ((pid = fork())) {
        exit(0);
    }
    if (chdir("/") < 0) {
        exit(0);
    }
    umask(0);
    for(i = 0; i < getdtablesize(); i++) 
    {
        close(i);

    }

    return 0;
}

// 初始化系统
void init_system()
{
    // 读取配置文件,获取程序参数的默认值
   if(read_conf() < 0)
    {
        printf("read_conf error\n");
        exit(-1);
    }

    // 打印配置文件信息
    if(print_data()<0)
    {
        printf("print_data error\n");
        exit(-1);
    }
    
    // 初始化日志系统 
    if(open_log(def_info->log_file,log_type))
    {
       perror("open_log error"); 
        exit(-1);
    }
    
    // 初始化磁盘信息
    if(init_disk_info() < 0)
    {
       perror("init_disk_info error"); 
        exit(-1);
    }
    // 打印磁盘信息
    if(print_disk_info() < 0)
    {
       perror("print_disk_info error"); 
        exit(-1);
    }
    
    printf("\n\n");
    // 初始化写盘线程信息
    if(init_write_data_info() < 0)
    {
       perror("init_write_data_info error"); 
        exit(-1);
    }
    
    // 打印写盘信息
    print_write_data();
    
    // 初始化数据处理线程信息
    
    printf("\n\n");
    if(init_deal_pth_info() < 0)
    {
       perror("init_deal_pth_info error"); 
        exit(-1);
    }
    
    print_deal_pth_info();
}


int main(int argc,char* argv[])
{
    // 解析命令行参数 
    if (get_opt(argc, argv) < 0) {
        printf("parse option error\n");
        return -1; 
    }             
    // 判断该系统是否进入后台运行模式（守护进程模式）
    if(daemon_flags)
    {
       daemon_instance();         
         strcpy(log_type, "f_cat");
    }
    else
    {
       strcpy(log_type, "o_cat");
    }
    
    // 初始化系统
    init_system();

    // 启动写数据模块
    start_write_data();
    
    // 启动数据处理模块
    start_deal_data();

    // 开始从网卡上抓包
    start_capture_package();
     
    while(1)
    {
        sleep(10);
    }
    return 0;
}
