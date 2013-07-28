#ifndef LINKC_GUI_H
#define LINKC_GUI_H
#include "../include/LinkC_Label.h"
#include "../include/data_type.h"
#include <qt4/QtCore/QObject>
#include <qt4/QtCore/QMap>
#include <qt4/QtGui/QLabel>
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
    explicit LinkcFriendItem(QWidget *parent = 0);
    void setFriend(const friend_data *data);
signals:
    void clicked();
protected:
    QHBoxLayout *layout;
    virtual void mousePressEvent(QMouseEvent *event);
};



class FriendArea : public QWidget{
    Q_OBJECT
public:
    explicit FriendArea(QWidget *parent = 0);
    void AddFriendToLayout(friend_data Myfriend);
    void resizeEvent(QResizeEvent *);
    void setFriendCount(const char s[]);
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
    explicit ChatDialog(QWidget *parent = 0);
protected:
    QPushButton *Send;
    QPushButton *Quit;

};

#endif
