//#define _LINUX_		1
#include <stdio.h>
#include <stdlib.h>
#include "csocket.h"
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../include/data_type.h"
#if _LINUX_
#include<netdb.h>
#elif _WIN32_
#include <ws2tcpip.h>
#endif

#define _LINUX_
#define MAXBUF 1024

void csocket::Set_IP(const char *Address)
{
    struct hostent *hp;
    hp=gethostbyname(Address);
    memcpy (&server.sin_addr.s_addr,hp->h_addr,4);
    IP = new char[strlen(inet_ntoa(server.sin_addr))];
    memcpy (IP,inet_ntoa(server.sin_addr),strlen(inet_ntoa(server.sin_addr)));
}

int csocket::ConnectToServer(void){
    if (IP == NULL){
        printf ("IP Has Not Set Yet!\n");
        return -1;
    }
    if (Port == 0){
        printf ("Port Has Not Set Yet!\n");
        return -1;
    }
    if ((Sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0){
            perror ("Socket");
            exit (EXIT_FAILURE);
    }
    server.sin_family=AF_INET;
    if (inet_aton(IP,(struct in_addr *)&server.sin_addr.s_addr) == 0){
        printf ("Wrong\n");
        return 0;
    }
    if (connect(Sockfd,(struct sockaddr *)&server,sizeof(server)) == -1){
        perror("Connect");
        Debug_Csocket_Sockfd();
        Sockfd = 0;
        return 0;
    }
    return 1;
}

void csocket::Set_Port(char* port){
    Port = atoi(port);
}

void csocket::Set_Port(int port){
    Port = port;
    server.sin_port=htons(Port);
}

int csocket::Recv_msg(void* Buffer,int Flag){
    if (recv (Sockfd,Buffer,MAXBUF,Flag) < 0){
        printf ("Recv Error!\n");
        return -1;
    }
    return 0;
}

int csocket::Send_msg(const char* Message,int Flag){
    if (send (Sockfd,Message,MAXBUF,Flag) < 0){
        printf ("Send Error!\n");
        return -1;
    }
    return 0;
}
int csocket::Send_msg(const void* Message,int Flag){
    if (send (Sockfd,Message,MAXBUF,Flag) < 0){
        printf ("Send Error!\n");
        return -1;
    }
    return 0;
}


void csocket::cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}

csocket::~csocket(){
    delete IP;

}

//Debug Functions
void csocket::Debug_Csocket_IP(void){
    printf ("Debug >> IP\t\t= [%s]\n",IP);
}
void csocket::Debug_Csocket_Port(){
    printf ("Debug >> Port\t\t= [%d]\n",Port);
}

void csocket::Debug_Csocket_Sockfd(void){
    printf ("Debug >> Socket\t\t= [%d]\n",Sockfd);
}
