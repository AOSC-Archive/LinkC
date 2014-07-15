#ifndef FRIENDSLIST_H
#define FRIENDSLIST_H

#include "linkc_def.h"
#include <stdint.h>
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

#define _FRIEND_LABEL_HEIGTH    30
#define _MESSAGE_HISTORY_HEIGTH 40

class FriendItem : public QLabel{
    Q_OBJECT
public:
    explicit    FriendItem(QWidget *parent = 0);
    void        SetFriend(const UserData data);
    UserData    GetFriend(void);
signals:
    void clicked(UserData);
protected:
    QHBoxLayout *layout;
    UserData  Friend;
    virtual void mousePressEvent(QMouseEvent *event);
};



class FriendsList : public QWidget{
    Q_OBJECT
public:
    explicit FriendsList(QWidget *parent = 0);
    void    ClearFriendsList(void);
    void    AddFriendToList(UserData Friend);
    void    AddFriendToList(UserData *Friend);
    void    resizeEvent(QResizeEvent *);
    void    SetFriendCount(int n);
    int     GetFriendCount(void);

    UserData GetFriendDataByUID(uint32_t UID);
public slots:
    void ItemClicked(UserData);
signals:
    void ChatTo(UserData);
protected:
    int         FriendCount;
    QWidget     *FriendLabelList;
    typedef QMap<int,FriendItem *>_Map;
    _Map::const_iterator Value;
    QScrollArea *List;
    QWidget     *FriendWidget;
    QVBoxLayout *FriendLayout;
    _Map         FriendMap;
    _Map::iterator result;
};

#endif // FRIENDS_LIST_H
