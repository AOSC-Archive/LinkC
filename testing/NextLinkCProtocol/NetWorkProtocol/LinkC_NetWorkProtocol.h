#ifndef LINKC_NETWORKPROTOCOL_H
#define LINKC_NETWORKPROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <time.h>
#include <netinet/in.h>
#include "../PackageList/PackageList.h"

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
    int                 Sockfd;         //  网络句柄
    int                 Available;      //  剩余可从缓冲区读出的数据包个数
    struct sockaddr_in  Addr;           //  目标地址
    char                *ErrorMessage;  //  错误信息
    PackageList         *SendList;      //  发送链表
    PackageList         *RecvList;      //  接收链表
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
typedef struct LinkC_Socket_t           LinkC_Socket;

#ifndef LINKC_UDP
#define LINKC_UDP
/* 函数部分 */
LinkC_Socket    LinkC_UDP_Socket_Init   (void);
int             LinkC_Socket_Set_Port   (int Port);
int             LinkC_Socket_Set_Addr   (struct sockaddr_in Addr);
int             LinkC_Socket_Perror     (void);
int             LinkC_UDP_Recv          (int Sockfd, char *Buffer, int BuffSize, int Flag);
int             LinkC_UDP_Send          (int Sockfd, char *Buffer, int SendSize, int Flag);
#endif



#endif
