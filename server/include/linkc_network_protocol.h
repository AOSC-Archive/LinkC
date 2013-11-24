#ifndef LINKC_NETWORK_PROTOCOL_H
#define LINKC_NETWORK_PROTOCOL_H
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "def.h"
#include "linkc_types.h"

/* 系统 */
#define MESSAGE_POOL_SIZE	15	// 发送消息池的大小
#define DATA_SIZE_LIMITED	512	// 单个包数据段最大限制
#define RECV_POOL_SIZE		10240	// 接受缓冲区大小
#define END_OF_LINKC_MESSAGE	0X011B	// Esc键 汗!
#define LINKC_MESSAGE_VERSION	1	// 协议版本

/* Error_Code 定义区 [Check_Message里]*/
#define DIFF_VERSION		-3	// 协议版本不一致
#define EXCEED_THE_LIMIT	-2	// 大小超出限制
#define NOT_MESSAGE		-1	// 非消息
#define MESSAGE_INCOMPLETE	1	// 数据不完整


/* 消息头[LinkC_Message_Header] */
#define EXIT			0	// 退出
#define SYS_MESSAGE_PUSHING	1	// 系统消息推送
#define SYS_ACTION_STATUS	2	// 系统操作状态
#define SYS_FRIEND_DATA		3	// 好友数据
#define USER_LOGIN		4	// 登录
#define USER_LOGOUT		5	// 登出
#define USER_STATUS_HIDDEN	6	// 状态-隐身
#define USER_STATUS_ONLINE	7	// 状态-在线
#define USER_FRIEND_DATA	8	// 请求好友数据
#define USER_CHAT		9	// 请求聊天
#define USER_ADD_FRIEND		10	// 添加好友
#define USER_DEL_FRIEND		11	// 删除好友
#define USER_REQUEST		100	// 用户请求 [以后扩展]

/* 以下是Error_Code 定义区 [LinkC_Sys_Status里面]*/
/*      操作者 操作项目 状态*/
#define USER_LOGIN_SUCCESS	1	// 用户登录成功

#ifndef SUCCESS
#define SUCCESS			0	// 成功
#endif

#define USER_LOGIN_FAILURE	-1	// 用户登录失败
#define USER_FRIEND_NULL	-2	// 用户没有好友
#define USER_LOGIN_LIMITED	-3	// 登录次数超过限制
#define SYS_LOGIN_FAILURE	-10	// 系统登录系统出现错误
#define SYS_FRIEND_GET_FAILURE	-11	// 系统获取好友信息失败


struct LinkC_Message_Header_t
{
	uint8_t  Version;		// 协议版本
	uint8_t  Totle;			// 总包数
	uint16_t MessageHeader;		// 服务类型
	uint16_t MessageLenth;		// 数据总长度
	time_t	 Time;			// 时间戳
	uint8_t  Current;		// 当前包标记
};
struct LinkC_User_Request_t{
	uint32_t UID;
	int32_t	 Flag;			// 补充说明操作情况，省略值为0
};
struct LinkC_Sys_Status_t{
	uint16_t Action;
	int32_t  Status;
};

typedef struct LinkC_Message_Header_t	LinkC_Message_Header;
typedef struct LinkC_User_Request_t	LinkC_User_Request;
typedef struct login_data		LinkC_User_Login;
typedef struct LinkC_Sys_Status_t	LinkC_Sys_Status;
typedef struct friend_data		LinkC_Sys_Friend_Data;

#define MESSAGE_HEADER_LENTH	sizeof(struct LinkC_Message_Header_t)
#define USER_REQUEST_LENTH	sizeof(struct LinkC_User_Request_t)
#define USER_LOGIN_LENTH	sizeof(struct login_data)
#define SYS_STATUS_LENTH	sizeof(struct LinkC_Sys_Status_t)
#define SYS_FRIEND_DATA_LENTH	sizeof(struct friend_data)

int16_t	check_message		(void *Message,uint16_t Recv_Lenth);
int16_t	pack_message		(uint16_t Header,void *Data,uint16_t Lenth,void *Out);
int16_t	unpack_message		(void *Message,uint16_t Recv_Lenth,void *Out);
int16_t std_m_message_send	(void *Message,int sockfd,uint16_t Lenth);
int16_t non_std_m_message_send	(void *Message,int sockfd,uint16_t Memb,uint16_t Each_Lenth,int Flag);

#endif
