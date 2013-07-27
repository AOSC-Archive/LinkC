#ifndef FRIEND_H
#define FRIEND_H
#include <qt4/Qt/qlistwidget.h>
#include "../include/LinkC_Label.h"
#include "../include/data_type.h"
#include <qt4/Qt/qlistwidget.h>
#include <qt4/QtGui/QtGui>
#include <QObject>
#include <QLabel>
#include <qt4/Qt/qlistwidget.h>
#include <QScrollArea>
#include <QMap>

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
    int chatTo(QListWidgetItem*);
protected:
    int friendcount;
    typedef QMap<QListWidgetItem *,int>_Map;
    _Map::const_iterator Value;
    QScrollArea *s;
    QListWidget *list;
    QWidget     *FriendWidget;
    QVBoxLayout *FriendLayout;
    _Map         FriendMap;
};

#endif
