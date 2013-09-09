#ifndef CSOCKET_H
#define CSOCKET_H
#include <arpa/inet.h>
#include <string.h>

typedef unsigned long ip_t;
typedef unsigned short port_t;

class csocket{
public:
    csocket();
    ~csocket();      // 析构函数

    //Useful Functions
    void Set_IP(const char* Address);   // 设置IP
    void Set_IP(ip_t ip);
    void Set_Port(char *port);
    void Set_Port(port_t port);
    int ConnectToServer(void);          // 链接到服务器
    int Recv_msg(void *Buffer, int Flag);   // 接受信息
    int Send_msg(const void *Message, int maxbuf, int Flag);   // 接受信息
    int Send_msg(const char *Message, int Flag);    //发送信息
    int Send_msg(const void *Message, int Flag);    // 发送信息
    void cls_buf(char *buffer,int size);    // 清理Buffer

    //Debug Functions
    void Debug_Csocket_IP(void);
    void Debug_Csocket_Sockfd(void);
    void Debug_Csocket_Port(void);
    
    int GetSockfd();
    int GetPort();
    ip_t GetIP();
protected:
    struct sockaddr_in server_addr;
    ip_t IP;
    port_t Port;
    int Sockfd;
};

class p2p_connection{
    p2p_connection();
};

#endif // CSOCKET_H
