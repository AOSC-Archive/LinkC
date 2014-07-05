#include "../../include/linkc_client.h"
#include "../../include/linkc_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

/*  初始化网络  */
int InitNetwork(){
    int sockfd;                                 // 网络句柄
    if ((sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0){ // 创建套接字[网络句柄]
        perror ("Socket");                      // 打印出错信息
        exit (EXIT_FAILURE);                    // 退出
    }
    LinkC_Debug("Init Socket",LINKC_SUCCESS);
    return sockfd;
}

int ConnectToServer(int Sockfd){
    struct sockaddr_in  NetAddr;
    memset((void*)&NetAddr,0,sizeof(struct sockaddr_in));
    NetAddr.sin_family  = AF_INET;
    NetAddr.sin_port    = htons(2341);
    inet_aton("127.0.0.1",(struct in_addr*)&NetAddr.sin_addr.s_addr);

    if(connect(Sockfd,(struct sockaddr *)&NetAddr, sizeof(struct sockaddr_in)) < 0){
        LinkC_Debug("Connect",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    LinkC_Debug("Connect",LINKC_SUCCESS);
    return 0;
}
