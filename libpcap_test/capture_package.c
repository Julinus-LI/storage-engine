/*************************************************************************
	> File Name: capture_package.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月23日 星期六 12时52分38秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pcap.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void show_ip_mask(char *dev)
{
    char errBuff[1024];
    struct in_addr addr;
    char *net,*mask;
    bpf_u_int32 netp,maskp;
    
    int err = pcap_lookupnet(dev,&netp,&maskp,errBuff);
    if(err == -1)
    {
       printf("couldn't detect the ip and maksp\n"); 
        return;
    }
    addr.s_addr  = netp;
    net = inet_ntoa(addr); 
    
    if(NULL == net)
    {
       printf("ip error\n"); 
        return ;
    }
    
    printf("ip : %s\n",net);
    addr.s_addr = maskp;
    mask = inet_ntoa(addr);

    if(mask == NULL)
    {
        printf("mask error\n");
        return;
     }
    
    printf("mask: %s\n",mask);
}



int main(void)
{
    char errBuff[1024],*dev;
    char select = 'a';

    printf("select(display the packet in detail)/n: ( Y/N ?))");
    scanf("%c",&select);

    while(select != 'Y' && select != 'y' && select != 'n' && select != 'N')
    {
        printf("input the error!\nplease input the Y/N/y/n:");
        scanf("%c",&select);
    }

    // look net device 
    dev = pcap_lookupdev(errBuff);
    if(dev == NULL)
    {
        printf("couldn't find default device: %s\n",errBuff);
        return 1;
    }
    else
    {
       printf("find success: device : %s\n",dev); 
    }

    // ip 掩码显示
    show_ip_mask(dev);
    return 0;
}
