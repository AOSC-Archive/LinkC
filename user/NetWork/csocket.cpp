//#define _LINUX_		1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*  ------------- socket_c ---------- */

socket_c::socket_c(){
    memset((void *)&server_addr,'0',sizeof(server_addr));
    addr_len = sizeof(struct sockaddr_in);
    IP = 0;
    Port = 0;
    Sockfd = 0;
    type = -1;
}

socket_c::~socket_c(){

}

int socket_c::build_socket(int _Flage){
    switch (_Flage)
    {
        case TCP :
        {
            if ((Sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0){
                perror ("Socket");
            }
            type = TCP;
            break;
        }
        case UDP :
        {
            if ((Sockfd = socket (AF_INET,SOCK_DGRAM, 0)) < 0){
                perror ("Socket");
            }
            type = UDP;
            break;
            }
        return -1;
        }
    return 0;
}

int socket_c::start_connect(void){
    if(type == UDP){
        printf("UDP Socket Can not Face Connection!\n");
        return -1;
    }
    if (IP == 0){
        printf ("IP Has Not Set Yet!\n");
        return -1;
    }
    if (Port == 0){
        printf ("Port Has Not Set Yet!\n");
        return -1;
    }
    server_addr.sin_family=AF_INET;
    if (connect(Sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
        perror("Connect");
        Debug_Csocket_Sockfd();
        Sockfd = 0;
        return -1;
    }
    return 0;
}

void socket_c::Set_IP(ip_t ip){
    server_addr.sin_addr.s_addr = ip;
    IP = ip;
}

void socket_c::Set_IP(const char *Address){
    struct hostent *hp;
    hp=gethostbyname(Address);
    if(hp==NULL){
        printf("Get Host By Name Error\n");
    }
    memcpy (&server_addr.sin_addr.s_addr,hp->h_addr,4);
    IP=server_addr.sin_addr.s_addr;
}

void socket_c::Set_Port(char* port){
    Port = atoi(port);
}

void socket_c::Set_Port(port_t port){
    Port = port;
    server_addr.sin_port=htons(Port);
}



void socket_c::Debug_Csocket_IP(void){
    printf ("Debug >> IP\t\t= [%s]\n",inet_ntoa(server_addr.sin_addr));
}
void socket_c::Debug_Csocket_Port(){
    printf ("Debug >> Port\t\t= [%d]\n",Port);
}

void socket_c::Debug_Csocket_Sockfd(void){
    printf ("Debug >> Socket\t\t= [%d]\n",Sockfd);
}


int socket_c::Recv_msg(void* Buffer,int Flag){
    if(type == TCP)
        if (recv (Sockfd,Buffer,MAXBUF,Flag) < 0){
            perror("TCP_Recv");
            return -1;
        }
    if(type == UDP)
        if (recvfrom (Sockfd,Buffer,MAXBUF,Flag,(struct sockaddr *)&server_addr,&addr_len) < 0){
            perror("UDP_Recv");
        return -1;
    }
    return 0;
}

int socket_c::Recv_msg(void* Buffer,int maxbuf,int Flag){
    if(type == TCP)
        if (recv (Sockfd,Buffer,maxbuf,Flag) < 0){
            perror("TCP_Recv");
            return -1;
        }
    if(type == UDP)
        if (recvfrom (Sockfd,Buffer,maxbuf,Flag,(struct sockaddr *)&server_addr,&addr_len) < 0){
            perror("UDP_Recv");
        return -1;
    }
    return 0;
}

int socket_c::Send_msg(const char* Message,int Flag){
    if (type == TCP)
        if (send (Sockfd,Message,MAXBUF,Flag) < 0){
            perror("TCP_Send");
            return -1;
        }
    if (type == UDP)
        if (sendto (Sockfd,Message,MAXBUF,Flag,(struct sockaddr *)&server_addr,addr_len) < 0){
            perror("UDP_Send");
            return -1;
        }
    return 0;
}

int socket_c::Send_msg(const void* Message,int Flag){
    if (type == TCP)
        if (send (Sockfd,Message,MAXBUF,Flag) < 0){
            perror("Send");
            return -1;
        }
    if (type == UDP)
        if (sendto (Sockfd,Message,MAXBUF,Flag,(struct sockaddr *)&server_addr,addr_len) < 0){
            perror("UDP_Send");
            return -1;
        }
    return 0;
}

int socket_c::Send_msg(const void* Message,int maxbuf,int Flag){
    if (type == TCP)
        if (send (Sockfd,Message,maxbuf,Flag) < 0){
            perror("TCP_Send");
            return -1;
        }
    if (type == UDP)
        if (sendto (Sockfd,Message,maxbuf,Flag,(struct sockaddr *)&server_addr,addr_len) < 0){
            perror("UDP_Send");
            return -1;
        }
    return 0;
}


/* -------------TCP_Socket ------------ */
TCP_csocket::TCP_csocket(){
    build_socket(TCP);
}



void TCP_csocket::cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}

TCP_csocket::~TCP_csocket(){

}

/* --------------UDP_Socket -------------*/
UDP_csocket::UDP_csocket(){
    build_socket(UDP);
}
