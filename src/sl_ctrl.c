#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "sl_ctrl.h"
#include "sl_log.h"

static ctrlor_key_t *ctrlor_key = NULL;
static int  argcs = 0;
static char *argvs[32];

int parse_param(char *buff)
{
    char *ptr = buff;
    int i = 0, j = 0;
    static char argv[32][32];

    if (!buff) {
        return -1;
    }
    while(*ptr != '\0') {
        if (*ptr == ' ') {
            if (j > 0) {
                argv[i][j] = '\0';
                argvs[i] = argv[i];
                i++;
                j = 0;
                memset(argv[i], 0x00, 32);
            }
            ptr++;
            continue;
        }
        if (j > 31) return -1;
        if (i > 31) return -1;
        argv[i][j++] = *ptr++;
        if (*ptr == '\0' && j > 0) {
            argv[i][j] = '\0';
            argvs[i] = argv[i];
            i++;
        }
    }
    argvs[i] = NULL;
    argcs = i;
    return i;
}
ctrlor_key_t *get_ctrl(char* buff) 
{
    ctrlor_key_t *p_node = NULL;

    if (!buff) {
        return NULL;
    }
    for (p_node = ctrlor_key; p_node != NULL; p_node = p_node->next){
        if (!strcmp(p_node->key, buff)) {
            break;
        }
    }
    return p_node;
}
void ctrl_usage()
{
    ctrlor_key_t *p_node = NULL;

    ERR("\n");
    ERR("-----------------------------------------------\n");
    ERR("name\t\texplain\n");
    ERR("-----------------------------------------------\n");
    for (p_node = ctrlor_key->next; p_node != NULL; p_node = p_node->next){
        ERR("%s\t\t%s\n", p_node->key, p_node->bak);
    }
    ERR("-----------------------------------------------\n");
    return;
}

void ctrlor_add(char *key, char *bak, bc_func func)
{
    ctrlor_key_t *p = NULL;
    ctrlor_key_t *entry = NULL;

    if (!key || !func) return ;

    if (NULL == ctrlor_key){
        ctrlor_key = (ctrlor_key_t*)malloc(sizeof(ctrlor_key_t));
        memset(ctrlor_key, 0x00, sizeof(ctrlor_key_t));
        ctrlor_key->next = NULL;
    }
    for (p = ctrlor_key; p->next != NULL; p = p->next) ;
    entry = (ctrlor_key_t*)malloc(sizeof(ctrlor_key_t));
    memset(entry, 0x00, sizeof(ctrlor_key_t));
    strncpy(entry->key, key, 32);
    if (bak) 
        strncpy(entry->bak, bak, 128);
    entry->ctrl_func = func;
    entry->next = NULL;
    p->next = entry;
    return;
}
void *ctrlor_start(void *args)
{
    char *FIFO_FILE = NULL;
    int fd = -1, ret = -1; 
    char buff[1024] = {0};
    ctrlor_key_t *p_ctrlkey = NULL;

    FIFO_FILE = (char*)args;

    pthread_detach(pthread_self());

    if (NULL == ctrlor_key){
        ctrlor_key = (ctrlor_key_t*)malloc(sizeof(ctrlor_key_t));
        memset(ctrlor_key, 0x00, sizeof(ctrlor_key_t));
        ctrlor_key->next = NULL;
    }
    if (mkfifo(FIFO_FILE, 0666) < 0) {
        unlink(FIFO_FILE);
        if (mkfifo(FIFO_FILE, 0666) < 0) {
            perror("mkfifo error:");
            return NULL; 
        }   
    }
    fd = open(FIFO_FILE, O_RDWR|O_NONBLOCK);
    while(1) {
        if ((ret = read(fd, buff, 1024)) <= 0) {
            usleep(1000);
            continue;
        }
        buff[ret - 1] = '\0';
        if (parse_param(buff) < 0) {
            ERR("Format of parameters is wrong!-[%s]\n", buff);
            continue;
        }
        if ((p_ctrlkey = get_ctrl(argvs[0])) == NULL) {
            ctrl_usage();
            ERR("Invalid parameters :[%s]\n", argvs[0]);
            continue;
        }
        p_ctrlkey->ctrl_func(argcs, argvs);
    }
    return NULL;
}
int ctrl_init(char *fifo_file)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, ctrlor_start, (void*)fifo_file)) {
        ERR("init ctrl failed!\n");
        return -1;
    }
    return 0;
}
