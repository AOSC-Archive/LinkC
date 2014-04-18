#include "LinkC_NetWorkProtocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>                 //  For 信号控制
#include <unistd.h>                 //  For 时钟信号
#include <pthread.h>

SocketList *List = NULL;            //  全局变量，套接字的链表

void TimerInt(int SigNo, siginfo_t *SigInfo, void *Arg){
    printf("Now Totle Socket is %d\n",List->TotalSocket);
    alarm(1);           //  1秒后发射信号
}

int InitSocketList(void){
    if(List != NULL){                                               //  如果链表为空
        printf("The LinkC Socket environment has been created!\n"); //  打印错误信息
        return 1;                                                   //  返回1
    }
    List = (SocketList*)malloc(sizeof(SocketList)); //  为链表分配内存
    List->StartNode     = NULL;                     //  开始节点挂空
    List->TotalSocket   = 0;                        //  套接字总数为0
    return 0;                                       //  返回0
}

int DestroySocketList(void){
    if(List == NULL){                               //  如果链表没有初始化
        printf("The LinkC Socket environment is not created!\n");   //  打印错误信息
        return 1;                                   //  返回1
    }
    if(List->TotalSocket == 0){                     //  如果没有套接字
        return 0;                                   //  返回0
    }
    LinkC_Socket *NowNode;                          //  当前节点
    NowNode = List->StartNode;                      //  设置节点为当前节点
    while(1){                                       //  循环
        pthread_mutex_lock(NowNode->Mutex_Lock);    //  申请互斥锁
        close(NowNode->Sockfd);                     //  关闭套接字
        if(NowNode->ErrorMessage != NULL){          //  如果错误消息指针不为空
            free(NowNode->ErrorMessage);            //  释放内存空间
        }
        DestroyPackageList(NowNode->RecvList);      //  释放接收缓冲区
        DestroyPackageList(NowNode->SendList);      //  释放发送缓冲区
        pthread_mutex_unlock(NowNode->Mutex_Lock);  //  解锁互斥锁
        pthread_mutex_destroy(NowNode->Mutex_Lock); //  销毁互斥锁
        if(NowNode->Next != NULL){
            NowNode = NowNode->Next;                //  设置为下一个节点
            free(NowNode->Perv);                    //  释放前一个节点
        }else{                                      //  或者
            free(NowNode);                          //  释放当前节点
            break;                                  //  跳出循环
        }
    }
    return 0;
}
int CreateSocket(LinkC_Socket *Socket, int Family, uint8_t SockType, const struct sockaddr *MyAddr, socklen_t addrlen){
    if(List == NULL){                                                           //  如果链表没有初始化
        printf("The LinkC Socket environment is not created!\n");               //  打印错误信息
        return 1;                                                               //  返回1
    }
    Socket                  =   (LinkC_Socket*)malloc(sizeof(LinkC_Socket));    //  为套接字结构体分配内存
    Socket->Available       =   0;                                              //  将可用包数设置为0
    memset((void *)&(Socket->Addr),0,sizeof(Socket->Addr));                     //  清空数据
    if(SockType == 1){                                                          //  如果要创建UDP套接字{
        Socket->Sockfd = socket(Family,SOCK_DGRAM,IPPROTO_UDP);                 //  创建UDP套接字
    }else{
        Socket->Sockfd = socket(Family,SOCK_STREAM,IPPROTO_TCP);                //  创建TCP套接字
    }
    bind(Socket->Sockfd,MyAddr,addrlen);                                        //  绑定本地地址
    // Add to List
    return 0;
}

int LinkC_UDP_Socket_Init (LinkC_Socket *Socket){
    Socket                  =   (LinkC_Socket*)malloc(sizeof(LinkC_Socket));    //  为套接字结构体分配内存
    Socket->RecvList        =   BuildPackageList();                             //  初始化接收缓冲区
    Socket->SendList        =   BuildPackageList();                             //  初始化发送缓冲区  
    memset((void *)&(Socket->Addr),0,sizeof(Socket->Addr));                     //  清空数据
    Socket->Sockfd          =   0;                                              //  将Sockfd设置为0
    Socket->Available       =   0;                                              //  将可用包数设置为0
    return 0;
}
