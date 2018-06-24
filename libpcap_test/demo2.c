#include <stdio.h>
#include <pcap.h>
#include <time.h>

void capture_packet1(pcap_t* device)
{
    struct pcap_pkthdr packet;
    char errbuf[1024];
    //capture the packet        
    const u_char* pkt=pcap_next(device,&packet);
    if(!pkt){
        printf("couldn't capture packet: %s\n",errbuf);
        return;

    }

    printf("Packet length: %d\n", packet.len);  
    printf("Number of bytes: %d\n", packet.caplen);  
    printf("Recieved time: %s\n", ctime((const time_t*)&packet.ts.tv_sec)); 
}

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
{
    int * id = (int *)arg;  

    printf("id: %d\n", ++(*id));  
    printf("Packet length: %d\n", pkthdr->len);  
    printf("Number of bytes: %d\n", pkthdr->caplen);  
    printf("Recieved time: %s\n", ctime((const time_t *)&pkthdr->ts.tv_sec));   
    //print packet 

    int i;  
    for(i=0; i<pkthdr->len; ++i)  {  
        printf(" %02x", packet[i]);  
        if( (i + 1) % 16 == 0  )   
        printf("\n");  
    }  
    printf("\n\n");
}

void capture_packet2(pcap_t* device)
{
    struct pcap_pkthdr packet;
    int id = 0;
    //capture the packet 
    pcap_loop(device,-1,getPacket,(u_char*)&id);
}

int main()
{
    char *dev, errbuf[1024];
    char select='a';
    printf("select(dispaly the packet in detail)/n:( Y/N ? ))");
    scanf("%c",&select);
    while(select!='Y'&&select!='y'&&select!='n'&&select!='N'){
        printf("input the error!\nplease input the Y/N/y/n:");
        scanf("%c",&select);

    }

    //look for the net device 

    dev=pcap_lookupdev(errbuf);
    if(dev==NULL){
        printf("couldn't find default device: %s\n",errbuf);
        return 1;

    }
    else{
        printf("fidn success: device :%s\n",dev);

    }

    //open the finded device(must set :ifconfig eth0 promisc)

    pcap_t* device=pcap_open_live(dev,65535,1,0,errbuf);
    if(!device){
        printf("couldn't open the net device: %s\n",errbuf);
        return 1;

    }
    if(select=='Y')
    capture_packet2(device);
    else
    while(1)//由于pcap_next()函数只返回下一个数据包的指针
    capture_packet1(device); 
    return 0;

}
