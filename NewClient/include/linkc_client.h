#ifndef LINKC_CILENT_H
#define LINKC_CILENT_H

#include "linkc_def.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <QMetaType>


int InitNetwork();
/*  初始化网络      */

int ConnectToServer(int Sockfd);
int ConnectToServerWithAddr(int Sockfd,struct sockaddr_in Addr);
/*  连接到服务器    */

int Login(int Sockfd,LoginData Data);

Q_DECLARE_METATYPE(LoginData)

#endif
