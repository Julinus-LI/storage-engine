/*************************************************************************
	> File Name: main.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月24日 星期日 15时34分44秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>

typedef struct ip_header
{
    unsigned char header_len:4;
    unsigned char version:4;
    unsigned char tos:8;
    unsigned short total_len;
    unsigned short ident;
    unsigned short flags;
    unsigned char  ttl:8;
    unsigned char  proto:8;
    unsigned short checksum;
    unsigned char  src_ip[4];
    unsigned char  dest_ip[4];

}ip_header;

struct buf
{
    char name[20];
    int age;
    char sex;
};
int main(void)
{
    char buf[1024] = {0};
    sprintf(buf,"==============="); 
    printf("buf = [%s]\n",buf);

    struct ip_header ip;
    printf("header_len = %d\n",ip.header_len);
    printf("header_version = %d\n",ip.version);
    printf("header_tos = %d\n",ip.tos);

    char buff[1024]; 
    char str[100];
    memset(buff,0x00,sizeof(buff));
    memset(str,0x00,sizeof(str));
   
    sprintf(str,"hello world\n");
    strcat(buff,str);

    sprintf(str,"hello linux\n");
    strcat(buff,str);

    sprintf(str,"hello c++\n");
    strcat(buff,str);

    printf("str = [%s]\n",str);
    printf("buf = [%s]\n",buf);


    struct buf b;
    strcpy(b.name,"aaaaa");
    b.age = 21;
    b.sex = 'm';
    
    memset(buff,0x0000,1024);
    memcpy(buff,(char*)&b,sizeof(struct buf));
    printf("buff = [%s]\n",buff);
    return 0;
}
