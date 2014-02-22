#include "LinkC_GUI.h"
#include "LinkC_Label.h"
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

//#########################################
FriendGroup::FriendGroup(QWidget *parent)
    :QListWidget(parent){

}

//##########################################
LinkcFriendItem::LinkcFriendItem(QWidget *parent)
    :QLabel(parent){
//    this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
//    layout = new QHBoxLayout;
//    this->setLayout(layout);
    this->show();
}
void LinkcFriendItem::setFriend(const LinkC_Friend_Data data){
//    QLabel *label = new QLabel(tr(data.name));
    Friend = data;
    this->setText(tr(data.name));

//    layout->addWidget(label);
}

LinkC_Friend_Data LinkcFriendItem::GetFriend(){
    return Friend;
}

void LinkcFriendItem::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit clicked(Friend);
    }
    else
    QFrame::mousePressEvent(event);
}

//##########################################
FriendArea::FriendArea(QWidget *parent)
    :QWidget(parent){
    FriendLayout = new QVBoxLayout;
    FriendLabelArea = new QWidget(this);
    FriendLabelArea->setLayout(FriendLayout);
    FriendLabelArea->resize(1,1);
    list = new QScrollArea(this);
    list->setWidget(FriendLabelArea);
}

void FriendArea::AddFriendToLayout(LinkC_Friend_Data Myfriend){
     LinkcFriendItem *f = new LinkcFriendItem(this);
     f->setFriend(Myfriend);
     FriendLabelArea->resize(list->width()-20,_FRIEND_LABEL_HEIGTH*friendcount);
     FriendLayout->addWidget(f);
     this->connect(f,SIGNAL(clicked(LinkC_Friend_Data)),this,SLOT(ItemClicked(LinkC_Friend_Data)));
     FriendMap[Myfriend.UID] = Myfriend;
//     FriendLayout->addWidget(f);
//     list->addItem(f);
//     FriendMap[f]=Myfriend.UID;
}

void FriendArea::resizeEvent(QResizeEvent *){
    list->resize(this->width(),this->height());
    FriendLabelArea->resize(list->width()-20,_FRIEND_LABEL_HEIGTH*friendcount);
}

void FriendArea::setFriendCount(const char s[]){
    friendcount=atoi(s);
}

void FriendArea::setFriendCount(int n){
    friendcount=n;
}

int FriendArea::FriendCount(){
    return friendcount;
}

void FriendArea::ItemClicked(LinkC_Friend_Data data){
    emit ChatTo(data);
}

LinkC_Friend_Data FriendArea::GetFriendDataByUID(int UID){
    result = FriendMap.find(UID);
    return result.value();
}
