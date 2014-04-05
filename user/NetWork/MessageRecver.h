/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 2, 2014
 */
#ifndef MESSAGERECVER_H
#define MESSAGERECVER_H

#include <QThread>
#include <QVariant>
#include "linkc_types.h"
#include "Csocket.h"
#include "linkc_network_protocol.h"

//-------TCP-------//
class TCP_MessageRecver : public QThread
{
    Q_OBJECT
public:
    explicit TCP_MessageRecver(TCP_csocket *sk, QThread *parent = 0);
    ~TCP_MessageRecver();
    void run();
    int RecvFriendsList(void);

signals:
    void MessageRecved  (const void *data);
    void UserMessage    (LinkC_User_Message);
    void SysActionStatus(LinkC_Sys_Status);
    void SysFriendData  (LinkC_Friend_Data);
    void SysUserInfo    (LinkC_User_Info);
    void RecvError      ();
    void SysFriendsList (void*,int);

protected:
    TCP_csocket Dest;
};

//-------UDP-------//
class UDP_MessageRecver : public QThread{
    Q_OBJECT
public:
    explicit UDP_MessageRecver(UDP_csocket sk, QThread *parent = 0);
    explicit UDP_MessageRecver(struct sockaddr_in Add,QThread *parent = 0);
    void run();

signals:
    void RecvedP2PMessage(QString);
    void HeartBeats(void);
    void RecvError();

protected:
    UDP_csocket Dest;
    void *buffer;
    void *package;
};

#endif // MESSAGERECVER_H
