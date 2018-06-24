#ifndef __SL_LOG_H__
#define __SL_LOG_H__

#include <zlog.h>

extern zlog_category_t *log_handle;

#define ALL(...) zlog_fatal(log_handle, __VA_ARGS__)
#define INF(...) zlog_info(log_handle, __VA_ARGS__)
#define WAR(...) zlog_warn(log_handle, __VA_ARGS__)
#define DBG(...) zlog_debug(log_handle, __VA_ARGS__)
#define ERR(...) zlog_error(log_handle, __VA_ARGS__)


int open_log(char *path, char *mode);
void close_log();

#endif
