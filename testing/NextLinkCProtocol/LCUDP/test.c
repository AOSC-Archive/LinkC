#include "Protocol/LinkC_NetWorkProtocol.h"
#include "Package/PackageCtl.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    InitLCUDPEnvironment();
    struct sockaddr_in addr;
    addr.sin_port = htons(2341);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int RecvSock;
    RecvSock = CreateSocket((struct sockaddr *)&(addr));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(2342);
    SetDestAddr(RecvSock,addr);
    LinkC_Socket *Socket = NULL;
    IsSocketInList(RecvSock,&Socket);
    printf("Sockfd = %d\n",RecvSock);
    char Buf[512];
    int Byte;
    while(1){
        Byte = RecvMessage(RecvSock,Buf,512,0);
        if(Byte > 0)
            printf("I Recved %s\n",(char *)Buf+8);
    }
    DestroySocketList();
    return 0;
}
