#include "../../../include/linkc_TCP_system/linkc_TCP_io.h"
#include "../../../include/linkc_def.h"
#include "../../../include/linkc_error.h"
#include "../../../include/linkc_package.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

char    recv_buffer[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];    // 接收缓冲区
char    Tmp[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];        // 临时缓冲区
int    is_remain    = 0;    // 上次数据是否有剩余
int    Length         = 0;    // 接收到数据的长度
int    tmp;
uint16_t    TmpLength;


int16_t TCP_recv(int Sockfd, void *Out, int Out_size, int flag){
    PackageHeader Header;
    int Status = 0;
    Status = recv(Sockfd,(void*)&Header,sizeof(PackageHeader),MSG_PEEK);
    if(Status <= 0){
        if(Status == 0)
            return LINKC_NO_DATA;
        else    
            return LINKC_FAILURE;
    }
    int PackageLength = ntohs(Header.MessageLength)+sizeof(PackageHeader);
    if(PackageLength > Out_size){
        LinkC_Debug("Send-Out Buffer Too Small",LINKC_FAILURE); // Well, after fixing those Chinese strings, someone should really merge the error 
	                                                        // outputs into some sort of function. MERGE PLEASE...
        printf("<---PackageHeader--->\n");
        printf("Version:\t\t%d\n",Header.ProtocolVersion);
        printf("Length:\t\t\tHEX:%x\tDEC:%d\n",Header.MessageLength,Header.MessageLength);
        printf("Type:\t\t\t%d\n",Header.MessageType);
        printf("Count:\t\t\t%d\n",Header.MessageCounts);
        
        return LINKC_FAILURE;
    }
    int NowRecv = 0;
    int TmpSize = 0;
    while(1){
        TmpSize = recv(Sockfd,(char*)Out+NowRecv,PackageLength - NowRecv,flag);
        if(TmpSize <= 0){
            LinkC_Debug("Fetched Data",LINKC_FAILURE);
            return LINKC_FAILURE;
        }
        NowRecv += TmpSize;
        if(NowRecv == PackageLength)
            break;
    }
    return LINKC_SUCCESS;
}

int16_t TCP_Recv(int sockfd, void *out, int out_size, int flag)
{
    if(is_remain == 1){    // 若上次数据有剩余
        TmpLength = ntohs(((PackageHeader*)recv_buffer)->MessageLength)+8;
        if(TmpLength == Length){    // 若上次剩余的数据是一个完整的包
            if(out_size < Length){                        // 若输出缓冲小于现在的数据长度
                fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);                // 复制数据
            Length        = 0;                        // 重设长度
            is_remain    = 0;                        // 设置为没有数据剩余
            return LINKC_SUCCESS;                        // 返回成功
        }
        else if(TmpLength < Length){    // 若上次剩余的数据大于一个完整包
            if(out_size < Length){                                   // 若输出缓冲小于现在的数据长度
                fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,TmpLength);
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,TmpLength);        // 复制数据
            bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
            memcpy(Tmp,(char *)recv_buffer+TmpLength,Length-TmpLength);
            memcpy(recv_buffer,Tmp,Length-TmpLength);
            Length          = Length - TmpLength;    // 重设长度
            is_remain       = 1;                        // 设置为有数据剩余
            return LINKC_SUCCESS;                        // 返回成功
        }else{                        // 若上次剩余的数据小于一个包[数据不完整]
            while(Length < TmpLength){            // 直到接收数据大于等于数据包长度
                tmp = recv(sockfd,recv_buffer+Length,STD_PACKAGE_SIZE,flag);
                if(tmp <= 0){
                    fprintf(stderr,"Recv Error[1]!\n");
                    return LINKC_FAILURE;
                }
                Length += tmp;
                if(Length > STD_PACKAGE_SIZE)
                    fprintf(stderr,"This data is larger than Standard :: Now Size = %d\nNow Stop Rrcv\n",Length);
                if(Length > MAX_BUFFER_SIZE){
                    fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                    Length        = 0;                // 重设长度
                    is_remain    = 0;                // 设置为没有数据
                    return LINKC_FAILURE;
                }
            }
            if(TmpLength == Length){        // 若本次接收的数据是一个完整的包
                if(out_size < Length){                        // 若输出缓冲小于现在的数据长度
                    fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,Length);                    // 复制数据
                Length        = 0;                        // 重设长度
                is_remain    = 0;                        // 设置为没有数据剩余
                return LINKC_SUCCESS;                        // 返回成功
            }
            else if(TmpLength < Length){        // 若本次接收的数据大于一个完整包
                if(out_size < Length){                                   // 若输出缓冲小于现在的数据长度
                    fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,TmpLength);
                    return LINKC_FAILURE;
                }
                memcpy(out,recv_buffer,TmpLength);        // 复制数据
                bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
                memcpy(Tmp,(char *)recv_buffer+TmpLength,Length-TmpLength);
                memcpy(recv_buffer,Tmp,Length-TmpLength);
                Length          = Length - TmpLength;    // 重设长度
                is_remain       = 1;                        // 设置为有数据剩余
                return LINKC_SUCCESS;
            }
        }
    }
    else        //若上次数据没有剩余
    {
        bzero(recv_buffer,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
        while(1){            // 直到接收数据大于等于数据包长度
            tmp = recv(sockfd,recv_buffer+Length,STD_PACKAGE_SIZE,flag);
            if(tmp <= 0){
                fprintf(stderr,"Recv Error[2]!\n");
                return LINKC_FAILURE;
            }
            Length += tmp;
            if(Length >= ntohs(((PackageHeader*)recv_buffer)->MessageLength))
                break;
            if(Length > STD_PACKAGE_SIZE)
                fprintf(stderr,"This data is larger than Standard :: Now Size = %d\nNow Stop Rrcv\n",Length);
            if(Length > MAX_BUFFER_SIZE){
                fprintf(stderr,"This data is beyond Recv :: Now Size = %d\n",Length);
                Length        = 0;                // 重设长度
                is_remain    = 0;                // 设置为没有数据
                return LINKC_FAILURE;
            }
        }
        TmpLength = ntohs(((PackageHeader*)recv_buffer)->MessageLength)+8;
        printf("Length = %d\n",TmpLength);
        if(TmpLength == Length){        // 若本次接收的数据是一个完整的包
            if(out_size < Length){                        // 若输出缓冲小于现在的数据长度
                fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,Length);                    // 复制数据
            Length        = 0;                        // 重设长度
            is_remain    = 0;                        // 设置为没有数据剩余
            return LINKC_SUCCESS;                        // 返回成功
        }
        else if(TmpLength > Length){        // 若本次接收的数据大于一个完整包
            if(out_size < Length){                                   // 若输出缓冲小于现在的数据长度
                fprintf(stderr,"Send-Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,ntohs(((PackageHeader*)recv_buffer)->MessageLength)+8);
                return LINKC_FAILURE;
            }
            memcpy(out,recv_buffer,TmpLength);        // 复制数据
            bzero(Tmp,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
            memcpy(Tmp,(char *)recv_buffer+TmpLength,Length-TmpLength);
            memcpy(recv_buffer,Tmp,Length-TmpLength);
            Length          = Length - TmpLength;    // 重设长度
            is_remain       = 1;                        // 设置为有数据剩余
            return LINKC_SUCCESS;                        // 返回成功
        }
    }
    return LINKC_FAILURE;
}

int16_t TCP_Send(int sockfd, void *in, int data_length, int flag)
{
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

int16_t std_m_message_send(void *Message,int sockfd,uint16_t Length)
{
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

// It's been so much time seeing Send-Out Buffer here... Sigh.
