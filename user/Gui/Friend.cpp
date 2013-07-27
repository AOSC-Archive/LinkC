#include "../include/Friend.h"
#include <stdio.h>
#include <qt4/Qt/qtoolbox.h>
#include <qt4/Qt/qlistwidget.h>
#include <qt4/Qt/qlabel.h>
#include "../include/LinkC_Label.h"
#include <QObject>
#include <qt4/QtGui/QtGui>
#include <qt4/QtGui/QListView>
#include <qt4/QtGui/QFrame>
#include <QScrollArea>
#include <Qt/qmap.h>

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
//    s               = new QScrollArea(this);
    list              = new QListWidget(this);
//    FriendLayout    = new QVBoxLayout;
//    FriendWidget    = new QWidget;
    list->connect(list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(chatTo(QListWidgetItem*)));
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

int FriendArea::FriendCount(){
    return friendcount;
}

int FriendArea::chatTo(QListWidgetItem *){

    return 0;
}
