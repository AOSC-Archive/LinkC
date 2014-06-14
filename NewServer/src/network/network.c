#include "../../include/linkc_basic_network.h"
#include "../../include/linkc_TCP_system/linkc_TCP_io.h"
#include "../../include/linkc_def.h"
#include "../../include/linkc_package_ctl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

int InitNetwork(int port){
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


int WaitForConnect(){
    int Sockfd = InitNetwork(2341);
    PthreadData Client;
    socklen_t len = sizeof (Client.Addr);
    pthread_t pid;
    while (1){
        Client.Sockfd = accept (Sockfd,(struct sockaddr *)&Client.Addr,(socklen_t *)&len);  //  接受链接请求
        //MainConnect(&Client);
        pthread_create (&pid,NULL,MainConnect,&Client);                          //  创建子进程，执行MainConnect函数
    }
    return -1;
}


void* MainConnect(void *Arg){
    struct sockaddr_in DestAddr    = ((PthreadData*)Arg)->Addr;
    int         Sockfd      = ((PthreadData*)Arg)->Sockfd;
    void*       Buffer      = malloc(STD_BUFFER_SIZE);
    void*       Package     = malloc(STD_BUFFER_SIZE);
    printf("Connected on port %d\n",ntohl(DestAddr.sin_port));
    if(TCP_Recv(Sockfd,Package,STD_BUFFER_SIZE,0) < 0)
        goto end;
    if(_UnPackage(Package,STD_BUFFER_SIZE,Buffer) < 0)
        goto end;
    if(((MessageHeader*)Buffer)->ServiceType != USER_LOGIN)
        goto end;

end:
    printf("Disonnected!\n");
    free(Buffer);
    free(Package);
    Buffer      = NULL;
    Package     = NULL;

    return NULL; 
}
