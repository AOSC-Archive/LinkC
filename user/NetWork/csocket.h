#ifndef CSOCKET_H
#define CSOCKET_H
#include <arpa/inet.h>
#include <string.h>
#include <data_type.h>

#define TCP  1
#define UDP  2

#ifndef MAXBUF
#define MAXBUF 1024
#endif

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

    void Debug_Csocket_IP(void);
    void Debug_Csocket_Sockfd(void);
    void Debug_Csocket_Port(void);

    int SetTimedOut(struct  timeval tv);
    int SetAddress(struct sockaddr_in addr);

    int GetSockfd();
    port_t GetPort();
    ip_t GetIP();

protected:
    struct sockaddr_in DestAddr;
    ip_t IP;
    port_t Port;
    int Sockfd;
    int type;
    socklen_t addr_len;
};

class TCP_csocket : public socket_c{
public:
    TCP_csocket();
    ~TCP_csocket();      // 析构函数

    void cls_buf(char *buffer,int size);    // 清理Buffer
};

class p2p_connection{
    p2p_connection();
};

class UDP_csocket : public socket_c{
public:
    UDP_csocket();
    ~UDP_csocket();      // 析构函数
};

#endif // CSOCKET_H
