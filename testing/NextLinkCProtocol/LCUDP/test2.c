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
    addr.sin_port = htons(2342);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int SendSock;
    SendSock = CreateSocket((struct sockaddr *)&(addr));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(2341);
    SetDestAddr(SendSock,addr);
    char ch[16] = "Hello!";
    socklen_t len = sizeof(struct sockaddr_in);
    while(1){
        sleep(2);
        sendto(SendSock,ch,6,0,(struct sockaddr *)&addr,len);
        perror(":");
    }
    DestroySocketList();
    return 0;
}
