#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libconfig.h>
#include <stdint.h>
#include "sl_conf.h"

config_t *p_conf = NULL;

int get_val_single(char *path, void *val, int dtype)
{
    char *ptr;

    if (!p_conf || !path || !val) {
        fprintf(stdout, "input arguments error!\n");
        return -1;
    }
    if (dtype == TYPE_INT) {
        if (config_lookup_int(p_conf, path, (int*)val) == 0) {
            fprintf(stdout, "parse int value error!\n");
            return -1;
        }
    }else if (dtype == TYPE_LONG){
        if (config_lookup_int64(p_conf, path, (long long int*)val) == 0) {
            fprintf(stdout, "parse int64 value error!\n");
            return -1;
        }
    }else if (dtype == TYPE_DOUBLE) {
        if (config_lookup_float(p_conf, path, val) == 0) {
            fprintf(stdout, "parse double value error!\n");
            return -1;
        }
    }else if (dtype == TYPE_STRING) {
        if (config_lookup_string(p_conf, path, (const char**)&ptr) == 0) {
            fprintf(stdout, "parse string value error!\n");
            return -1;
        }
        strcpy(val, ptr);
    }else{
        fprintf(stdout, "arguments value type error!\n");
        return -1;
    }

    return 0;
}

int get_val_arry(char *path, void **val, int count, int dtype)
{
    int num = -1, i;
    config_setting_t *p_set;

    if (!p_conf || !path || !val) {
        fprintf(stdout, "input arguments error!\n");
        return -1;
    }
    p_set = config_lookup(p_conf, path);
    if (p_set == NULL) {
        fprintf(stdout, "parse arguments:[%s] error!\n", path);
        return -1;
    }
    num = config_setting_length(p_set);
    if (count > 0 && num != count) {
        fprintf(stdout, "list or arry elem is not eque!\n");
        return -1;
    }
    if (dtype == TYPE_INT) {
        int **ptr = (int**)val;
        for (i = 0; i < num; i++) {
            (*ptr)[i] = config_setting_get_int_elem(p_set, i);
        }
    }else if (dtype == TYPE_LONG) {
        long long int **ptr = (long long int**)val;
        for (i = 0; i < num; i++) {
            (*ptr)[i] = config_setting_get_int64_elem(p_set, i);
        }
    }else if (dtype == TYPE_DOUBLE) {
        double **ptr = (double**)val;
        for (i = 0; i < num; i++) {
            (*ptr)[i] = config_setting_get_float_elem(p_set, i);
        }
    }else if (dtype == TYPE_STRING) {
        for (i = 0; i < num; i++) {
            ((char**)val)[i] = (char *)malloc(256);
            strcpy(((char**)val)[i], config_setting_get_string_elem(p_set, i));
        }
    }else {
        fprintf(stdout, "list or arry type error!\n");
        return -1;
    }
    return 0;
}

int open_conf(char *name)
{
    if (!name) 
    {
        fprintf(stdout, "config file error!\n");
        return -1;
    }
    p_conf = (config_t *)malloc(sizeof(config_t));
    if (!p_conf) 
    {
        fprintf(stdout, "malloc config error!\n");
        return -1;
    }
    //初始化libconfig
    config_init(p_conf);

	puts(name);
    //打开配置文件，并与libconfig、句柄关联
    if (config_read_file(p_conf,name) == 0) 
    {
        fprintf(stdout, "config read file error:[%s]!\n", name);
        return -1;
    }
    return 0;
}

void close_conf()
{
    config_destroy(p_conf);
}

/*
int test(char *name)
{
    config_t cfg;
    int val;
    char *ptr = NULL;
    config_setting_t *p_set;
    int i = 0;

    //初始化
    config_init(&cfg);
    if (config_read_file(&cfg, name) == 0) {
        return -1;
    }
    
    //获取数组或列表的节点
    p_set = config_lookup(&cfg, "base.disk_path");
    val = config_setting_length(p_set);
    for (i = 0; i < val; i++) {
        ptr = (char*)config_setting_get_string_elem(p_set, i);
        printf("base.disk_path:[%s]\n", ptr);
    }
    //获取单个数据
    if (config_lookup_int(&cfg, "base.disk_num", &val) == 0) {
        printf("config_lookup_int error\n");
        return -1;
    }
    printf("base.disk_num:[%d]\n", val);
    //关闭配置文件， 释放资源
    config_destroy(&cfg);
    return 0;
}

int main(int argc, char *argv[])
{
    test("../etc/store.cfg");
    return 0;
}
*/
