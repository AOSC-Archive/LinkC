#ifndef LINKC_SOCKET_LIST_H
#define LINKC_SOCKET_LIST_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdint.h>
#include "../PackageList/PackageList.h"

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
};

struct SocketList_t{
    int                     TotalSocket;            //  总共的套接字数目
    pthread_mutex_t         *Mutex_Lock;            //  互斥锁
    struct LinkC_Socket_t   *StartNode;             //  开始节点
};

typedef struct SocketList_t     SocketList;
typedef struct LinKC_Socket_t   LinkC_Socket;

/* Functions */
int     InitSocketList(void);                       //  初始LinkC_Socket环境[整个程序中只能被调用一次]
int     DestroySocketList(void);                    //  销毁LinkC_Socket环境[同上]

int     CreateSocket(LinkC_Socket *Socket, uint8_t SockType, const struct sockaddr *MyAddr, socklen_t addrlen);
/* 
 * TODO:    创建一个LinkC_Socket套接字
 *
 * ARGS:
 *      [1] Type :  LinkC_Socket*           一个空的LinkC_Socket指针，用于返回创建好了的LinkC_Socket
 *      [2] Type :  uint8_t                 一个一字节的无符号整形数，用于设定套接字的类型
 *                                              --> [1] TCP
 *                                              --> [2] UDP
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





#endif
