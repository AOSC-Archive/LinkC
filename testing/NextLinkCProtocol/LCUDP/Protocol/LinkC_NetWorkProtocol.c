#include "LinkC_NetWorkProtocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>                 //  For 信号控制
#include <unistd.h>                 //  For 时钟信号
#include <pthread.h>

SocketList *List = NULL;            //  全局变量，套接字的链表

void TimerInt(int SigNo, siginfo_t* SigInfo , void* Arg){
    printf("SigNo = %d\nSigCode = %d\n",SigNo,SigInfo->si_code);
    printf("Arg's Addr = [%lx]\n",(unsigned long)Arg);
    printf("Now Totle Socket is %d\n",List->TotalSocket);
//    alarm(1);           //  1秒后发射信号
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

int IsSocketInList(LinkC_Socket *Socket){
    if(Socket == NULL){                             //  如果传入参数是空指针
        printf("Argument is NULL!\n");              //  打印错误信息
        return 2;                                   //  返回错误
    }
    SocketListNode *NowNode = List->StartNode;      //  新建一个节点，指向链表的开始节点
    while(NowNode){                                 //  循环[当前节点不为空]
        if(NowNode->Socket == Socket)   return 0;   //  返回找到
        NowNode = NowNode->Next;                    //  设置为下一个节点
    }
    return 1;                                       //  返回未找到
}
int CreateSocket(LinkC_Socket *Socket, int Family, int SockType, const struct sockaddr *MyAddr, socklen_t addrlen){
    if(List == NULL){                                                           //  如果链表没有初始化
        printf("The LinkC Socket environment is not created!\n");               //  打印错误信息
        return 1;                                                               //  返回1
    }
    Socket                  =   (LinkC_Socket*)malloc(sizeof(LinkC_Socket));    //  为套接字结构体分配内存
    Socket->Available       =   0;                                              //  将可用包数设置为0
    Socket->Sockfd          =   socket(Family,SockType,0);                      //  创建套接字
    Socket->SendList        =   BuildPackageList();                             //  创建链表
    Socket->RecvList        =   BuildPackageList();                             //  创建链表
    bind(Socket->Sockfd,MyAddr,addrlen);                                        //  绑定地址

    AddSocketToList(Socket);
    return 0;
}

int AddSocketToList(LinkC_Socket *Socket){
    if(Socket == NULL){                             //  如果参数为空指针
        printf("The Argument is NULL\n");           //  打印错误信息
        return 1;
    }
    if(IsSocketInList(Socket) == 0){                //  当前Socket是否已经存在于链表中，如果存在
        printf("bad addition");                     //  打印错误信息
        return 1;
    }
    SocketListNode* Node;
    Node = (SocketListNode*)malloc(sizeof(SocketListNode));     //  分配内存
    Node->Socket    = Socket;                       //  保存当前新建的Socket
    if(List->TotalSocket == 0){                     //  如果现在还没有Socket
        List->StartNode = Node;                     //  将当前节点设置为开始节点
        Node->Perv      = NULL;                     //  当前节点的前一个节点挂空
        Node->Next      = NULL;                     //  当前节点的后一个节点挂空
        List->TotalSocket++;                        //  Socket总数加一
    }else{
        Node->Next              = List->StartNode;  //  新建节点的下一个设置成现在的起始节点
        Node->Perv              = NULL;             //  当前节点的前一个节点挂空
        List->StartNode->Perv   = Node;             //  链表起始节点的前一个为新建节点
        List->StartNode         = Node;             //  链表起始节点为当前节点
    }
    return 0;                                       //  返回函数
}

int DelSocketFromList(LinkC_Socket *Socket){
    if(Socket == NULL){                                 //  如果传入参数为空
        printf("The Argument is NULL\n");               //  打印错误信息
        return 1;                                       //  返回错误
    }
    SocketListNode *NowNode = List->StartNode;          //  声明一个节点指针，指向链表的开头
    while(NowNode){
        if(NowNode->Socket == Socket){                  //  如果找到
            pthread_mutex_lock(NowNode->Mutex_Lock);    //  申请互斥锁
            close(Socket->Sockfd);                      //  关闭套接字
            if(Socket->ErrorMessage != NULL){           //  如果错误消息指针不为空
                free(Socket->ErrorMessage);             //  释放内存空间
            }
            DestroyPackageList(Socket->RecvList);       //  释放接收缓冲区
            DestroyPackageList(Socket->SendList);       //  释放发送缓冲区
            if(NowNode->Perv != NULL){                  //  如果当前节点的前一个节点不为空
                NowNode->Perv->Next = NowNode->Next;    //  设置当前节点的前一个节点的后一个节点为当前节点的后一个节点
            }else{                                      //  否则
                List->StartNode     = NowNode->Next;    //  设置链表的开始节点为当前节点的后一个节点
            }
            if(NowNode->Next != NULL){                  //  如果当前节点的后一个节点不为空
                NowNode->Next->Perv = NowNode->Perv;    //  设置当前节点的后一个节点的前一个节点为当前节点的前一个节点
            }
            pthread_mutex_unlock(NowNode->Mutex_Lock);  //  解锁互斥锁
            pthread_mutex_destroy(NowNode->Mutex_Lock); //  销毁互斥锁
            Socket = NULL;                              //  请随时注意挂空指针以防意外
            return 0;
        }else{                                          //  否则
            NowNode = NowNode->Next;                    //  设置当前节点为当前节点的下一个节点
        }
    }
    return 1;                                           //  返回失败
}

int DestroySocketList(){
    if(List == NULL){                                   //  如果链表没有初始化
        printf("The LinkC Socket environment is not created!\n");
        return 1;                                       //  返回1
    }
    if(List->TotalSocket == 0){                         //  如果没有套接字
        return 0;                                       //  返回0
    }
    SocketListNode *NowNode;                            //  当前节点
    NowNode = List->StartNode;                          //  设置节点为当前节点
    while(1){                                           //  循环
        pthread_mutex_lock(NowNode->Mutex_Lock);        //  申请互斥锁
        close(NowNode->Socket->Sockfd);                 //  关闭套接字
        if(NowNode->Socket->ErrorMessage != NULL){      //  如果错误消息指针不为空
            free(NowNode->Socket->ErrorMessage);        //  释放内存空间
        }
        DestroyPackageList(NowNode->Socket->RecvList);  //  释放接收缓冲区
        DestroyPackageList(NowNode->Socket->SendList);  //  释放发送缓冲区
        pthread_mutex_unlock(NowNode->Mutex_Lock);      //  解锁互斥锁
        pthread_mutex_destroy(NowNode->Mutex_Lock);     //  销毁互斥锁
        if(NowNode->Next != NULL){
            NowNode = NowNode->Next;                    //  设置为下一个节点
            free(NowNode->Perv);                        //  释放前一个节点
        }else{                                          //  或者
            free(NowNode);                              //  释放当前节点
            break;                                      //  跳出循环
        }
    }
    return 0;                                           //  返回函数
}

int DeleteSocket(LinkC_Socket *Socket){
    return DelSocketFromList(Socket);
}
