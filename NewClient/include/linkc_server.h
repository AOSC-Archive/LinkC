#ifndef LINKC_SERVER_H
#define LINKC_SERVER_H
#include <stdint.h>

#define SINGLE_USER_TESTING     //  单用户测试[连接互斥]

int     SendActionStatus    (uint16_t StatusCode);

#endif
