#include "linkc_socket.h"
#include "linkc_def.h"
#include "linkc_client.h"
#include "linkc_TCP_io.h"
#include "linkc_error.h"
#include "linkc_package_ctl.h"
#include <sys/socket.h>

TCP_Socket::TCP_Socket(QObject *parent) :
    QObject(parent){
    Package = new char[STD_PACKAGE_SIZE];
    Buffer  = new char[STD_PACKAGE_SIZE];
    Sockfd  = 0;
    bzero((void*)&DestAddr,sizeof(struct sockaddr_in));
    if ((Sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0){ // 创建套接字[网络句柄]
        perror ("Socket");                      // 打印出错信息
        return;
    }
}

TCP_Socket::~TCP_Socket(){
    delete  (char*)Package;
    delete  (char*)Buffer;
}

void TCP_Socket::SetDestAddr(sockaddr_in Addr){
    memcpy((void*)&DestAddr,(void*)&Addr,sizeof(struct sockaddr_in));
}

int TCP_Socket::Connect(){
    return ConnectToServerWithAddr(Sockfd,DestAddr);
}

int TCP_Socket::Send(void *Message, size_t Length, int Flag){
    return send(Sockfd,Message,Length,Flag);
}

int TCP_Socket::Recv(void *Message, size_t MaxBuf, int Flag){
    int Status = TCP_Recv(Sockfd,Message,MaxBuf,Flag);
    return Status;
}


UDP_Socket::UDP_Socket(QObject *parent) :
    QObject(parent){
    Package = new char[STD_PACKAGE_SIZE];
    Buffer  = new char[STD_PACKAGE_SIZE];
    Sockfd  = 0;
    bzero((void*)&DestAddr,sizeof(struct sockaddr_in));
    Sockfd = CreateSocket();
    if (Sockfd < 0){         // 创建套接字[网络句柄]
        perror ("CreateSocket[UDP]");           // 打印出错信息
        return;
    }
}

UDP_Socket::~UDP_Socket(){
    delete  (char*)Package;
    delete  (char*)Buffer;
}

void UDP_Socket::SetDestAddr(sockaddr_in Addr){
    memcpy((void*)&DestAddr,(void*)&Addr,sizeof(struct sockaddr_in));
}

int UDP_Socket::Send(void *Message, size_t Length, int Flag){
    return SendMessage(Sockfd,Message,Length,Flag);
}

int UDP_Socket::Recv(void *Buffer, size_t MaxBuf, int Flag){
    return RecvMessage(Sockfd,Buffer,MaxBuf,Flag);
}

int UDP_Socket::GetSockfd(){
    return Sockfd;
}

int UDP_Socket::DoConnect(){
    int Status;
    if(IsSocketInList(this->GetSockfd(),NULL) == 0){   // 如果没有找到
        Status = AddSocketToList(this->GetSockfd());
        DelSocketFromList(this->GetSockfd());
        return Status;
    }
    return Connect(this->GetSockfd(),DestAddr);
}

int UDP_Socket::DoAccept(){
    int Status;
    if(IsSocketInList(this->GetSockfd(),NULL) == 0){   // 如果没有找到
        Status = AddSocketToList(this->GetSockfd());
        DelSocketFromList(this->GetSockfd());
        return Status;
    }
    return Accept(this->GetSockfd(),DestAddr);
}

int UDP_Socket::DoP2PConnect(uint32_t IP32){
    struct sockaddr_in NetAddr;
    P2PInfo Info;
    memset((void*)&NetAddr,0,sizeof(struct sockaddr_in));
    NetAddr.sin_family  = AF_INET;
    NetAddr.sin_port    = htons(2342);
    inet_aton("127.0.0.1",(struct in_addr*)&NetAddr.sin_addr.s_addr);
    socklen_t len = sizeof(struct sockaddr_in);
    sendto(Sockfd,(void*)&IP32,4,0,(struct sockaddr *)&NetAddr,len);
    recvfrom(Sockfd,this->Package,STD_PACKAGE_SIZE,0,(struct sockaddr*)&(Info.Dest),&len);
    _UnPackage(Package,sizeof(Info),(void*)&Info);
    this->SetDestAddr(Info.Dest);
    if(Info.is_server == 1){
        if(IsSocketInList(this->Sockfd,NULL) == 0){
            AddSocketToList(this->Sockfd);
        }
        LinkC_Debug("You are server!",LINKC_DEBUG);
        return P2PAccept(this->Sockfd,DestAddr,NULL,NULL);
    }else{
        if(IsSocketInList(this->Sockfd,NULL) == 0){
            AddSocketToList(this->Sockfd);
        }
        LinkC_Debug("You are client!",LINKC_DEBUG);
        return P2PConnect(this->Sockfd,DestAddr);
    }
    return LINKC_FAILURE;
}
