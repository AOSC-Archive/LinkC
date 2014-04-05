/*
 * Author        ： Junfeng Zhang <564691478@qq.com>
 * Last-Change        ： March 15, 2014
 */

#ifndef _P2P_HELPER_H_
#define _P2P_HELPER_H_

#include "def.h"
#include "conn_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

struct peer_data_t
{
    int sockfd;
    struct sockaddr_in addr;
    conn_list list;
};

typedef struct peer_data_t peer_data;

errorcode p2p_helper(void);

#endif
