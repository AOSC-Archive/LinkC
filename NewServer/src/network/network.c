#include "../../include/linkc_server.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_db.h"
#include "../../include/linkc_basic_network.h"
#include "../../include/linkc_TCP_system/linkc_TCP_io.h"
#include "../../include/linkc_def.h"
#include "../../include/linkc_package_ctl.h"
#include "../../include/linkc_package.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

/*  初始化网络  */
int InitNetwork(int port){
    int sockfd;                                 // 网络句柄
    socklen_t len;                              // 长度
    struct sockaddr_in local_addr;              // 用于保存网络地址
    memset(&local_addr,0,sizeof(local_addr));   // 清空数据
    if ((sockfd = socket (AF_INET,SOCK_STREAM, IPPROTO_TCP)) < 0) // 创建套接字[网络句柄]
    {
        perror ("Socket");                      // 打印出错信息
        exit (EXIT_FAILURE);                    // 退出
    }
    local_addr.sin_port = htons (port);         // 绑定端口
    local_addr.sin_family = AF_INET;            // 设置协议类型

    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);     // 任意IP可链接
    len = sizeof(local_addr);

    if(bind(sockfd,(struct sockaddr* )&local_addr, len) < 0){
        perror("Bind");
        exit(0);
    }
    listen (sockfd,5);                          // 设置最大等待链接数
    return sockfd;
}

/*  等待链接    */
int WaitForConnect(){
    int Sockfd = InitNetwork(2341);
    PthreadData Client;
    socklen_t len = sizeof (Client.Addr);
#ifndef SINGLE_USER_TESTING
    pthread_t pid;
#endif
    LinkC_Debug("Waiting for Connection",LINKC_DEBUG);
    while (1){
        Client.Sockfd = accept (Sockfd,(struct sockaddr *)&Client.Addr,(socklen_t *)&len);  //  接受链接请求
#ifdef SINGLE_USER_TESTING
        MainConnect((void*)&Client);
#else
        pthread_create (&pid,NULL,MainConnect,&Client);                          //  创建子进程，执行MainConnect函数
#endif
    }
    return -1;
}

int SendActionStatus(int Sockfd, uint16_t StatusCode){
    char Package[sizeof(PackageHeader)+sizeof(MessageHeader)];
    MessageHeader Header;
    int Length = 0;
    Header.StatusCode  = htons(StatusCode);     //  Set StatusCode
    Length = _Package((void*)&Header,sizeof(MessageHeader),NORMAL_MESSAGE,Package);  //  Package
    return send(Sockfd,Package,Length,0);
}


/*  主函数  */
void* MainConnect(void *Arg){
    struct sockaddr_in NetAddr  = ((PthreadData*)Arg)->Addr; //  结构体
    int         Sockfd          = ((PthreadData*)Arg)->Sockfd;
    void*       Buffer          = malloc(STD_PACKAGE_SIZE);
    void*       Package         = malloc(STD_PACKAGE_SIZE);
    UserData    User;
    int         Status          = 0;
    uint8_t     ActionType;
START:
    if(TCP_recv(Sockfd,Package,STD_BUFFER_SIZE,0) < 0){     //  接收数据失败
        LinkC_Debug("Receiving",LINKC_FAILURE);
        goto END;                                           //  跳转到end位置
    }
    if(_UnPackage(Package,STD_BUFFER_SIZE,Buffer) < 0){     //  解包
        LinkC_Debug("Unpacking",LINKC_FAILURE);
        goto END;
    }
    if(((MessageHeader*)Buffer)->ActionType != USER_LOGIN){    //  如果请求的服务类型不是USER_LOGIN
        LinkC_Debug("Bad Request Type",LINKC_WARNING);
        goto END;
    }
    if(CheckPassword((LoginData*)((char*)Buffer+sizeof(MessageHeader)),&(User.UID))== LINKC_FAILURE){   //  检查密码
        LinkC_Debug("Login",LINKC_FAILURE);
        SendActionStatus(Sockfd,LOGIN_FAILURE);
        goto START;
    }
    SendActionStatus(Sockfd,LOGIN_SUCCESS);                     //  设置状态
    if(SetStatus(&User,NetAddr,STATUS_ONLINE) == LINKC_FAILURE){
        SendActionStatus(Sockfd,SET_STATUS_FAILURE);
        goto START;
    }
    SendActionStatus(Sockfd,SET_STATUS_SUCCESS);
    LinkC_Debug("Login",LINKC_DONE);
    while(1){
        Status = TCP_recv(Sockfd,Package,STD_BUFFER_SIZE,0);
        if(Status == LINKC_NO_DATA){                            //  没有数据就视作对方关闭了链接
            LinkC_Debug("Disconnection",LINKC_WARNING);
            goto END;
        }
        if(Status < 0){
            LinkC_Debug("Receiving",LINKC_FAILURE);
            goto END;                                           //  跳转到end位置
        }
        if(_UnPackage(Package,STD_BUFFER_SIZE,Buffer) < 0){     //  解包
            LinkC_Debug("Unpacking",LINKC_FAILURE);
            goto END;
        }
        ActionType = GetActionType(((MessageHeader*)Buffer)->ActionType);
        if(((MessageHeader*)Buffer)->ActionType == USER_LOGOUT){
            if(SetStatus(&User,NetAddr,STATUS_OFFLINE) == LINKC_FAILURE){
                SendActionStatus(Sockfd,LOGOUT_FAILURE);
                LinkC_Debug("Logged Off",LINKC_FAILURE);
                continue;
            }
            SendActionStatus(Sockfd,LOGOUT_SUCCESS);
            LinkC_Debug("Logged Off",LINKC_SUCCESS);
            goto END;
        }else if(ActionType == RQUEST_DATA){
            ReplyData(&User,Sockfd,GetDataType(((MessageHeader*)Buffer)->ActionType),(RequestUser*)(char*)Buffer+sizeof(MessageHeader));
            continue;
        }else if(ActionType == UPLOAD_DATA){
            continue;
        }
        LinkC_Debug("No Relevant Action",LINKC_WARNING);
    }

END:
    LinkC_Debug("Disconnection",LINKC_DEBUG);
    free(Buffer);               // 释放内存！［不释放内存线程可能死得惨］
    free(Package);
    Buffer      = NULL;
    Package     = NULL;

    pthread_exit(NULL);
}
