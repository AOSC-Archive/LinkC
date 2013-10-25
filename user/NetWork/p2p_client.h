#ifndef P2P_CLIENT_H
#define P2P_CLIENT_H

#include "csocket.h"

typedef struct conn_info_t conn_info;

class p2p_client : public socket_c{
    
public:
    p2p_client();
    ~p2p_client();
    int SetDestIP(ip_t ip);
    int SetDestIP(const char* ip);

    int SendP2PMessage();
    int HeartBeats();

    int DirectConnect(void);
    int inDirectConnect(void);

protected:
    ip_t DestIP;
    int ip_size;
    char buffer[MAXBUF];
    struct conn_info_t P2PInfo;
    
};
#endif // P2P_CLIENT_H
