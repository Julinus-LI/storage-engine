#include"capture_package.h"

void tcp_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
    struct tcphdr *tcpptr=(struct tcphdr *)(packet_content+14+20);  
    printf("----tcp protocol-----\n");  
    printf("source port:%d\n",ntohs(tcpptr->source));  
    printf("dest port:%d\n",ntohs(tcpptr->dest));  

    printf("sequence number:%u\n",ntohl(tcpptr->seq));  
    printf("acknowledgement number:%u\n",ntohl(tcpptr->ack_seq));  
    printf("header length:%d\n",tcpptr->doff*4);  
    printf("check sum:%d\n",ntohs(tcpptr->check));  
    printf("window size:%d\n",ntohs(tcpptr->window));  
    printf("urgent pointer:%d\n",ntohs(tcpptr->urg_ptr));  
}  

void ip_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
   
    /*
    int fd = open("./data.txt",O_RDWR|O_APPEND);
    if(fd < 0)
    {
        perror("open ./data.txt error");
    }
    // 把标准输出重定向到文件中
    dup2(fd,STDOUT_FILENO);
    */

   
     data_node_t *data_node = (data_node_t*)malloc(sizeof(data_node_t));
    if(data_node == NULL)
    {
        perror("malloc data_node_t failed");
        return;
    }
    struct ethhdr *ethptr;  
    struct iphdr *ipptr;  
    unsigned char *mac;  
    char buf[256];

    memset(data_node,0x00,sizeof(data_node_t));
    memset(buf,0x00,256);

    sprintf(data_node->buff,"--------------------------context----------\n");  
    //printf("%s\n", packet_content);  
    ethptr=(struct ethhdr *)packet_content;  
    strcpy(buf,"\n----ethernet protocol(phydical layer)-----\n");  
    strcat(data_node->buff,buf);
    
    memset(buf,0x00,256);
    sprintf(buf,"MAC source Address:\n");  
    strcat(data_node->buff,buf);

    mac=ethptr->h_source;  
    memset(buf,0x00,256);
    sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x\n",*mac,*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));  
    strcat(data_node->buff,buf);
    
    memset(buf,0x00,256);
    sprintf(buf,"MAC destination Address:\n");  
    strcat(data_node->buff,buf);

    mac=ethptr->h_dest;  
    memset(buf,0x00,256);
    sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x\n",*mac,*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));  
    strcat(data_node->buff,buf);
    
    memset(buf,0x00,256);
    sprintf(buf,"protocol:%04x\n",ntohs(ethptr->h_proto)); 
    strcat(data_node->buff,buf);
    
    struct in_addr s,d;  
    /* 这一块数据包 的偏移位置为 以太网的帧头部：目的地址(6 字节)，源地址(6 字节)，类型 2(字节)*/
    ipptr=(struct iphdr *)(packet_content+14);  

    
    memset(buf,0x00,256);
    sprintf(buf,"-----IP Protocol (network layer)-----\n");  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"version:%d\n",ipptr->version);  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"header length:%d\n",ipptr->ihl*4);  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"tos:%d\n",ipptr->tos);  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"total length:%d\n",ntohs(ipptr->tot_len));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"identification:%d\n",ntohs(ipptr->id));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"offset:%d\n",ntohs((ipptr->frag_off&0x1fff)*8));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"TTL:%d\n",ipptr->ttl);  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"checksum:%d\n",ntohs(ipptr->check));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"protocol:%d\n",ipptr->protocol);  
    strcat(data_node->buff,buf);

    s.s_addr=ipptr->saddr;  
    d.s_addr=ipptr->daddr;  
    memset(buf,0x00,256);
    sprintf(buf,"source address:%s\n",inet_ntoa(s));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"destination address:%s\n",inet_ntoa(d));  
    strcat(data_node->buff,buf);
    
    /* 这一块数据包 的偏移位置为 以太网的帧头部：目的地址(6 字节)，源地址(6 字节)，类型 2(字节)，再跳过 ip 数据报的头部 (20 字节) 
    *
    *  后面的偏移位置就是真正的数据部分 
    * */
    ipptr=(struct iphdr *)(packet_content+14);  
    struct tcphdr *tcpptr=(struct tcphdr *)(packet_content+14+20);  
    memset(buf,0x00,256);
    sprintf(buf,"----tcp protocol-----\n");  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"source port:%d\n",ntohs(tcpptr->source));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"dest port:%d\n",ntohs(tcpptr->dest));  
    strcat(data_node->buff,buf);


    memset(buf,0x00,256);
    sprintf(buf,"sequence number:%u\n",ntohl(tcpptr->seq));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"acknowledgement number:%u\n",ntohl(tcpptr->ack_seq));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"header length:%d\n",tcpptr->doff*4);  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"check sum:%d\n",ntohs(tcpptr->check));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"window size:%d\n",ntohs(tcpptr->window));  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"urgent pointer:%d\n",ntohs(tcpptr->urg_ptr));
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"urgent pointer:%d\n",ntohs(tcpptr->urg_ptr));
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    sprintf(buf,"-------------------print data ----------\n");
    strcat(data_node->buff,buf);
    /* 发送的二进制数据不重要，只要知道数据包的前 60 个字节就知道网络状况  */
/*
    char data[1024];
    //打印数据 

    int i;  
    for(i=0; i<pcap_header->len; ++i)  
    {  
         memset(data,0x00,1024);
         sprintf(data," %02x", packet_content[i]);  
         strcat(data_node->buff,data);

        if( (i + 1) % 16 == 0  )  
        {  
            memset(data,0x00,1024);
            sprintf(data,"\n");  
            strcat(data_node->buff,data);
        }  
    }  

    memset(buf,0x00,256);
    sprintf(buf,"\n\n");  
    strcat(data_node->buff,buf);

    memset(buf,0x00,256);
    printf(buf,"-------------------print data ----------\n");
    strcat(data_node->buff,buf);
    
   // printf("data_node->buff:\n\n%s\n",data_node->buff);
*/
    static int j = 0;
    j = j % def_info->rthr_num;
    if(can_write(rthr_info[j].data_que))
    {
        write_inval(rthr_info[j].data_que,node_info_t,data_node);
        printf("数据包放入成功!\n");
        j++;
        if(j == 65536)
        {
           j = 0; 
        }
    }
    
    sleep(1);
}  

void arp_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
    printf("------ARP Protocol-------\n");  
}  

void ethernet_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
    struct ethhdr *ethptr;  
    //struct iphdr *ipptr;  
    unsigned char *mac;  
    printf("--------------------------context----------\n");  
    //printf("%s\n", packet_content);  
    ethptr=(struct ethhdr *)packet_content;  
    printf("\n----ethernet protocol(phydical layer)-----\n");  
    printf("MAC source Address:\n");  
    mac=ethptr->h_source;  
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac,*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));  
    printf("MAC destination Address:\n");  
    mac=ethptr->h_dest;  
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac,*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));  
    printf("protocol:%04x\n",ntohs(ethptr->h_proto));  

    switch(ntohs(ethptr->h_proto)) {  
        case 0x0800:  
        printf("this is a IP protocol\n");  
        ip_packet_callback(argument,pcap_header,packet_content);  
        break;  
        case 0x0806:  
        printf("this is a ARP protocol\n");  
        arp_packet_callback(argument,pcap_header,packet_content);  
        break;  
        case 0x8035:  
        printf("this is a RARP protocol\n");  
        break;  
        default:  
        break;  

    }  
}

void *capture_package(void *args)
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
    pcap_loop(device, -1, ip_packet_callback, (u_char*)&id);  

    /* 关闭网卡设备 */
    pcap_close(device);  


    return NULL;
}

int start_capture_package()
{
    pthread_t tid;
    int ret =  pthread_create(&tid,NULL,(void*)capture_package,NULL);
    if( ret == -1  )
    {   
        //ERR("create capture_package pthread faile!\n");
        return -1; 
    }   
    return 0;
}

/*
int main()
{  
    capture_package(NULL);
    return 0;  
}  
*/
