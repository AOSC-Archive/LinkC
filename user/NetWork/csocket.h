#ifndef CSOCKET_H
#define CSOCKET_H
#include <arpa/inet.h>
#include <string.h>

typedef unsigned long ip_t;

class csocket{
public:
    csocket()
    {
        IP = NULL;
        Port = 0;
        Sockfd = 0;
        memset(&server,0,sizeof(server));
    }

    ~csocket();      // 析构函数

    //Useful Functions
    void Set_IP(const char* Address);
    void Set_Port(char *port);
    void Set_Port(int port);
    int ConnectToServer(void);
    int Recv_msg(void *Buffer, int Flag);
    int Send_msg(const char *Message, int Flag);
    int Send_msg(const void *Message, int Flag);
    void cls_buf(char *buffer,int size);

    //Debug Functions
    void Debug_Csocket_IP(void);
    void Debug_Csocket_Sockfd(void);
    void Debug_Csocket_Port(void);
protected:
    struct sockaddr_in server;
	char *IP;
    int Port;
    int Sockfd;
};

class p2p_connection{
    p2p_connection();
};

#endif // CSOCKET_H
