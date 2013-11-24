#include "def.h"
#include "linkc_network_protocol.h"
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


int16_t unpack_message(void *Message,uint16_t Recv_Lenth,void *Out)
{
	memcpy(Out,Message+MESSAGE_HEADER_LENTH,Recv_Lenth-MESSAGE_HEADER_LENTH);
	return 0;
}
