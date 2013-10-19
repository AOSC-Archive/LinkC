#ifndef P2P_CLIENT_H
#define P2P_CLIENT_H

#include "csocket.h"


struct conn_info_t
{
        /* where the request comes from */
        ip_t SrcIP;
        /* where is the request goes to */
        ip_t DestIP;
        port_t SrcPort;
        port_t DestPort;
};
typedef struct conn_info_t conn_info;

class p2p_client : public socket_c{
    
public:
    p2p_client();
    ~p2p_client();
    int SetDestIP(ip_t ip);
    int SetDestIP(const char* ip);

    int DirectConnect(void);
    int inDirectConnect(void);

protected:
    ip_t DestIP;
    int ip_size;
    
};
#endif // P2P_CLIENT_H
