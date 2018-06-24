/*************************************************************************
	> File Name: capture_package.h
	> Author: 
	> Mail: 
	> Created Time: Sat 17 Mar 2018 02:57:45 AM PDT
 ************************************************************************/

#ifndef _CAPTURE_PACKAGE_H
#define _CAPTURE_PACKAGE_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<time.h>
#pragma pack(1)

#define HEAD_LENGTE 60
typedef unsigned short  u_short;
typedef unsigned int u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;

//数据帧头
typedef struct FramHeader_t
{ 
	//Pcap捕获的数据帧头
	u_int8 DstMAC[6]; //目的MAC地址
	u_int8 SrcMAC[6]; //源MAC地址
	u_short FrameType;    //帧类型
} FramHeader_t;

//IP数据报头
typedef struct IPHeader_t
{ 
	u_int8 Ver_HLen;      	//版本+报头长度
	u_int8 TOS;            	//服务类型
	u_int16 TotalLen;       //总长度
	u_int16 ID; 			//标识
	u_int16 Flag_Segment;   //标志+片偏移
	u_int8 TTL;            	//生存周期
	u_int8 Protocol;       	//协议类型
	u_int16 Checksum;       //头部校验和
	u_int32 SrcIP; 			//源IP地址
	u_int32 DstIP; 			//目的IP地址
} IPHeader_t;

//TCP数据报头
typedef struct TCPHeader_t
{
	u_int16 SrcPort; 		//源端口
	u_int16 DstPort; 		//目的端口
	u_int32 SeqNO; 			//序号
	u_int32 AckNO; 			//确认号
	u_int8 HeaderLen; 		//数据报头的长度(4 bit) + 保留(4 bit)
	u_int8 Flags; 			//标识TCP不同的控制消息
	u_int16 Window; 		//窗口大小
	u_int16 Checksum; 		//校验和
	u_int16 UrgentPointer;  //紧急指针
}TCPHeader_t;

typedef struct head
{
    FramHeader_t  mac_head;
    IPHeader_t    ip_head;
    TCPHeader_t   tcp_head;
}HEAD;

int start_capture_package();
#endif
