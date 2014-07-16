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
#include "linkc_def.h"

#define _MESSAGE_HISTORY_HEIGTH 40


class ChatHistoryView : public QWidget{
    Q_OBJECT
public:
    explicit ChatHistoryView(char *Name, QWidget *parent = 0);
    void AddChatMessage(QString Msg);
    void AddChatMessage(QString Msg,QString Name);
    int  GetMessageCount(void);
    void resizeEvent(QResizeEvent *);
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

private slots:
    void    Send(void);
    void    InputEditChanged();
protected:
    QPushButton *SendButton;
    QPushButton *QuitButton;
    ChatHistoryView *History;
    QTextEdit   *Input;
    QVBoxLayout *Layout;
    UserData     MyFriend;
    QTimer *timer;
};

#endif // CHATDIALOG_H
