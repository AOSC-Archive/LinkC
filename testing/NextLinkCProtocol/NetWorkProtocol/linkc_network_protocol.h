/*
 * Author           ： Junfeng Zhang <564691478@qq.com>
 * Last-Change      ： April 13 , 2014
 */

#ifndef LINKC_NETWORK_PROTOCOL_H
#define LINKC_NETWORK_PROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <time.h>

/* 系统 */
#define MESSAGE_POOL_SIZE       15      // 发送消息池的大小
#define LINKC_MESSAGE_VERSION   2       // 协议版本

/* Error_Code 定义区 [Check_Message里]*/
#define DIFF_VERSION            -3      // 协议版本不一致
#define EXCEED_THE_LIMIT        -2      // 大小超出限制
#define NOT_MESSAGE             -1      // 非消息
#define MESSAGE_INCOMPLETE      1       // 数据不完整
#define OVER_RECV               2       // 收到的数据大于一个包，并且前面的是一个完整的包


/* 消息头[LinkC_Message_Header] */
#define HEART_BEATS             0       //  心跳包
#define CONNECT                 1       //  连接
#define DISCONNECT              2       //  断开连接
#define MESSAGE_DATA            2       //  消息数据
#define MESSAGE_RESEND          3       //  重发的数据

/* 用户消息[LinkC_User_Message]*/
#define USER_CHAT               1       // 聊天
#define USER_ACCEPT             2       // 接受
#define USER_REFUSE             3       // 拒绝
#define USER_ADD_FRIEND         4       // 添加好友
#define USER_DEL_FRIEND         5       // 删除好友
#define USER_CONNECT_READY      6       // P2P连接准备完毕

struct LinkC_Socket_t{
    int Sockfd;
};

struct LinkC_Message_Header_t
{
    uint8_t  Version;                   // 协议版本
    uint8_t  Totle;                     // 总包数
    uint8_t  Current;                   // 当前包标记
    uint8_t  MessageHeader;             // 服务类型
    uint16_t MessageLength;             // 数据总长度
    time_t   Time;                      // 时间戳
};
typedef struct LinkC_Message_Header_t   LinkC_Message_Header;

#ifndef LINKC_UDP
#define LINKC_UDP
/* 函数部分 */
int     LinkC_UDP_Init  (void);
/*TODO:
 *      创建一个UDP套接字
 *ARGS:
 *      NONE
 *RETN:
 *      一个已经创建好的UDP
 */
int     LinkC_UDP_Recv  (int Sockfd, char *Buffer, int BuffSize, int Flag);
int     LinkC_UDP_Send  (int Sockfd, char *Buffer, int SendSize, int Flag);
#endif



#endif
