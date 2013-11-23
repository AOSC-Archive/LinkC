#ifndef UTILITIES_H
#define UTILITIES_H
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "def.h"
#include "linkc_types.h"

#define END_OF_LINKC_MESSAGE	0X011B	// Esc键 汗!
#define BUFFER_SIZE		2048

#define EXCEED_THE_LIMIT	-2	// 大小超出限制
#define NOT_MESSAGE		-1	// 非消息
#define MESSAGE_INCOMPLETE	1	// 数据不完整
#define MESSAGE_POOL_SIZE	15	// 发送消息池的大小
#define DATA_SIZE_LIMITED	512	// 单个包数据段最大限制
#define RECV_POOL_SIZE		10240	// 接受缓冲区大小

#define FRIEND_MESSAGE		1	// 好友发送的聊天信息
#define SYS_MESSAGE_PUSHING	2	// 系统消息推送
#define SYS_LOGIN_STATUS	3	// 系统登录状况
#define SYS_FRIEND_DATA		4	// 好友数据
#define USER_ADD_FRIEND		5	// 添加好友
#define USER_DEL_FRIEND		6	// 删除好友
#define USER_LOGIN		7	// 登录
#define USER_LOGOUT		8	// 登出
#define USER_CHAT		9	// 聊天请求
struct LinkC_Message_Header_t
{
	uint16_t MessageHeader;
	uint16_t MessageLenth;
	time_t	 Time;
	uint8_t  Totle;
	uint8_t  Current;
};
#define HEADER_SIZE		sizeof(LinkC_Message_Header)	// Header的大小
struct LinkC_User_Request_t{
	uint32_t UID;
	int32_t	 Flag;			// 补充说明操作情况，省略值为0
};
struct LinkC_Sys_Status_t{
	int16_t	Error_Code;
};

typedef struct LinkC_Message_Header_t	LinkC_Message_Header;
typedef struct LinkC_User_Requiest_t	LinkC_User_Request;
typedef struct LinkC_Sys_Statue_t	LinkC_Sys_Status;
typedef struct friend_data		LinkC_Sys_Friend_data;

int16_t	Check_Message	(void *Message,uint16_t Recv_Lenth);
int16_t	Pack_Message	(uint16_t Header,void *Data,uint16_t Lenth,void *Out);
int16_t	UnPack_Message	(void *Message,uint16_t Recv_Lenth,void *Out);

#endif
