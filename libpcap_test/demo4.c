/*************************************************************************
	> File Name: demo4.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 14时32分20秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<arpa/inet.h>
#include<netinet/in.h>


#include <pcap.h>  
#include <stdlib.h>  
#include <stdio.h>  

int main(int argc, char *argv[])  
{  
      pcap_if_t *alldevs;  
      pcap_if_t *device;  
      char errbuf[PCAP_ERRBUF_SIZE];  

      if(pcap_findalldevs(&alldevs, errbuf) == -1)  
      {  
          fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);  
          exit(EXIT_FAILURE);  
      }  
      device = alldevs;  
      for(; device != NULL; device = device->next)  
      {  
          printf("Device name: %s\n", device->name);  
          printf("Description: %s\n", device->description);  
      }  
      /* 不再需要设备列表了，释放它 */  
      pcap_freealldevs(alldevs);  
      return 0;  
}  

