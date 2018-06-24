#ifndef __SL_WRITE_H__
#define __SL_WRITE_H__
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

int init_write_pth();
void print_write_pth_info();
int start_write_data();
#endif
