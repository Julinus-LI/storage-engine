/*************************************************************************
	> File Name: demo10.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月24日 星期日 09时52分37秒
 ************************************************************************/

#include <pcap.h>  
#include <time.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include<arpa/inet.h>
#include<netinet/in.h>
#include"demo10.h"

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)  
{  
    int * id = (int *)arg;  

    printf("id: %d\n", ++(*id));  
    printf("Packet length: %d\n", pkthdr->len);  
    printf("Number of bytes: %d\n", pkthdr->caplen);  
    printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));   
   
    HEAD *head = NULL;
    head = (HEAD*)packet;

    int i;  
    for(i=0; i<pkthdr->len; ++i)  
    {  
        printf(" %02x", packet[i]);  
        if( (i + 1) % 16 == 0  )  
        {  
            printf("\n");  
        }  
    }  

    printf("\n\n");  
}  


int main(void)
{
    char errBuf[PCAP_ERRBUF_SIZE],*devStr;

    /* 获得网卡设备  */
    devStr = pcap_lookupdev(errBuf);
    if(devStr)
    {
        printf("success: device: %s\n", devStr);   
    }
    else
    {
        printf("error: %s\n", errBuf);  
        exit(1);  
    }

    /* 打开网卡设备直到数据包到来 */
    pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);  

    if(!device)  
    {  
        printf("error: pcap_open_live(): %s\n", errBuf);  
        exit(1);  
    }  
    /* 一直循环等待数据包 */  
    int id = 0;  
    pcap_loop(device, -1, getPacket, (u_char*)&id);  

    pcap_close(device);  

    return 0;
}


