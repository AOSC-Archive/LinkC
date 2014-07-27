#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QObject>
#include <QMap>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QtGui>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QMenu>
#include <QThread>
#include <stdint.h>
#include "linkc_def.h"
#include "linkc_netowrk_UDP_system.h"
#include "linkc_socket.h"

#define _MESSAGE_HISTORY_HEIGTH 40

class P2PMessageRecver : public QThread{
    Q_OBJECT
public:
    explicit P2PMessageRecver(int Sockfd, QThread *parent = 0);
    ~P2PMessageRecver();
    void run();
signals:
    void    MessageRecved(QString Message);
    void    Disconnected(void);

protected:
    int     Sockfd;
    void*   Buffer;
    void*   Package;
};



class ChatHistoryView : public QWidget{
    Q_OBJECT
public:
    explicit ChatHistoryView(char *Name, QWidget *parent = 0);
    void AddChatMessage(QString Msg);
    void AddChatMessage(QString Msg,QString Name);
    int  GetMessageCount(void);
    void resizeEvent(QResizeEvent *);
public slots:
    void    MessageRecved(QString);
protected:
    int MessageCount;
    QWidget     *MessageBase;
    QScrollArea *List;
    QVBoxLayout *MessageLayout;
    QString     FriendName;
};


class ChatDialog : public QWidget{
    Q_OBJECT
public:
    explicit ChatDialog(UserData MyFriend, QWidget *parent = 0);
    ~ChatDialog();
    void resizeEvent(QResizeEvent *);
    int UID;

signals:
    void    DoP2PConnect(uint32_t);

private slots:
    void    Send(void);
    void    InputEditChanged();
    void    StartConnect();
protected:
    QPushButton         *SendButton;
    QPushButton         *QuitButton;
    QPushButton         *ConnectButton;
    ChatHistoryView     *History;
    QTextEdit           *Input;
    QVBoxLayout         *Layout;
    UserData             MyFriend;
    UDP_Socket          *Socket;
    P2PMessageRecver    *Recver;
    void                *Buffer;
    void                *Package;

};
#endif // CHATDIALOG_H
