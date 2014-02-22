#ifndef CSOCKET_H
#define CSOCKET_H
#include "linkc_types.h"
#include "linkc_network_protocol.h"
#include <arpa/inet.h>
#include <string.h>
#include <linkc_types.h>

#define TCP  1
#define UDP  2

#define SERVER_IP   "127.0.0.1"


class socket_c{
public:
    socket_c();
    ~socket_c();

    int build_socket(int _Flag);

    void Set_IP(const char* Address);
    void Set_IP(ip_t ip);
    void Set_Port(char *port);
    void Set_Port(port_t port);

    int start_connect(void);

    int Recv_msg(void *Buffer, int Flag);   // 接受信息
    int Recv_msg(void *Buffer, int maxbuf, int Flag);
    int Send_msg(const void *Message, int maxbuf, int Flag);   // 接受信息
    int Send_msg(const char *Message, int Flag);    //发送信息
    int Send_msg(const void *Message, int Flag);    // 发送信息
    int Recv_Remain(void *buffer);

    void Debug_Csocket_IP(void);
    void Debug_Csocket_Sockfd(void);
    void Debug_Csocket_Port(void);

    int SetTimedOut(struct  timeval tv);
    int SetAddress(struct sockaddr_in addr);

    int GetSockfd();
    port_t GetPort();
    ip_t GetIP();
    int Is_remain();

protected:
    struct sockaddr_in DestAddr;
    void* recv_buffer;	// 接收缓冲区
    int	is_remain;   	// 上次数据是否有剩余
    int	Length;     	// 接收到数据的长度
    ip_t IP;
    port_t Port;
    int Sockfd;
    int type;
    char Tmp[STD_PACKAGE_SIZE];
    socklen_t AddrLen;
    int TmpLength;
    int tmp;
};

class TCP_csocket : public socket_c{
public:
    TCP_csocket();
    ~TCP_csocket();      // 析构函数
    int TCP_Recv(void *out, int out_size, int flag);

    void cls_buf(char *buffer,int size);    // 清理Buffer
};

class p2p_connection{
    p2p_connection();
};

class UDP_csocket : public socket_c{
public:
    UDP_csocket();
    ~UDP_csocket();      // 析构函数
    int UDP_Recv(void *out, int out_size, int flag);
};

#endif // CSOCKET_H
