/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string.h>
#include <unistd.h>
#include "Csocket.h"
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "linkc_types.h"
#if linux
#include<netdb.h>
#elif _WIN32_
#include <ws2tcpip.h>
#endif


#ifndef MAXBUF
#define MAXBUF 1024
#endif

/*  ------------- socket_c ---------- */

socket_c::socket_c(){
    memset((void *)&DestAddr,'0',sizeof(DestAddr));
    AddrLen = sizeof(struct sockaddr_in);
    IP = 0;
    Port = 0;
    Sockfd = 0;
    type = -1;
    is_remain = 0;
    Length = 0;
    recv_buffer = new char[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];
}

socket_c::~socket_c(){
    close(Sockfd);
    delete [] (char *)recv_buffer;
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

int socket_c::SetTimedOut(timeval tv){
    int len= sizeof(tv);
    setsockopt(Sockfd,SOL_SOCKET,SO_SNDTIMEO,(void *)&tv,len);
    return 0;
}

int socket_c::SetAddress(sockaddr_in addr){
    DestAddr = addr;
    return 0;
}

int socket_c::start_connect(void){
    if(type == UDP){
        printf("UDP Socket Can not Face Connection!\n");
        return -1;
    }
    if (IP == 0){
        printf ("IP is empty!\n");
        return -1;
    }
    if (Port == 0){
        printf ("Port is empty!\n");
        return -1;
    }
    DestAddr.sin_family=AF_INET;
    if (connect(Sockfd,(struct sockaddr *)&DestAddr,AddrLen) < 0){
        perror("Connect");
        Debug_Csocket_Sockfd();
        Sockfd = 0;
        return -1;
    }
    return 0;
}

void socket_c::Set_IP(ip_t ip){
    DestAddr.sin_addr.s_addr = ip;
    IP = ip;
}

void socket_c::Set_IP(const char *Address){
    struct hostent *hp;
    hp=gethostbyname(Address);
    if(hp==NULL){
        printf("Get Host By Name Error\n");
    }
    memcpy (&DestAddr.sin_addr.s_addr,hp->h_addr,4);
    IP=DestAddr.sin_addr.s_addr;
}

void socket_c::Set_Port(char* port){
    Port = atoi(port);
}

void socket_c::Set_Port(port_t port){
    Port = port;
    DestAddr.sin_port=htons(Port);
}



void socket_c::Debug_Csocket_IP(void){
    printf ("Debug >> IP\t\t= [%s]\n",inet_ntoa(DestAddr.sin_addr));
}
void socket_c::Debug_Csocket_Port(){
    printf ("Debug >> Port\t\t= [%d]\n",Port);
}

void socket_c::Debug_Csocket_Sockfd(void){
    printf ("Debug >> Socket\t\t= [%d]\n",Sockfd);
}


int socket_c::Recv_msg(void* Buffer,int Flag){
    int byte=0;
    if(type == TCP){
        byte = recv (Sockfd,Buffer,MAXBUF,Flag);
        if (byte < 0){
            perror("TCP_Recv[0]");
            return -1;
        }
    }
    if(type == UDP){
        byte = recvfrom (Sockfd,Buffer,MAXBUF,Flag,(struct sockaddr *)&DestAddr,&AddrLen);
        if (byte < 0){
            perror("UDP_Recv[0]");
        return -1;
        }
    }
    return byte;
}

int socket_c::Recv_msg(void* Buffer,int maxbuf,int Flag){
    int byte=0;
    if(type == TCP){
        byte = recv (Sockfd,Buffer,maxbuf,Flag);
        if (byte < 0){
            perror("TCP_Recv[0]");
            return -1;
        }
    }
    if(type == UDP){
        byte = recvfrom (Sockfd,Buffer,maxbuf,Flag,(struct sockaddr *)&DestAddr,&AddrLen);
        if (byte < 0){
            perror("UDP_Recv[0]");
        return -1;
        }
    }
    return byte;
}

int socket_c::Is_remain(){
    return is_remain;
}

int socket_c::Recv_Remain(void *buffer){
    memcpy(buffer,recv_buffer,Length);
    is_remain = 0;
    Length = 0;
    return 0;
}

int socket_c::Send_msg(const char* Message,int Flag){
    int byte=0;
    if (type == TCP){
        byte = send (Sockfd,Message,MAXBUF,Flag);
        if (byte < 0){
            perror("TCP_Send[0]");
            return -1;
        }
    }
    if (type == UDP){
        byte = sendto (Sockfd,Message,MAXBUF,Flag,(struct sockaddr *)&DestAddr,AddrLen);
        if (byte < 0){
            perror("UDP_Send[0]");
            return -1;
        }
    }
    return byte;
}

int socket_c::Send_msg(const void* Message,int Flag){
    int byte=0;
    if (type == TCP){
        byte = send (Sockfd,Message,MAXBUF,Flag);
        if (byte < 0){
            perror("Send[0]");
            return -1;
        }
    }
    if (type == UDP){
        byte = sendto (Sockfd,Message,MAXBUF,Flag,(struct sockaddr *)&DestAddr,AddrLen);
        if (byte < 0){
            perror("UDP_Send[0]");
            return -1;
        }
    }
    return byte;
}

int socket_c::Send_msg(const void* Message,int maxbuf,int Flag){
    int byte=0;
    if (type == TCP){
        byte = send (Sockfd,Message,maxbuf,Flag);
        if (byte < 0){
            perror("TCP_Send[0]");
            return -1;
        }
    }
    if (type == UDP){
        byte = sendto (Sockfd,Message,maxbuf,Flag,(struct sockaddr *)&DestAddr,AddrLen);
        if (byte < 0){
            perror("UDP_Send[0]");
            return -1;
        }
    }
    return byte;
}

int TCP_csocket::TCP_Recv(void *out, int out_size, int flag)
{
    if(is_remain == 1)      // 若上次数据有剩余
    {
        if(((LMH*)recv_buffer)->MessageLength == Length)        // 若上次剩余的数据是一个完整的包
        {
            if(out_size < Length)						// 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);				// 复制数据
            Length		= 0;						// 重设长度
            is_remain	= 0;						// 设置为没有数据剩余
            return LINKC_SUCCESS;						// 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength < Length)	// 若上次剩余的数据大于一个完整包
        {
            if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
            bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
            memcpy(Tmp,(char *)recv_buffer+(((LMH*)recv_buffer)->MessageLength),Length-((LMH*)recv_buffer)->MessageLength);
            TmpLength = ((LMH*)recv_buffer)->MessageLength;
            memcpy(recv_buffer,Tmp,Length-((LMH*)recv_buffer)->MessageLength);
            Length          = Length - TmpLength;	// 重设长度
            is_remain       = 1;						// 设置为有数据剩余
            return LINKC_SUCCESS;						// 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength > Length)    // 若上次剩余的数据小于一个包[数据不完整]
        {;
            while(1)			// 直到接收数据大于等于数据包长度
            {
                tmp = recv(Sockfd,(char *)recv_buffer+Length,STD_PACKAGE_SIZE,flag);
                if(tmp <= 0)
                {
                    perror("TCP_Recv[1]");
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                Length += tmp;
                if(Length >= ((LMH*)recv_buffer)->MessageLength)
                                break;
                if(Length > STD_PACKAGE_SIZE)
                    fprintf(stderr,"This data is larger than Standard :: Now Size = %d\nNow Stop Rrcv\n",Length);
                if(Length > MAX_BUFFER_SIZE)
                {
                    fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                    Length		= 0;				// 重设长度
                    is_remain	= 0;				// 设置为没有数据
                    return LINKC_FAILURE;
                }
            }
            if(((LMH*)recv_buffer)->MessageLength == Length)		// 若本次接收的数据是一个完整的包
            {
                if(out_size < Length)						// 若输出缓冲小于现在的数据长度
                {
                    fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,Length);					// 复制数据
                Length		= 0;						// 重设长度
                is_remain	= 0;						// 设置为没有数据剩余
                return LINKC_SUCCESS;						// 返回成功
            }
            else if(((LMH*)recv_buffer)->MessageLength < Length)		// 若本次接收的数据大于一个完整包
            {
                if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
                {
                    fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
                bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
                memcpy(Tmp,(char *)recv_buffer+(((LMH*)recv_buffer)->MessageLength),Length-((LMH*)recv_buffer)->MessageLength);
                TmpLength = ((LMH*)recv_buffer)->MessageLength;
                memcpy(recv_buffer,Tmp,Length-((LMH*)recv_buffer)->MessageLength);
                Length          = Length - TmpLength;	// 重设长度
                is_remain       = 1;						// 设置为有数据剩余
                return LINKC_SUCCESS;						// 返回成功
            }
        }
        else                                                    // 不可能的情况
        {
            printf("It's inpossible!\n");
            Length = 0;
            is_remain = 0;
            return LINKC_FAILURE;
        }
    }
    else if(is_remain == 0) //若上次数据没有剩余
    {
        bzero(recv_buffer,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
        while(!Length >= ((LMH*)recv_buffer)->MessageLength)    // 直到接收数据大于等于数据包长度
        {
            tmp = recv(Sockfd,(char *)recv_buffer+Length,STD_PACKAGE_SIZE,flag);
            if(tmp <= 0)
            {
                perror("TCP_Recv[2]");
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            Length += tmp;
            if(Length > STD_PACKAGE_SIZE)
                fprintf(stderr,"This data is larger than Standard :: Now Size = %d\n",Length);
            if(Length > MAX_BUFFER_SIZE)
            {
                fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                Length		= 0;				// 重设长度
                is_remain	= 0;				// 设置为没有数据
                return LINKC_FAILURE;
            }
        }
        if     (((LMH*)recv_buffer)->MessageLength == Length)   // 若本次接收的数据是一个完整的包
        {
            if(out_size < Length)                           // 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);					// 复制数据
            Length		= 0;                                // 重设长度
            is_remain	= 0;                                // 设置为没有数据剩余
            return LINKC_SUCCESS;                           // 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength < Length)	// 若本次接收的数据大于一个完整包
        {
            if(out_size < Length)                           // 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }

            TmpLength = ((LMH*)recv_buffer)->MessageLength; // 第一个数据包的长度
            memcpy(out,recv_buffer,20);                  	// 复制数据
            memcpy(Tmp,(char *)recv_buffer+TmpLength,Length-TmpLength);
            memcpy(recv_buffer,Tmp,Length-TmpLength);
            Length          = Length - TmpLength;           // 重设长度
            is_remain       = 1;                            // 设置为有数据剩余

            return LINKC_SUCCESS;                           // 返回成功
        }
        else                                                    // 不可能的情况
        {
            printf("It's inpossible!\n");
            return LINKC_FAILURE;
        }
    }
    else
        printf("..........\n");
    return LINKC_FAILURE;
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
    DestAddr.sin_family = AF_INET;
}

UDP_csocket::~UDP_csocket(){

}

int UDP_csocket::UDP_Recv(void *out, int out_size, int flag)
{
    if(is_remain == 1)      // 若上次数据有剩余
    {
        if(((LMH*)recv_buffer)->MessageLength == Length)        // 若上次剩余的数据是一个完整的包
        {
            if(out_size < Length)						// 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);				// 复制数据
            Length		= 0;						// 重设长度
            is_remain	= 0;						// 设置为没有数据剩余
            return LINKC_SUCCESS;						// 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength < Length)	// 若上次剩余的数据大于一个完整包
        {
            if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
            bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
            memcpy(Tmp,(char *)recv_buffer+(((LMH*)recv_buffer)->MessageLength),Length-((LMH*)recv_buffer)->MessageLength);
            TmpLength = ((LMH*)recv_buffer)->MessageLength;
            memcpy(recv_buffer,Tmp,Length-((LMH*)recv_buffer)->MessageLength);
            Length          = Length - TmpLength;	// 重设长度
            is_remain       = 1;						// 设置为有数据剩余
            return LINKC_SUCCESS;						// 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength > Length)    // 若上次剩余的数据小于一个包[数据不完整]
        {;
            while(1)			// 直到接收数据大于等于数据包长度
            {
                tmp = recvfrom(Sockfd,(char *)recv_buffer+Length,STD_PACKAGE_SIZE,flag,(struct sockaddr *)&DestAddr,&AddrLen);
                if(tmp <= 0)
                {
                    perror("UDP_Recv[1]");
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                Length += tmp;
                if(Length >= ((LMH*)recv_buffer)->MessageLength)
                                break;
                if(Length > STD_PACKAGE_SIZE)
                    fprintf(stderr,"This data is larger than Standard :: Now Size = %d\nNow Stop Rrcv\n",Length);
                if(Length > MAX_BUFFER_SIZE)
                {
                    fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                    Length		= 0;				// 重设长度
                    is_remain	= 0;				// 设置为没有数据
                    return LINKC_FAILURE;
                }
            }
            if(((LMH*)recv_buffer)->MessageLength == Length)		// 若本次接收的数据是一个完整的包
            {
                if(out_size < Length)						// 若输出缓冲小于现在的数据长度
                {
                    fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,Length);					// 复制数据
                Length		= 0;						// 重设长度
                is_remain	= 0;						// 设置为没有数据剩余
                return LINKC_SUCCESS;						// 返回成功
            }
            else if(((LMH*)recv_buffer)->MessageLength < Length)		// 若本次接收的数据大于一个完整包
            {
                if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
                {
                    fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                    Length = 0;
                    is_remain = 0;
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
                bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
                memcpy(Tmp,(char *)recv_buffer+(((LMH*)recv_buffer)->MessageLength),Length-((LMH*)recv_buffer)->MessageLength);
                TmpLength = ((LMH*)recv_buffer)->MessageLength;
                memcpy(recv_buffer,Tmp,Length-((LMH*)recv_buffer)->MessageLength);
                Length          = Length - TmpLength;	// 重设长度
                is_remain       = 1;						// 设置为有数据剩余
                return LINKC_SUCCESS;						// 返回成功
            }
        }
        else                                                    // 不可能的情况
        {
            printf("It's inpossible!\n");
            Length = 0;
            is_remain = 0;
            return LINKC_FAILURE;
        }
    }
    else if(is_remain == 0) //若上次数据没有剩余
    {
        bzero(recv_buffer,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
        while(!Length >= ((LMH*)recv_buffer)->MessageLength)    // 直到接收数据大于等于数据包长度
        {
            tmp = recvfrom(Sockfd,(char *)recv_buffer+Length,STD_PACKAGE_SIZE,flag,(struct sockaddr *)&DestAddr,&AddrLen);
            if(tmp <= 0)
            {
                perror("UDP_Recv[2]");
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            Length += tmp;
            if(Length > STD_PACKAGE_SIZE)
                fprintf(stderr,"This data is larger than Standard :: Now Size = %d\n",Length);
            if(Length > MAX_BUFFER_SIZE)
            {
                fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                Length		= 0;				// 重设长度
                is_remain	= 0;				// 设置为没有数据
                return LINKC_FAILURE;
            }
        }
        if     (((LMH*)recv_buffer)->MessageLength == Length)   // 若本次接收的数据是一个完整的包
        {
            if(out_size < Length)                           // 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);					// 复制数据
            Length		= 0;                                // 重设长度
            is_remain	= 0;                                // 设置为没有数据剩余
            return LINKC_SUCCESS;                           // 返回成功
        }
        else if(((LMH*)recv_buffer)->MessageLength < Length)	// 若本次接收的数据大于一个完整包
        {
            if(out_size < Length)                           // 若输出缓冲小于现在的数据长度
            {
                fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
                Length = 0;
                is_remain = 0;
                return LINKC_FAILURE;
            }

            TmpLength = ((LMH*)recv_buffer)->MessageLength; // 第一个数据包的长度
            memcpy(out,recv_buffer,20);                  	// 复制数据
            memcpy(Tmp,(char *)recv_buffer+TmpLength,Length-TmpLength);
            memcpy(recv_buffer,Tmp,Length-TmpLength);
            Length          = Length - TmpLength;           // 重设长度
            is_remain       = 1;                            // 设置为有数据剩余

            return LINKC_SUCCESS;                           // 返回成功
        }
        else                                                    // 不可能的情况
        {
            printf("It's inpossible!\n");
            return LINKC_FAILURE;
        }
    }
    else
        printf("..........\n");
    return LINKC_FAILURE;
}

int socket_c::GetSockfd(){
    return Sockfd;
}


