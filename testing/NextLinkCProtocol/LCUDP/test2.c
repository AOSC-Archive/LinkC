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
    char Buffer[512];
    LinkC_Socket *Socket = NULL;
    IsSocketInList(SendSock,&Socket);

    FILE *file;
    if((file=fopen("./test_pub.key","r"))==NULL){
        perror("open key file error");
        return -1;
    }   
    if((Socket->PublicKey=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        DestroySocketList();
        return -1;
    }   
    if((file=fopen("./test.key","r"))==NULL){
        perror("open key file error");
        return -1;
    }   
    if((Socket->PrivateKey=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        DestroySocketList();
        return -1;
    }
    Socket->IsSecurity = 1;



    int Length = PackMessage(ch,6,Socket,Buffer);
    printf("Length == %d\n",Length);
    printf("Sockfd = %d\n",SendSock);
    char c;
    while(1){
        /*c = getchar();
        if(c != 'c')   continue;
        __LinkC_Send(Socket,Buffer,Length,0);*/
        Connect(SendSock,addr);
        sleep(100);
    }
    DestroySocketList();
    return 0;
}
