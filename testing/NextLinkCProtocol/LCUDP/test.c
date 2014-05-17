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
    int SendSock;
    SendSock = CreateSocket((struct sockaddr *)&(addr));
    addr.sin_port = htons(2342);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int RecvSock;
    RecvSock = CreateSocket((struct sockaddr *)&(addr));
    char send[1024];
    int Length = PackMessage(NULL,0,send);
    printf("Length = %d\n",Length);
    while(1){
        sleep(2);
        SendMessage(SendSock,send,Length,0);
        printf("Send\n");
    };
    DestroySocketList();
    return 0;
}
