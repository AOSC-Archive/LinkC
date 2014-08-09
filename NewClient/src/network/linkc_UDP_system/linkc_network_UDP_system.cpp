#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "../../../include/linkc_UDP_system/linkc_netowrk_UDP_system.h"
#include "../../../include/linkc_UDP_system/linkc_package_list.h"
#include "../../../include/linkc_package_ctl.h"
#include "../../../include/linkc_package.h"
#include "../../../include/linkc_error.h"
#include "../../../include/linkc_def.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>                 //  For 信号控制
#include <unistd.h>                 //  For 时钟信号
#include <pthread.h>
#include <fcntl.h>          /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <semaphore.h>

int     DeepDebug = 0;

SocketList *List = NULL;            //  全局变量，套接字的链表

void TimerInt(int SigNo, siginfo_t* SigInfo , void* Arg){
    if(!SigNo)   perror("SigNo");
    if(!SigInfo) perror("SigInfo");
    if(!Arg)     perror("Arg");
    PackageListNode *PackNode   = NULL;                             //  缓冲区链表节点指针
    SocketListNode  *Node       = List->StartNode;                  //  赋值为开始节点

    while(Node){                                                    //  循环直到当前节点为空
        if(Node->Socket->ErrorMessage != NULL){                     //  如果错误信息不为空
            LinkC_Debug(Node->Socket->ErrorMessage,LINKC_FAILURE);
            free(Node->Socket->ErrorMessage);                       //  释放内存
            Node->Socket->ErrorMessage = NULL;                      //  挂空指针
        }
        if(Node->Socket->DoRecvCheck == 0){
            Node = Node->Next;
            continue;              //  如果没有开启接受检验，则跳过
        }
        if(Node->Socket->SendList->TotalNode != 0){                 //  如果发送链表中还有剩余[也就是收到确认没有到达]
            pthread_mutex_lock(Node->Socket->SendList->MutexLock);  //  给链表上锁上锁
            PackNode = Node->Socket->SendList->StartNode;           //  设置为链表开始节点
            while(PackNode){
                if(PackNode->ResendTime > MAX_RESEND_TIME){         //  如果大于最大重发次数
                    LinkC_Debug("TimerInt:断线",LINKC_WARNING);
                    //  执行断线操作
                }else if(PackNode->TimeToLive == 0){                //  在没有断线的情况下如果当前数据剩迟迟没有收到收到确认
                    LinkC_Debug("TimerInt:数据重发",LINKC_DEBUG);
                    ResendMessage(Node->Socket,PackNode->Package ,MSG_DONTWAIT);    //  重发消息
                    PackNode->TimeToLive = MAX_TIME_TO_LIVE;        //  重设剩余生存时间
                    PackNode->ResendTime ++;                        //  重发次数自增加一
                }else{
                    PackNode->TimeToLive --;                        //  剩余生存时间减一
                }
                PackNode = PackNode->Next;                          //  跳转到下一个节点
            }
            pthread_mutex_unlock(Node->Socket->SendList->MutexLock);//  给链表解锁
            PackNode = NULL;                                        //  挂空指针
        }
        Node = Node->Next;
    }
    alarm(1);           //  1秒后发射信号
    return;
}

void IOReadyInt(int SigNo, siginfo_t *SigInfo, void *Arg){
    if(!SigNo)   perror("SigNo");
    if(!Arg)     perror("Arg");
    SocketListNode  *Node       = List->StartNode;                      //  赋值为开始节点
    while(Node){
        if(Node->Socket->Sockfd == SigInfo->si_fd){
            if(__LinkC_Recv(Node->Socket,Node->Socket->RecvBuffer,STD_BUFFER_SIZE,MSG_DONTWAIT)>0){
                return;
            }
        }
        Node = Node->Next;
    }
    return;
}

int AskForResend(LinkC_Socket *Socket, int Count){
    ConfirmationMessage confirm;                                        //  数据结构
    confirm.isRecved    = 0;                                            //  说明没有收到
    confirm.Count       = Count;                                        //  说明哪个包没有收到
    int Length = LCUDP_Package((void *)&confirm,sizeof(confirm),Socket,Socket->SendBuffer);  //  打包数据
    ___LinkC_Send(Socket,Socket->SendBuffer,Length,MSG_DONTWAIT);                   //  发送数据
    return 0;                                                           //  返回成功
}
int ConfirmRecved(LinkC_Socket *Socket, int Count){
    ((PackageHeader*)Socket->SendBuffer)->MessageType       = CONFIRMATION_MESSAGE;
    ((PackageHeader*)Socket->SendBuffer)->MessageLength     = sizeof(ConfirmationMessage);
    ((PackageHeader*)Socket->SendBuffer)->ProtocolVersion   = PROTOCOL_VERSION;
    ((PackageHeader*)Socket->SendBuffer)->MessageCounts     = 0;
    ((ConfirmationMessage*)((char*)(Socket->SendBuffer)+8))->isRecved = 1;
    ((ConfirmationMessage*)((char*)(Socket->SendBuffer)+8))->Count = Count;
    return ___LinkC_Send(Socket,Socket->SendBuffer,sizeof(ConfirmationMessage)+8,0); // 
}

int Connect (int Sockfd, struct sockaddr_in Dest){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("Connect:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket)==0){
        LinkC_Debug("Connect:没有这个套接字",LINKC_FAILURE);
        return -1;
    }
    EmptyPackageList(Socket->RecvList);         //  清空发送和接收缓冲区
    EmptyPackageList(Socket->SendList); 
    SetDestAddr(Sockfd,Dest);
    int Length = _LCUDP_Package(NULL,0,Socket,CONNECTION_MESSAGE,Socket->SendBuffer);
    if(Length < 0){
        LinkC_Debug("Connect:打包数据失败",LINKC_FAILURE);
        return -1;
    }
    _LinkC_Send(Socket,Socket->SendBuffer,Length,0);
    LinkC_Debug("Connect:已发出请求",LINKC_DEBUG);
    _LinkC_Recv(Socket,Socket->RecvBuffer,512,0);
    LinkC_Debug("Connect:正在确认请求",LINKC_DEBUG);
    if(((PackageHeader*)Socket->RecvBuffer)->MessageType != CONNECTION_MESSAGE){
        LinkC_Debug("Connect:消息头不正确",LINKC_FAILURE);
        return -1;
    }
    LinkC_Debug("Connect:已连接上",LINKC_DEBUG);
    return 0;
}
int Accept(int Sockfd, struct sockaddr_in Dest){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("Accept:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket)==0){
        LinkC_Debug("Accept:没有这个套接字",LINKC_FAILURE);
        return -1;
    }
    EmptyPackageList(Socket->RecvList);         //  清空发送和接收缓冲区
    EmptyPackageList(Socket->SendList); 
    SetDestAddr(Sockfd,Dest);
    LinkC_Debug("Accept:等待对方发起请求",LINKC_DEBUG);
    _LinkC_Recv(Socket,Socket->RecvBuffer,512,0);
    LinkC_Debug("Accept:已收到请求",LINKC_DEBUG);
    int Length = _LCUDP_Package(NULL,0,Socket,CONNECTION_MESSAGE,Socket->SendBuffer);
    if(Length < 0){
        LinkC_Debug("Accept:打包数据失败",LINKC_FAILURE);
        return -1;
    }
    _LinkC_Send(Socket,Socket->SendBuffer,Length,0);
    LinkC_Debug("Accept:已发出确认",LINKC_DEBUG);
    return 0;
}
int P2PConnect(int Sockfd, struct sockaddr_in Dest){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("P2PConnect:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket)==0){
        LinkC_Debug("P2PConnect:没有这个套接字",LINKC_FAILURE);
        return -1;
    }
    EmptyPackageList(Socket->RecvList);         //  清空发送和接收缓冲区
    EmptyPackageList(Socket->SendList); 
    SetDestAddr(Sockfd,Dest);
    EnableRecvCheck(Sockfd,0);                  //  禁用收到回复
    int Length = _LCUDP_Package(NULL,0,Socket,CONNECTION_MESSAGE,Socket->SendBuffer);
    _LinkC_Send(Socket,Socket->SendBuffer,Length,0);
    LinkC_Debug("P2PConnect:已发出请求",LINKC_DEBUG);
    _LinkC_Recv(Socket,Socket->RecvBuffer,512,0);
    LinkC_Debug("P2PConnect:正在确认请求",LINKC_DEBUG);
    if(((PackageHeader*)Socket->RecvBuffer)->MessageType != CONNECTION_MESSAGE){
        LinkC_Debug("P2PConnect:消息头不正确",LINKC_FAILURE);
        return -1;
    }
    LinkC_Debug("P2PConnect:已连接上",LINKC_DEBUG);
    EnableRecvCheck(Sockfd,0);                  //  启动收到回复
    return 0;
}

int P2PAccept(int Sockfd, struct sockaddr_in Dest, void(*Function) (void*), void* Arg){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("P2PAccept:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket)==0){
        LinkC_Debug("P2PAccept:没有这个套接字",LINKC_FAILURE);
        return -1;
    }
    EmptyPackageList(Socket->RecvList);         //  清空发送和接收缓冲区
    EmptyPackageList(Socket->SendList); 
    SetDestAddr(Sockfd,Dest);
    EnableRecvCheck(Sockfd,0);
    bzero(Socket->SendBuffer,32);
    int Length = _Package(NULL,0,HEART_BEATS,Socket->SendBuffer);
    ___LinkC_Send(Socket,Socket->SendBuffer,Length,MSG_DONTWAIT);
    if(Function != NULL){                                   //  执行函数，这里是我发送了无用信息后向服务端确认
        Function(Arg);
    }
    LinkC_Debug("P2PAccept:等待对方发起请求",LINKC_DEBUG);
    _LinkC_Recv(Socket,Socket->RecvBuffer,512,0);
    LinkC_Debug("P2PAccept:已收到请求",LINKC_DEBUG);
    Length = _LCUDP_Package(NULL,0,Socket,CONNECTION_MESSAGE,Socket->SendBuffer);
    if(Length < 0){
        LinkC_Debug("P2PAccept:打包数据失败",LINKC_FAILURE);
        return -1;
    }
    _LinkC_Send(Socket,Socket->SendBuffer,Length,0);
    LinkC_Debug("P2PAccept:已发出确认",LINKC_DEBUG);
    LinkC_Debug("P2PAccept:已连接上",LINKC_DEBUG);
    EnableRecvCheck(Sockfd,1);

    return 0;
}

int SendMessage(int Sockfd, void *Message, size_t Length, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("SendMessage:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket) == 0){
        return -1;
    }
    return _LinkC_Send(Socket,Message,Length,Flag);
}
int RecvMessage(int Sockfd, void *Buffers, size_t MaxBuf, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("RecvMessage:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    int Byte;
    LinkC_Socket *Socket = NULL;
    if(IsSocketInList(Sockfd,&Socket) == 0){
        LinkC_Debug("套接字不在链表中",LINKC_WARNING);
        return LINKC_FAILURE;
    }
    Byte = _LinkC_Recv(Socket,Buffers,MaxBuf,Flag);
    return Byte;
}

int _LinkC_Send(LinkC_Socket *Socket, void *Message, size_t size, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("_LinkC_Send:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    return __LinkC_Send(Socket,Message,size,Flag);
}

int _LinkC_Recv(LinkC_Socket *Socket, void *Message, size_t size, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("_LinkC_Recv:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    int Result=0;
    if(Flag == MSG_DONTWAIT)
        Result = sem_trywait(Socket->RecvList->Semaphore);             //  非阻塞请求数据
    else{
        Result = sem_wait(Socket->RecvList->Semaphore);             //  阻塞请求数据
    }
    if(Result < 0){
        perror("Sem Wait[Trywait]");
        return 0;
    }
    pthread_mutex_lock(Socket->RecvList->MutexLock);                //  上互斥锁
    if(Socket->RecvList->TotalNode <= 0){
        printf("出现错误[file = %s\tline = %d]\n",__FILE__,__LINE__);
        pthread_mutex_unlock(Socket->RecvList->MutexLock);          //  解锁
        return -1;
    }
    PackageListNode *Node = Socket->RecvList->StartNode;
    while(Node->Next)   Node = Node->Next;                          //  跳转到最后一个Node
    uint16_t Length = ntohs(Node->MessageLength)+8;
    if(Length > size){
        LinkC_Debug("_LinkC_Recv:传入缓冲区过小!",LINKC_FAILURE);
        printf("Size = %d\n",Length);
        sem_post(Socket->RecvList->Semaphore);
        pthread_mutex_unlock(Socket->RecvList->MutexLock);
        return -1;
    }
    memcpy(Message,Node->Package,Length);
    if(Flag == MSG_PEEK){
        pthread_mutex_unlock(Socket->RecvList->MutexLock);
        sem_post(Socket->RecvList->Semaphore);
    }else{
        if(Socket->DoRecvCheck == 1){
            RemovePackageListNode(Socket->RecvList,((PackageHeader*)Message)->MessageCounts);
        }
        pthread_mutex_unlock(Socket->RecvList->MutexLock);
    }
    return Length   ;
}

int __LinkC_Send(LinkC_Socket *Socket, void *Message, size_t size, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("__LinkC_Send:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    if(Socket->DoRecvCheck == 1){
        ((PackageHeader *)Message)->MessageCounts = Socket->SendList->NowCount+1;               //  将本数据包的计数次设置为之前发送的数据包总数加一
        if(InsertPackageListNode(Socket->SendList,Message,Socket->SendList->NowCount +1) != 0){ //  将本数据包存入发送缓冲区失败
            return -1;
        }else{
            InsertPackageListNode(Socket->SendList,Message,Socket->RecvList->NowCount+1);       //  插入已经收到的消息
        }
    }else{
        return ___LinkC_Send(Socket,Message,size,Flag);
    }
    Socket->SendList->NowCount ++;                                                          //  发送缓冲区总计数自增加一
    return ___LinkC_Send(Socket,Message,size,Flag);
}

int __LinkC_Recv(LinkC_Socket *Socket, void *Message, size_t size, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("__LinkC_Recv:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    int Byte;
    if(size < 8){
        LinkC_Debug("__LinkC_Recv:传入缓冲区过小",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    Byte = ___LinkC_Recv(Socket,Message,8,MSG_PEEK);
    uint16_t Length;
    if(Byte < 8){                                                               //  recvfrom返回收到数据长度小于8[出错或者无数据]
        if(Byte == 0){                                                          //  recvfrom返回收到数据长度为0[断开链接或者无数据]
            LinkC_Debug("__LinkC_Recv:未收到数据",LINKC_WARNING);
            return 0;                                                           //  返回0
        }else if(Byte < 0){                                                     //  recvfrom返回负数[出错]
            LinkC_Debug("__LinkC_Recv:错误",LINKC_FAILURE);
            perror("__LinkC_Recv");                                             //  打印错误细信息
            return -1;                                                          //  返回错误
        }else{                                                                  //  recvfrom返回收到数据长度介于0到8之间[残缺数据包，舍弃]
            LinkC_Debug("__LinkC_Recv:未收到合法数据",LINKC_WARNING);
            return -1;                                                          //  返回错误
        }
    }else{                                                                      //  recvfrom返回收到数据长度为8[可能为一个消息头]
        Length = ntohs(((PackageHeader*)Message)->MessageLength) + 8;
        if(((PackageHeader*)Message)->ProtocolVersion != PROTOCOL_VERSION){     //  如果协议版本号不一致
            printf("Version = %d\nMy Version = %d\n",((PackageHeader*)Message)->ProtocolVersion,PROTOCOL_VERSION);
            LinkC_Debug("协议版本不一致",LINKC_WARNING);
            ___LinkC_Recv(Socket,(char *)Message,STD_BUFFER_SIZE,Flag);         //  清除数据
            return -1;                                                          //  返回错误
        }
        if(((PackageHeader*)Message)->MessageType == HEART_BEATS){              //  若是心跳包
            ___LinkC_Recv(Socket,(char *)Message,Length,Flag);                  //  把数据从接收缓冲区提出
            return 0;                                                           //  直接返回[忽略]
        }else if(((PackageHeader*)Message)->MessageType == RESEND_MESSAGE){     //  若是重发的数据包
            if(___LinkC_Recv(Socket,(char *)Message,Length,Flag) < 0){          // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return -1;                                                      //  返回无数据
            }
            if(Socket->DoRecvCheck == 1){
                ConfirmRecved(Socket,((PackageHeader*)Message)->MessageCounts); //  发送确认收到消息
                InsertPackageListNode(Socket->RecvList,Message,((PackageHeader*)Message)->MessageCounts);       //  插入已经收到的消息
            }else{
                InsertPackageListNode(Socket->RecvList,Message,Socket->RecvList->NowCount+1);       //  插入已经收到的消息
            }
        }else if(((PackageHeader*)Message)->MessageType == SSL_KEY_MESSAGE){    //  如果是SSL密钥
            if(___LinkC_Recv(Socket,(char *)Message,Length,Flag) < 0){          // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv:错误",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return 0;                                                       //  返回无数据
            }
            if(Socket->DoRecvCheck == 1)
                ConfirmRecved(Socket,((PackageHeader*)Message)->MessageCounts); //  发送确认收到消息
            //      保存密钥
        }else if(((PackageHeader*)Message)->MessageType == CONFIRMATION_MESSAGE){
            if(___LinkC_Recv(Socket,Message,Length,Flag) < 0){                    // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv:错误",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return 0;                                                       //  返回无数据
            }
            if(((ConfirmationMessage*)(((char *)Message)+8))->isRecved == 0){       //  如果说对面没有收到指定数据包
                PackageListNode *Node = Socket->SendList->StartNode;
                while(Node){
                    if(Node->Count == ((ConfirmationMessage*)((char*)Message)+8)->Count){
                        int result = ResendMessage(Socket,Node->Package,MSG_DONTWAIT);
                        if(result != 0){
                            Node = NULL;
                            return -1;
                        }else{
                            Node->TimeToLive = MAX_TIME_TO_LIVE;
                            Node->ResendTime++;
                            Node = NULL;
                            return 0;
                        }
                    }
                    Node = Node->Next;
                }
                LinkC_Debug("__LinkC_Recv:重发数据",LINKC_FAILURE);
                Node = NULL;
                return -1;
            }else{                                                              //  如果对面收到了这个数据包
                pthread_mutex_lock(Socket->RecvList->MutexLock);
                RemovePackageListNode(Socket->SendList,((ConfirmationMessage*)(((char *)Message)+8))->Count);
                pthread_mutex_unlock(Socket->RecvList->MutexLock);
                return 0;
            }
        }else if(((PackageHeader*)Message)->MessageType == NORMAL_MESSAGE){     //  如果是普通数据
            if(___LinkC_Recv(Socket,(char *)Message,Length,Flag) <= 0){         // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return 0;                                                        //  返回无数据
            }
            if(Socket->DoRecvCheck == 1){
                ConfirmRecved(Socket,((PackageHeader*)Message)->MessageCounts); //  发送确认收到消息
                InsertPackageListNode(Socket->RecvList,Message,((PackageHeader*)Message)->MessageCounts);       //  插入已经收到的消息
            }else{
                InsertPackageListNode(Socket->RecvList,Message,Socket->RecvList->NowCount+1);       //  插入已经收到的消息
            }
        }else if(((PackageHeader*)Message)->MessageType == P2P_DATA){           //  如果是P2P数据
            if(___LinkC_Recv(Socket,(char *)Message,Length,Flag) <= 0){         // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return 0;                                                        //  返回无数据
            }
            if(Socket->DoRecvCheck == 1){
                ConfirmRecved(Socket,((PackageHeader*)Message)->MessageCounts); //  发送确认收到消息
                InsertPackageListNode(Socket->RecvList,Message,((PackageHeader*)Message)->MessageCounts);       //  插入已经收到的消息
            }else{
                InsertPackageListNode(Socket->RecvList,Message,Socket->RecvList->NowCount+1);       //  插入已经收到的消息
            }
        }else if(((PackageHeader*)Message)->MessageType == CONNECTION_MESSAGE){
            if(___LinkC_Recv(Socket,(char *)Message,Length,Flag) <= 0){         // 如果接收剩余数据失败
                LinkC_Debug("__LinkC_Recv",LINKC_FAILURE);
                if(Socket->DoRecvCheck == 1)
                    AskForResend(Socket,((PackageHeader*)Message)->MessageCounts);  //  请求重发
                return 0;                                                        //  返回无数据
            }
            if(Socket->DoRecvCheck == 1){
                ConfirmRecved(Socket,((PackageHeader*)Message)->MessageCounts); //  发送确认收到消息
                InsertPackageListNode(Socket->RecvList,Message,((PackageHeader*)Message)->MessageCounts);       //  插入已经收到的消息
            }else{
                InsertPackageListNode(Socket->RecvList,Message,Socket->RecvList->NowCount+1);       //  插入已经收到的消息
            }
        }else{
            LinkC_Debug("__LinkC_Recv:无法识别消息头",LINKC_WARNING);
            ___LinkC_Recv(Socket,(char *)Message,STD_BUFFER_SIZE,Flag);
            return 0;
        }
    }
    return Byte;
}


int ___LinkC_Send(LinkC_Socket *Socket, void *Message, size_t Length, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("___LinkC_Send:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    int Byte = 0;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGALRM);
    sigprocmask(SIG_SETMASK,&set,NULL);
    Byte =  sendto(Socket->Sockfd, Message, Length, Flag, (struct sockaddr *)&(Socket->Addr),sizeof(struct sockaddr_in));   //  发送UDP数据报
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    return Byte;
}


int ___LinkC_Recv(LinkC_Socket *Socket, void *Message, size_t size, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("___LinkC_Recv:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    int Byte = 0;                                                         //  保存本次接收的长度
    size_t Length = 0;                                                      //  保存已经接收的长度
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGALRM);
    sigprocmask(SIG_SETMASK,&set,NULL);
    while(1)
    {
        Byte = recvfrom(Socket->Sockfd,(char *)Message+Length,size-Length,Flag,(struct sockaddr*)&(Socket->Addr),&(Socket->SockLen));   //  接收数据
        if(Byte < 0){                                                       //  recvfrom返回小于等于0 [1]没有数据 [2]链接关闭 [3]出错
            perror("___LinkC_Recv");                                        //  打印错误信息
            sigprocmask(SIG_UNBLOCK,&set,NULL);
            return -1;                                                      //  返回出错
        }else if(Byte == 0){
            LinkC_Debug("___LinkC_Recv : 接收收据为0",LINKC_WARNING);
            sigprocmask(SIG_UNBLOCK,&set,NULL);
            return Length;                                                  //  返回已经收到的数据长度    
        }
        if(Flag == MSG_WAITALL){
            Length += Byte;                                                     //  设置当前总计接收的数据长度
            if(Length == size){                                                 //  正好接收到要接收的数据的长度
                break;                                                          //  跳出循环
            }
        }else{
            sigprocmask(SIG_UNBLOCK,&set,NULL);
            return Byte;
        }
    }
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    return Length;                                                          //  返回成功
}
int InitLCUDPEnvironment(void){
	    if(InitSocketList()!=0)                 //  如果初始化链表失败
            return 1;                           //  返回错误
	    struct sigaction Act;                   //  定义处理信号的参数集合
	    sigemptyset(&Act.sa_mask);              //  将数据清空
	    Act.sa_sigaction=IOReadyInt;            //  设置回调函数[上面第二个函数]
	    Act.sa_flags=SA_RESTART|SA_SIGINFO;                //  使用sa_sigaction参数的函数最为信号发来后的处理函数[也就是上面定义的]
        if(sigaction(SIGIO,&Act,NULL)==-1){     //  安装SIGIO信号
            perror("Signal");                   //  打印错误信息
            return 1;                           //  返回错误
        }
	    sigemptyset(&Act.sa_mask);              //  将数据清空
	    Act.sa_sigaction=TimerInt;              //  设置回调函数[上面第一个函数]
	    Act.sa_flags=SA_RESTART;                //  使用sa_sigaction参数的函数最为信号发来后的处理函数[也就是上面定义的]
        if(sigaction(SIGALRM,&Act,NULL)==-1){   //  安装SIGALRM信号
            perror("Signal");                   //  打印错误信息
            return 1;                           //  返回错误
        }
        
        alarm(1);                               //  1秒后发射信号
        return 0;                               //  返回成功
}

int InitSocketList(void){
    if(List != NULL){                                               //  如果链表为空
        LinkC_Debug("InitSocketList:LinkC Socket环境已经初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    List = (SocketList*)malloc(sizeof(SocketList)); //  为链表分配内存
    List->StartNode     = NULL;                     //  开始节点挂空
    List->TotalSocket   = 0;                        //  套接字总数为0
    return 0;                                       //  返回0
}

int IsSocketInList(int Sockfd, LinkC_Socket** Socket){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("IsSocketInList:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    SocketListNode *NowNode = List->StartNode;      //  新建一个节点，指向链表的开始节点
    while(NowNode){                                 //  循环[当前节点不为空]
        if(NowNode->Socket->Sockfd == Sockfd){      //  如果当前Socket等于传入的Socket
            if(Socket != NULL)
                *Socket = NowNode->Socket;
            return 1;                               //  返回找到
        }
        NowNode = NowNode->Next;                    //  设置为下一个节点
    }
    Socket = NULL;
    return 0;                                       //  返回未找到
}
int CreateSocket(void){
    int Sockfd          =   socket(AF_INET,SOCK_DGRAM,0);         //  创建UDP套接字
    if(Sockfd < 0){                                                     //  如果创建套接字失败
        perror("Create LCUDP");                                                 //  打印错误信息                                                          //  释放内存
        return 1;                                                               //  返回错误
    }
    return Sockfd;                                                      //  返回创建的套接子
}

int AddSocketToList(int Sockfd){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("AddSocketToList:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    if(IsSocketInList(Sockfd,NULL) == 1){   //  当前Socket是否已经存在于链表中，如果存在
        LinkC_Debug("AddSocketToList:重复添加",LINKC_FAILURE);
        return 1;
    }

    LinkC_Socket *Socket = (LinkC_Socket*)malloc(sizeof(LinkC_Socket));
    Socket->Sockfd = Sockfd;

    if(fcntl(Socket->Sockfd,F_SETOWN,getpid()) == -1){
        perror("Set Own");
        close(Socket->Sockfd);                                                  //  关闭套接字
        free(Socket);                                                           //  释放内存
        return 1;                                                               //  返回错误
    }
    int flag = fcntl(Socket->Sockfd,F_GETFL,0);                                 //  获得那啥FL?
    if(flag == -1){                                                             //  如果出错
        perror("Get FL");                                                       //  打印错误信息
        close(Socket->Sockfd);                                                  //  关闭套接字
        free(Socket);                                                           //  释放内存
        return 1;                                                               //  返回错误
    }
    if(fcntl(Socket->Sockfd,F_SETFL,flag | O_ASYNC) == -1){                     //  如果设置成受到数据就发信号的那啥套接字失败
        perror("Set FL");                                                       //  打印错误信息
        close(Socket->Sockfd);                                                  //  关闭套接字
        free(Socket);                                                           //  释放内存
        return 1;                                                               //  返回错误
    }
    if (fcntl(Socket->Sockfd,F_SETSIG,SIGIO) == -1){
        perror("fault");
    }
    /*if(bind(Socket->Sockfd,MyAddr,sizeof(struct sockaddr_in)) < 0){             //  绑定地址
        perror("Bind LCUDP");                                                   //  输出错误信息
        close(Socket->Sockfd);                                                  //  关闭套接字
        free(Socket);                                                           //  释放内存
        return 1;
    }*/


    Socket->Available       =   0;                                              //  将可用包数设置为0
    Socket->SendList        =   BuildPackageList();                             //  创建链表
    Socket->RecvList        =   BuildPackageList();                             //  创建链表
    Socket->RecvBuffer      =   malloc(STD_BUFFER_SIZE);                        //  建立接收缓冲区
    Socket->SendBuffer      =   malloc(STD_BUFFER_SIZE);                        //  建立发送缓冲区
    Socket->ErrorMessage    =   NULL;                                           //  挂空指针
    Socket->DoRecvCheck     =   1;                                              //  启动LCUDP功能


    SocketListNode* Node;
    Node            = (SocketListNode*)malloc(sizeof(SocketListNode));     //  分配内存
    Node->Mutex_Lock= (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(Node->Mutex_Lock,NULL);
    Node->Socket    = Socket;                       //  保存当前新建的Socket
    if(List->TotalSocket == 0){                     //  如果现在还没有Socket
        List->StartNode = Node;                     //  将当前节点设置为开始节点
        Node->Perv      = NULL;                     //  当前节点的前一个节点挂空
        Node->Next      = NULL;                     //  当前节点的后一个节点挂空
    }else{
        Node->Next              = List->StartNode;  //  新建节点的下一个设置成现在的起始节点
        Node->Perv              = NULL;             //  当前节点的前一个节点挂空
        List->StartNode->Perv   = Node;             //  链表起始节点的前一个为新建节点
        List->StartNode         = Node;             //  链表起始节点为当前节点
    }
    List->TotalSocket++;                            //  Socket总数加一
    return 0;                                       //  返回函数
}

int SetDestAddr(int Socket, struct sockaddr_in DestAddr){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("SetDestAddr:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    SocketListNode *Node = List->StartNode;
    while(Node){
        if(Node->Socket->Sockfd == Socket){
            pthread_mutex_lock(Node->Mutex_Lock);
            memcpy((void*)&(Node->Socket->Addr),(void *)&DestAddr,sizeof(struct sockaddr_in));
            if(Node->Socket->IsSecurity == 1){
                RSA_free(Node->Socket->PublicKey);
                RSA_free(Node->Socket->PrivateKey);
                Node->Socket->IsSecurity = 0;
            }
            pthread_mutex_unlock(Node->Mutex_Lock);
            return 0;
        }
        Node = Node->Next;
    }
    return -1;
}

int EnableRecvCheck(int Sockfd, int Enabled){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("SetDestAddr:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    LinkC_Socket *Socket;
    if(IsSocketInList(Sockfd,&Socket) == 0){
        LinkC_Debug("套接字不在链表中",LINKC_WARNING);
        return LINKC_FAILURE;
    }
    Socket->DoRecvCheck = Enabled;
    return LINKC_SUCCESS;
}

int DestroySocketList(){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("DestroySocketList:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
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
        free(NowNode->Socket->RecvBuffer);              //  
        free(NowNode->Socket->SendBuffer);              //  
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

int DelSocketFromList(int Socket){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("DeleteSocket:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
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
            free(NowNode->Socket->RecvBuffer);              //  
            free(NowNode->Socket->SendBuffer);              //  
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

int ResendMessage(LinkC_Socket *Socket, void *Message, int Flag){
    if(List == NULL){                                               //  如果链表为空
        LinkC_Debug("ResendMessage:LinkC Socket环境没有初始化",LINKC_FAILURE);
        return LINKC_FAILURE;                                       //  返回错误
    }
    if(Socket == NULL){                         //  如果参数为空
        printf("Argument is NULL!\n");          //  打印出错信息
        return -1;                              //  返回错误
    }
    int Byte = 0;                               //  保存发送状态
    ((PackageHeader*)Message)->MessageType  = RESEND_MESSAGE;   //  
    Byte = ___LinkC_Send(Socket,Message,((PackageHeader*)Message)->MessageLength,Flag); //  发送消息
    if(Byte < 0){
        perror("___LinkC_Send");
        return -1;
    }
    return Byte;
}
