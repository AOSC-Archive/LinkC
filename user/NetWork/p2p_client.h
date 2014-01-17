#ifndef P2P_CLIENT_H
#define P2P_CLIENT_H

#ifndef P2P_CLIENT
#define HEARTBEAT       0
#define CHANGE_STAT     1
#define MESSAGE         2
#endif

#include "Csocket.h"
#include <QTimer>
struct conn_info_t
{
    struct sockaddr_in Src;
    struct sockaddr_in Dest;
};

typedef struct conn_info_t conn_info;

class p2p_client : public UDP_csocket{
    
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
