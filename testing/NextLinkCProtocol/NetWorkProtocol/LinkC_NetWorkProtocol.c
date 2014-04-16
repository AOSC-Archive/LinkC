#include "LinkC_NetWorkProtocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>                 //  For 信号控制
#include <unistd.h>                 //  For 时钟信号


void TimerInt(int SignalNo, siginfo_t *info, void *Arg){
    printf("SignalNo = %d\n",SignalNo);
    printf("sig pid is %d\n", (int)(info->si_pid));
    printf("sig uid is %d\n", (int)(info->si_uid));
    alarm(1);           //  1秒后发射信号
}

LinkC_Socket* LinkC_UDP_Socket_Init (void){
    LinkC_Socket *Socket    =   (LinkC_Socket*)malloc(sizeof(LinkC_Socket));    //  为套接字结构体分配内存
    Socket->RecvList        =   BuildPackageList();                             //  初始化接收缓冲区
    Socket->SendList        =   BuildPackageList();                             //  初始化发送缓冲区  
    memset((void *)&(Socket->Addr),0,sizeof(Socket->Addr));                     //  清空数据
    Socket->Sockfd          =   0;                                              //  将Sockfd设置为0
    Socket->Available       =   0;                                              //  将可用包数设置为0
    return Socket;
}

int             LinkC_Socket_Bind       (LinkC_Socket *Socket, const struct sockaddr *address);     //  绑定本地地址
