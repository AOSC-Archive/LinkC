#include "def.h"
#include "linkc_network_protocol.h"
#include "linkc_network.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int16_t check_message(void *Message,uint16_t Recv_Lenth)
{
	if(Recv_Lenth < MESSAGE_HEADER_LENTH)
		return NOT_MESSAGE;			// 检查数据长度是否小于8
	if(Message == NULL)
		return NOT_MESSAGE;			// 检查数据是否为空
	if(Recv_Lenth == ((LinkC_Message_Header*)Message)->MessageLenth)
		return ((LinkC_Message_Header*)Message)->MessageHeader;
	if(Recv_Lenth < ((LinkC_Message_Header*)Message)->MessageLenth)
		return MESSAGE_INCOMPLETE;
	return NOT_MESSAGE;
}

int16_t pack_message(uint16_t Header,void *Data,uint16_t Lenth,void *Out)
{
	if(Data == NULL)	return -1;
	((LinkC_Message_Header*)Out)->Version = LINKC_MESSAGE_VERSION;
	((LinkC_Message_Header*)Out)->MessageHeader = Header;
	((LinkC_Message_Header*)Out)->MessageLenth  = Lenth + MESSAGE_HEADER_LENTH;
	time(&(((LinkC_Message_Header*)Out)->Time));
	((LinkC_Message_Header*)Out)->Totle = 1;
	((LinkC_Message_Header*)Out)->Current = 1;
	memcpy(Out+MESSAGE_HEADER_LENTH,Data,Lenth);
	return MESSAGE_HEADER_LENTH+Lenth;
}

int16_t pack_m_message(uint16_t Header,void *Data,uint16_t Lenth,void *Out,uint16_t Totle,uint16_t Current)
{
	if(Data == NULL)	return -1;
	((LinkC_Message_Header*)Out)->Version = LINKC_MESSAGE_VERSION;
	((LinkC_Message_Header*)Out)->MessageHeader = Header;
	((LinkC_Message_Header*)Out)->MessageLenth  = Lenth + MESSAGE_HEADER_LENTH;
	time(&(((LinkC_Message_Header*)Out)->Time));
	((LinkC_Message_Header*)Out)->Totle = Totle;
	((LinkC_Message_Header*)Out)->Current = Current;
	memcpy(Out+MESSAGE_HEADER_LENTH,Data,Lenth);
	return MESSAGE_HEADER_LENTH+Lenth;
}


int16_t unpack_message(void *Message,uint16_t Recv_Lenth,void *Out)
{
	memcpy(Out,Message+MESSAGE_HEADER_LENTH,Recv_Lenth-MESSAGE_HEADER_LENTH);
	return 0;
}

int16_t std_m_message_send(void *Message,int sockfd,uint16_t Lenth)
{
	uint16_t Totle,i;
	if(Lenth < 8 || Message == NULL)	return -1;
	if(Lenth <= STD_PACKAGE_SIZE)
	{
		if(send(sockfd,Message,Lenth,MSG_DONTWAIT) < 0)
			return -1;
	}
	else
	{
		Totle = Lenth / STD_PACKAGE_SIZE;
		for(i=1;i<=Totle;i++)
			send(sockfd,Message+(i-1)*STD_PACKAGE_SIZE,STD_PACKAGE_SIZE,MSG_DONTWAIT);
	}
	return SUCCESS;
}

int16_t non_std_m_message_send(void *Message,int sockfd,uint16_t Memb,uint16_t Each_Lenth,uint16_t Header,int Flag)
{
	void *data = malloc(Each_Lenth + Memb + MESSAGE_HEADER_LENTH);
	uint16_t Totle,Current,i,lenth;
	Totle =  Memb;
	for(i=1;i<=Totle;i++)
	{
		lenth = pack_m_message(Header,Message+(i-1)*Each_Lenth,Each_Lenth,data,Totle,i);
		send(sockfd,data,lenth,Flag);
	}
	free(data);
	return SUCCESS;
}
