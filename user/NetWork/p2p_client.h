#ifndef P2P_CLIENT_H
#define P2P_CLIENT_H

#ifndef P2P_CLIENT
#define HEARTBEAT       0
#define CHANGE_STAT     1
#define MESSAGE         2
#endif

#include "Csocket.h"
#include "def.h"
#include <QTimer>
struct conn_info_t
{
    struct sockaddr_in Src;
    struct sockaddr_in Dest;
};

typedef struct conn_info_t conn_info;

struct p2pinfo{
    struct sockaddr_in Dest;
    int is_server;
};

class p2p_client : public UDP_csocket{
    
public:
    p2p_client();
    ~p2p_client();
    int SetDestIP(ip_t ip);
    int SetDestIP(const char* ip);

    int SendP2PMessage();
    int HeartBeats();

    int DirectConnect(void);
    int WaitPeer(void);
    int inDirectConnect(void);
    int inDirectAccept(void);
    int Is_server(void);

protected:
    int flag;
    ip_t DestIP;
    int ip_size;
    char buffer[MAXBUF];
    struct p2pinfo P2PInfo;
    void *package;
};
#endif // P2P_CLIENT_H
