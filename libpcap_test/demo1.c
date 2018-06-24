/*************************************************************************
	> File Name: demo1.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 12时45分20秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pcap.h>

int main(void)
{
    char errBuff[PCAP_ERRBUF_SIZE],*device;

    device = pcap_lookupdev(errBuff);
    if(device)
    {
        printf("success : device : %s\n",device);
    }
    else
    {
        printf("error : %s\n",errBuff);     
    }

    return 0;
}
