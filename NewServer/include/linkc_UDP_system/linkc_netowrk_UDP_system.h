#ifndef LINKC_NETWORK_UDP_PROTOCOL_H
#define LINKC_NETWORK_UDP_PROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <netinet/in.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "linkc_package_list.h"
#include "../linkc_def.h"

/* 数据类型定义 */
#ifndef LINKC_SOCKET_TYPES
#define LINKC_SOCKET_TYPES
struct LinkC_Socket_t{
    int                     Sockfd;                 //  基础网络句柄
    int                     Available;              //  剩余可从缓冲区读出的数据包个数
    int                     IsSecurity;             //  是否为安全套接字
    RSA*                    PublicKey;              //  私钥
    RSA*                    PrivateKey;             //  公钥
    struct sockaddr_in      Addr;                   //  目标地址
    socklen_t               SockLen;                //  长度
    void                    *RecvBuffer;            //  接收缓冲区
    void                    *SendBuffer;            //  发送缓冲区
    char                    *ErrorMessage;          //  错误信息
    PackageList             *SendList;              //  发送链表
    PackageList             *RecvList;              //  接收链表
};

typedef struct LinkC_Socket_t           LinkC_Socket;

/* 链表定义 */
#ifndef LINKC_SOCKET_LIST
#define LINKC_SOCKET_LIST

struct SocketListNode_t{
    LinkC_Socket            *Socket;                //  套接字
    pthread_mutex_t         *Mutex_Lock;            //  互斥锁
    struct SocketListNode_t *Perv;                  //  前一个
    struct SocketListNode_t *Next;                  //  后一个
};

struct SocketList_t{
    int                     TotalSocket;            //  总共的套接字数目
    struct SocketListNode_t *StartNode;             //  开始节点
};

typedef struct SocketList_t     SocketList;
typedef struct SocketListNode_t SocketListNode;

/* 链表函数定义 */
int     InitSocketList      (void);                             //  初始LinkC_Socket环境[整个程序中只能被调用一次]
int     AddSocketToList     (LinkC_Socket *Socket);             //  添加LinkC_Socket到链表中去
int     IsSocketInList      (int Sockfd, LinkC_Socket**Socket); //  查询这个Socket是否存在于链表中
int     GetSocketInList     (int Sockfd,LinkC_Socket *Socket);  //  获取对应的LinkC_Socket
int     FindNodeInList      (SocketListNode *Node);             //  在链表中查找结点
int     DelSocketFromList   (int Socket);                       //  从链表中删除指定LinkC_Socket
int     DestroySocketList   (void);                             //  销毁LinkC_Socket环境[必须初始化LinkC_Socket环境后才能调用]

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
int     InitLCUDPEnvironment(void);
/*
 * TODO:    初始化LCUDP环境
 *
 */


int     CreateSocket(const struct sockaddr *MyAddr);
/* 
 * TODO:    创建一个LinkC_Socket套接字[IPV4 Only]，同时加入片轮链表
 *
 * ARGS:
 *      [1] Type :  const struct sockaddr*  一个静态的sockaddr结构体，用于绑定本地地址
 *
 * RETN:
 *      [A] 成功，返回值为一个socket
 *      [0] 失败
 */

int     SetDestAddr(int Socket, struct sockaddr_in DestAddr);
/*
 * TODO:    设置连接对象的地址
 *
 * ARGS:
 *      [1] Tyep :  int                     一个创建好了并添加在片轮链表中的Socket，做为设置目标
 *      [2] Type :  const struct sockaddr_in* 一个静态的sockaddr结构体，用于传入目标的地址
 * RETN:
 *      [0] 成功
 *      [1] 失败
 */

int     DeleteSocket(int Socket);
/*
 * TODO:    删除一个已经被添加到片轮链表中的Socket
 *
 * ARGS:
 *      [1] Type :  int                     一个Socket
 */

/*  Network IO  */
#ifndef LINKC_NETWORK_IO
#define LINKC_NETWORK_IO
/*  High level functions    */
int     Connect         (int Sockfd, struct sockaddr_in Dest);                          //  基准连接函数
int     Accept          (int Sockfd, struct sockaddr_in Dest);                          //  基准接受函数
int     P2PConnect      (int Sockfd, struct sockaddr_in Dest);
int     P2PAccept       (int Sockfd, struct sockaddr_in Dest, void(*Function) (void*), void* Arg);
int     SecurityConnect (int Sockfd, struct sockaddr_in Dest, char* PublicKey, char* MyKey);//  安全连接函数
int     SecurityAccept  (int Sockfd, struct sockaddr_in Dest, char* PublicKey, char* MyKey);
int     SendMessage     (int Sockfd, void *Message, size_t Length, int Flag);           //  标准数据发送
int     RecvMessage     (int Sockfd, void *Buffers, size_t MaxBuf, int Flag);           //  标准数据接收
/*  High level functions    */

/*  Basic functions         */
int     AskForResend    (LinkC_Socket *Socket, int Count);                              //  请求重发数据
int     ConfirmRecved   (LinkC_Socket *Socket, int Count);                              //  确认收到
int     ResendMessage   (LinkC_Socket *Socket, void *Message, int Flag);                //  重发数据
int     _LinkC_Send     (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  基础的数据发送
int     _LinkC_Recv     (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  基础的数据接收
int     __LinkC_Send    (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  低级的数据发送
int     __LinkC_Recv    (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  低级的数据接收
int     ___LinkC_Recv   (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  最低级的数据接收
int     ___LinkC_Send   (LinkC_Socket *Socket, void *Message, size_t size, int Flag);   //  最低级的数据发送
/*  Basic functions         */
#endif  /* LINKC_NETWORK_IO         */
#endif  /* LINKC_SOCKET_FUNCTIONS   */
#endif  /* LINKC_NETWORK_PROTOCO    */
