#include "Protocol/LinkC_NetWorkProtocol.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

int main(){
    InitLCUDPEnvironment();
    struct sockaddr_in addr;
    addr.sin_port = htons(1024);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int Sockfd;
    Sockfd = CreateSocket((struct sockaddr *)&(addr));
    Sockfd = CreateSocket((struct sockaddr *)&(addr));
    while(1)sleep(10);
    DestroySocketList();
    return 0;
}
