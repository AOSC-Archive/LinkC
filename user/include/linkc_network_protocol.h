/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 2, 2014
 */

#ifndef LINKC_NETWORK_PROTOCOL_H
#define LINKC_NETWORK_PROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <QMetaType>
#include "Def/LinkC_Error.h"
#include "Def/LinkC_Def.h"
#include "linkc_types.h"

#define LINKC_STATUS
#define LINKC_SUCCESS	 0
#define LINKC_FAILURE	-1
#define LINKC_LIMITED	-2



struct LinkC_Message_Header_t
{
    uint8_t  Version;			// 协议版本
    uint8_t  Totle;             // 总包数
    uint8_t  Current;			// 当前包标记
    uint8_t  MessageHeader;		// 服务类型
    uint16_t MessageLength;		// 数据总长度
    time_t   Time;              // 时间戳
};
struct LinkC_User_Request_t{
	uint16_t Action;
	uint16_t Flag;			// 补充说明操作情况，省略值为0
	uint32_t UID;			// 若是自己则UID为0
};
struct LinkC_Sys_Status_t{
	uint16_t Action;
	int16_t  Status;
};
struct LinkC_Msg_Buf_t{
	char buffer[1024];
	int type;			// 数据类型
	int byte;			// 接收到的数据长度
	int flag;			// flag = 1 时表示还有数据，= 0时表示无数据
	int offset;
};
struct LinkC_User_Message_t{
    uint32_t SrcUID;
    uint16_t Action;
};

typedef struct LinkC_Message_Header_t	LinkC_Message_Header;
typedef struct LinkC_User_Request_t	LinkC_User_Request;
typedef struct LinkC_User_Message_t	LinkC_User_Message;
typedef struct login_data		LinkC_User_Login;
typedef struct LinkC_Sys_Status_t	LinkC_Sys_Status;
typedef struct LinkC_Friend_Data_t      LinkC_Friend_Data;
typedef struct LinkC_User_Info_t		LinkC_User_Info;

#define MESSAGE_HEADER_LENGTH	sizeof(struct LinkC_Message_Header_t)
#define USER_REQUEST_LENGTH 	sizeof(struct LinkC_User_Request_t)
#define USER_MESSAGE_LENGTH     sizeof(struct LinkC_User_Message_t)
#define USER_LOGIN_LENGTH       sizeof(struct login_data)
#define SYS_STATUS_LENGTH   	sizeof(struct LinkC_Sys_Status_t)
#define SYS_FRIEND_DATA_LENGTH	sizeof(struct LinkC_Friend_Data_t)
#define SYS_USER_INFO_LENGTH	sizeof(struct LinkC_User_Info_t)

Q_DECLARE_METATYPE(LinkC_Sys_Status)
Q_DECLARE_METATYPE(LinkC_Friend_Data)
Q_DECLARE_METATYPE(LinkC_User_Message)
Q_DECLARE_METATYPE(LinkC_User_Request)
Q_DECLARE_METATYPE(LinkC_User_Info)

// 缩写一下，啊哈哈哈哈哈
#define LMH	LinkC_Message_Header
#define LUR	LinkC_User_Request
#define LUM	LinkC_User_Message
#define LUL	LinkC_User_Login
#define LSS	LinkC_Sys_Status
#define LFD	LinkC_Friend_Data
#define LUI	LinkC_User_Info

#define LMH_L	MESSAGE_HEADER_LENGTH
#define LUR_L	USER_REQUEST_LENGTH
#define LUM_L   USER_MESSAGE_LENGTH
#define LUL_L	USER_LOGIN_LENGTH
#define LSS_L	SYS_STATUS_LENGTH
#define LFD_L	SYS_FRIEND_DATA_LENGTH
#define LUI_L	SYS_USER_INFO_LENGTH
// 缩写 End

int16_t	check_message		(void *Message,uint16_t Recv_Length);
int16_t	pack_message		(uint16_t Header, const void *Data, uint16_t Length, void *Out);
int16_t pack_m_message		(uint16_t Header,void *Data,uint16_t Length,void *Out,uint16_t Totle,uint16_t Current);
int16_t	unpack_message		(void *Message,void *Out);
int16_t std_m_message_send	(void *Message,int sockfd,uint16_t Length);
int16_t non_std_m_message_send	(void *Message,int sockfd,uint16_t Memb,uint16_t Each_Length,uint16_t Header,int Flag);
int16_t non_std_m_message_recv	(int Sockfd,int Echo_Size,void *Out);
int16_t get_message_header	(void *Message);


#endif
