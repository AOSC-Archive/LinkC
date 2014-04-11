/*
 * Author        ： Junfeng Zhang <564691478@qq.com>
 * Last-Change        ： April 5, 2014
 */

#include "conn_list.h"
#include "p2p_helper.h"
#include "def.h"
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
extern int init_network(int port);

int ip_size     = sizeof(ip_t);
int addr_size    = sizeof(struct sockaddr_in);
socklen_t len    = sizeof(struct sockaddr_in);

int Network_init(int port);

struct p2pinfo{
    struct sockaddr_in Dest;
    int is_server;
};
int info_size    = sizeof(struct p2pinfo);

int p2p_helper(void)
{
    int sockfd;                 //  网络链接句柄
    conn_list list;             //  链表[用来存储等待配对的网络对象]
    conn_list_item item;        //  操作用的item
    struct sockaddr_in addr;    //  保存网络地址用
    struct p2pinfo info;        //  发回数据用
    if ((sockfd = Network_init(2342)) < 0)  //  如果初始化网络链接失败
        return ERROR_NETWORK_INIT;          //      返回错误
    CHECK_FAILED(conn_list_init(&(list)),ERROR_LIST_INIT);  //  初始化链表
    while(1)                    //  无限循环
    {
        bzero((void *)&addr,addr_size);     //  清空addr[保存网络地址用]数据
        recvfrom(sockfd,(void *)&(item.info.Dest.sin_addr.s_addr),ip_size,0,(struct sockaddr *)&addr,&len); //  等待从网络任何地方发来的UDP数据包
        item.info.Src=addr;                 //  保存发来数据包的来源地址
//        printf("DestIP=%s\n",inet_ntoa(item.info.Dest.sin_addr));                // Debug
//        printf("SrcIP\t=%s\nSrcPort\t=%d\n",inet_ntoa(item.info.Src.sin_addr),addr.sin_port);    // Debug
        if(conn_list_find(&list,item.info,&(item.info.Dest)) == -14)    //  在链表中搜寻是否有这个地址[作为以前某个地址请求的对象][如果没找到]
        {
            conn_list_add(&list,item);                                  //      将请求添加到链表中
            continue;                                                   //      回到循环开始
        }
        info.Dest = item.info.Dest;
        info.is_server = 1;
        sendto(sockfd,(void *)&info,info_size,0,(struct sockaddr *)&(item.info.Src),len);   //  发回信息
        info.Dest = item.info.Src;
        info.is_server = 0;
        sendto(sockfd,(void *)&info,info_size,0,(struct sockaddr *)&(item.info.Dest),len);  //  发回信息
        conn_list_remove(&list,&item);                                  //  把这组信息删掉
    }
    exit(0);
}


int Network_init(int port)
{
    int sockfd;                    // Network Socket
    struct sockaddr_in local_addr;
    memset(&local_addr,0,addr_size);
    if ((sockfd = socket (AF_INET,SOCK_DGRAM, 0)) < 0)    // UDP Socket
    {
        perror ("Socket");
        exit (EXIT_FAILURE);
    }
    local_addr.sin_port = htons (port);
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind (sockfd, (struct sockaddr *)&local_addr, len)) == -1)
    {
        perror("bind");
        return (-1);
    }
    return (sockfd);
}
