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
#include <qt4/QtCore/QObject>
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

class p2p_client : public QObject{
    Q_OBJECT
public:
    explicit p2p_client(QObject *parent = 0);
    ~p2p_client();
    int SetDestIP(ip_t ip);
    int SetDestIP(const char* ip);

    int SendP2PMessage(char *ch);
    int HeartBeats();

    int DirectConnect(void);
    void inDirectConnect(void);
    void inDirectAccept(void);
    int IsServer(void);
    bool IsPeerConnected(void);

    p2pinfo GetP2PInfo(void);
    UDP_csocket GetCsocket(void);

    void SetPeerIP(ip_t ip);

signals:
    void ConnectToPeerDone(bool);

public slots:
    void ConnectToPeer(void);

protected:
    UDP_csocket Dest;
    int flag;
    ip_t DestIP;
    int ip_size;
    char buffer[MAXBUF];
    struct p2pinfo P2PInfo;
    void *package;
    bool isPeerConnected;
};
#endif // P2P_CLIENT_H
