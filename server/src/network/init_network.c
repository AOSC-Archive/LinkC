/*
 * Author        ： Junfeng Zhang <564691478@qq.com>
 * Last-Change        ： April 6, 2014
 */

#include "linkc_db.h"
#include "linkc_network.h"
#include "p2p_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define DEBUG       1       // DEBUG模
#define PORT        2341    // 端口

int init_network(int port)
{
    int sockfd;                                 // 网络句柄
    socklen_t len;                              // 长度
    struct sockaddr_in local_addr;              // 用于保存网络地址
    memset(&local_addr,0,sizeof(local_addr));   // 清空数据
    if ((sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0) // 创建套接字[网络句柄]
    {
        perror ("Socket");                      // 打印出错信息
        exit (EXIT_FAILURE);                    // 退出
    }
    local_addr.sin_port = htons (port);         // 绑定端口
    local_addr.sin_family = AF_INET;            // 设置协议类型

    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);     // 任意IP可链接
    len = sizeof(local_addr);

    bind (sockfd, (struct sockaddr *)&local_addr, len); // 绑定地址
    listen (sockfd,5);                          // 设置最大等待链接数
    return sockfd;
}

int start_connect(void)
{
    struct user_data client;
    socklen_t len;
    int result = init_sqlite3_db();                 // 初始化数据库
    if (result != LINKC_SUCCESS)                    //  如果返回值不是 LINKC_SUCCESS [0]
    {
        printf ("DB Init error!\n");
        exit(EXIT_FAILURE);
    }
    int sockfd = init_network(PORT);                // 初始化网络模块
    if (sockfd == -1)
    {
        printf ("network_init error!\n");
        exit(EXIT_FAILURE);
    }
    len = sizeof (client.addr);
    pthread_t pid;
    while (1)
    {
#if DEBUG               //  如果是DEBUG情况下
        sleep (1);
        printf ("Server is waiting for connect!\n");
#endif                  //  结束判断
        client.sockfd = accept (sockfd,(struct sockaddr *)&client.addr,(socklen_t *)&len);  //  接受链接请求
#if PER_USER_TEST       //  如果是单用户测试
        keep_connect(&client);
#else                   //  否则
        pthread_create (&pid,NULL,(void*)*keep_connect,&(client));                          //  创建子进程，执行keep_connect函数
#endif                  //  结束判断
    }
}
