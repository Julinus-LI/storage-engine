/*************************************************************************
> File Name: demo6.c
> Author:    arrayli
> Mail:      1374367549@qq.com
> Created Time: 2018年06月23日 星期六 23时56分44秒
************************************************************************/

#include <pcap.h>  
#include <stdio.h>  
#include<netinet/in.h>
#include<arpa/inet.h>
#include <netinet/ip.h>  
#include <netinet/if_ether.h>  
#include <netinet/tcp.h>  

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
    struct in_addr s,d;  
    struct iphdr *ipptr;  
    ipptr=(struct iphdr *)(packet_content+14);  

    printf("-----IP Protocol (network layer)-----\n");  
    printf("version:%d\n",ipptr->version);  
    printf("header length:%d\n",ipptr->ihl*4);  
    printf("tos:%d\n",ipptr->tos);  
    printf("total length:%d\n",ntohs(ipptr->tot_len));  
    printf("identification:%d\n",ntohs(ipptr->id));  
    printf("offset:%d\n",ntohs((ipptr->frag_off&0x1fff)*8));  
    printf("TTL:%d\n",ipptr->ttl);  
    printf("checksum:%d\n",ntohs(ipptr->check));  
    printf("protocol:%d\n",ipptr->protocol);  
    s.s_addr=ipptr->saddr;  
    d.s_addr=ipptr->daddr;  
    printf("source address:%s\n",inet_ntoa(s));  
    printf("destination address:%s\n",inet_ntoa(d));  

    switch(ipptr->protocol) {  
        case 6:  
        printf("tcp protocol\n");  
        tcp_packet_callback(argument,pcap_header,packet_content);  
        break;  
        case 1:  
        printf("icmp protocol\n");  
        break;  
        case 17:  
        printf("udp protocol\n");  
        break;  
        default:  
        break;  
    }  

}  

void arp_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
          printf("------ARP Protocol-------\n");  
      }

void cket_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
    struct ethhdr *ethptr;  
    struct iphdr *ipptr;  
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

void ethernet_packet_callback(unsigned char *argument,const struct pcap_pkthdr* pcap_header,const unsigned char *packet_content) {  
    struct ethhdr *ethptr;  
    struct iphdr *ipptr;  
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


int main(){  
    pcap_t *pt;  
    char *dev;  
    char errbuf[128];  
    struct bpf_program fp;  
    bpf_u_int32 maskp,netp;  
    int ret,i=0,inum;  
    int pcap_time_out=5;  
    char filter[128];  
    unsigned char *packet;  
    struct pcap_pkthdr hdr;  
    pcap_if_t *alldevs,*d;  

    if(pcap_findalldevs(&alldevs,errbuf)==-1) {  
        fprintf(stderr,"find interface failed!\n");  
        return -1;  
    }  
    for(d=alldevs;d;d=d->next){  
        printf("%d. %s\n",++i,d->name);  
        if(d->description)  
        printf("(%s)\n",d->description);  
        else  
        printf("(no description available)\n");  
    }  

    if(i==1)  
        dev=alldevs->name;  
    else {  
        printf("input a interface:(1-%d)",i);  
        scanf("%d",&inum);  
        if(inum<1||inum>i) {  
            printf("interface number out of range\n");  
            return -1;
        }  

        for(d=alldevs,i=1;i<inum;d=d->next,i++);  
        dev=d->name;  
    }  
                                                                        
    printf("dev:%s\n",dev);  
    ret=pcap_lookupnet(dev,&netp,&maskp,errbuf);  
    if(ret==-1){  
        fprintf(stderr,"%s\n",errbuf);  
        return -1;
    }  
        pcap_dump_open(pt, "t.pcap");  
    pt=pcap_open_live(dev,BUFSIZ,1,pcap_time_out,errbuf);  
    if(pt==NULL){  
             fprintf(stderr,"open error :%s\n",errbuf);  
            return -1;
    }  
    sprintf(filter,"");  
    if(pcap_compile(pt,&fp,filter,0,netp)==-1) {  
        fprintf(stderr,"compile error\n");  
        return -1;
    }  
    if(pcap_setfilter(pt,&fp)==-1) {  
            fprintf(stderr,"setfilter error\n");  
             return -1;
    }  

    pcap_loop(pt,-1,ethernet_packet_callback,NULL);  
    pcap_close(pt);  

    return 0;  

}  
