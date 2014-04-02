/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： Aprli 2, 2014
 */

#include "def.h"
#include "linkc_network_protocol.h"
#include "linkc_network.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int16_t check_message(void *Message,uint16_t Recv_Length)
{
	if(Message == NULL || Recv_Length == 0)	return LINKC_FAILURE;
	return ((LMH*)Message)->MessageHeader;
}

int16_t get_message_header(void *Message)
{
	return ((LMH*)Message)->MessageHeader;
}

int16_t pack_message(uint16_t Header,const void *Data,uint16_t Length,void *Out)
{
	((LMH*)Out)->Version = LINKC_MESSAGE_VERSION;
	((LMH*)Out)->MessageHeader = Header;
	time(&(((LMH*)Out)->Time));
	((LMH*)Out)->Totle = 1;
	((LMH*)Out)->Current = 1;
	if(Length == 0)
	{
		((LMH*)Out)->MessageLength  = LMH_L;
		return LMH_L;
	}
	((LMH*)Out)->MessageLength  = Length + LMH_L;
	memcpy((char *)Out+LMH_L,Data,Length);
	return LMH_L+Length;
}

int16_t pack_m_message(uint16_t Header,void *Data,uint16_t Length,void *Out,uint16_t Totle,uint16_t Current)
{
    if(Data == NULL)	return -1;
    ((LMH*)Out)->Version = LINKC_MESSAGE_VERSION;
    ((LMH*)Out)->MessageHeader = Header;
    ((LMH*)Out)->MessageLength  = Length + LMH_L;
    time(&(((LMH*)Out)->Time));
    ((LMH*)Out)->Totle = Totle;
    ((LMH*)Out)->Current = Current;
    memcpy((char *)Out+LMH_L,Data,Length);
    return LMH_L+Length;
}


int16_t unpack_message(void *Message,void *Out)
{
    memcpy(Out,(char *)Message + LMH_L,((LMH*)Message)->MessageLength - LMH_L);
    return 0;
}


