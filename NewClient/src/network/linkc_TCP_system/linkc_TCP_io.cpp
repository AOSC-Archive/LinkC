#include "../../../include/linkc_TCP_system/linkc_TCP_io.h"
#include "../../../include/linkc_def.h"
#include "../../../include/linkc_error.h"
#include "../../../include/linkc_package.h"
#include "../../../include/linkc_client_nonui.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <curses.h>

int16_t TCP_Recv(int Sockfd, void *Out, int Out_size, int flag){
    PackageHeader Header;
    if(recv(Sockfd,(void*)&Header,sizeof(PackageHeader),MSG_PEEK) == LINKC_FAILURE)
        return LINKC_FAILURE;
    int PackageLength = ntohs(Header.MessageLength)+sizeof(PackageHeader);
    if(PackageLength > Out_size){
        LinkC_Debug("Send-Out Buffer Too Small",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    int NowRecv = 0;
    int TmpSize = 0;
    while(1){
        TmpSize = recv(Sockfd,(char*)Out+NowRecv,PackageLength - NowRecv,flag);
        if(TmpSize <= 0){
            LinkC_Debug("Accepted Data",LINKC_FAILURE);
            return LINKC_FAILURE;
        }
        NowRecv += TmpSize;
        if(NowRecv == PackageLength)
            break;
    }
    return LINKC_SUCCESS;
}

int16_t TCP_Send(int sockfd, void *in, int data_length, int flag){
    if(in == NULL){
        fprintf(stderr,"The Data is NULL!\n");
        return LINKC_FAILURE;
    }
    if(send(sockfd,in,data_length,flag) < 0){
        fprintf(stderr,"Send Error!\n");
        return LINKC_FAILURE;
    }
    return LINKC_SUCCESS;
}

int16_t std_m_message_send(void *Message,int sockfd,uint16_t Length){
    uint16_t Totle,i;
    if(Length < 8 || Message == NULL)    return -1;
    if(Length <= STD_PACKAGE_SIZE){
        if(send(sockfd,Message,Length,0) < 0)
        return -1;
    }else{
        Totle = Length / STD_PACKAGE_SIZE;
        if(Length % STD_PACKAGE_SIZE != 0)
            Totle ++;
        for(i=1;i<=Totle;i++)
            TCP_Send(sockfd,(char *)Message+(i-1)*STD_PACKAGE_SIZE,STD_PACKAGE_SIZE,0);
    }
    return LINKC_SUCCESS;
}

