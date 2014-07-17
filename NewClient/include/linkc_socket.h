#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <QObject>
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdint.h>
#include "linkc_netowrk_UDP_system.h"

class TCP_Socket : public QObject{
    Q_OBJECT
public:
    explicit    TCP_Socket(QObject *parent = 0);
    ~TCP_Socket();
    void        SetDestAddr(struct sockaddr_in Addr);
    int         Connect(void);
    int         Send(void* Message,size_t Length,int Flag);
    int         Recv(void* Message,size_t MaxBuf,int Flag);
signals:

public slots:

private:
    struct sockaddr_in DestAddr;
    int         Sockfd;
    void        *Buffer;
    void        *Package;
};


class UDP_Socket : public QObject{
    Q_OBJECT
public:
    explicit    UDP_Socket(QObject *parent = 0);
    ~UDP_Socket();
    void        SetDestAddr(struct sockaddr_in  Addr);
    int         Connect(void);
    int         Send(void* Message,size_t Length,int Flag);
    int         Recv(void* Buffer ,size_t MaxBuf,int Flag);
    int         GetSockfd(void);
signals:

public slots:
    void        DoP2PConnect(uint32_t IP32);
private slots:

private:
    struct sockaddr_in DestAddr;
    int         Sockfd;
    void        *Buffer;
    void        *Package;
};

#endif // TCP_SOCKET_H
