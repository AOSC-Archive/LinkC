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


#ifndef MAXBUF
#define MAXBUF 1024
#endif

TCP_csocket::TCP_csocket(){
    IP = 0;
    Port = 0;
    Sockfd = 0;
    memset(&server_addr,0,sizeof(server_addr));
}


void TCP_csocket::Set_IP(const char *Address){
    struct hostent *hp;
    hp=gethostbyname(Address);
    if(hp==NULL){
        printf("Get Host By Name Error\n");
    }
    memcpy (&server_addr.sin_addr.s_addr,hp->h_addr,4);
    IP=server_addr.sin_addr.s_addr;
}

void TCP_csocket::Set_IP(ip_t ip){
    server_addr.sin_addr.s_addr = ip;
    IP = ip;
}

int TCP_csocket::ConnectToServer(void){
    if (IP == 0){
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
    server_addr.sin_family=AF_INET;
    if (connect(Sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1){
        perror("Connect");
        Debug_Csocket_Sockfd();
        Sockfd = 0;
        return -1;
    }
    return 0;
}

void TCP_csocket::Set_Port(char* port){
    Port = atoi(port);
}

void TCP_csocket::Set_Port(port_t port){
    Port = port;
    server_addr.sin_port=htons(Port);
}

int TCP_csocket::Recv_msg(void* Buffer,int Flag){
    if (recv (Sockfd,Buffer,MAXBUF,Flag) < 0){
        printf ("Recv Error!\n");
        return -1;
    }
    return 0;
}

int TCP_csocket::Send_msg(const char* Message,int Flag){
    if (send (Sockfd,Message,MAXBUF,Flag) < 0){
        printf ("Send Error!\n");
        return -1;
    }
    return 0;
}
int TCP_csocket::Send_msg(const void* Message,int Flag){
    if (send (Sockfd,Message,MAXBUF,Flag) < 0){
        printf ("Send Error!\n");
        return -1;
    }
    return 0;
}

int TCP_csocket::Send_msg(const void* Message,int maxbuf,int Flag){
    if (send (Sockfd,Message,maxbuf,Flag) < 0){
        printf ("Send Error!\n");
        return -1;
    }
    return 0;
}


void TCP_csocket::cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}

TCP_csocket::~TCP_csocket(){

}

//Debug Functions
void TCP_csocket::Debug_Csocket_IP(void){
    printf ("Debug >> IP\t\t= [%s]\n",inet_ntoa(server_addr.sin_addr));
}
void TCP_csocket::Debug_Csocket_Port(){
    printf ("Debug >> Port\t\t= [%d]\n",Port);
}

void TCP_csocket::Debug_Csocket_Sockfd(void){
    printf ("Debug >> Socket\t\t= [%d]\n",Sockfd);
}
