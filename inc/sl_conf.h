#ifndef __SL_CONF_H__
#define __SL_CONF_H__
#include "sl_com.h"

enum __dat_type_t
{
    TYPE_INT = 0,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_STRING
};

int get_val_single(char *path, void *val, int dtype);

int get_val_arry(char *path, void **val, int count, int dtype);

int open_conf(char *name);

void close_conf();

#endif
