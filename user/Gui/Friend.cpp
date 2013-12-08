#include "LinkC_GUI.h"
#include "LinkC_Label.h"
#include <stdio.h>
#include <qt4/QtGui/QToolBox>
#include <qt4/QtGui/QListWidget>
#include <qt4/QtGui/QLabel>
#include <qt4/QtGui/QtGui>
#include <qt4/QtGui/QListView>
#include <qt4/QtGui/QFrame>
#include <qt4/QtGui/QScrollArea>
#include <qt4/QtCore/QMap>
#include <qt4/QtCore/QDebug>
#include <qt4/QtCore/QObject>

//#########################################
FriendGroup::FriendGroup(QWidget *parent)
    :QListWidget(parent){

}

//##########################################
LinkcFriendItem::LinkcFriendItem(QWidget *parent)
    :QFrame(parent){
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    layout = new QHBoxLayout;
    this->setLayout(layout);
    this->show();
}
void LinkcFriendItem::setFriend(const friend_data *data){
    QLabel *label = new QLabel(tr(data->name));
    layout->addWidget(label);
}

void LinkcFriendItem::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit clicked();
    }
    else
    QFrame::mousePressEvent(event);
}

//##########################################
FriendArea::FriendArea(QWidget *parent)
    :QWidget(parent){
    list              = new QListWidget(this);
    connect(list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ItemClicked(QListWidgetItem*)));
}

void FriendArea::AddFriendToLayout(friend_data Myfriend){
     QListWidgetItem *f = new QListWidgetItem;
     f->setText(Myfriend.name);
     list->addItem(f);
     FriendMap[f]=Myfriend.UID;
}

void FriendArea::resizeEvent(QResizeEvent *){
    list->resize(this->width(),this->height());

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

void FriendArea::ItemClicked(QListWidgetItem *item){
    result = FriendMap.find(item);
    emit ChatTo(result.value());
}
