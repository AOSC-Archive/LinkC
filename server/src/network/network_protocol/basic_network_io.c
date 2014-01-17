#include "linkc_network_protocol.h"
#include "linkc_network.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

char	recv_buffer[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];	// 接收缓冲区
char	send_buffer[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];	// 发送缓冲区
int	is_remain	= 0;	// 上次数据是否有剩余
int	Length 		= 0;	// 接收到数据的长度
int	tmp;


int16_t TCP_Recv(int sockfd, void *out, int out_size, int flag)
{
	if(is_remain == 1)	// 若上次数据有剩余
	{
		if(((LMH*)recv_buffer)->MessageLength == Length)	// 若上次剩余的数据是一个完整的包
		{
			if(out_size < Length)						// 若输出缓冲小于现在的数据长度
			{
				fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,Length);
				return LINKC_FAILURE;
			}
			memcpy(out,recv_buffer,Length);				// 复制数据
			Length		= 0;						// 重设长度
			is_remain	= 0;						// 设置为没有数据剩余
			return LINKC_SUCCESS;						// 返回成功
		}
		else if(((LMH*)recv_buffer)->MessageLength > Length)	// 若上次剩余的数据大于一个完整包
		{
			if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
			{
				fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
				return LINKC_FAILURE;
			}
			memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);	// 复制数据
			Length          = Length - ((LMH*)recv_buffer)->MessageLength;	// 重设长度
			is_remain       = 1;						// 设置为有数据剩余
			bzero(recv_buffer+Length,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1 - Length);	// 清空多余数据
			return LINKC_SUCCESS;						// 返回成功
		}
		else						// 若上次剩余的数据小于一个包[数据不完整]
		{
			while(!Length >= ((LMH*)recv_buffer)->MessageLength)			// 直到接收数据大于等于数据包长度
			{
				tmp = recv(sockfd,recv_buffer+Length,STD_PACKAGE_SIZE,flag);
				if(tmp <= 0)
				{
					fprintf(stderr,"Recv Error!\n",Length);
					return LINKC_FAILURE;
				}
				Length += tmp;
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
					return LINKC_FAILURE;
				}
				memcpy(out,recv_buffer,Length);					// 复制数据
				Length		= 0;						// 重设长度
				is_remain	= 0;						// 设置为没有数据剩余
				return LINKC_SUCCESS;						// 返回成功
			}
			else if(((LMH*)recv_buffer)->MessageLength > Length)		// 若本次接收的数据大于一个完整包
			{
				if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
				{
					fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
					return LINKC_FAILURE;
				}
				memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
				Length          = Length - ((LMH*)recv_buffer)->MessageLength;	// 重设长度
				is_remain       = 1;						// 设置为有数据剩余
				bzero(recv_buffer+Length,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1 - Length);	// 清空多余数据
				return LINKC_SUCCESS;						// 返回成功
			}
		}
	}
	else		//若上次数据没有剩余
	{
		bzero(recv_buffer,MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1);
		while(!Length >= ((LMH*)recv_buffer)->MessageLength)			// 直到接收数据大于等于数据包长度
		{
			tmp = recv(sockfd,recv_buffer+Length,STD_PACKAGE_SIZE,flag);
			if(tmp <= 0)
			{
				fprintf(stderr,"Recv Error!\n",Length);
				return LINKC_FAILURE;
			}
			Length += tmp;
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
				return LINKC_FAILURE;
			}
			memcpy(out,recv_buffer,Length);					// 复制数据
			Length		= 0;						// 重设长度
			is_remain	= 0;						// 设置为没有数据剩余
			return LINKC_SUCCESS;						// 返回成功
		}
		else if(((LMH*)recv_buffer)->MessageLength > Length)		// 若本次接收的数据大于一个完整包
		{
			if(out_size < Length)                           		// 若输出缓冲小于现在的数据长度
			{
				fprintf(stderr,"Out Buffer is too small to copy data!\nBuffer Size = %d\tData Size = %d\n",out_size,((LMH*)recv_buffer)->MessageLength);
				return LINKC_FAILURE;
			}
			memcpy(out,recv_buffer,((LMH*)recv_buffer)->MessageLength);		// 复制数据
			Length          = Length - ((LMH*)recv_buffer)->MessageLength;	// 重设长度
			is_remain       = 1;						// 设置为有数据剩余
			return LINKC_SUCCESS;						// 返回成功
		}
	}
	return LINKC_FAILURE;
}

int16_t TCP_Send(int sockfd, void *in, int data_length, int flag)
{
	if(in == NULL)
	{
		fprintf(stderr,"The Data is NULL!\n");
		return LINKC_FAILURE;
	}
	if(send(sockfd,in,data_length,flag) < 0)
	{
		fprintf(stderr,"Send Error!\n");
		return LINKC_FAILURE;
	}
	return LINKC_SUCCESS;
}

int16_t std_m_message_send(void *Message,int sockfd,uint16_t Length)
{
	uint16_t Totle,i;
	if(Length < 8 || Message == NULL)	return -1;
	if(Length <= STD_PACKAGE_SIZE)
	{
		if(send(sockfd,Message,Length,0) < 0)
		return -1;
	}
	else
	{
		Totle = Length / STD_PACKAGE_SIZE;
		if(Length % STD_PACKAGE_SIZE != 0)
			Totle ++;
		for(i=1;i<=Totle;i++)
			TCP_Send(sockfd,(char *)Message+(i-1)*STD_PACKAGE_SIZE,STD_PACKAGE_SIZE,0);
	}
	return SUCCESS;
}

int16_t non_std_m_message_send(void *Message,int sockfd,uint16_t Memb,uint16_t Each_Length,uint16_t Header,int Flag)
{
	void *data = malloc(Each_Length + Memb + LMH_L);
	uint16_t Totle,i,length;
	Totle =  Memb;
	for(i=1;i<=Totle;i++)
	{
		length = pack_m_message(Header,(char *)Message+(i-1)*Each_Length,Each_Length,data,Totle,i);
		send(sockfd,data,length,Flag);
	}
	free (data);
	return SUCCESS;
}
