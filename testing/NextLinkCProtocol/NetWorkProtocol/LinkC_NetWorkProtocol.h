#ifndef LINKC_NETWORKPROTOCOL_H
#define LINKC_NETWORKPROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <netinet/in.h>
#include "../PackageList/PackageList.h"

/* 系统 */
#define MAX_MESSAGE_POOL_SIZE   15      // 最大缓冲区保存数据报的数量
#define LINKC_MESSAGE_VERSION   2       // 协议版本

/* Error_Code 定义区 [Check_Message里]*/
#define DIFF_VERSION            1       // 协议版本不一致
#define EXCEED_THE_LIMIT        2       // 大小超出限制
#define NOT_MESSAGE             3       // 非消息
#define MESSAGE_INCOMPLETE      4       // 数据不完整
#define OVER_RECV               5       // 收到的数据大于一个包，并且前面的是一个完整的包

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

#ifndef LINKC_SOCKET_INT
#define LINKC_SOCKET_INT

void TimerInt(int SignalNumber, siginfo_t *info, void *Arg);
/*
 * TODO:    用于处理时钟中断信号 
 * 
 * 注意:    这里必须使用这种函数参数结构,因为系统发送信号时设置的捕捉函数
 *          参数只有这些
 *          原型:   void (*sa_sigaction) (int, siginfo_t *, void *);
 *
 * 参数:    这里前两个参数是系统直接赋值的,分别是
 *          [1]-->  int             信号的值
 *          [2]-->  siginf_t*       信号的具体信息
 *          [3]-->  void*           用户自己传递的数据
 */

void IOReadyInt(int SignalNumber, siginfo_t *info, void *Arg);
 /*
  * TODO:   用于处理IO准备完毕信号
  *
  * 注意:   同上
  *
  * 参数:   同上
  */

#endif

#ifndef LINKC_UDP_FUNCTIONS
#define LINKC_UDP_FUNCTIONS
/* 函数部分 */
LinkC_Socket*   LinkC_UDP_Socket_Init   (void);                                                         //  创建LinkC_UDP_Socket
int             LinkC_Socket_Bind       (LinkC_Socket *Socket, const struct sockaddr *address);         //  绑定本地地址
int             LinkC_Socket_SetAddr    (LinkC_Socket *Socket, struct sockaddr_in Addr);                //  设定远程地址
int             LinkC_Socket_Perror     (LinkC_Socket *Socket);                                         //  打印错误信息
int             LinkC_Socket_Ctl        (void);                                                         //  还在设计中
int             LinkC_UDP_Recv          (LinkC_Socket *Socket, char *Buffer, int BuffSize, int Flag);   //  接受数据
int             LinkC_UDP_Send          (LinkC_Socket *Socket, char *Buffer, int SendSize, int Flag);   //  发送数据
#endif



#endif
