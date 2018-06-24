/*************************************************************************
	> File Name: demo11.h
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月24日 星期日 12时07分29秒
 ************************************************************************/

#ifndef _DEMO11_H
#define _DEMO11_H

#include <pcap.h>  
#include <stdio.h>  
#include<unistd.h>
#include<stdlib.h>
#include <netinet/ip.h>  
#include <netinet/if_ether.h>  
#include <netinet/tcp.h>  
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include"sl_que.h"
#include"def_info.h"
#include"disk_info.h"
#include"data_deal.h"
#include"capture_package.h"


/* 以太网帧的头部 */
typedef struct ethernet_header
{
    unsigned char   dest_mac[6];         /* 目标 MAC 地址*/
    unsigned char   src_mac[6];          /* 源MAC地址 */
    unsigned short  eth_type;            /* 上层协议类型: 0x0800　　网际协议（IP）0x0806　　地址解析协议（ARP）  0x8035　　反向地址解析协议 */
    
}ethernet_header_t;

/* ip 数据报的头部 */
typedef struct ip_header_t
{
    unsigned char version;              /* 版本 */
    unsigned char header_len;           /* 首部长度 */
    unsigned char tos;                  /* 服务类型  */
    unsigned short total_len;           /* 总长度 */
    unsigned short ident;               /* 标识 */
    unsigned short flags;               /* 标志位 */
    unsigned char  ttl;                 /* 生存时间 */
    unsigned char  proto;               /* 协议类型 */
    unsigned short checksum;            /* 校验和 */
    unsigned char  src_ip[4];           /* 源 IP 地址 */
    unsigned char  dest_ip[4];          /* 目的 IP 地址 */

}ip_header_t;

/* tcp 数据报头 */
typedef struct tcp_header_t
{
    unsigned int src_port;              /* 源主机端口 */
    unsigned int dest_port;             /* 目标主机端口 */
    unsigned int sequence_number;       /* 序号 */
    unsigned int flags;                 /* 确认号 */
    unsigned int header_len;            /* 头部长度 */
    unsigned int checksum;              /* 校验和 */
    unsigned int windows;               /* 滑动窗口 */
    unsigned int pointer;               /* 紧急指针 */
}tcp_header_t;

/* 数据包 */
typedef struct package_t
{
    ethernet_header_t mac_head;
    ip_header_t       ip_head; 
    tcp_header_t      tcp_head;
    char data[1024];
}package_t;

#endif
