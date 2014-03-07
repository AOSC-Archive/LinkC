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

#define _FRIEND_LABEL_HEIGTH    30

class FriendGroup : public QListWidget{
    Q_OBJECT
public:
    explicit FriendGroup(QWidget *parent = 0);
protected:
};



class LinkcFriendItem : public QLabel{
    Q_OBJECT
public:
    explicit    LinkcFriendItem(QWidget *parent = 0);
    void        setFriend(const LinkC_Friend_Data data);
    LinkC_Friend_Data GetFriend(void);
signals:
    void clicked(LinkC_Friend_Data);
protected:
    QHBoxLayout *layout;
    LinkC_Friend_Data Friend;
    virtual void mousePressEvent(QMouseEvent *event);
};



class FriendArea : public QWidget{
    Q_OBJECT
public:
    explicit FriendArea(QWidget *parent = 0);
    void AddFriendToLayout(LinkC_Friend_Data Myfriend);
    void resizeEvent(QResizeEvent *);
    void setFriendCount(const char s[]);
    void setFriendCount(int n);
    int FriendCount(void);

    LinkC_Friend_Data GetFriendDataByUID(int UID);
public slots:
    void ItemClicked(LinkC_Friend_Data);
signals:
    void ChatTo(LinkC_Friend_Data);
protected:
    int friendcount;
    QWidget     *FriendLabelArea;
    typedef QMap<int,struct LinkC_Friend_Data_t>_Map;
    _Map::const_iterator Value;
    QScrollArea *list;
    QWidget     *FriendWidget;
    QVBoxLayout *FriendLayout;
    _Map         FriendMap;
    _Map::iterator result;
};

class ChatDialog : public QWidget{
    Q_OBJECT
public:
    explicit ChatDialog(LinkC_Friend_Data MyFriend, QWidget *parent = 0);
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
    void GetFriendData(LinkC_Friend_Data);
protected:
    QPushButton *SendButton;
    QPushButton *QuitButton;
    QTextEdit   *History;
    QTextEdit   *Input;
    QVBoxLayout *Layout;
    LinkC_Friend_Data  MyFriend;
    p2p_client   peer;
    QTimer *timer;
    int MessageSize;
};

#endif
