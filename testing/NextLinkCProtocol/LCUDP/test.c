#include "Protocol/LinkC_NetWorkProtocol.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    InitLCUDPEnvironment();
    struct sockaddr_in addr;
    addr.sin_port = htons(2341);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int SendSock;
    SendSock = CreateSocket((struct sockaddr *)&(addr));
    addr.sin_port = htons(2342);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int RecvSock;
    RecvSock = CreateSocket((struct sockaddr *)&(addr));
    socklen_t len = sizeof(addr);
    char buffer[16];
    while(1){
        sleep(10);
        sendto(SendSock,"...",4,0,(struct sockaddr*)&addr,len);
        printf("Send\n");
        recvfrom(RecvSock,buffer,16,0,NULL,&len);
        printf("%s\n",buffer);
    };
    DestroySocketList();
    return 0;
}
