#include "linkc_socket.h"
#include "linkc_def.h"
#include "linkc_client.h"
#include "linkc_TCP_io.h"
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
