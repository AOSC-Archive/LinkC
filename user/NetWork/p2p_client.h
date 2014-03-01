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
#include <QObject>
#include <QThread>
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

class P2PConnecter : public QThread{
    Q_OBJECT
public:
    explicit P2PConnecter(UDP_csocket k, p2pinfo info, QThread *parent = 0);
    void run();
    void inDirectConnect(void);
    void inDirectAccept(void);

    bool IsConnected(void);
signals:
    void ConnectToPeerDone(bool);
    void ReadyToAccept(void);
protected:
    UDP_csocket Dest;
    char buffer[MAXBUF];
    struct p2pinfo P2PInfo;
    void *package;
};

class p2p_client : public QThread{
    Q_OBJECT
public:
    explicit p2p_client(QThread *parent = 0);
    ~p2p_client();
    void run();
    int SetDestIP(ip_t ip);
    int SetDestIP(const char* ip);

    int SendP2PMessage(char *ch);
    int HeartBeats();

    int DirectConnect(void);
    int IsServer(void);
    bool IsPeerConnected(void);

    p2pinfo GetP2PInfo(void);
    UDP_csocket GetCsocket(void);

    void SetPeerIP(ip_t ip);
    void inDirectConnectStart(void);

signals:
    void AcceptReady(void);
    void ConnectToPeerDone(bool);

public slots:
    void ConnectToPeer(void);
    void ReadyToAccept(void);
    void ConnectDone(bool);

protected:
    UDP_csocket Dest;
    int flag;
    ip_t DestIP;
    int ip_size;
    char buffer[MAXBUF];
    struct p2pinfo P2PInfo;
    void *package;
    bool isPeerConnected;
    P2PConnecter *Connecter;
};



#endif // P2P_CLIENT_H
