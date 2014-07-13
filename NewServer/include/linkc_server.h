#ifndef LINKC_SERVER_H
#define LINKC_SERVER_H
#include <stdint.h>
#include "linkc_def.h"

//#define SINGLE_USER_TESTING     //  单用户测试[连接互斥]

int     SendActionStatus    (int Sockfd, uint16_t StatusCode);

uint8_t GetActionType       (uint8_t Type);
uint8_t GetDataType         (uint8_t Type);

int     ReplyData           (UserData* User,int Sockfd,uint8_t Request,RequestUser* Dest);

#endif
