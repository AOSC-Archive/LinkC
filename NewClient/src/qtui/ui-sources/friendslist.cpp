#include <stdio.h>
#include <QToolBox>
#include <QListWidget>
#include <QLabel>
#include <QtGui>
#include <QListView>
#include <QFrame>
#include <QScrollArea>
#include <QMap>
#include <QDebug>
#include <QObject>
#include "friendslist.h"

FriendItem::FriendItem(QWidget *parent)
    :QLabel(parent){
    this->show();
}
void FriendItem::SetFriend(const UserData data){
    Friend = data;
    this->setText(tr(data.UserName));
}

UserData FriendItem::GetFriend(){
    return Friend;
}

void FriendItem::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit clicked(Friend);
    }else
        QFrame::mousePressEvent(event);
}

//#############################################################

FriendsList::FriendsList(QWidget *parent)
    :QWidget(parent){
    FriendLayout = new QVBoxLayout;
    FriendLabelList = new QWidget(this);
    FriendLabelList->setLayout(FriendLayout);
    FriendLabelList->resize(1,1);
    List = new QScrollArea(this);
    List->setWidget(FriendLabelList);
}

void FriendsList::AddFriendToList(UserData Myfriend){
     FriendItem *f = new FriendItem(this);
     f->SetFriend(Myfriend);
     FriendLabelList->resize(List->width()-20,_FRIEND_LABEL_HEIGTH*FriendCount);
     FriendLayout->addWidget(f);
     this->connect(f,SIGNAL(clicked(UserData)),this,SLOT(ItemClicked(UserData)));
     FriendMap[Myfriend.UID] = f;
}

void FriendsList::AddFriendToList(UserData *Friend){
    FriendItem *f = new FriendItem(this);
    UserData MyFriend;
    memcpy((void*)&MyFriend,(void*)Friend,sizeof(UserData));
    f->SetFriend(MyFriend);
    FriendLabelList->resize(List->width()-20,_FRIEND_LABEL_HEIGTH*FriendCount);
    FriendLayout->addWidget(f);
    this->connect(f,SIGNAL(clicked(UserData)),this,SLOT(ItemClicked(UserData)));
    FriendMap[MyFriend.UID] = f;
}

void FriendsList::ClearFriendsList(){
    if(FriendMap.isEmpty() == false){
        for(result = FriendMap.begin();result != FriendMap.end();result++)
            delete result.value();
        FriendMap.clear();
        FriendCount = 0;
    }
}

void FriendsList::resizeEvent(QResizeEvent *){
    List->resize(this->width(),this->height());
    FriendLabelList->resize(List->width()-20,_FRIEND_LABEL_HEIGTH*FriendCount);
}

void FriendsList::SetFriendCount(int n){
    FriendCount=n;
}

int FriendsList::GetFriendCount(){
    return FriendCount;
}

void FriendsList::ItemClicked(UserData data){
    emit ChatTo(data);
}

UserData FriendsList::GetFriendDataByUID(uint32_t UID){
    result = FriendMap.find(UID);
    return result.value()->GetFriend();
}
