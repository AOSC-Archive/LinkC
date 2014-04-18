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

/* Error_Code */
#define DIFF_VERSION            1       // 协议版本不一致
#define EXCEED_THE_LIMIT        2       // 大小超出限制
#define NOT_MESSAGE             3       // 非消息
#define MESSAGE_INCOMPLETE      4       // 数据不完整
#define OVER_RECV               5       // 收到的数据大于一个包，并且前面的是一个完整的包


/* 数据类型定义 */
#ifndef LINKC_SOCKET_TYPES
#define LINKC_SOCKET_TYPES
struct LinkC_Socket_t{
    int                     Sockfd;                 //  网络句柄
    uint8_t                 SockType;               //  套接字类型  [1]TCP  [2]UDP
    int                     Available;              //  剩余可从缓冲区读出的数据包个数
    struct sockaddr_in      Addr;                   //  目标地址
    char                    *ErrorMessage;          //  错误信息
    PackageList             *SendList;              //  发送链表
    PackageList             *RecvList;              //  接收链表
    struct LinkC_Socket_t   *Perv;                  //  前一个
    struct LinkC_Socket_t   *Next;                  //  后一个
    pthread_mutex_t         *Mutex_Lock;            //  互斥锁
};

struct LinkC_Message_Header_t
{
    uint8_t  Version;                               // 协议版本
    uint8_t  Totle;                                 // 总包数
    uint8_t  Current;                               // 当前包标记
    uint8_t  MessageHeader;                         // 服务类型
    uint16_t MessageLength;                         // 数据总长度
    time_t   Time;                                  // 时间戳
};
typedef struct LinkC_Message_Header_t   LinkC_Message_Header;
typedef struct LinkC_Socket_t           LinkC_Socket;

/* 链表定义 */
#ifndef LINKC_SOCKET_LIST
#define LINKC_SOCKET_LIST

struct SocketList_t{
    int                     TotalSocket;            //  总共的套接字数目
    struct LinkC_Socket_t   *StartNode;             //  开始节点
};

typedef struct SocketList_t     SocketList;

/* 链表函数定义 */
int     InitSocketList(void);                       //  初始LinkC_Socket环境[整个程序中只能被调用一次]
int     AddSocketToList(LinkC_Socket *Socket);      //  添加LinkC_Socket到链表中去
int     FindSocketInList(LinkC_Socket *Socket);     //  在链表中查找结点
int     DelSocketFromList(LinkC_Socket *Socket);    //  从链表中删除LinkC_Socket
int     DestroySocketList(void);                    //  销毁LinkC_Socket环境[同上]

#endif  /* LINKC_SOCKET_LIST  */
#endif  /* LINKC_SOCKET_TYPES */

#ifndef LINKC_SOCKET_INT
#define LINKC_SOCKET_INT

void TimerInt(int SigNo, siginfo_t *SigInfo, void *Arg);
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
 *          [3]-->  void*           真心不知道干嘛用的这个参数
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

/* 套接字操作函数定义 */
#ifndef LINKC_SOCKET_FUNCTIONS
#define LINKC_SOCKET_FUNCTIONS
int     CreateSocket(LinkC_Socket *Socket, int Family, int SockType, const struct sockaddr *MyAddr, socklen_t addrlen);
/* 
 * TODO:    创建一个LinkC_Socket套接字
 *
 * ARGS:
 *      [1] Type :  LinkC_Socket*           一个空的LinkC_Socket指针，用于返回创建好了的LinkC_Socket
 *      [2] Type :  int                     一个一字节的无符号整形数，用于设定套接字的类型
 *                                              --> [SOCK_STREAM]   字节流套接字[TCP]
 *                                              --> [SOCK_DGRAM]    数据报套接字[UDP]
 *      [3] Type :  const struct sockaddr*  一个静态的sockaddr结构体，用于绑定本地地址
 *      [4] Type :  sockaddr_l              套接字长度
 *
 * RETN:
 *      [0] 成功
 *      [1] 失败
 */

int     SetDestAddr(LinkC_Socket *Socket, const struct sockaddr* DestAddr);
/*
 * TODO:    设置连接对象的地址
 *
 * ARGS:
 *      [1] Tyep :  LinkC_Socket*           一个创建好了的LinkC_Socket指针，做为设置目标
 *      [2] Type :  const struct sockaddr*  一个静态的sockaddr结构体，用于传入目标的地址
 * RETN:
 *      [0] 成功
 *      [1] 失败
 */

int     DeleteSocket(LinkC_Socket *Socket);         //  删除一个LinkC_Socket套接字
#endif  /* LINKC_SOCKET_FUNCTIONS   */
#endif  /* LINKC_NETWORKPROTOCO     */
