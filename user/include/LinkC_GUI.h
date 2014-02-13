#ifndef LINKC_GUI_H
#define LINKC_GUI_H
#include "LinkC_Label.h"
#include "linkc_types.h"
#include "p2p_client.h"
#include <qt4/QtCore/QObject>
#include <qt4/QtCore/QMap>
#include <qt4/QtGui/QLabel>
#include <qt4/QtGui/QGridLayout>
#include <qt4/QtGui/QScrollArea>
#include <qt4/QtGui/QtGui>
#include <qt4/QtGui/QListWidget>

class FriendGroup : public QListWidget{
    Q_OBJECT
public:
    explicit FriendGroup(QWidget *parent = 0);
protected:
};



class LinkcFriendItem : public QFrame{
    Q_OBJECT
public:
    explicit    LinkcFriendItem(QWidget *parent = 0);
    void        setFriend(const friend_data data);
    friend_data GetFriend(void);
signals:
    void clicked();
protected:
    QHBoxLayout *layout;
    friend_data Friend;
    virtual void mousePressEvent(QMouseEvent *event);
};



class FriendArea : public QWidget{
    Q_OBJECT
public:
    explicit FriendArea(QWidget *parent = 0);
    void AddFriendToLayout(friend_data Myfriend);
    void resizeEvent(QResizeEvent *);
    void setFriendCount(const char s[]);
    void setFriendCount(int n);
    int FriendCount(void);
public slots:
    void ItemClicked(QListWidgetItem *item);
signals:
    void ChatTo(int);
protected:
    int friendcount;
    typedef QMap<QListWidgetItem *,int>_Map;
    _Map::const_iterator Value;
    QScrollArea *s;
    QListWidget *list;
    QWidget     *FriendWidget;
    QVBoxLayout *FriendLayout;
    _Map         FriendMap;
    _Map::iterator result;
};

class ChatDialog : public QWidget{
    Q_OBJECT
public:
    explicit ChatDialog(friend_data *MyFriend, QWidget *parent = 0);
    ~ChatDialog();
    void resizeEvent(QResizeEvent *);
    int ConnectToPeer(void);
    void Recver(void);
    int UID;
signals:
    void Recved(const char *message);
public slots:
    int Send(void);
    int HeartBeats(void);
protected:
    QPushButton *SendButton;
    QPushButton *QuitButton;
    QTextEdit   *History;
    QTextEdit   *Input;
    QVBoxLayout *Layout;
    struct friend_data MyFriend;
    p2p_client  peer;
    QTimer *timer;
    int MessageSize;
};

#endif
