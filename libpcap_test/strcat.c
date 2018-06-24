/*************************************************************************
	> File Name: strcat.c
	> Author:    arrayli
	> Mail:      1374367549@qq.com
	> Created Time: 2018年06月24日 星期日 16时45分58秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(void)
{
    char p[1024];
    char str1[] = "aaaa";
    char str2[] = "bbbb";
    char str3[] = "cccc";
    char str4[] = "dddd";
    
    strcat(p,str1);
    printf("p = %s,str = %s\n",p,str1);

    strcat(p,str2);
    printf("p = %s,str = %s\n",p,str2);

    strcat(p,str3);
    printf("p = %s,str = %s\n",p,str3);

    strcat(p,str4);
    printf("p = %s,str = %s\n",p,str4);
    return 0;
}
