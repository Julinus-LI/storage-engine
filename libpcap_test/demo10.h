/*************************************************************************
> File Name: demo10.h
> Author:    arrayli
> Mail:      1374367549@qq.com
> Created Time: 2018年06月24日 星期日 09时55分10秒
************************************************************************/

#ifndef _DEMO10_H
#define _DEMO10_H

#include<stdio.h>
#include<unistd.h>
#include<stdint.h>
#include<pcap.h>

#define IP_RF 0x8000        /* reserved fragment flag */
#define IP_DF 0x4000        /* dont fragment flag */
#define IP_MF 0x2000        /* more fragments flag */
#define IP_OFFMASK 0x1fff   /* mask for fragmenting bits */

#define IP_HL(ip)       (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)        (((ip)->ip_vhl) >> 4)

#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80

#define TH_OFF(th)  (((th)->th_offx2 & 0xf0) >> 4)
/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN  6

//typedef unsigned char u_char;
//typedef unsigned int u_short;

/* 以太网帧头部信息 */
typedef struct Ethernet_head_t{
    u_char ether_dst_mac[ETHER_ADDR_LEN]; /* 目标主机 mac 地址 */
    u_char ether_src_mac[ETHER_ADDR_LEN]; /* 源主机 mac 地址 */
    u_short ether_type; /* 以太网类型：IP? ARP? RARP? etc */
    int a;

}Ethernet_head_t;

/* IP 数据报头部被信息 */
typedef struct IP_head_t{
    u_char ip_ver_len;      /* 版本和头部长度 */
    u_char ip_tos;      /* 服务类型 */
    u_short ip_len;     /* 总长度 */
    u_short ip_id;      /* 标识 */
    u_short ip_off;     /* 片偏移*/
    
    u_char ip_ttl;      /* 生存周期 */
    u_char ip_p;        /* 协议类型 */
    u_short ip_sum;     /* 校验和 */
    struct in_addr ip_src; /* 源地址 */
    struct in_addr ip_dst; /* 目标地址 */
}IP_head_t;

/* TCP 数据报头部 */
typedef u_int tcp_seq;

struct TCP_Head_t{
    u_short th_sport;   /* 源端口 */
    u_short th_dport;   /* 目的端口 */
    tcp_seq th_seq;     /* 序列号 */
    tcp_seq th_ack;     /* 确认号 */
    u_char th_offx2;    /* 数据偏移（4 bit ),保留(4 bit)*/
    u_char th_flags;    /* 标识 TCP 控制不同的消息 */
    u_short th_win;     /* 滑动窗口 */
    u_short th_sum;     /* 校验和 */
    u_short th_urp;     /* 紧急指针 */
}TCP_Head_t;

typedef struct Package_Head
{
   Ethernet_Head_t mac_head;  /* 以太网帧头部 */     
   IP_head_t       ip_head;   /* ip 数据报头部 */
   TCP_Head_t      tcp_head;  /* tcp 数据报头细信息 */
}HEAD;

#endif
