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
    printf("Arg's Addr = [%lx]\n",      (unsigned long)Arg);
    printf("Now Totle Socket is %d\n",  List->TotalSocket);

    PackageListNode *PackNode   = NULL;                             //  缓冲区链表节点指针
    SocketListNode  *Node       = List->StartNode;                  //  赋值为开始节点

    while(Node){                                                    //  循环直到当前节点为空
        if(Node->Socket->ErrorMessage != NULL){                     //  如果错误信息不为空
            printf("Socket Error [%s]\n",Node->Socket->ErrorMessage);// 输出错误信息
            free(Node->Socket->ErrorMessage);                       //  释放内存
            Node->Socket->ErrorMessage = NULL;                      //  挂空指针
        }
        if(Node->Socket->SendList->TotalNode != 0){                 //  如果发送链表中还有剩余[也就是收到确认没有到达]
            pthread_mutex_lock(Node->Socket->SendList->MutexLock);  //  给链表上锁上锁
            PackNode = Node->Socket->SendList->StartNode;           //  设置为链表开始节点
            while(PackNode){
                if(PackNode->ResendTime > MAX_RESEND_TIME){         //  如果大于最大重发次数
                    //  执行断线操作
                }else if(PackNode->TimeToLive == 0){                //  在没有断线的情况下如果当前数据剩迟迟没有收到收到确认
                    ResendMessage(Node->Socket,PackNode->Package ,PackNode->MessageLength); //  重发消息
                    PackNode->TimeToLive = MAX_TIME_TO_LIVE;        //  重设剩余生存时间
                    PackNode->ResendTime ++;                        //  重发次数自增加一
                }
                PackNode = PackNode->Next;                          //  跳转到下一个节点
            }
            pthread_mutex_unlock(Node->Socket->SendList->MutexLock);//  给链表解锁
            PackNode = NULL;                                        //  挂空指针
        }
    }
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

int IsSocketInList(int Socket){
    SocketListNode *NowNode = List->StartNode;      //  新建一个节点，指向链表的开始节点
    while(NowNode){                                 //  循环[当前节点不为空]
        if(NowNode->Socket->Sockfd == Socket)
            return 0;                               //  返回找到
        NowNode = NowNode->Next;                    //  设置为下一个节点
    }
    return 1;                                       //  返回未找到
}
int CreateSocket(const struct sockaddr *MyAddr){
    LinkC_Socket *Socket    =   (LinkC_Socket*)malloc(sizeof(LinkC_Socket));    //  为套接字结构体分配内存
    Socket->Available       =   0;                                              //  将可用包数设置为0
    Socket->Sockfd          =   socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);         //  创建UDP套接字
    Socket->SendList        =   BuildPackageList();                             //  创建链表
    Socket->RecvList        =   BuildPackageList();                             //  创建链表
    bind(Socket->Sockfd,MyAddr,sizeof(struct sockaddr_in));                     //  绑定地址

    AddSocketToList(Socket);
    return Socket->Sockfd;
}

int AddSocketToList(LinkC_Socket *Socket){
    if(Socket == NULL){                             //  如果参数为空指针
        printf("The Argument is NULL\n");           //  打印错误信息
        return 1;
    }
    if(IsSocketInList(Socket->Sockfd) == 0){        //  当前Socket是否已经存在于链表中，如果存在
        printf("Bad addition\n");                   //  打印错误信息
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
            NowNode->Socket->ErrorMessage = NULL;       //  挂空指针
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

int DeleteSocket(int Socket){
    SocketListNode *NowNode = List->StartNode;          //  声明一个节点指针，指向链表的开头
    while(NowNode){
        if(NowNode->Socket->Sockfd == Socket){          //  如果找到
            pthread_mutex_lock(NowNode->Mutex_Lock);    //  申请互斥锁
            close(Socket);                              //  关闭套接字[真是简单粗暴啊]
            if(NowNode->Socket->ErrorMessage != NULL){  //  如果错误消息指针不为空
                free(NowNode->Socket->ErrorMessage);    //  释放内存空间
                NowNode->Socket->ErrorMessage = NULL;   //  挂空指针
            }
            DestroyPackageList(NowNode->Socket->RecvList);  //  释放接收缓冲区
            DestroyPackageList(NowNode->Socket->SendList);  //  释放发送缓冲区
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
            return 0;
        }else{                                          //  否则
            NowNode = NowNode->Next;                    //  设置当前节点为当前节点的下一个节点
        }
    }
    return 1;                                           //  返回失败
}

int __LinkC_Send(LinkC_Socket *Socket, void *Message, size_t Length, int Flag){
    return sendto(Socket->Sockfd, Message, Length, Flag, (struct sockaddr *)&(Socket->Addr),sizeof(struct sockaddr_in));   //  发送UDP数据报
}


int ResendMessage(LinkC_Socket *Socket, void *Message, int Length){
    if(Socket == NULL){                         //  如果参数为空
        printf("Argument is NULL!\n");          //  打印出错信息
        return 1;                               //  返回错误
    }
    void *Package = malloc(Length + 2);         //  分配内存
    int Byte = 0;                               //  保存发送状态
    strncpy((char *)Package,RESEND_PACKAGE,2);  //  设置重发标志
    memcpy((char *)Package+2,Message,Length);   //  复制消息数据  
    Byte = __LinkC_Send(Socket,Package,Length+2,MSG_DONTWAIT); //  发送消息
    free(Package);
    return Byte;
}
