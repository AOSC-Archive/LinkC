#ifndef LINKC_CILENT_H
#define LINKC_CILENT_H

#include "linkc_def.h"

int NonUiMode();
/*  进入NonUi模式   */

int InitNetwork();
/*  初始化网络      */

int ConnectToServer(int Sockfd);
/*  连接到服务器    */

int Login(int Sockfd,LoginData Data);
/*  登录            */

#endif
