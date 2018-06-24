/*************************************************************************
	> File Name: def_info.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 09时34分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"sl_conf.h"
#include"def_info.h"

def_info_t  *def_info = NULL;
char * conf = "../etc/store.cfg";

int read_conf()
{
    if (open_conf(conf) < 0) 
    {
        return -1; 

    }
    def_info = (def_info_t*)malloc(sizeof(def_info_t));
    if (def_info == NULL) 
    {
        return -1;

    }
    if (get_val_single("base.disk_num", &def_info->disk_num, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.rthr_num", &def_info->rthr_num, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.wthr_num", &def_info->wthr_num, TYPE_INT) < 0) {
        return -1;

    }                                                        

    if (get_val_single("base.sthr_num", &def_info->sthr_num, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.rcpu_id", &def_info->rcpu_id, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.wcpu_id", &def_info->wcpu_id, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.scpu_id", &def_info->scpu_id, TYPE_INT) < 0) {
        return -1;

    }

    def_info->path = (char **)malloc(sizeof(char*) * def_info->disk_num);
    if (get_val_arry("base.disk_path", (void**)def_info->path, def_info->disk_num, TYPE_STRING) < 0) {
        return -1;

    }
    if (get_val_single("base.seg_type", &def_info->seg_type, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.stime", &def_info->stime, TYPE_LONG) < 0) {
         return -1;
    }

    if (get_val_single("base.ssize", &def_info->ssize, TYPE_LONG) < 0) {
        return -1;

    }

    def_info->ssize *= SIZE_M;
    if (get_val_single("base.node_num", &def_info->node_num, TYPE_INT) < 0) {
        return -1;

    }
    if (get_val_single("base.node_size", &def_info->node_size, TYPE_LONG) < 0) {
        return -1;

    }

    def_info->node_size *= SIZE_M;
    if (get_val_single("base.conf_path.ctrl_file", def_info->ctrl_file, TYPE_STRING) < 0) {
        return -1;

    }
    if (get_val_single("base.conf_path.log_file", def_info->log_file, TYPE_STRING) < 0) {
        return -1;

    }
    return 0;

}

int print_data()
{
    int i = 0;
    printf("disk_num:[%d]\n", def_info->disk_num);
    for (i = 0; i < def_info->disk_num; i++) {
        printf("\tdisk_path:[%s]\n", def_info->path[i]);

    }
    printf("rthr_num:[%d]\n", def_info->rthr_num);
    for (i = 0; i < def_info->rthr_num; i++) {
        printf("\trcpu_id:[%d]\n", def_info->rcpu_id);

    }
    printf("wthr_num:[%d]\n", def_info->wthr_num);
    for (i = 0; i < def_info->wthr_num; i++) {
        printf("\twcpu_id:[%d]\n", def_info->wcpu_id);

    }
    printf("sthr_num:[%d]\n", def_info->sthr_num);
    for (i = 0; i < def_info->sthr_num; i++) {
        printf("\tscpu_id:[%d]\n", def_info->scpu_id);

    }
    printf("seg_type:[%d]\n", def_info->seg_type);
    printf("node_num:[%d]\n", def_info->node_num);
    printf("node_size:[%ld]\n", def_info->node_size);
    printf("stime:[%ld]\n", def_info->stime);
    printf("ssize:[%ld]\n", def_info->ssize);
    printf("ctrl_file:[%s]\n", def_info->ctrl_file);  
    printf("log_file:[%s]\n", def_info->log_file);

    return 0;
}


