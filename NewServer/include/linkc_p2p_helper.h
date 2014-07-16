#ifndef _P2P_HELPER_H_
#define _P2P_HELPER_H_

#include "linkc_def.h"
#include "linkc_p2p_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

struct peer_data_t{
    int sockfd;
    struct sockaddr_in addr;
    conn_list list;
};

typedef struct peer_data_t peer_data;

int p2p_helper(void);

#endif
