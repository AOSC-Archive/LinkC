#ifndef MESSAGERECVER_H
#define MESSAGERECVER_H

#include <qt4/QtCore/QThread>
#include <qt4/QtCore/QVariant>
#include "linkc_types.h"
#include "Csocket.h"
#include "linkc_network_protocol.h"

//-------TCP-------//
class TCP_MessageRecver : public QThread
{
    Q_OBJECT
public:
    explicit TCP_MessageRecver(TCP_csocket *sk);
    ~TCP_MessageRecver();
    void run();

signals:
    void MessageRecved  (const void *data);
    void UserMessage    (LinkC_User_Message);
    void SysActionStatus(QVariant);
    void SysFriendData  (LinkC_Friend_Data);
    void RecvError      ();

public slots:
    void MessageSend(const void *data, int legth, int flag);

protected:
    TCP_csocket Server;
    void *buffer;
    void *package;
};

//-------UDP-------//
class UDP_MessageRecver : public QThread{
    Q_OBJECT
public:
    explicit UDP_MessageRecver(UDP_csocket *sk, QThread *parent);
    void run();

signals:
    void MessageRecved(const LinkC_User_Message *data);

public slots:
    void MessageSend(const void *data, int legth, int flag);

protected:
    UDP_csocket Server;
};

#endif // MESSAGERECVER_H
