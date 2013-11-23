#include "def.h"
#include "utilities.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int16_t Check_Message(void *Message,uint16_t Recv_Lenth)
{
	if(Recv_Lenth < HEADER_SIZE)
		return NOT_MESSAGE;			// 检查数据长度是否小于8
	if(Message == NULL)
		return NOT_MESSAGE;			// 检查数据是否为空
	if(Recv_Lenth == ((LinkC_Message_Header*)Message)->MessageLenth)
		return ((LinkC_Message_Header*)Message)->MessageHeader;
	return NOT_MESSAGE;
}

int16_t Pack_Message(uint16_t Header,void *Data,uint16_t Lenth,void *Out)
{
	if(Data == NULL)	return -1;
	((LinkC_Message_Header*)Out)->MessageHeader = Header;
	((LinkC_Message_Header*)Out)->MessageLenth  = Lenth + HEADER_SIZE;
	time(&(((LinkC_Message_Header*)Out)->Time));
	((LinkC_Message_Header*)Out)->Totle = 1;
	((LinkC_Message_Header*)Out)->Current = 1;
	memcpy(Out+HEADER_SIZE,Data,Lenth);
	return 0;
}	


int16_t UnPack_Message(void *Message,uint16_t Recv_Lenth,void *Out)
{
	memcpy(Out,Message+HEADER_SIZE,Recv_Lenth-HEADER_SIZE);
	return 0;
}
